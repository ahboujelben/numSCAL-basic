/////////////////////////////////////////////////////////////////////////////
/// Name:        twoPhaseFlowPT.cpp
/// Purpose:     methods related to steady-state two-phase flow
///              in pore/throat networks are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

void network::runTwoPhaseSSModelPT()
{
    cout<<"Starting two Phase SS Simulation... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //post-processing
    if(videoRecording)
        record=true;

    initializeTwoPhaseSimulationPT();

    assignWWWettabilityPT();

    if(primaryDrainageSimulation && !cancel)
        primaryDrainagePT();

    restoreWettabilityPT();

    if(primaryImbibitionSimulation && !cancel)
        primaryImbibitionPT();

    if(secondaryDrainageSimulation && !cancel)
        secondaryDrainagePT();

    if(secondaryImbibitionSimulation && !cancel)
        secondaryImbibitionPT();

    if(tertiaryDrainageSimulation && !cancel)
        tertiaryDrainagePT();

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

    fillWithPhasePT('w');

    restoreWettabilityPT();
}

void network::primaryDrainagePT(double finalSaturation)

{
    cout<<"Starting Primary Drainage... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare results
    ofstream file("Results/primaryDrainagePcCurve.txt");
    ofstream file2("Results/primaryDrainageRelativePermeabilies.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    double oilVolume=getVolumePT('o');
    double waterSaturation=(1-oilVolume/totalElementsVolume);
    bool spanningOil=false;

    double maxPc=0;
    for(int i=0;i<totalElements;++i)
    {
        element* e=getElement(i);
        if(!e->getClosed())
        {
            double pc=abs((1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius());
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalElements;++i)
    {
        element* e=getElement(i);
        if(!e->getClosed())
        {
            double pc=abs((1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius());
            if(pc<minPc)minPc=pc;
        }
    }

    double effectiveMinRadius=2*OWSurfaceTension/maxPc;
    double effectiveMaxRadius=2*OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMaxRadius;
    double currentPc;

    set<element*> accessibleElements;

    clusterWaterElements();
    setWaterTrappedPT(1);

    for(int i=0;i<totalElements;++i)
    {
        element* e=getElement(i);
        if(e->getPhaseFlag()=='w' && !e->getWaterTrapped())
            accessibleElements.insert(e);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        loop++;
        currentRadius-=radiusStep;
        currentPc=2*OWSurfaceTension/currentRadius;

        bool stillMore=true;
        while(stillMore)
        {
            stillMore=false;
            clusterOilElements();

            vector<element*> invadedElements;
            for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
            {
                element* e=*it;
                double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                bool connectedToInletOilCluster=false;
                if(currentPc+1e-5>=entryPressure && !e->getWaterTrapped())
                {
                    for(unsigned i=0;i<e->getNeighs().size();++i)
                    {
                        element* n=e->getNeighs()[i];
                        if(!n->getClosed() && n->getPhaseFlag()=='o')
                            if(n->getClusterOil()->getInlet())
                            {
                                connectedToInletOilCluster=true;
                                break;
                            }
                    }
                    if(connectedToInletOilCluster || (e->getType()==1 && e->getInlet()))
                        invadedElements.push_back(e);
                }
            }


            for(unsigned it=0;it<invadedElements.size();++it)
            {
                element* e=invadedElements[it];
                e->setPhaseFlag('o');
                oilVolume+=e->getVolume();
                accessibleElements.erase(e);
                stillMore=true;
            }

            clusterWaterElements();
            setWaterTrappedPT(1);

            //Update Graphics
            emit plot();

            if(finalSaturation!=0 && 1-oilVolume/totalElementsVolume<finalSaturation)
                break;

            //Thread Management
            if(cancel)break;
        }

        clusterWaterElements();
        setWaterTrappedPT(1);
        vector<element*> trappedElements;
        for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
        {
            element* e=*it;
            if(e->getWaterTrapped())
                trappedElements.push_back(e);
        }

        for(unsigned it=0;it<trappedElements.size();++it)
        {
            element* e=trappedElements[it];
            accessibleElements.erase(e);
        }

        if(relativePermeabilitiesCalculation)
            if(1-oilVolume/totalElementsVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(1-oilVolume/totalElementsVolume>0.001)
                    file2<<abs(1-oilVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.05;
            }

        if(1-oilVolume/totalElementsVolume>0.001)
            file<<abs(1-oilVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;


        if(!spanningOil)
        {
            clusterOilElements();
            if (isOilSpanning)
            {
                spanningOil=true;
                criticalPcPD=currentPc;
                criticalSaturationPD=abs(1-oilVolume/totalElementsVolume);
            }
        }

        finalPcPD=currentPc;
        finalSaturationPD=abs(1-oilVolume/totalElementsVolume);

        if(finalSaturation!=0 && 1-oilVolume/totalElementsVolume<finalSaturation)
            break;

        //Thread Management
        if(cancel)break;
    }
    endTime=tools::getCPUTime();
    cout<<"Primary Drainage Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::primaryImbibitionPT()
{
    cout<<"Starting Primary Imbibition... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare results
    ofstream file("Results/primaryImbibitionPcCurve.txt");
    ofstream file2("Results/primaryImbibitionRelativePermeabilies.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    double waterVolume=getVolumePT('w');
    double waterSaturation=waterVolume/totalElementsVolume;
    bool spanningWater=false;

    double maxPc=0;
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed() && p->getPhaseFlag()=='o' && p->getWettabilityFlag()=='w')
        {
            double pc=(1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius();
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed() && p->getPhaseFlag()=='o' && p->getWettabilityFlag()=='w')
        {
            double pc=OWSurfaceTension*cos(p->getTheta())/p->getRadius();
            if(pc<minPc)minPc=pc;
        }
    }

    double effectiveMinRadius=OWSurfaceTension/maxPc;
    double effectiveMaxRadius=OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMinRadius;
    double currentPc;

    set<element*> accessibleElements;

    clusterOilElements();
    setOilTrappedPT(1);

    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed())
            if(p->getPhaseFlag()=='o' && p->getWettabilityFlag()=='w' && !p->getOilTrapped())
                accessibleElements.insert(p);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        loop++;
        currentRadius+=radiusStep;
        currentPc=OWSurfaceTension/currentRadius;

        vector<element*> invadedElements;

        //snap off
        for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
        {
            element* p=*it;
            double entryPressure=OWSurfaceTension*cos(p->getTheta())/p->getRadius();
            if(currentPc-1e-5<=entryPressure && p->getClusterWaterWet()->getInlet())
                invadedElements.push_back(p);
        }

        for(unsigned it=0;it<invadedElements.size();++it)
        {
            element* e=invadedElements[it];
            e->setPhaseFlag('w');
            waterVolume+=e->getVolume();
            accessibleElements.erase(e);
        }

        //Update Graphics
        emit plot();



        //bulk Imbibition
        if(true)//if(!snapOffOnly)
        {
            bool stillMore=true;
            while(stillMore)
            {
                stillMore=false;
                clusterWaterElements();

                vector<element*> invadedElements;
                for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
                {
                    element* e=*it;
                    double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                    bool connectedToInletWaterCluster=false;
                    if(currentPc-1e-5<=entryPressure && !e->getOilTrapped())
                    {
                        int oilNeighboorsNumber(0);
                        int totalNeighboorsNumber(0);
                        for(unsigned i=0;i<e->getNeighs().size();++i)
                        {
                            element* n=e->getNeighs()[i];
                            if(!n->getClosed() && n->getPhaseFlag()=='w')
                                if(n->getClusterWater()->getInlet())
                                    connectedToInletWaterCluster=true; 

                            if(!n->getClosed() && n->getPhaseFlag()=='o')
                                oilNeighboorsNumber++;

                            if(!n->getClosed())
                                totalNeighboorsNumber++;
                        }

                        //pore
                        if(e->getType()==1)
                            if(connectedToInletWaterCluster || e->getInlet())
                                invadedElements.push_back(e);

                        //node : pore body filling
                        if(e->getType()==0)
                            if(connectedToInletWaterCluster)
                            {
                               double entryPressureBodyFilling=entryPressure;
                               if(oilNeighboorsNumber>1)
                                   entryPressureBodyFilling=OWSurfaceTension*cos(e->getTheta())/e->getRadius()*(2-(double(oilNeighboorsNumber)/double(totalNeighboorsNumber-1)));

                                if(currentPc-1e-5<=entryPressureBodyFilling)
                                    invadedElements.push_back(e);
                            }
                    }
                }

                for(unsigned it=0;it<invadedElements.size();++it)
                {
                    element* p=invadedElements[it];
                    p->setPhaseFlag('w');
                    waterVolume+=p->getVolume();
                    accessibleElements.erase(p);
                    stillMore=true;
                }

                clusterOilElements();
                setOilTrappedPT(1);

                //Update Graphics
                emit plot();

                //Thread Management
                if(cancel)break;
            }
        }

        clusterOilElements();
        setOilTrappedPT(1);

        vector<element*> trappedElements;
        for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
        {
            element* p=*it;
            if(p->getOilTrapped())
                trappedElements.push_back(p);
        }

        for(unsigned it=0;it<trappedElements.size();++it)
        {
            element* p=trappedElements[it];
            accessibleElements.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(waterVolume/totalElementsVolume>waterSaturation-1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(waterVolume/totalElementsVolume<1)
                    file2<<abs(waterVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation+=0.05;
            }

        if(waterVolume/totalElementsVolume>0.001)
            file<<abs(waterVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningWater)
        {
            clusterWaterElements();
            if (isWaterSpanning)
            {
                spanningWater=true;
                criticalPcPI=currentPc;
                criticalSaturationPI=abs(waterVolume/totalElementsVolume);
            }
        }

        finalPcPI=currentPc;
        finalSaturationPI=abs(waterVolume/totalElementsVolume);

        //Thread Management
        if(cancel)break;
    }

    endTime=tools::getCPUTime();
    cout<<"Primary Imbibition Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::secondaryDrainagePT()
{
    cout<<"Starting Secondary Drainage... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare results
    ofstream file("Results/secondaryDrainagePcCurve.txt");
    ofstream file2("Results/secondaryDrainageRelativePermeabilies.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    double waterVolume=getVolumePT('w');
    double waterSaturation=waterVolume/totalElementsVolume;
    bool spanningWater=false;

    double maxPc=0;
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed() && p->getPhaseFlag()=='o' && p->getWettabilityFlag()=='o')
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed() && p->getPhaseFlag()=='o' && p->getWettabilityFlag()=='o')
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc<minPc)minPc=pc;
        }
    }

    double effectiveMinRadius=2*OWSurfaceTension/maxPc;
    double effectiveMaxRadius=2*OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMaxRadius;
    double currentPc;

    set<element*> accessibleElements;

    clusterOilElements();
    setOilTrappedPT(1);

    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed())
            if(p->getPhaseFlag()=='o' && !p->getOilTrapped())
                accessibleElements.insert(p);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        loop++;
        currentRadius-=radiusStep;
        currentPc=-2*OWSurfaceTension/currentRadius;

        bool stillMore=true;
        while(stillMore)
        {
            stillMore=false;
            clusterWaterElements();

            vector<element*> invadedElements;
            for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
            {
                element* e=*it;
                double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                bool connectedToInletWaterCluster=false;
                if(currentPc-1e-5<=entryPressure && !e->getOilTrapped())
                {
                    for(unsigned i=0;i<e->getNeighs().size();++i)
                    {
                        element* n=e->getNeighs()[i];
                        if(!n->getClosed() && n->getPhaseFlag()=='w')
                            if(n->getClusterWater()->getInlet())
                            {
                                connectedToInletWaterCluster=true;
                                break;
                            }
                    }
                    if(connectedToInletWaterCluster || (e->getType()==1 && e->getInlet()))
                        invadedElements.push_back(e);
                }
            }

            for(unsigned it=0;it<invadedElements.size();++it)
            {
                element* e=invadedElements[it];
                e->setPhaseFlag('w');
                waterVolume+=e->getVolume();
                accessibleElements.erase(e);
                stillMore=true;
            }

            clusterOilElements();
            setOilTrappedPT(1);

            //Update Graphics
            emit plot();

            //Thread Management
            if(cancel)break;
        }

        clusterOilElements();
        setOilTrappedPT(1);

        vector<element*> trappedElements;
        for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
        {
            element* p=*it;
            if(p->getOilTrapped())
                trappedElements.push_back(p);
        }

        for(unsigned it=0;it<trappedElements.size();++it)
        {
            element* p=trappedElements[it];
            accessibleElements.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(waterVolume/totalElementsVolume>waterSaturation-1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(waterVolume/totalElementsVolume<1)
                    file2<<abs(waterVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation+=0.05;
            }

        if(waterVolume/totalElementsVolume>0.001)
            file<<abs(waterVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningWater)
        {
            clusterWaterElements();
            if (isWaterSpanning)
            {
                spanningWater=true;
                criticalPcSD=currentPc;
                criticalSaturationSD=abs(waterVolume/totalElementsVolume);
            }
        }

        finalPcSD=currentPc;
        finalSaturationSD=abs(waterVolume/totalElementsVolume);

        //Thread Management
        if(cancel)break;
    }

    endTime=tools::getCPUTime();
    cout<<"Secondary Drainage Time: "<<endTime-startTime<<" s"<<endl;
}

void network::secondaryImbibitionPT()
{
    cout<<"Starting Secondary Imbibition... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare results
    ofstream file("Results/secondaryImbibitionPcCurve.txt");
    ofstream file2("Results/secondaryImbibitionRelativePermeabilies.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    double oilVolume=getVolumePT('o');
    double waterSaturation=1-oilVolume/totalElementsVolume;
    bool spanningOil=false;

    double maxPc=0;
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed() && p->getPhaseFlag()=='w' && p->getWettabilityFlag()=='o')
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed() && p->getPhaseFlag()=='w' && p->getWettabilityFlag()=='o')
        {
            double pc=abs(OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc<minPc)minPc=pc;
        }
    }

    double effectiveMinRadius=OWSurfaceTension/maxPc;
    double effectiveMaxRadius=OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMinRadius;
    double currentPc;

    set<element*> accessibleElements;

    clusterWaterElements();
    setWaterTrappedPT(1);

    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed())
            if(p->getPhaseFlag()=='w' && p->getWettabilityFlag()=='o' && !p->getWaterTrapped())
                accessibleElements.insert(p);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        loop++;
        currentRadius+=radiusStep;
        currentPc=-OWSurfaceTension/currentRadius;

        vector<element*> invadedElements;

        //snap off
        for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
        {
            element* p=*it;
            double entryPressure=OWSurfaceTension*cos(p->getTheta())/p->getRadius();
            if(currentPc+1e-5>=entryPressure && p->getClusterOilWet()->getInlet())
                invadedElements.push_back(p);
        }

        for(unsigned it=0;it<invadedElements.size();++it)
        {
            element* p=invadedElements[it];
            p->setPhaseFlag('o');
            oilVolume+=p->getVolume();
            accessibleElements.erase(p);
        }

        //Update Graphics
        emit plot();


        //bulk Imbibition
        if(true)//if(!snapOffOnly)
        {
            bool stillMore=true;
            while(stillMore)
            {
                stillMore=false;
                clusterOilElements();

                vector<element*> invadedElements;
                for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
                {
                    element* e=*it;
                    double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                    bool connectedToInletOilCluster=false;
                    if(currentPc+1e-5>=entryPressure && !e->getWaterTrapped())
                    {
                        int waterNeighboorsNumber(0);
                        int totalNeighboorsNumber(0);
                        for(unsigned i=0;i<e->getNeighs().size();++i)
                        {
                            element* n=e->getNeighs()[i];
                            if(!n->getClosed() && n->getPhaseFlag()=='o' && n->getClusterOil()->getInlet())
                                connectedToInletOilCluster=true;

                            if(!n->getClosed() && n->getPhaseFlag()=='w')
                                waterNeighboorsNumber++;

                            if(!n->getClosed())
                                totalNeighboorsNumber++;
                        }

                        //pore
                        if(e->getType()==1)
                        if(connectedToInletOilCluster || e->getInlet())
                            invadedElements.push_back(e);

                        //node : pore body filling
                        if(e->getType()==0)
                            if(connectedToInletOilCluster)
                            {
                                double entryPressureBodyFilling=entryPressure;
                                if(waterNeighboorsNumber>1)
                                    entryPressureBodyFilling=OWSurfaceTension*cos(e->getTheta())/e->getRadius()*(2-(double(waterNeighboorsNumber)/double(totalNeighboorsNumber-1)));

                                if(currentPc+1e-5>=entryPressureBodyFilling)
                                    invadedElements.push_back(e);
                            }

                    }
                }

                for(unsigned it=0;it<invadedElements.size();++it)
                {
                    element* e=invadedElements[it];
                    e->setPhaseFlag('o');
                    oilVolume+=e->getVolume();
                    accessibleElements.erase(e);
                    stillMore=true;
                }

                clusterWaterElements();
                setWaterTrappedPT(1);

                //Update Graphics
                emit plot();

                //Thread Management
                if(cancel)break;
            }
        }

        clusterWaterElements();
        setWaterTrappedPT(1);

        vector<element*> trappedElements;
        for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
        {
            element* p=*it;
            if(p->getWaterTrapped())
                trappedElements.push_back(p);
        }

        for(unsigned it=0;it<trappedElements.size();++it)
        {
            element* p=trappedElements[it];
            accessibleElements.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(1-oilVolume/totalElementsVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(1-oilVolume/totalElementsVolume<1)
                    file2<<abs(1-oilVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.05;
            }

        if(1-oilVolume/totalElementsVolume>0.001)
            file<<abs(1-oilVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningOil)
        {
            clusterOilElements();
            if (isOilSpanning)
            {
                spanningOil=true;
                criticalPcSI=currentPc;
                criticalSaturationSI=abs(1-oilVolume/totalElementsVolume);
            }
        }

        finalPcSI=currentPc;
        finalSaturationSI=abs(1-oilVolume/totalElementsVolume);

        //Thread Management
        if(cancel)break;
    }

    endTime=tools::getCPUTime();
    cout<<"Secondary Imbibition Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::tertiaryDrainagePT()
{
    cout<<"Starting Tertiary Drainage... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //prepare results
    ofstream file("Results/tertiaryDrainagePcCurve.txt");
    ofstream file2("Results/tertiaryDrainageRelativePermeabilies.txt");
    file<<"Sw Pc"<<endl;
    if(relativePermeabilitiesCalculation)
        file2<<"Sw Krw Kro"<<endl;

    double oilVolume=getVolumePT('o');
    double waterSaturation=1-oilVolume/totalElementsVolume;
    bool spanningOil=false;

    double maxPc=0;
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed() && p->getPhaseFlag()=='w' && p->getWettabilityFlag()=='w')
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed() && p->getPhaseFlag()=='w' && p->getWettabilityFlag()=='w')
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc<minPc)minPc=pc;
        }
    }

    double effectiveMinRadius=2*OWSurfaceTension/maxPc;
    double effectiveMaxRadius=2*OWSurfaceTension/minPc;
    double radiusStep=(effectiveMaxRadius-effectiveMinRadius)/twoPhaseSimulationSteps;
    double currentRadius=effectiveMaxRadius;
    double currentPc;

    set<element*> accessibleElements;

    clusterWaterElements();
    setWaterTrappedPT(1);

    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed())
            if(p->getPhaseFlag()=='w' && !p->getWaterTrapped())
                accessibleElements.insert(p);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        loop++;
        currentRadius-=radiusStep;
        currentPc=2*OWSurfaceTension/currentRadius;

        bool stillMore=true;
        while(stillMore)
        {
            stillMore=false;
            clusterOilElements();

            vector<element*> invadedElements;
            for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
            {
                element* e=*it;
                double entryPressure=(1+2*sqrt(tools::pi()*e->getShapeFactor()))*OWSurfaceTension*cos(e->getTheta())/e->getRadius();
                bool connectedToInletOilCluster=false;
                if(currentPc+1e-5>=entryPressure && !e->getWaterTrapped())
                {
                    for(unsigned i=0;i<e->getNeighs().size();++i)
                    {
                        element* n=e->getNeighs()[i];
                        if(!n->getClosed() && n->getPhaseFlag()=='o')
                            if(n->getClusterOil()->getInlet())
                            {
                                connectedToInletOilCluster=true;
                                break;
                            }
                    }
                    if(connectedToInletOilCluster || (e->getType()==1 && e->getInlet()))
                        invadedElements.push_back(e);
                }
            }

            for(unsigned it=0;it<invadedElements.size();++it)
            {
                element* e=invadedElements[it];
                e->setPhaseFlag('o');
                oilVolume+=e->getVolume();
                accessibleElements.erase(e);
                stillMore=true;
            }

            clusterWaterElements();
            setWaterTrappedPT(1);

            //Thread Management
            if(cancel)break;

            //Update Graphics
            emit plot();
        }

        clusterWaterElements();
        setWaterTrappedPT(1);

        vector<element*> trappedElements;
        for(set<element*>::iterator it = accessibleElements.begin(); it != accessibleElements.end(); ++it )
        {
            element* p=*it;
            if(p->getWaterTrapped())
                trappedElements.push_back(p);
        }

        for(unsigned it=0;it<trappedElements.size();++it)
        {
            element* p=trappedElements[it];
            accessibleElements.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(1-oilVolume/totalElementsVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(1-oilVolume/totalElementsVolume<1)
                    file2<<abs(1-oilVolume/totalElementsVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.05;
            }

        if(1-oilVolume/totalElementsVolume>0.001)
            file<<abs(1-oilVolume/totalElementsVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningOil)
        {
            clusterOilElements();
            if (isOilSpanning)
            {
                spanningOil=true;
                criticalPcTD=currentPc;
                criticalSaturationTD=abs(1-oilVolume/totalElementsVolume);
            }
        }

        finalPcTD=currentPc;
        finalSaturationTD=abs(1-oilVolume/totalElementsVolume);

        //Thread Management
        if(cancel)break;
    }

    endTime=tools::getCPUTime();
    cout<<"Tertiary Drainage Time: "<<endTime-startTime<<" s"<<endl;
}

void network::backupWettabilityPT()
{
    wettabiltyThetaBackup.clear();
    for(int i=0;i<totalElements;++i)
    {
        element* e=getElement(i);
        if(!e->getClosed())
            wettabiltyThetaBackup[e]=e->getTheta();
    }
}

void network::assignWWWettabilityPT()
{
    for(int i=0;i<totalElements;++i)
    {
        element* e=getElement(i);
        if(!e->getClosed())
        {
            //e->setTheta(0);
            e->setTheta(30.0/180.0*tools::pi());
            e->setWettabilityFlag('w');
        }
    }
    clusterWaterWetElements();
    clusterOilWetElements();
}

void network::restoreWettabilityPT()
{
    for(int i=0;i<totalElements;++i)
    {
        element* e=getElement(i);
        if(!e->getClosed())
        {
            e->setTheta(wettabiltyThetaBackup[e]);
            if(e->getTheta()<tools::pi()/2)e->setWettabilityFlag('w');
            else e->setWettabilityFlag('o');
        }
    }
    clusterWaterWetElements();
    clusterOilWetElements();
}

