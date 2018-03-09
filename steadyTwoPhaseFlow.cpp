/////////////////////////////////////////////////////////////////////////////
/// Name:        twoPhaseFlowPT.cpp
/// Purpose:     methods related to steady-state two-phase flow
///              in pore/throat networks are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

namespace PNM {

void network::runTwoPhaseSSModelPT()
{
    cout<<"Starting Two-Phase SS Simulation... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //post-processing
    if(videoRecording)
        record=true;

    initializeTwoPhaseSimulationPT();

    if(primaryDrainageSimulation && !cancel)
        primaryDrainagePT();

    restoreWettabilityPT();

    if(spontaneousImbibitionSimulation && !cancel)
        spontaneousImbibitionPT();

    if(forcedWaterInjectionSimulation && !cancel)
        forcedWaterInjectionPT();

    if(spontaneousOilInvasionSimulation && !cancel)
        spontaneousOilInvasionPT();

    if(secondaryOilDrainageSimulation && !cancel)
        secondaryOilDrainagePT();

    //post-processing
    if(videoRecording)
    {
        record=false;
        extractVideo();
    }

    endTime=tools::getCPUTime();
    cout<<"Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::initializeTwoPhaseSimulationPT()
{
    cancel=false;

    assignWWWettabilityPT();

    if(twoPhaseSS)
    {
        fillWithPhase(phase::water);
        initialiseCapillaries();
    }

    else
    {
        if(waterDistribution!=4)
        {
            fillWithPhase(phase::water,initialWaterSaturation,waterDistribution,phase::oil);
            initialiseCapillaries();
        }
        else
        {
            fillWithPhase(phase::water);
            initialiseCapillaries();
            primaryDrainagePT(initialWaterSaturation);
            initialiseCapillaries();
        }

        restoreWettabilityPT();

        if(overrideByInjectedPVs)
        {
            simulationTime=totalElementsVolume*injectedPVs/flowRate;
            cout<<"PVs to inject: "<<injectedPVs<<endl;
        }
    }
}

void network::primaryDrainagePT(double finalSaturation)

{
    cout<<"Starting Primary Drainage... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //Prepare result files
    ofstream file("Results/1-primaryDrainagePcCurve.txt");
    ofstream file2("Results/1-primaryDrainageRelativePermeabilies.txt");
    ofstream file3("Results/1-primaryDrainageCriticalValues.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double outputWaterSaturation=1;//used to print Kr plots at regular intervals
    double currentWaterVolume(totalElementsVolume);
    bool spanningOil=false;//used to detect the onset of breakthrough

    //Calculate the incremental increase of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:accessibleElements)
    {
        double pc=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(pc<minPc)
        {
            minPc=pc;
        }
        if(pc>maxPc)
        {
            maxPc=pc;
        }
    }

    double effectiveMinRadius=2*OWSurfaceTension/maxPc;
    double effectiveMaxRadius=2*OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMaxRadius;

    //Look for capillaries with water film potential
    assignFilmStability();

    //Look for capillaries connected to water
    clusterWaterFlowingElements();
    set<element*> elementsToInvade;
    for(pore* e:accessiblePores)
        if(e->getPhaseFlag()==phase::water && e->getInlet())
            elementsToInvade.insert(e);

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        //set current capillary pressure
        loop++;
        currentRadius-=radiusStep;
        double currentPc=2*OWSurfaceTension/currentRadius;

        //Invade capillaries with capillary entry pressure < current capillary pressure
        bool stillMore=true;
        while(stillMore)
        {
            stillMore=false;
            clusterOilElements();
            clusterWaterFlowingElements();

            vector<element*> invadedElements;
            for(element* e: elementsToInvade)
            {
                double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                if(currentPc+1e-5>=entryPressure && e->getClusterWaterFilm()->getOutlet())
                    invadedElements.push_back(e);
            }

            for(element* e : invadedElements)
            {
                e->setPhaseFlag(phase::oil);
                if(e->getWaterCanFlowViaFilm())
                    e->setWaterCornerActivated(true);
                currentWaterVolume-=e->getVolume();
                elementsToInvade.erase(e);

                for(element* n:e->getNeighboors())
                    if(!n->getClosed() && n->getPhaseFlag()==phase::water && e->getClusterWaterFilm()->getOutlet())
                        elementsToInvade.insert(n);

                stillMore=true;
            }

            //Update Graphics
            emitPlotSignal();

            if(finalSaturation!=0 && currentWaterVolume/totalElementsVolume<finalSaturation)
                break;

            //Thread Management
            if(cancel)break;
        }

        //Remove trapped water capillaries from the set potentially-invaded capillaries
        clusterWaterFlowingElements();
        vector<element*> trappedElements;
        for(set<element*>::iterator it = elementsToInvade.begin(); it != elementsToInvade.end(); ++it )
        {
            element* e=*it;
            if(!e->getClusterWaterFilm()->getOutlet())
                trappedElements.push_back(e);
        }
        for(element* e : trappedElements)
            elementsToInvade.erase(e);


        //Update water film volume
        double waterVolume(0);
        for(element* e: accessibleElements)
        {
            double rSquared=pow(OWSurfaceTension/currentPc,2);
            if(e->getPhaseFlag()==phase::oil)
            {
                if(e->getWaterCornerActivated() && e->getClusterWaterFilm()->getOutlet())
                {
                    double filmVolume=min(rSquared*e->getFilmAreaCoefficient()*e->getLength(),(1-4*tools::pi()*e->getShapeFactor())*e->getVolume());
                    double filmConductance=rSquared*filmVolume/e->getLength()/(waterViscosity*e->getLength());
                    e->setWaterFilmVolume(filmVolume);
                    e->setWaterFilmConductivity(filmConductance);//cout<<filmConductance<<endl;
                    e->setEffectiveVolume(e->getVolume()-e->getWaterFilmVolume());
                    if(e->getWaterFilmVolume()>e->getVolume()){cout<<"FATAL EROOR in PD: water film > capillary volume."<<endl;cancel=true;}
                    waterVolume+=e->getWaterFilmVolume();
                }
            }
            if(e->getPhaseFlag()==phase::water)
                waterVolume+=e->getVolume();
        }
        currentWaterVolume=waterVolume;

        //Extract relative permeability results
        if(relativePermeabilitiesCalculation)
            if(currentWaterVolume/totalElementsVolume<outputWaterSaturation+1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                outputWaterSaturation-=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalElementsVolume>0.001)
            file<<abs(currentWaterVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << tools::PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalElementsVolume)*100;
        simulationNotification = ss.str();

        //Extract data at Breakthrough
        if(!spanningOil)
        {
            clusterOilElements();
            if (isOilSpanning)
            {
                spanningOil=true;
                criticalPcPD=currentPc;
                criticalSaturationPD=abs(currentWaterVolume/totalElementsVolume);
                file3<<"Pc at Breakthrough: "<<criticalPcPD<<endl;
                file3<<"Sw at Breakthrough: "<<criticalSaturationPD<<endl;
            }
        }

        finalPcPD=currentPc;
        finalSaturationPD=abs(currentWaterVolume/totalElementsVolume);

        if(finalSaturation!=0 && currentWaterVolume/totalElementsVolume<finalSaturation)
            break;

        //Thread Management
        if(cancel)break;
    }

    file3<<"Pc after PD: "<<finalPcPD<<endl;
    file3<<"Sw after PD: "<<finalSaturationPD<<endl;

    endTime=tools::getCPUTime();
    //cout<<"Primary Drainage Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::spontaneousImbibitionPT()
{
    cout<<"Starting Spontaneous Imbibition... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare result files
    ofstream file("Results/2-spontaneousImbibitionPcCurve.txt");
    ofstream file2("Results/2-spontaneousImbibitionRelativePermeabilies.txt");
    ofstream file3("Results/2-spontaneousImbibitionCriticalValues.txt");

    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double waterSaturation=getWaterSaturationWithFilms();
    double currentWaterVolume(waterSaturation*totalElementsVolume);

    //Calculate the incremental reduction of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:accessibleElements)
    {
        double pc=OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(abs(pc)<minPc)
        {
            minPc=abs(pc);
        }

        pc=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(abs(pc)>maxPc)
        {
            maxPc=abs(pc);
        }
    }

    double effectiveMinRadius=OWSurfaceTension/maxPc;
    double effectiveMaxRadius=OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMinRadius;

    //Look for capillaries with water film potential
    assignFilmStability();

    //Look for capillaries connected to water
    set<element*> elementsToInvade;
    for(element* e:accessibleElements)
        if(e->getPhaseFlag()==phase::oil && e->getWettabilityFlag()==wettability::waterWet)
            elementsToInvade.insert(e);

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        //set current capillary pressure
        loop++;
        currentRadius+=radiusStep;
        double currentPc=OWSurfaceTension/currentRadius;

        //Invade capillaries with capillary entry pressure < current capillary pressure

        vector<element*> invadedElements;

        //Snap off
        clusterWaterFlowingElements();
        clusterOilFlowingElements();
        for(element* e:elementsToInvade)
        {
            double entryPressure=OWSurfaceTension*cos(e->getTheta())/e->getRadius();
            if(currentPc-1e-5<=entryPressure && e->getWaterCornerActivated() && e->getClusterWaterFilm()->getInlet() && e->getClusterOilFilm()->getOutlet())
                invadedElements.push_back(e);
        }

        for(element* e:invadedElements)
        {
            e->setPhaseFlag(phase::water);
            elementsToInvade.erase(e);
        }

        //Update Graphics
        emitPlotSignal();

        //Bulk Imbibition

        bool stillMore=true;
        while(stillMore)
        {
            stillMore=false;
            clusterWaterFlowingElements();
            clusterOilFlowingElements();

            vector<element*> invadedElements;
            for(element* e:elementsToInvade)
            {
                bool connectedToInletWaterCluster=false;
                for(element* n : e->getNeighboors())
                    if(!n->getClosed() && n->getPhaseFlag()==phase::water && n->getClusterWaterFilm()->getInlet())
                    {connectedToInletWaterCluster=true;break;}

                //throat
                if(e->getType()==capillaryType::throat && (e->getInlet() || connectedToInletWaterCluster) && e->getClusterOilFilm()->getOutlet())
                {
                    double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                    if(currentPc-1e-5<=entryPressure)
                        invadedElements.push_back(e);
                }

                //pore

                if(e->getType()==capillaryType::poreBody && connectedToInletWaterCluster && e->getClusterOilFilm()->getOutlet())
                {
                    int oilNeighboorsNumber(0);
                    int totalNeighboorsNumber(0);
                    for(element* n : e->getNeighboors())
                    {
                        if(!n->getClosed() && n->getPhaseFlag()==phase::oil)
                            oilNeighboorsNumber++;

                        if(!n->getClosed())
                            totalNeighboorsNumber++;
                    }

                   double entryPressureBodyFilling=0;
                   if(oilNeighboorsNumber==1)
                       entryPressureBodyFilling=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                   if(oilNeighboorsNumber>1)
                       entryPressureBodyFilling=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius()/double(oilNeighboorsNumber);

                    if(currentPc-1e-5<=entryPressureBodyFilling)
                        invadedElements.push_back(e);
                }
            }

            for(element* e:invadedElements)
            {
                e->setPhaseFlag(phase::water);
                elementsToInvade.erase(e);
                stillMore=true;
            }

            //Update Graphics
            emitPlotSignal();

            //Thread Management
            if(cancel)break;
        }

        //Remove trapped oil capillaries from the set potentially-invaded capillaries
        clusterOilFlowingElements();
        vector<element*> trappedElements;
        for(element* e:elementsToInvade)
        {
            if(!e->getClusterOilFilm()->getOutlet())
                trappedElements.push_back(e);
        }

        for(element* e:trappedElements)
            elementsToInvade.erase(e);

        //Update water film volume
        clusterWaterFlowingElements();
        double waterVolume(0);
        for(element* e: accessibleElements)
        {
            double rSquared=pow(OWSurfaceTension/currentPc,2);
            if(e->getPhaseFlag()==phase::oil && e->getWettabilityFlag()==wettability::waterWet)
            {
                if(e->getWaterCornerActivated() && e->getClusterWaterFilm()->getInlet() && e->getClusterOilFilm()->getOutlet())
                {
                    double filmVolume=min(rSquared*e->getFilmAreaCoefficient()*e->getLength(),(1-4*tools::pi()*e->getShapeFactor())*e->getVolume());
                    double filmConductance=rSquared*filmVolume/e->getLength()/(waterViscosity*e->getLength());
                    e->setWaterFilmVolume(filmVolume);
                    e->setWaterFilmConductivity(filmConductance);
                    e->setEffectiveVolume(e->getVolume()-e->getWaterFilmVolume());
                }
                waterVolume+=e->getWaterFilmVolume();
            }
            if(e->getPhaseFlag()==phase::oil && e->getWettabilityFlag()==wettability::oilWet)
                waterVolume+=e->getWaterFilmVolume();
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::oilWet)
                waterVolume+=e->getEffectiveVolume()+e->getWaterFilmVolume();
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::waterWet)
                waterVolume+=e->getVolume();
        }
        currentWaterVolume=waterVolume;


        //Extract relative permeability results
        if(relativePermeabilitiesCalculation)
            if(currentWaterVolume/totalElementsVolume>waterSaturation-1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation+=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalElementsVolume>0.001)
            file<<abs(currentWaterVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << tools::PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalElementsVolume)*100;
        simulationNotification = ss.str();

        finalPcPI=currentPc;
        finalSaturationPI=abs(currentWaterVolume/totalElementsVolume);

        //Thread Management
        if(cancel)break;
    }

    file3<<"Pc after SI: "<<finalPcPI<<endl;
    file3<<"Sw after SI: "<<finalSaturationPI<<endl;

    endTime=tools::getCPUTime();
    //cout<<"Spontaneous Imbibition Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::forcedWaterInjectionPT()
{
    cout<<"Starting Forced Water Injection ... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare results files
    ofstream file("Results/3-forcedWaterInjectionPcCurve.txt");
    ofstream file2("Results/3-forcedWaterInjectionRelativePermeabilies.txt");
    ofstream file3("Results/3-forcedWaterInjectionCriticalValues.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double waterSaturation=getWaterSaturationWithFilms();
    double currentWaterVolume=waterSaturation*totalElementsVolume;

    //Calculate the incremental reduction of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:accessibleElements)
    {
        double pc=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(abs(pc)<minPc)
        {
            minPc=abs(pc);
        }
        if(abs(pc)>maxPc)
        {
            maxPc=abs(pc);
        }
    }

    double effectiveMinRadius=2*OWSurfaceTension/maxPc;
    double effectiveMaxRadius=2*OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMaxRadius;

    //Look for capillaries connected to water
    set<element*> elementsToInvade;
    for(element* e: accessibleElements)
    {
        if(e->getPhaseFlag()==phase::oil)
            elementsToInvade.insert(e);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        //set current capillary pressure
        loop++;
        currentRadius-=radiusStep;
        double currentPc=-2*OWSurfaceTension/currentRadius;

        //Invade capillaries with capillary entry pressure < current capillary pressure
        bool stillMore=true;
        while(stillMore)
        {
            stillMore=false;
            clusterOilFlowingElements();
            clusterWaterFlowingElements();

            vector<element*> invadedElements;
            for(element* e: elementsToInvade)
            {
                bool connectedToInletWaterCluster=false;
                for(element* n : e->getNeighboors())
                    if(!n->getClosed() && n->getPhaseFlag()==phase::water && n->getClusterWaterFilm()->getInlet())
                    {connectedToInletWaterCluster=true;break;}

                if((e->getType()==capillaryType::throat && (e->getInlet() || connectedToInletWaterCluster)) || (e->getType()==capillaryType::poreBody && connectedToInletWaterCluster))
                {
                    if(e->getClusterOilFilm()->getOutlet())
                    {
                        double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                        if(currentPc-1e-5<=entryPressure)
                            invadedElements.push_back(e);
                    }
                }
            }

            for(element* e: invadedElements)
            {
                e->setPhaseFlag(phase::water);
                if(e->getOilCanFlowViaFilm())
                    e->setOilLayerActivated(true);
                elementsToInvade.erase(e);
                stillMore=true;
            }

            //Update Graphics
            emitPlotSignal();

            //Thread Management
            if(cancel)break;
        }

        //Remove trapped oil capillaries from the set potentially-invaded capillaries
        clusterOilFlowingElements();
        vector<element*> trappedElements;
        for(element* e: elementsToInvade)
        {
            if(!e->getClusterOilFilm()->getOutlet())
                trappedElements.push_back(e);
        }
        for(element* e: trappedElements)
            elementsToInvade.erase(e);

        //Update film volumes
        clusterWaterFlowingElements();
        clusterOilFlowingElements();
        double waterVolume(0);
        for(element* e: accessibleElements)
        {
            double rSquared=pow(OWSurfaceTension/currentPc,2);
            if(e->getPhaseFlag()==phase::oil && e->getWettabilityFlag()==wettability::oilWet)
                waterVolume+=e->getWaterFilmVolume();
            if(e->getPhaseFlag()==phase::oil && e->getWettabilityFlag()==wettability::waterWet)
                waterVolume+=e->getWaterFilmVolume();
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::oilWet)
            {
                if(e->getClusterWaterFilm()->getInlet() && e->getOilLayerActivated() && e->getClusterOilFilm()->getOutlet())
                {
                    double filmVolume=min(rSquared*e->getFilmAreaCoefficient()*e->getLength(),(1-4*tools::pi()*e->getShapeFactor())*e->getVolume());
                    double effectiveOilFilmVolume=max(0.0,filmVolume-e->getWaterFilmVolume());
                    double filmConductance=rSquared*effectiveOilFilmVolume/e->getLength()/(oilViscosity*e->getLength());

                    e->setOilFilmVolume(effectiveOilFilmVolume);
                    e->setOilFilmConductivity(filmConductance);
                    //if(effectiveOilFilmVolume==0) // oil layer collapse
                    //    e->setOilLayerActivated(false);
                    e->setEffectiveVolume(e->getVolume()-e->getOilFilmVolume()-e->getWaterFilmVolume());
                    if(e->getEffectiveVolume()<0){cancel=true;cout<<"effective volume < 0"<<endl;}
                }
                waterVolume+=e->getEffectiveVolume()+e->getWaterFilmVolume();
            }
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::waterWet)
                waterVolume+=e->getVolume();
        }
        currentWaterVolume=waterVolume;

        //Extract relative permeability results
        if(relativePermeabilitiesCalculation)
            if(currentWaterVolume/totalElementsVolume>waterSaturation-1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation+=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalElementsVolume>0.001)
            file<<abs(currentWaterVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << tools::PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalElementsVolume)*100;
        simulationNotification = ss.str();

        finalPcSD=currentPc;
        finalSaturationSD=abs(currentWaterVolume/totalElementsVolume);

        //Thread Management
        if(cancel)break;
    }

    file3<<"Pc after FWI: "<<finalPcSD<<endl;
    file3<<"Sw after FWI: "<<finalSaturationSD<<endl;

    endTime=tools::getCPUTime();
    //cout<<"Forced Water Injection Time: "<<endTime-startTime<<" s"<<endl;
}

void network::spontaneousOilInvasionPT()
{
    cout<<"Starting Spontaneous Oil Invasion ... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare results files
    ofstream file("Results/4-spontaneousOilnvasionPcCurve.txt");
    ofstream file2("Results/4-spontaneousOilnvasionRelativePermeabilies.txt");
    ofstream file3("Results/4-spontaneousOilnvasionCriticalValues.txt");

    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double waterSaturation=getWaterSaturationWithFilms();
    double currentWaterVolume=waterSaturation*totalElementsVolume;

    //Calculate the incremental increase of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:accessibleElements)
    {
        double pc=OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(abs(pc)<minPc)
        {
            minPc=abs(pc);
        }

        pc=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(abs(pc)>maxPc)
        {
            maxPc=abs(pc);
        }
    }

    double effectiveMinRadius=OWSurfaceTension/maxPc;
    double effectiveMaxRadius=OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMinRadius;

    //Look for capillaries connected to oil
    clusterWaterFlowingElements();
    set<element*> elementsToInvade;
    for(element* e:accessibleElements)
        if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::oilWet)
            elementsToInvade.insert(e);

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        //set current capillary pressure
        loop++;
        currentRadius+=radiusStep;
        double currentPc=-OWSurfaceTension/currentRadius;

        //Invade capillaries with capillary entry pressure < current capillary pressure

        vector<element*> invadedElements;

        //snap off
        clusterOilFlowingElements();
        clusterWaterFlowingElements();
        for(element* e:elementsToInvade)
        {
            double entryPressure=OWSurfaceTension*cos(e->getTheta())/e->getRadius();
            if(currentPc+1e-5>=entryPressure && e->getOilLayerActivated() && e->getClusterOilFilm()->getInlet() && e->getClusterWaterFilm()->getOutlet())
                invadedElements.push_back(e);
        }

        for(element* e:invadedElements)
        {
            e->setPhaseFlag(phase::oil);
            elementsToInvade.erase(e);
        }

        //Update Graphics
        emitPlotSignal();

        //Bulk Imbibition

        bool stillMore=true;
        while(stillMore)
        {
            stillMore=false;
            clusterOilFlowingElements();
            clusterWaterFlowingElements();

            vector<element*> invadedElements;
            for(element* e:elementsToInvade)
            {
                bool connectedToInletOilCluster=false;
                for(element* n : e->getNeighboors())
                    if(!n->getClosed() && n->getPhaseFlag()==phase::oil && n->getClusterOilFilm()->getInlet())
                    {connectedToInletOilCluster=true;break;}

                //throat
                if(e->getType()==capillaryType::throat && (e->getInlet() || connectedToInletOilCluster) && e->getClusterWaterFilm()->getOutlet())
                {
                    double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                    if(currentPc+1e-5>=entryPressure)
                        invadedElements.push_back(e);
                }

                //pore

                if(e->getType()==capillaryType::poreBody && connectedToInletOilCluster && e->getClusterWaterFilm()->getOutlet())
                {
                    int waterNeighboorsNumber(0);
                    int totalNeighboorsNumber(0);
                    for(element* n : e->getNeighboors())
                    {
                        if(!n->getClosed() && n->getPhaseFlag()==phase::water)
                            waterNeighboorsNumber++;

                        if(!n->getClosed())
                            totalNeighboorsNumber++;
                    }

                   double entryPressureBodyFilling=0;
                   if(waterNeighboorsNumber==1)
                       entryPressureBodyFilling=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                   if(waterNeighboorsNumber>1)
                       entryPressureBodyFilling=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius()/double(waterNeighboorsNumber);

                    if(currentPc+1e-5>=entryPressureBodyFilling)
                        invadedElements.push_back(e);
                }
            }

            for(element* e:invadedElements)
            {
                e->setPhaseFlag(phase::oil);
                elementsToInvade.erase(e);
                stillMore=true;
            }

            //Update Graphics
            emitPlotSignal();

            //Thread Management
            if(cancel)break;
        }

        //Remove trapped water capillaries from the set potentially-invaded capillaries
        clusterWaterFlowingElements();
        vector<element*> trappedElements;
        for(element* e:elementsToInvade)
        {
            if(!e->getClusterWaterFilm()->getOutlet())
                trappedElements.push_back(e);
        }

        for(element* e:trappedElements)
            elementsToInvade.erase(e);

        //Update oil film volume
        clusterOilFlowingElements();
        double waterVolume(0);
        for(element* e: accessibleElements)
        {
            double rSquared=pow(OWSurfaceTension/currentPc,2);
            if(e->getPhaseFlag()==phase::oil && e->getWettabilityFlag()==wettability::oilWet)
                waterVolume+=e->getWaterFilmVolume();
            if(e->getPhaseFlag()==phase::oil && e->getWettabilityFlag()==wettability::waterWet)
                waterVolume+=e->getWaterFilmVolume();
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::oilWet)
            {
                if(e->getOilLayerActivated() && e->getClusterOilFilm()->getInlet() && e->getClusterWaterFilm()->getOutlet() )
                {
                    double filmVolume=min(rSquared*e->getFilmAreaCoefficient()*e->getLength(),(1-4*tools::pi()*e->getShapeFactor())*e->getVolume()-e->getWaterFilmVolume());
                    double filmConductance=rSquared*filmVolume/e->getLength()/(oilViscosity*e->getLength());
                    e->setOilFilmVolume(filmVolume);//if(e->getType()==0 && e->getId()==177)cout<<e->getOilFilmVolume()/e->getVolume()<<endl;
                    e->setOilFilmConductivity(filmConductance);
                    e->setEffectiveVolume(e->getVolume()-e->getOilFilmVolume()-e->getWaterFilmVolume());
                }
                waterVolume+=e->getEffectiveVolume()+e->getWaterFilmVolume();
            }
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::waterWet)
                waterVolume+=e->getVolume();
            //cout<<currentPc<<" "<<currentRadius<<" "<<e->getEffectiveVolume()<<" "<<e->getWaterFilmVolume()<<endl;
        }
        currentWaterVolume=waterVolume;

        //Extract relative permeability results
        if(relativePermeabilitiesCalculation)
            if(currentWaterVolume/totalElementsVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalElementsVolume>0.001)
            file<<abs(currentWaterVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << tools::PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalElementsVolume)*100;
        simulationNotification = ss.str();

        finalPcSI=currentPc;
        finalSaturationSI=abs(currentWaterVolume/totalElementsVolume);

        //Thread Management
        if(cancel)break;
    }

    file3<<"Pc after SOI: "<<finalPcSI<<endl;
    file3<<"Sw after SOI: "<<finalSaturationSI<<endl;

    endTime=tools::getCPUTime();
    //cout<<"Spontaneous Oil Invasion Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::secondaryOilDrainagePT()
{
    cout<<"Starting Secondary Oil Drainage... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare results files
    ofstream file("Results/5-secondaryOilDrainagePcCurve.txt");
    ofstream file2("Results/5-secondaryOilDrainageRelativePermeabilies.txt");
    ofstream file3("Results/5-secondaryOilDrainageCriticalValues.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double waterSaturation=getWaterSaturationWithFilms();
    double currentWaterVolume=waterSaturation*totalElementsVolume;

    //Calculate the incremental reduction of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:accessibleElements)
    {
        double pc=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(abs(pc)<minPc)
        {
            minPc=abs(pc);
        }
        if(abs(pc)>maxPc)
        {
            maxPc=abs(pc);
        }
    }

    double effectiveMinRadius=2*OWSurfaceTension/maxPc;
    double effectiveMaxRadius=2*OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMaxRadius;

    //Look for capillaries connected to oil
    set<element*> elementsToInvade;
    for(element* e: accessibleElements)
    {
        if(e->getPhaseFlag()==phase::water)
            elementsToInvade.insert(e);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        //set current capillary pressure
        loop++;
        currentRadius-=radiusStep;
        double currentPc=2*OWSurfaceTension/currentRadius;

        //Invade capillaries with capillary entry pressure < current capillary pressure
        bool stillMore=true;
        while(stillMore)
        {
            stillMore=false;
            clusterWaterFlowingElements();
            clusterOilFlowingElements();

            vector<element*> invadedElements;
            for(element* e: elementsToInvade)
            {
                bool connectedToInletOilCluster=false;
                for(element* n : e->getNeighboors())
                    if(!n->getClosed() && n->getPhaseFlag()==phase::oil && n->getClusterOilFilm()->getInlet())
                    {connectedToInletOilCluster=true;break;}

                if((e->getType()==capillaryType::throat && (e->getInlet() || connectedToInletOilCluster)) || (e->getType()==capillaryType::poreBody && connectedToInletOilCluster))
                {
                    if(e->getClusterWaterFilm()->getOutlet())//change
                    {
                        double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                        if(currentPc+1e-5>=entryPressure)
                            invadedElements.push_back(e);
                    }
                }
            }

            for(element* e: invadedElements)
            {
                e->setPhaseFlag(phase::oil);
                if(e->getWaterCanFlowViaFilm())
                    e->setWaterCornerActivated(true);
                elementsToInvade.erase(e);
                stillMore=true;
            }

            //Thread Management
            if(cancel)break;

            //Update Graphics
            emitPlotSignal();
        }

        //Remove trapped water capillaries from the set potentially-invaded capillaries
        clusterWaterFlowingElements();
        vector<element*> trappedElements;
        for(element* e: elementsToInvade)
            if(!e->getClusterWaterFilm()->getOutlet())
                trappedElements.push_back(e);
        for(element* e: trappedElements)
        elementsToInvade.erase(e);

        //Account for film volumes
        clusterOilFlowingElements();
        double waterVolume(0);
        for(element* e: accessibleElements)
        {
            if(e->getPhaseFlag()==phase::oil)
                waterVolume+=e->getWaterFilmVolume();
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::oilWet)
                waterVolume+=e->getEffectiveVolume()+e->getWaterFilmVolume();
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::waterWet)
                waterVolume+=e->getVolume();
        }
        currentWaterVolume=waterVolume;

        //Extract relative permeability results
        if(relativePermeabilitiesCalculation)
            if(currentWaterVolume/totalElementsVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalElementsVolume>0.001)
            file<<abs(currentWaterVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << tools::PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalElementsVolume)*100;
        simulationNotification = ss.str();

        finalPcTD=currentPc;
        finalSaturationTD=abs(currentWaterVolume/totalElementsVolume);

        //Thread Management
        if(cancel)break;
    }

    file3<<"Pc after SOD: "<<finalPcTD<<endl;
    file3<<"Sw after SOD: "<<finalSaturationTD<<endl;

    endTime=tools::getCPUTime();
    //cout<<"Secondary Oil Drainage Time: "<<endTime-startTime<<" s"<<endl;
}

}
