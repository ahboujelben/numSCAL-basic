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
#include "iterator.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace PNM {

void network::runTwoPhaseSSModel()
{
    cout<<"Starting Two-Phase SS Simulation... "<<endl;

    //post-processing
    if(videoRecording)
        record=true;

    initialiseTwoPhaseSSModel();

    if(primaryDrainageSimulation && !simulationInterrupted)
        primaryDrainage();

    restoreWettability();

    if(spontaneousImbibitionSimulation && !simulationInterrupted)
        spontaneousImbibition();

    if(forcedWaterInjectionSimulation && !simulationInterrupted)
        forcedWaterInjection();

    if(spontaneousOilInvasionSimulation && !simulationInterrupted)
        spontaneousOilInvasion();

    if(secondaryOilDrainageSimulation && !simulationInterrupted)
        secondaryOilDrainage();

    //post-processing
    if(videoRecording)
    {
        record=false;
        extractVideo();
    }
}

void network::initialiseTwoPhaseSSModel()
{
    simulationInterrupted=false;
    assignWWWettability();
    fillWithPhase(phase::water);
    initialiseCapillaries();
    assignHalfAngles();
}

void network::primaryDrainage(double finalSaturation)

{
    cout<<"Starting Primary Drainage... "<<endl;

    //Prepare result files
    ofstream file("Results/1-primaryDrainagePcCurve.txt");
    ofstream file2("Results/1-primaryDrainageRelativePermeabilies.txt");
    ofstream file3("Results/1-primaryDrainageCriticalValues.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double outputWaterSaturation=1;//used to print Kr plots at regular intervals
    double currentWaterVolume(totalNetworkVolume);
    bool spanningOil=false;//used to detect the onset of breakthrough

    //Calculate the incremental increase of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:networkRange<element*>(this))
    {
        double pc=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
    unordered_set<element*> elementsToInvade;
    for(pore* e:networkRange<pore*>(this))
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
                double entryPressure=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
                    if(n->getPhaseFlag()==phase::water && e->getClusterWaterFilm()->getOutlet())
                        elementsToInvade.insert(n);

                stillMore=true;
            }

            //Update Graphics
            emitPlotSignal();

            if(finalSaturation!=0 && currentWaterVolume/totalNetworkVolume<finalSaturation)
                break;

            //Thread Management
            if(simulationInterrupted)break;
        }

        //Remove trapped water capillaries from the set potentially-invaded capillaries
        clusterWaterFlowingElements();
        vector<element*> trappedElements;
        for(element* e: elementsToInvade )
        {
            if(!e->getClusterWaterFilm()->getOutlet())
                trappedElements.push_back(e);
        }
        for(element* e : trappedElements)
            elementsToInvade.erase(e);


        //Update water film volume
        double waterVolume(0);
        for(element* e: networkRange<element*>(this))
        {
            double rSquared=pow(OWSurfaceTension/currentPc,2);
            if(e->getPhaseFlag()==phase::oil)
            {
                if(e->getWaterCornerActivated() && e->getClusterWaterFilm()->getOutlet())
                {
                    double filmVolume=min(rSquared*e->getFilmAreaCoefficient()*e->getLength(),(1-4*pi()*e->getShapeFactor())*e->getVolume());
                    double filmConductance=rSquared*filmVolume/e->getLength()/(waterViscosity*e->getLength());
                    e->setWaterFilmVolume(filmVolume);
                    e->setWaterFilmConductivity(filmConductance);//cout<<filmConductance<<endl;
                    e->setEffectiveVolume(e->getVolume()-e->getWaterFilmVolume());
                    if(e->getWaterFilmVolume()>e->getVolume()){cout<<"FATAL EROOR in PD: water film > capillary volume."<<endl;simulationInterrupted=true;}
                    waterVolume+=e->getWaterFilmVolume();
                }
            }
            if(e->getPhaseFlag()==phase::water)
                waterVolume+=e->getVolume();
        }
        currentWaterVolume=waterVolume;

        //Extract relative permeability results
        if(relativePermeabilitiesCalculation)
            if(currentWaterVolume/totalNetworkVolume<outputWaterSaturation+1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                outputWaterSaturation-=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalNetworkVolume>0.001)
            file<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalNetworkVolume)*100;
        simulationNotification = ss.str();
        emitUpdateNotificationSignal();

        //Extract data at Breakthrough
        if(!spanningOil)
        {
            if (isOilSpanning)
            {
                spanningOil=true;
                criticalPcPD=currentPc;
                criticalSaturationPD=abs(currentWaterVolume/totalNetworkVolume);
                file3<<"Pc at Breakthrough: "<<criticalPcPD<<endl;
                file3<<"Sw at Breakthrough: "<<criticalSaturationPD<<endl;
            }
        }

        finalPcPD=currentPc;
        finalSaturationPD=abs(currentWaterVolume/totalNetworkVolume);

        if(finalSaturation!=0 && currentWaterVolume/totalNetworkVolume<finalSaturation)
            break;

        //Thread Management
        if(simulationInterrupted)break;
    }

    file3<<"Pc after PD: "<<finalPcPD<<endl;
    file3<<"Sw after PD: "<<finalSaturationPD<<endl;
}

void network::spontaneousImbibition()
{
    cout<<"Starting Spontaneous Imbibition... "<<endl;

    //prepare result files
    ofstream file("Results/2-spontaneousImbibitionPcCurve.txt");
    ofstream file2("Results/2-spontaneousImbibitionRelativePermeabilies.txt");
    ofstream file3("Results/2-spontaneousImbibitionCriticalValues.txt");

    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double waterSaturation=getWaterSaturationWithFilms();
    double currentWaterVolume(waterSaturation*totalNetworkVolume);

    //Calculate the incremental reduction of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:networkRange<element*>(this))
    {
        double pc=OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(abs(pc)<minPc)
        {
            minPc=abs(pc);
        }

        pc=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
    unordered_set<element*> elementsToInvade;
    for(element* e:networkRange<element*>(this))
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
                    if(n->getPhaseFlag()==phase::water && n->getClusterWaterFilm()->getInlet())
                    {connectedToInletWaterCluster=true;break;}

                //throat
                if(e->getType()==capillaryType::throat && (e->getInlet() || connectedToInletWaterCluster) && e->getClusterOilFilm()->getOutlet())
                {
                    double entryPressure=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
                        if(n->getPhaseFlag()==phase::oil)
                            oilNeighboorsNumber++;
                        totalNeighboorsNumber++;
                    }

                   double entryPressureBodyFilling=0;
                   if(oilNeighboorsNumber==1)
                       entryPressureBodyFilling=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                   if(oilNeighboorsNumber>1)
                       entryPressureBodyFilling=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius()/double(oilNeighboorsNumber);

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
            if(simulationInterrupted)break;
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
        for(element* e: networkRange<element*>(this))
        {
            double rSquared=pow(OWSurfaceTension/currentPc,2);
            if(e->getPhaseFlag()==phase::oil && e->getWettabilityFlag()==wettability::waterWet)
            {
                if(e->getWaterCornerActivated() && e->getClusterWaterFilm()->getInlet() && e->getClusterOilFilm()->getOutlet())
                {
                    double filmVolume=min(rSquared*e->getFilmAreaCoefficient()*e->getLength(),(1-4*pi()*e->getShapeFactor())*e->getVolume());
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
            if(currentWaterVolume/totalNetworkVolume>waterSaturation-1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation+=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalNetworkVolume>0.001)
            file<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalNetworkVolume)*100;
        simulationNotification = ss.str();
        emitUpdateNotificationSignal();

        finalPcPI=currentPc;
        finalSaturationPI=abs(currentWaterVolume/totalNetworkVolume);

        //Thread Management
        if(simulationInterrupted)break;
    }

    file3<<"Pc after SI: "<<finalPcPI<<endl;
    file3<<"Sw after SI: "<<finalSaturationPI<<endl;
}

void network::forcedWaterInjection()
{
    cout<<"Starting Forced Water Injection ... "<<endl;

    //prepare results files
    ofstream file("Results/3-forcedWaterInjectionPcCurve.txt");
    ofstream file2("Results/3-forcedWaterInjectionRelativePermeabilies.txt");
    ofstream file3("Results/3-forcedWaterInjectionCriticalValues.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double waterSaturation=getWaterSaturationWithFilms();
    double currentWaterVolume=waterSaturation*totalNetworkVolume;

    //Calculate the incremental reduction of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:networkRange<element*>(this))
    {
        double pc=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
    unordered_set<element*> elementsToInvade;
    for(element* e: networkRange<element*>(this))
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
                    if(n->getPhaseFlag()==phase::water && n->getClusterWaterFilm()->getInlet())
                    {connectedToInletWaterCluster=true;break;}

                if((e->getType()==capillaryType::throat && (e->getInlet() || connectedToInletWaterCluster)) || (e->getType()==capillaryType::poreBody && connectedToInletWaterCluster))
                {
                    if(e->getClusterOilFilm()->getOutlet())
                    {
                        double entryPressure=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
            if(simulationInterrupted)break;
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
        for(element* e: networkRange<element*>(this))
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
                    double filmVolume=min(rSquared*e->getFilmAreaCoefficient()*e->getLength(),(1-4*pi()*e->getShapeFactor())*e->getVolume());
                    double effectiveOilFilmVolume=max(0.0,filmVolume-e->getWaterFilmVolume());
                    double filmConductance=rSquared*effectiveOilFilmVolume/e->getLength()/(oilViscosity*e->getLength());

                    e->setOilFilmVolume(effectiveOilFilmVolume);
                    e->setOilFilmConductivity(filmConductance);
                    //if(effectiveOilFilmVolume==0) // oil layer collapse
                    //    e->setOilLayerActivated(false);
                    e->setEffectiveVolume(e->getVolume()-e->getOilFilmVolume()-e->getWaterFilmVolume());
                    if(e->getEffectiveVolume()<0){simulationInterrupted=true;cout<<"effective volume < 0"<<endl;}
                }
                waterVolume+=e->getEffectiveVolume()+e->getWaterFilmVolume();
            }
            if(e->getPhaseFlag()==phase::water && e->getWettabilityFlag()==wettability::waterWet)
                waterVolume+=e->getVolume();
        }
        currentWaterVolume=waterVolume;

        //Extract relative permeability results
        if(relativePermeabilitiesCalculation)
            if(currentWaterVolume/totalNetworkVolume>waterSaturation-1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation+=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalNetworkVolume>0.001)
            file<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalNetworkVolume)*100;
        simulationNotification = ss.str();
        emitUpdateNotificationSignal();

        finalPcSD=currentPc;
        finalSaturationSD=abs(currentWaterVolume/totalNetworkVolume);

        //Thread Management
        if(simulationInterrupted)break;
    }

    file3<<"Pc after FWI: "<<finalPcSD<<endl;
    file3<<"Sw after FWI: "<<finalSaturationSD<<endl;
}

void network::spontaneousOilInvasion()
{
    cout<<"Starting Spontaneous Oil Invasion ... "<<endl;

    //prepare results files
    ofstream file("Results/4-spontaneousOilnvasionPcCurve.txt");
    ofstream file2("Results/4-spontaneousOilnvasionRelativePermeabilies.txt");
    ofstream file3("Results/4-spontaneousOilnvasionCriticalValues.txt");

    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double waterSaturation=getWaterSaturationWithFilms();
    double currentWaterVolume=waterSaturation*totalNetworkVolume;

    //Calculate the incremental increase of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:networkRange<element*>(this))
    {
        double pc=OWSurfaceTension*cos(e->getTheta())/e->getRadius();
        if(abs(pc)<minPc)
        {
            minPc=abs(pc);
        }

        pc=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
    unordered_set<element*> elementsToInvade;
    for(element* e:networkRange<element*>(this))
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
                    if(n->getPhaseFlag()==phase::oil && n->getClusterOilFilm()->getInlet())
                    {connectedToInletOilCluster=true;break;}

                //throat
                if(e->getType()==capillaryType::throat && (e->getInlet() || connectedToInletOilCluster) && e->getClusterWaterFilm()->getOutlet())
                {
                    double entryPressure=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
                        if(n->getPhaseFlag()==phase::water)
                            waterNeighboorsNumber++;
                        totalNeighboorsNumber++;
                    }

                   double entryPressureBodyFilling=0;
                   if(waterNeighboorsNumber==1)
                       entryPressureBodyFilling=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                   if(waterNeighboorsNumber>1)
                       entryPressureBodyFilling=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius()/double(waterNeighboorsNumber);

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
            if(simulationInterrupted)break;
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
        for(element* e: networkRange<element*>(this))
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
                    double filmVolume=min(rSquared*e->getFilmAreaCoefficient()*e->getLength(),(1-4*pi()*e->getShapeFactor())*e->getVolume()-e->getWaterFilmVolume());
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
            if(currentWaterVolume/totalNetworkVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalNetworkVolume>0.001)
            file<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalNetworkVolume)*100;
        simulationNotification = ss.str();
        emitUpdateNotificationSignal();

        finalPcSI=currentPc;
        finalSaturationSI=abs(currentWaterVolume/totalNetworkVolume);

        //Thread Management
        if(simulationInterrupted)break;
    }

    file3<<"Pc after SOI: "<<finalPcSI<<endl;
    file3<<"Sw after SOI: "<<finalSaturationSI<<endl;
}

void network::secondaryOilDrainage()
{
    cout<<"Starting Secondary Oil Drainage... "<<endl;

    //prepare results files
    ofstream file("Results/5-secondaryOilDrainagePcCurve.txt");
    ofstream file2("Results/5-secondaryOilDrainageRelativePermeabilies.txt");
    ofstream file3("Results/5-secondaryOilDrainageCriticalValues.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    //Initialise variables
    double waterSaturation=getWaterSaturationWithFilms();
    double currentWaterVolume=waterSaturation*totalNetworkVolume;

    //Calculate the incremental reduction of capillary pressure
    double minPc(1e20),maxPc(0);
    for(element* e:networkRange<element*>(this))
    {
        double pc=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
    unordered_set<element*> elementsToInvade;
    for(element* e: networkRange<element*>(this))
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
                    if(n->getPhaseFlag()==phase::oil && n->getClusterOilFilm()->getInlet())
                    {connectedToInletOilCluster=true;break;}

                if((e->getType()==capillaryType::throat && (e->getInlet() || connectedToInletOilCluster)) || (e->getType()==capillaryType::poreBody && connectedToInletOilCluster))
                {
                    if(e->getClusterWaterFilm()->getOutlet())//change
                    {
                        double entryPressure=e->getEntryPressureCoefficient()*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
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
            if(simulationInterrupted)break;

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
        for(element* e: networkRange<element*>(this))
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
            if(currentWaterVolume/totalNetworkVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilities();
                file2<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.01;
            }

        //Extract capillary pressure results
        if(currentWaterVolume/totalNetworkVolume>0.001)
            file<<abs(currentWaterVolume/totalNetworkVolume)<<" "<<PaToPsi(currentPc)<<endl;

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "PC(psi): " << PaToPsi(currentPc)<<" / Sw(%): "<<abs(currentWaterVolume/totalNetworkVolume)*100;
        simulationNotification = ss.str();
        emitUpdateNotificationSignal();

        finalPcTD=currentPc;
        finalSaturationTD=abs(currentWaterVolume/totalNetworkVolume);

        //Thread Management
        if(simulationInterrupted)break;
    }

    file3<<"Pc after SOD: "<<finalPcTD<<endl;
    file3<<"Sw after SOD: "<<finalSaturationTD<<endl;
}

}
