/////////////////////////////////////////////////////////////////////////////
/// Name:        twoPhaseFlow.cpp
/// Purpose:     methods related to steady-state two-phase flow
///              in simple bond networks are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

void network::runTwoPhaseSSModel()
{
    cout<<"Starting two Phase SS Simulation... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //post-processing
    if(videoRecording)
        record=true;

    initializeTwoPhaseSimulation();

    assignWWWettability();

    if(primaryDrainageSimulation && !cancel)
        primaryDrainage();

    restoreWettability();

    if(primaryImbibitionSimulation && !cancel)
        primaryImbibition();

    if(secondaryDrainageSimulation && !cancel)
        secondaryDrainage();

    if(secondaryImbibitionSimulation && !cancel)
        secondaryImbibition();

    if(tertiaryDrainageSimulation && !cancel)
        tertiaryDrainage();

    //post-processing
    if(videoRecording)
    {
        record=false;
        extractVideo();
    }

    endTime=tools::getCPUTime();
    cout<<"Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::initializeTwoPhaseSimulation()
{
    cancel=false;

    assignWWWettability();

    fillWithPhase('w');

    restoreWettability();
}

void network::primaryDrainage(double finalSaturation)
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

    double oilVolume=getVolume('o');
    double waterSaturation=(1-oilVolume/totalPoresVolume);
    bool spanningOil=false;

    double maxPc=0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
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

    set<pore*> accessiblePores;

    clusterWaterPores();
    setWaterTrapped(1);

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(p->getPhaseFlag()=='w' && !p->getWaterTrapped())
                accessiblePores.insert(p);
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
            vector<pore*> invadedPores;
            for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
            {
                pore* p=*it;
                double entryPressure=(1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius();
                bool connectedToInletOilCluster=false;
                if(currentPc+1e-5>=entryPressure)
                {
                    for(unsigned i=0;i<p->getNeighboors().size();++i)
                    {
                        pore* n=p->getNeighboors()[i];
                        if(!n->getClosed() && n->getPhaseFlag()=='o')
                            if(n->getClusterOil()->getInlet())
                            {
                                connectedToInletOilCluster=true;
                                break;
                            }
                    }
                    if(connectedToInletOilCluster || p->getInlet())
                        invadedPores.push_back(p);//great opt here
                }
            }

            for(unsigned it=0;it<invadedPores.size();++it)
            {
                pore* p=invadedPores[it];
                p->setPhaseFlag('o');
                oilVolume+=p->getVolume();
                accessiblePores.erase(p);
                stillMore=true;
            }

            clusterOilPores();

            //Update Graphics
            emit plot();

            if(finalSaturation!=0 && 1-oilVolume/totalPoresVolume<finalSaturation)
                break;

            //Thread Management
            if(cancel)break;
        }

        clusterWaterPores();
        setWaterTrapped(1);

        vector<pore*> trappedPores;
        for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
        {
            pore* p=*it;
            if(p->getWaterTrapped())
                trappedPores.push_back(p);
        }

        for(unsigned it=0;it<trappedPores.size();++it)
        {
            pore* p=trappedPores[it];
            accessiblePores.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(1-oilVolume/totalPoresVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(1-oilVolume/totalPoresVolume>=0.00)
                    file2<<abs(1-oilVolume/totalPoresVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.05;
            }

        if(1-oilVolume/totalPoresVolume>0.001)
            file<<abs(1-oilVolume/totalPoresVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningOil)
        {
            clusterOilPores();
            if (isOilSpanning)
            {
                spanningOil=true;
                criticalPcPD=currentPc;
                criticalSaturationPD=abs(1-oilVolume/totalPoresVolume);
            }
        }
       // else cancel=true;

        finalPcPD=currentPc;
        finalSaturationPD=abs(1-oilVolume/totalPoresVolume);

        if(finalSaturation!=0 && 1-oilVolume/totalPoresVolume<finalSaturation)
            break;

        //Thread Management
        if(cancel)break;
    }
    endTime=tools::getCPUTime();
    cout<<"Primary Drainage Processing Time: "<<endTime-startTime<<" s"<<endl;
}


void network::primaryImbibition()
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

    double waterVolume=getVolume('w');
    double waterSaturation=waterVolume/totalPoresVolume;
    bool spanningWater=false;

    double maxPc=0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed() && p->getPhaseFlag()=='o' && p->getWettabilityFlag()=='w')
        {
            double pc=(1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius();
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
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

    set<pore*> accessiblePores;

    clusterOilPores();
    setOilTrapped(1);

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
            if(p->getPhaseFlag()=='o' && p->getWettabilityFlag()=='w' && !p->getOilTrapped())
                accessiblePores.insert(p);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        loop++;
        currentRadius+=radiusStep;
        currentPc=OWSurfaceTension/currentRadius;

        vector<pore*> invadedPores;

        //snap off
        for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
        {
            pore* p=*it;
            double entryPressure=OWSurfaceTension*cos(p->getTheta())/p->getRadius();
            if(currentPc-1e-5<=entryPressure && p->getClusterWaterWet()->getInlet())
                invadedPores.push_back(p);
        }

        for(unsigned it=0;it<invadedPores.size();++it)
        {
            pore* p=invadedPores[it];
            p->setPhaseFlag('w');
            waterVolume+=p->getVolume();
            accessiblePores.erase(p);
        }

        //Update Graphics
        emit plot();

        clusterWaterPores();

        //bulk Imbibition
        if(true)//if(!snapOffOnly)
        {
            bool stillMore=true;
            while(stillMore)
            {
                stillMore=false;
                vector<pore*> invadedPores;
                for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
                {
                    pore* p=*it;
                    double entryPressure=(1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius();
                    bool connectedToInletWaterCluster=false;
                    if(currentPc<=entryPressure)
                    {
                        for(unsigned i=0;i<p->getNeighboors().size();++i)
                        {
                            pore* n=p->getNeighboors()[i];
                            if(!n->getClosed() && n->getPhaseFlag()=='w')
                                if(n->getClusterWater()->getInlet())
                                {
                                    connectedToInletWaterCluster=true;
                                    break;
                                }
                        }
                        if(connectedToInletWaterCluster || p->getInlet())
                            invadedPores.push_back(p);
                    }
                }

                for(unsigned it=0;it<invadedPores.size();++it)
                {
                    pore* p=invadedPores[it];
                    p->setPhaseFlag('w');
                    waterVolume+=p->getVolume();
                    accessiblePores.erase(p);
                    stillMore=true;
                }

                clusterWaterPores();

                //Update Graphics
                emit plot();

                //Thread Management
                if(cancel)break;
            }
        }

        clusterOilPores();
        setOilTrapped(1);

        vector<pore*> trappedPores;
        for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
        {
            pore* p=*it;
            if(p->getOilTrapped())
                trappedPores.push_back(p);
        }

        for(unsigned it=0;it<trappedPores.size();++it)
        {
            pore* p=trappedPores[it];
            accessiblePores.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(waterVolume/totalPoresVolume>waterSaturation-1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(waterVolume/totalPoresVolume<1)
                    file2<<abs(waterVolume/totalPoresVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation+=0.05;
            }

        if(waterVolume/totalPoresVolume>0.001)
            file<<abs(waterVolume/totalPoresVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningWater)
        {
            clusterWaterPores();
            if (isWaterSpanning)
            {
                spanningWater=true;
                criticalPcPI=currentPc;
                criticalSaturationPI=abs(waterVolume/totalPoresVolume);
            }
        }

        finalPcPI=currentPc;
        finalSaturationPI=abs(waterVolume/totalPoresVolume);

        //Thread Management
        if(cancel)break;
    }

    endTime=tools::getCPUTime();
    cout<<"Primary Imbibition Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::secondaryDrainage()
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

    double waterVolume=getVolume('w');
    double waterSaturation=waterVolume/totalPoresVolume;
    bool spanningWater=false;

    double maxPc=0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed() && p->getPhaseFlag()=='o' && p->getWettabilityFlag()=='o')
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
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

    set<pore*> accessiblePores;

    clusterOilPores();
    setOilTrapped(1);

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
            if(p->getPhaseFlag()=='o' && !p->getOilTrapped())
                accessiblePores.insert(p);
    }

    clusterWaterPores();

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
            vector<pore*> invadedPores;
            for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
            {
                pore* p=*it;
                double entryPressure=(1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius();
                bool connectedToInletWaterCluster=false;
                if(currentPc-1e-5<=entryPressure)
                {
                    for(unsigned i=0;i<p->getNeighboors().size();++i)
                    {
                        pore* n=p->getNeighboors()[i];
                        if(!n->getClosed() && n->getPhaseFlag()=='w')
                            if(n->getClusterWater()->getInlet())
                            {
                                connectedToInletWaterCluster=true;
                                break;
                            }
                    }
                    if(connectedToInletWaterCluster || p->getInlet())
                        invadedPores.push_back(p);
                }
            }

            for(unsigned it=0;it<invadedPores.size();++it)
            {
                pore* p=invadedPores[it];
                p->setPhaseFlag('w');
                waterVolume+=p->getVolume();
                accessiblePores.erase(p);
                stillMore=true;
            }

            clusterWaterPores();

            //Update Graphics
            emit plot();

            //Thread Management
            if(cancel)break;
        }

        clusterOilPores();
        setOilTrapped(1);

        vector<pore*> trappedPores;
        for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
        {
            pore* p=*it;
            if(p->getOilTrapped())
                trappedPores.push_back(p);
        }

        for(unsigned it=0;it<trappedPores.size();++it)
        {
            pore* p=trappedPores[it];
            accessiblePores.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(waterVolume/totalPoresVolume>waterSaturation-1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(waterVolume/totalPoresVolume<1)
                    file2<<abs(waterVolume/totalPoresVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation+=0.05;
            }

        if(waterVolume/totalPoresVolume>0.001)
            file<<abs(waterVolume/totalPoresVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningWater)
        {
            clusterWaterPores();
            if (isWaterSpanning)
            {
                spanningWater=true;
                criticalPcSD=currentPc;
                criticalSaturationSD=abs(waterVolume/totalPoresVolume);
            }
        }

        finalPcSD=currentPc;
        finalSaturationSD=abs(waterVolume/totalPoresVolume);

        //Thread Management
        if(cancel)break;
    }

    endTime=tools::getCPUTime();
    cout<<"Secondary Drainage Time: "<<endTime-startTime<<" s"<<endl;
}

void network::secondaryImbibition()
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

    double oilVolume=getVolume('o');
    double waterSaturation=1-oilVolume/totalPoresVolume;
    bool spanningOil=false;

    double maxPc=0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed() && p->getPhaseFlag()=='w' && p->getWettabilityFlag()=='o')
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
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

    set<pore*> accessiblePores;

    clusterWaterPores();
    setWaterTrapped(1);

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
            if(p->getPhaseFlag()=='w' && p->getWettabilityFlag()=='o' && !p->getWaterTrapped())
                accessiblePores.insert(p);
    }

    int loop(0);
    while(loop<twoPhaseSimulationSteps)
    {
        loop++;
        currentRadius+=radiusStep;
        currentPc=-OWSurfaceTension/currentRadius;

        vector<pore*> invadedPores;

        //snap off
        for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
        {
            pore* p=*it;
            double entryPressure=OWSurfaceTension*cos(p->getTheta())/p->getRadius();
            if(currentPc+1e-5>=entryPressure && p->getClusterOilWet()->getInlet())
                invadedPores.push_back(p);
        }

        for(unsigned it=0;it<invadedPores.size();++it)
        {
            pore* p=invadedPores[it];
            p->setPhaseFlag('o');
            oilVolume+=p->getVolume();
            accessiblePores.erase(p);
        }

        //Update Graphics
        emit plot();

        clusterOilPores();

        //bulk Imbibition
        if(true)//if(!snapOffOnly)
        {
            bool stillMore=true;
            while(stillMore)
            {
                stillMore=false;
                vector<pore*> invadedPores;
                for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
                {
                    pore* p=*it;
                    double entryPressure=(1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius();
                    bool connectedToInletOilCluster=false;
                    if(currentPc>=entryPressure)
                    {
                        for(unsigned i=0;i<p->getNeighboors().size();++i)
                        {
                            pore* n=p->getNeighboors()[i];
                            if(!n->getClosed() && n->getPhaseFlag()=='o')
                                if(n->getClusterOil()->getInlet())
                                {
                                    connectedToInletOilCluster=true;
                                    break;
                                }
                        }
                        if(connectedToInletOilCluster || p->getInlet())
                            invadedPores.push_back(p);
                    }
                }

                for(unsigned it=0;it<invadedPores.size();++it)
                {
                    pore* p=invadedPores[it];
                    p->setPhaseFlag('o');
                    oilVolume+=p->getVolume();
                    accessiblePores.erase(p);
                    stillMore=true;
                }

                clusterOilPores();

                //Update Graphics
                emit plot();

                //Thread Management
                if(cancel)break;
            }
        }

        clusterWaterPores();
        setWaterTrapped(1);

        vector<pore*> trappedPores;
        for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
        {
            pore* p=*it;
            if(p->getWaterTrapped())
                trappedPores.push_back(p);
        }

        for(unsigned it=0;it<trappedPores.size();++it)
        {
            pore* p=trappedPores[it];
            accessiblePores.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(1-oilVolume/totalPoresVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(1-oilVolume/totalPoresVolume<1)
                    file2<<abs(1-oilVolume/totalPoresVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.05;
            }

        if(1-oilVolume/totalPoresVolume>0.001)
            file<<abs(1-oilVolume/totalPoresVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningOil)
        {
            clusterOilPores();
            if (isOilSpanning)
            {
                spanningOil=true;
                criticalPcSI=currentPc;
                criticalSaturationSI=abs(1-oilVolume/totalPoresVolume);
            }
        }

        finalPcSI=currentPc;
        finalSaturationSI=abs(1-oilVolume/totalPoresVolume);

        //Thread Management
        if(cancel)break;
    }

    endTime=tools::getCPUTime();
    cout<<"Secondary Imbibition Processing Time: "<<endTime-startTime<<" s"<<endl;
}

void network::tertiaryDrainage()
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

    double oilVolume=getVolume('o');
    double waterSaturation=1-oilVolume/totalPoresVolume;
    bool spanningOil=false;

    double maxPc=0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed() && p->getPhaseFlag()=='w' && p->getWettabilityFlag()=='w')
        {
            double pc=abs((1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
            if(pc>maxPc)maxPc=pc;
        }
    }

    double minPc=1e20;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
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

    set<pore*> accessiblePores;

    clusterWaterPores();
    setWaterTrapped(1);

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
            if(p->getPhaseFlag()=='w' && !p->getWaterTrapped())
                accessiblePores.insert(p);
    }

    clusterOilPores();

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
            vector<pore*> invadedPores;
            for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
            {
                pore* p=*it;
                double entryPressure=(1+2*sqrt(tools::pi()*p->getShapeFactor()))*OWSurfaceTension*cos(p->getTheta())/p->getRadius();
                bool connectedToInletOilCluster=false;
                if(currentPc+1e-5>=entryPressure)
                {
                    for(unsigned i=0;i<p->getNeighboors().size();++i)
                    {
                        pore* n=p->getNeighboors()[i];
                        if(!n->getClosed() && n->getPhaseFlag()=='o')
                            if(n->getClusterOil()->getInlet())
                            {
                                connectedToInletOilCluster=true;
                                break;
                            }
                    }
                    if(connectedToInletOilCluster || p->getInlet())
                        invadedPores.push_back(p);
                }
            }

            for(unsigned it=0;it<invadedPores.size();++it)
            {
                pore* p=invadedPores[it];
                p->setPhaseFlag('o');
                oilVolume+=p->getVolume();
                accessiblePores.erase(p);
                stillMore=true;
            }

            clusterOilPores();

            //Thread Management
            if(cancel)break;

            //Update Graphics
            emit plot();
        }

        clusterWaterPores();
        setWaterTrapped(1);

        vector<pore*> trappedPores;
        for(set<pore*>::iterator it = accessiblePores.begin(); it != accessiblePores.end(); ++it )
        {
            pore* p=*it;
            if(p->getWaterTrapped())
                trappedPores.push_back(p);
        }

        for(unsigned it=0;it<trappedPores.size();++it)
        {
            pore* p=trappedPores[it];
            accessiblePores.erase(p);
        }

        if(relativePermeabilitiesCalculation)
            if(1-oilVolume/totalPoresVolume<waterSaturation+1e-5)
            {
                calculateRelativePermeabilitiesOW();
                //if(1-oilVolume/totalPoresVolume<1)
                    file2<<abs(1-oilVolume/totalPoresVolume)<<" "<<waterRelativePermeability<<" "<<oilRelativePermeability<<endl;
                waterSaturation-=0.05;
            }

        if(1-oilVolume/totalPoresVolume>0.001)
            file<<abs(1-oilVolume/totalPoresVolume)<<" "<<tools::PaToPsi(currentPc)<<endl;

        if(!spanningOil)
        {
            clusterOilPores();
            if (isOilSpanning)
            {
                spanningOil=true;
                criticalPcTD=currentPc;
                criticalSaturationTD=abs(1-oilVolume/totalPoresVolume);
            }
        }

        finalPcTD=currentPc;
        finalSaturationTD=abs(1-oilVolume/totalPoresVolume);

        //Thread Management
        if(cancel)break;
    }

    endTime=tools::getCPUTime();
    cout<<"Tertiary Drainage Time: "<<endTime-startTime<<" s"<<endl;
}

void network::backupWettability()
{
    wettabiltyThetaBackup.clear();
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
            wettabiltyThetaBackup[p]=p->getTheta();
    }
}

void network::assignWWWettability()
{
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            //p->setTheta(0);
            p->setTheta(30.0/180.0*tools::pi());
            p->setWettabilityFlag('w');
        }
    }
    clusterWaterWetPores();
    clusterOilWetPores();
}

void network::restoreWettability()
{
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            p->setTheta(wettabiltyThetaBackup[p]);
            if(p->getTheta()<tools::pi()/2)p->setWettabilityFlag('w');
            else p->setWettabilityFlag('o');
        }
    }
    clusterWaterWetPores();
    clusterOilWetPores();
}


