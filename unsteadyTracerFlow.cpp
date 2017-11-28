/////////////////////////////////////////////////////////////////////////////
/// Name:        unsteadyTracerFlow.cpp
/// Purpose:     methods related to tracer flow simulation
///              in pore/throat networks are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

void network::runTracerFlowPT()
{
    cout<<"Starting Tracer Flow Model... "<<endl;

    double startTime,endTime;
    startTime=tools::getCPUTime();

    //post-processing
    if(videoRecording)
        record=true;

    initializeTwoPhaseSimulationPT();

    //initialise flags
    double timeSoFar(0);
    double injectedPVs(0);

    //Solve pressure in Oil
    clusterOilElements();
    if (isOilSpanning)
    {
        assignViscositiesWithMixedFluids();
        assignConductivities();
        for (pore* p :accessiblePores)
        {
            if(p->getPhaseFlag()=='w')
               p->setConductivity(1e-200);
            if((p->getPhaseFlag()=='o' && !p->getClusterOil()->getSpanning()) || (p->getNodeIn()!=0 && !p->getNodeIn()->getClosed() && p->getNodeIn()->getPhaseFlag()=='w') || (p->getNodeOut()!=0 && !p->getNodeOut()->getClosed() && p->getNodeOut()->getPhaseFlag()=='w'))
               p->setConductivity(1e-200);
        }

        //Link pressure gradient to flow rate (Aker,1998)
        double Q1(0),Q2(0),A,B;

        pressureIn=1;
        pressureOut=0;
        solvePressures();
        updateFlows();
        Q1=getOutletFlow();

        pressureIn=2;
        pressureOut=0;
        solvePressures();
        updateFlows();
        Q2=getOutletFlow();

        B=(Q1-Q2*1/2)/(1-1/2);
        A=(Q1-B);

        deltaP=flowRate/A-B/A;

        pressureIn=deltaP;
        pressureOut=0;
        solvePressures();
        updateFlows();

        //Set timestep
        timeStep=1e50;

        for(pore* p : accessiblePores)
        {
            if(p->getPhaseFlag()=='o' && p->getClusterOil()->getSpanning())
            {  
                //Diffusion
                double sumDiffusionSource=0;
                for(element*e : p->getNeighs())
                {
                    if(!e->getClosed() && e->getPhaseFlag()=='o')
                    {
                        double area=min(e->getVolume()/e->getLength(), p->getVolume()/p->getLength());
                        sumDiffusionSource+=tracerDiffusionCoef/area;
                    }
                }

                //Convection
                if((abs(p->getFlow())/p->getVolume()+sumDiffusionSource)>1e-20)
                {
                    double step=1./(abs(p->getFlow())/p->getVolume()+sumDiffusionSource);
                    if(step<timeStep)
                    {
                        timeStep=step;
                    }
                }

                if(p->getFlow()>1e-24 && p->getConductivity()!=1e-200)
                {
                    node* n=p->getNodeIn();
                    if(n!=0)
                    {
                        n->setFlow((n->getFlow()+abs(p->getFlow())));
                    }
                }
                if(p->getFlow()<-1e-24 && p->getConductivity()!=1e-200)
                {
                    node* n=p->getNodeOut();
                    if(n!=0)
                    {
                        n->setFlow((n->getFlow()+abs(p->getFlow())));
                    }
                }
            }
        }

        for(node* p : accessibleNodes)
        {
            if(p->getPhaseFlag()=='o' && p->getClusterOil()->getSpanning())
            {
                //Diffusion
                double sumDiffusionSource=0;
                for(element*e : p->getNeighs())
                {
                    if(!e->getClosed() && e->getPhaseFlag()=='o')
                    {
                        double area=min(e->getVolume()/e->getLength(), p->getVolume()/p->getLength());
                        sumDiffusionSource+=tracerDiffusionCoef/area;
                    }
                }

                //Convection
                if((abs(p->getFlow())/p->getVolume()+sumDiffusionSource)>1e-20)
                {
                    double step=1./(abs(p->getFlow())/p->getVolume()+sumDiffusionSource);
                    if(step<timeStep)
                    {
                        timeStep=step;
                    }
                }
            }
        }
    }
    else
    {
        cancel=true;
        cout<<"ERROR: Tracer will only flow in the oil. Oil is not spanning in the current configuration."<<endl;
    }

    while(!cancel && timeSoFar<simulationTime)
    {
        vector<double> newConcentration;

        for(node* n: accessibleNodes)
        {
            if(n->getPhaseFlag()=='o'  && n->getClusterOil()->getSpanning())
            {
                //Convection
                double massIn=0;
                for(unsigned j : n->getConnectedPores())
                {
                    pore* p=getPore(j-1);
                    if(!p->getClosed() && p->getPhaseFlag()=='o' && p->getConductivity()!=1e-200)
                    {
                        if((p->getNodeIn()==n && p->getFlow()>1e-24) || (p->getNodeOut()==n && p->getFlow()<-1e-24))
                        {
                            massIn+=p->getConcentration()*abs(p->getFlow());
                        }
                    }
                }
                n->setMassFlow(massIn);

                //Diffusion
                double sumDiffusionIn=0;
                double sumDiffusionOut=0;
                for(element*e : n->getNeighs())
                {
                    if(!e->getClosed() && e->getPhaseFlag()=='o')
                    {
                        double area=min(e->getVolume()/e->getLength(), n->getVolume()/n->getLength());
                        sumDiffusionIn+=e->getConcentration()*tracerDiffusionCoef/area;
                        sumDiffusionOut+=n->getConcentration()*tracerDiffusionCoef/area;
                    }
                }

                //Load new concentration in a temporary vector
                newConcentration.push_back(n->getConcentration()+(massIn-abs(n->getFlow())*n->getConcentration())*timeStep/n->getVolume()+sumDiffusionIn*timeStep-sumDiffusionOut*timeStep);
            }
        }

        for(pore* p : accessiblePores)
        { 
            if(p->getPhaseFlag()=='o'  && p->getClusterOil()->getSpanning())
            {
                double massIn=0;
                double flowIn=0;
                double sumDiffusionIn=0;
                double sumDiffusionOut=0;

                //Convection
                if(p->getInlet())
                {
                    if(abs(p->getFlow())>1e-24 && p->getConductivity()!=1e-200)
                    {
                        massIn=abs(p->getFlow());
                        flowIn=abs(p->getFlow());
                    }
                }
                else
                {
                    if(p->getFlow()>1e-24 && p->getConductivity()!=1e-200 && p->getNodeOut()->getPhaseFlag()=='o')
                    {
                        massIn=p->getNodeOut()->getMassFlow();
                        flowIn=p->getNodeOut()->getFlow();
                    }
                    if(p->getFlow()<1e-24 && p->getConductivity()!=1e-200 && p->getNodeIn()->getPhaseFlag()=='o')
                    {
                        massIn=p->getNodeIn()->getMassFlow();
                        flowIn=p->getNodeIn()->getFlow();
                    }
                }

                if(abs(p->getFlow())<1e-24 || flowIn<1e-24 || p->getConductivity()==1e-200)
                {
                    massIn=0;
                    flowIn=1;
                }

                //Diffusion
                for(element*e : p->getNeighs())
                {
                    if(!e->getClosed() && e->getPhaseFlag()=='o')
                    {
                        double area=min(e->getVolume()/e->getLength(), p->getVolume()/p->getLength());
                        sumDiffusionIn+=e->getConcentration()*tracerDiffusionCoef/area;
                        sumDiffusionOut+=p->getConcentration()*tracerDiffusionCoef/area;
                    }
                }

                //Load new concentration in a temporary vector
                newConcentration.push_back(p->getConcentration()+(abs(p->getFlow())/flowIn*massIn-abs(p->getFlow())*p->getConcentration())*timeStep/p->getVolume()+sumDiffusionIn*timeStep-sumDiffusionOut*timeStep);
            }
        }

        //Update concentrations
        unsigned index(0);
        for(node* n: accessibleNodes)
        {
            if(n->getPhaseFlag()=='o' && n->getClusterOil()->getSpanning())
            {
                n->setConcentration(newConcentration[index]);
                index++;
                if(n->getConcentration()<-0.00001 || n->getConcentration()>1.0001)
                {
                    cancel=true;
                    cout<<"ERROR: N Concentration out of range: "<< n->getConcentration()<<endl;
                }
            }
        }
        for(pore* p: accessiblePores)
        {
            if(p->getPhaseFlag()=='o' && p->getClusterOil()->getSpanning())
            {
                p->setConcentration(newConcentration[index]);
                index++;

                if(p->getConcentration()<-0.00001 || p->getConcentration()>1.00001)
                {
                    cancel=true;
                    cout<<"ERROR: P Concentration out of range: "<< p->getConcentration()<<endl;
                }
            }
        }

        if(timeStep!=1e50)
        {
            timeSoFar+=timeStep;
            double injectedVolume=timeStep*flowRate/totalElementsVolume;
            injectedPVs+=injectedVolume;
        }

        //Display notification
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "Injected PVs: " << injectedPVs;
        simulationNotification = ss.str();

        emitPlotSignal();

        //Thread Management
        if(cancel)break;
    }

    //post-processing
    if(videoRecording)
    {
        record=false;
        extractVideo();
    }

    cout<<"Simulation Time: "<<timeSoFar<<" s"<<endl;
    cout<<"Injected PVs: "<<injectedPVs<<endl;
    endTime=tools::getCPUTime();
    cout<<"Tracer Flow Time: "<<endTime-startTime<<" s"<<endl;
}

