/////////////////////////////////////////////////////////////////////////////
/// Name:        unsteadyTwoPhaseFlowFastPT.cpp
/// Purpose:     methods related to unsteady-state drainage simulation
///              in pore/throat networks are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"
#include "iterator.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace PNM {

void network::runUSSDrainageModel()
{
    cout<<"Starting Unsteady State Drainage Model... "<<endl;

    //post-processing
    if(videoRecording)
        record=true;

    initialiseUSSDrainageModel();
    initializeTwoPhaseOutputs();
    setInitialFlags();

    assignViscosities();
    assignConductivities();

    //initialise flags
    double timeSoFar(0);
    double injectedPVs(0);
    double injectThreshold(0.01);
    double outputPVs(0.0);
    int outputCount(0);
    bool solvePressure=true;
    bool waterSpanning=isWaterSpanning;
    deltaP=1;

    unordered_set<pore*> poresToCheck;
    unordered_set<node*> nodesToCheck;

    while(!cancel && timeSoFar<simulationTime)
    {
        //Update viscosities/conductivities/thetas and trapping conditions
        if(solvePressure)
        {
            setAdvancedTrapping();

            poresToCheck.clear();
            nodesToCheck.clear();
            updateCapillaryProperties(poresToCheck,nodesToCheck);

            //Solve pressure distribution and block counter imbibition flow
            solvePressureWithoutCounterImbibition();
            solvePressure=false;
        }

        //Calculate timestep
        calculateTimeStepUSS(poresToCheck,nodesToCheck,waterSpanning);

        //Update Fluid fractions
        updateElementaryFluidFractions(poresToCheck,nodesToCheck,solvePressure);

        //Update Fluid Flags
        updateElementaryFluidFlags(poresToCheck, nodesToCheck);

        if(timeStep!=1e50)
        {
            timeSoFar+=timeStep;
            double injectedVolume=timeStep*flowRate/totalNetworkVolume;
            injectedPVs+=injectedVolume;
            outputPVs+=injectedVolume;
        }

        // Write up data

        if(outputPVs>injectThreshold)
        {
            double waterSat=getWaterSaturation();
            outputTwoPhaseData(injectedPVs,outputCount, waterSat);
            outputPVs=0;

            //Display notification
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2);
            ss << "Injected PVs: " << injectedPVs<<" / Sw(%): "<<waterSat*100;
            simulationNotification = ss.str();
        }

        //Update simulation flags

        if(!waterSpanning)
        {
            clusterWaterElements();
            if(isWaterSpanning)
            {
                waterSpanning=true;
                cout<<"Water Breakthrough..."<<endl<<"Time at Water Breakthough: "<<timeSoFar<<endl<<"Injected PVs at Water Breakthough: "<<injectedPVs<<endl;
            }
        }

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
}

void network::initialiseUSSDrainageModel()
{
    cancel=false;
    if(waterDistribution!=4){ //not after primary drainage
        assignWWWettability();
        fillWithPhase(phase::water,initialWaterSaturation,waterDistribution,phase::oil);
    }
    else{ //after primary drainage
        initialiseTwoPhaseSSModel();
        primaryDrainage(initialWaterSaturation);
    }

    addWaterChannel();
    initialiseCapillaries();

    restoreWettability();

    if(overrideByInjectedPVs){
        simulationTime=totalNetworkVolume*injectedPVs/flowRate;
        cout<<"PVs to inject: "<<injectedPVs<<endl;
    }
}

void network::addWaterChannel()
{
    for_each(networkRange<pore*>(this).begin(), networkRange<pore*>(this).end(), [this](pore* p){
        if(p->getInlet() || p->getNodeIn()!=0 & p->getNodeOut()!=0 && p->getNodeIn()->getInlet() && p->getNodeOut()->getInlet()){
            p->setPhaseFlag(phase::water);
        }
    });

    for_each(networkRange<node*>(this).begin(), networkRange<node*>(this).end(), [this](node* n){
        if(n->getInlet()){
            n->setPhaseFlag(phase::water);
        }
    });
}

void network::setInitialFlags()
{
    clusterWaterElements();

    for(pore* p : networkRange<pore*>(this))
    {
        if(p->getInlet() && p->getPhaseFlag()==phase::oil) //create Pc in oil-filled inlet pores
        {
            p->setNodeOutOil(false);
            p->setNodeOutWater(true);
        }
    }

    for(node* p: networkRange<node*>(this)) //create Pc in oil-filled pores next to inlet-connected water
    {
        if(p->getPhaseFlag()==phase::water)
        for(auto  e : p->getNeighboors())
        {
            pore* neigh= static_cast<pore*>(e);
            if(neigh->getPhaseFlag()==phase::oil)
            {
                if(p==neigh->getNodeIn())
                {
                    neigh->setNodeInOil(false);
                    neigh->setNodeInWater(true);
                }

                if(p==neigh->getNodeOut())
                {
                    neigh->setNodeOutOil(false);
                    neigh->setNodeOutWater(true);
                }
            }
        }
    }
}

void network::setAdvancedTrapping()
{
    clusterOilElements();
    clusterWaterElements();

    vector<pore*> partiallyFilled;
    for(pore* p :networkRange<pore*>(this))
    {
        if(p->getPhaseFlag()==phase::oil)
        {
            p->setOilTrapped(true);

            if(p->getOutlet())
                p->setOilTrapped(false);

            else if(p->getNodeInWater() || p->getNodeOutWater())
            {
                p->setPhaseFlag(phase::temp);
                partiallyFilled.push_back(p);
            }
        }
        if(p->getPhaseFlag()==phase::water)
        {
            p->setWaterTrapped(true);
            if(p->getClusterWater()->getInlet())
                    p->setWaterTrapped(false);
        }
    }

    for(node* p :networkRange<node*>(this))
    {
        if(p->getPhaseFlag()==phase::oil)
        {
            p->setOilTrapped(true);
            if(p->getPhaseFlag()==phase::oil && p->getClusterOil()->getOutlet())
                p->setOilTrapped(false);

        }
        if(p->getPhaseFlag()==phase::water)
        {
            p->setWaterTrapped(true);
            if(p->getClusterWater()->getInlet())
                p->setWaterTrapped(false);
        }
    }

    clusterOilElements();

    for(pore* p :networkRange<pore*>(this))
    {
        if(p->getPhaseFlag()==phase::oil && p->getClusterOil()->getOutlet())
            p->setOilTrapped(false);
    }

    //Identify oil-filled pores without bulk oil to outlet
    for(pore* p :partiallyFilled)
    {
        if(p->getNodeInOil() && p->getNodeIn()!=0 && p->getNodeIn()->getPhaseFlag()==phase::oil && p->getNodeIn()->getClusterOil()->getOutlet())
            p->setOilTrapped(false);

        if(p->getNodeOutOil() && p->getNodeOut()!=0 && p->getNodeOut()->getPhaseFlag()==phase::oil && p->getNodeOut()->getClusterOil()->getOutlet())
            p->setOilTrapped(false);

        //Allow slow filling of throats connecting water clusters (a workaround to mimic experimental observations)
        if(enhancedWaterConnectivity)
            if(!p->getNodeOutOil() && !p->getNodeInOil() && p->getWettabilityFlag()==wettability::oilWet && p->getClusterOilWet()->getOutlet())
                p->setOilTrapped(false);
    }

    for(pore* p: partiallyFilled)
        p->setPhaseFlag(phase::oil);

    clusterOilElements();
}

void network::updateCapillaryProperties(unordered_set<pore *> &poresToCheck, unordered_set<node *> &nodesToCheck)
{
    for(node* p : networkRange<node*>(this)){
        p->setActive(true);

        if(p->getPhaseFlag()==phase::oil && p->getOilTrapped())
            p->setActive(false);

        if(p->getPhaseFlag()==phase::water && p->getWaterTrapped())
            p->setActive(false);
    }

    for(pore* p : networkRange<pore*>(this)){
        p->setActive(true);
        p->setCapillaryPressure(0);

        if(p->getPhaseFlag()==phase::oil){

            if(p->getOilTrapped())
                p->setActive(false);
            else{
                //Determine pores to check for phase changes
                if((p->getNodeIn()!=0 && p->getNodeIn()->getPhaseFlag()==phase::water && !p->getNodeIn()->getWaterTrapped())
                      ||(p->getNodeOut()!=0 && p->getNodeOut()->getPhaseFlag()==phase::water && !p->getNodeOut()->getWaterTrapped()))
                    poresToCheck.insert(p);

                //Update capilary pressures a pores with an oil/water interface
                if(!p->getInlet() && !p->getOutlet() && p->getNodeIn()!=0 && p->getNodeOut()!=0){
                    if(p->getNodeOut()->getPhaseFlag()==phase::oil && p->getNodeIn()->getPhaseFlag()==phase::water)
                        p->setCapillaryPressure(p->getEntryPressureCoefficient()*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
                }
                if(!p->getInlet() && !p->getOutlet() && p->getNodeIn()!=0 && p->getNodeOut()!=0){
                    if(p->getNodeOut()->getPhaseFlag()==phase::water && p->getNodeIn()->getPhaseFlag()==phase::oil)
                        p->setCapillaryPressure(-p->getEntryPressureCoefficient()*OWSurfaceTension*cos(p->getTheta())/p->getRadius());
                }
            }
        }

        if(p->getPhaseFlag()==phase::water){
            if(p->getWaterTrapped())
                p->setActive(false);
            else
            {
                //Determine nodes to check for phase changes
                node* nodeIn=p->getNodeIn();
                node* nodeOut=p->getNodeOut();
                if(nodeIn!=0 && nodeIn->getPhaseFlag()==phase::oil && !nodeIn->getOilTrapped())
                    nodesToCheck.insert(nodeIn);
                if(nodeOut!=0 && nodeOut->getPhaseFlag()==phase::oil && !nodeOut->getOilTrapped())
                    nodesToCheck.insert(nodeOut);

                //Update capilary pressures a nodes with an oil/water interfac
                if(!p->getInlet() && !p->getOutlet() && nodeIn!=0 && nodeOut!=0)
                {
                    //pore filling mechanism
                    int oilNeighboorsNumber(0);
                    for(element* n : nodeOut->getNeighboors())
                        if(n->getPhaseFlag()==phase::oil)
                            oilNeighboorsNumber++;

                    if(nodeOut->getPhaseFlag()==phase::oil && nodeIn->getPhaseFlag()==phase::water){
                        if(p->getTheta()>pi()/2)//drainage
                            p->setCapillaryPressure(p->getEntryPressureCoefficient()*OWSurfaceTension*cos(p->getTheta())/nodeOut->getRadius());
                        if(p->getTheta()<pi()/2)//imbibition
                            p->setCapillaryPressure(p->getEntryPressureCoefficient()*OWSurfaceTension*cos(p->getTheta())/nodeOut->getRadius()-oilNeighboorsNumber*OWSurfaceTension/nodeOut->getRadius());
                    }

                    if(nodeOut->getPhaseFlag()==phase::water && nodeIn->getPhaseFlag()==phase::oil){
                        if(p->getTheta()>pi()/2)//drainage
                            p->setCapillaryPressure(-p->getEntryPressureCoefficient()*OWSurfaceTension*cos(p->getTheta())/nodeIn->getRadius());
                        if(p->getTheta()<pi()/2)//imbibition
                            p->setCapillaryPressure(-p->getEntryPressureCoefficient()*OWSurfaceTension*cos(p->getTheta())/nodeIn->getRadius()+oilNeighboorsNumber*OWSurfaceTension/nodeIn->getRadius());
                    }
                }
            }
        }
    }
}

void network::solvePressureWithoutCounterImbibition()
{
    bool stillMorePoresToClose=true;

    while(stillMorePoresToClose)
    {
        clusterActiveElements();
        for(pore* p : networkRange<pore*>(this)){
            if(p->getActive() && p->getClusterActive()->getSpanning()==false){
                p->setCapillaryPressure(0);
                p->setActive(false);
            }
        }

        stillMorePoresToClose=false;

        solvePressuresWithCapillaryPressures();
        updateFlowsWithCapillaryPressure();

        for(pore* p : networkRange<pore*>(this)){
            if(p->getActive() && p->getNodeIn()!=0 && p->getNodeOut()!=0
                    && ((p->getFlow()>0 && p->getNodeOut()->getPhaseFlag()==phase::oil && p->getNodeIn()->getPhaseFlag()==phase::water)
                        || (p->getFlow()<0 && p->getNodeOut()->getPhaseFlag()==phase::water && p->getNodeIn()->getPhaseFlag()==phase::oil)
                        )){
                p->setCapillaryPressure(0);
                p->setActive(false);
                stillMorePoresToClose=true;
            }
            if(p->getActive() && p->getOutlet() &&  p->getFlow()<0){
                p->setCapillaryPressure(0);
                p->setActive(false);
                stillMorePoresToClose=true;
            }
        }

        if(cancel)break;
    }

    for(node* n : networkRange<node*>(this))
        n->setFlow(0);

    for(pore* p : networkRange<pore*>(this)){
        if(p->getPhaseFlag()==phase::water)
        {
            if(p->getFlow()>1e-24 && p->getActive()){
                node* n=p->getNodeIn();
                if(n!=0)
                    n->setFlow((n->getFlow()+abs(p->getFlow())));
            }
            if(p->getFlow()<-1e-24 && p->getActive()){
                node* n=p->getNodeOut();
                if(n!=0)
                    n->setFlow((n->getFlow()+abs(p->getFlow())));
            }
        }
    }
}

void network::calculateTimeStepUSS(unordered_set<pore *> &poresToCheck, unordered_set<node *> &nodesToCheck, bool includeWater)
{
    timeStep=1e50;
    for(pore* p : poresToCheck)
    {
        if(p->getActive() && abs(p->getFlow())>1e-24)
        {
            double step=p->getVolume()*p->getOilFraction()/abs(p->getFlow());
            if(step<timeStep)
                timeStep=step;
        }
    }

    for(node* p : nodesToCheck)
    {
        if(p->getActive() && abs(p->getFlow())>1e-24)
        {
            double step=p->getVolume()*p->getOilFraction()/abs(p->getFlow());
            if(step<timeStep)
                timeStep=step;
        }
    }

    if(includeWater) //if water is forming a spanning cluster
    {
        for(pore* p: networkRange<pore*>(this))
        {
            if(p->getActive() && p->getPhaseFlag()==phase::water && abs(p->getFlow())>1e-24)
            {
                double step=p->getVolume()/abs(p->getFlow());
                if(step<timeStep)
                    timeStep=step;
            }
        }

        for(node* p : networkRange<node*>(this))
        {
            if(p->getActive() && p->getPhaseFlag()==phase::water && abs(p->getFlow())>1e-24)
            {
                double step=p->getVolume()/abs(p->getFlow());
                if(step<timeStep)
                    timeStep=step;
            }
        }
    }

    if(timeStep==1e50)
    {
        cancel=true;
        cout<<"ERROR in calculateTimeStepUSS: infinite time step "<<timeStep<<endl;
    }
}

double network::updateElementaryFluidFractions(unordered_set<pore *> &poresToCheck, unordered_set<node *> &nodesToCheck, bool &solvePressure)
{
    for(pore* p : poresToCheck)
    {
        if(p->getActive() && abs(p->getFlow())>1e-24)
        {
            if(p->getFlow()>0 && p->getNodeOutWater() || p->getFlow()<0 && p->getNodeInWater())
            {
                double incrementalWater=abs(p->getFlow())*timeStep;
                p->setWaterFraction(p->getWaterFraction()+incrementalWater/p->getVolume());
                p->setOilFraction(1-p->getWaterFraction());

                p->setViscosity(oilViscosity*p->getOilFraction()+waterViscosity*p->getWaterFraction());
                updateConductivity(p);

                if(p->getWaterFraction()>1-1e-8)
                {
                    p->setPhaseFlag(phase::water);
                    p->setWaterFraction(1.0);
                    p->setOilFraction(0.0);
                    solvePressure=true;
                }
            }
        }
    }

    for(node* p : nodesToCheck)
    {
        if(p->getActive() && abs(p->getFlow())>1e-24)
        {
            double incrementalWater=abs(p->getFlow())*timeStep;
            p->setWaterFraction(p->getWaterFraction()+incrementalWater/p->getVolume());
            p->setOilFraction(1-p->getWaterFraction());

            p->setViscosity(oilViscosity*p->getOilFraction()+waterViscosity*p->getWaterFraction());

            if(p->getWaterFraction()>1-1e-8)
            {
                p->setPhaseFlag(phase::water);
                p->setWaterFraction(1.0);
                p->setOilFraction(0.0);
                solvePressure=true;
            }

            //mass conservation check
            if(p->getOilFraction()>1.0001 || p->getOilFraction()<-0.0001) {cout<<"Something wrong: oil fraction >1 or <0 "<<p->getOilFraction()<<endl;cancel=true;}
            if(p->getWaterFraction()>1.0001 || p->getWaterFraction()<-0.0001) {cout<<"Something wrong: water fraction >1 or <0 "<<p->getWaterFraction()<<endl;cancel=true;}
        }
    }
}

void network::updateConductivity(pore * p)
{
    node* nodeIn=p->getNodeIn();
    node* nodeOut=p->getNodeOut();
    auto throatConductivityInverse(0.0),nodeInConductivityInverse(0.0),nodeOutConductivityInverse(0.0);

    throatConductivityInverse=1/(p->getShapeFactorConstant()*pow(p->getRadius(),4)/(16*p->getShapeFactor())/(p->getViscosity()*p->getLength()));

    if(nodeIn!=0)
        nodeInConductivityInverse=1/(nodeIn->getShapeFactorConstant()*pow(nodeIn->getRadius(),4)/(16*nodeIn->getShapeFactor())/(nodeIn->getViscosity()*p->getNodeInLength()));
    if(nodeOut!=0)
        nodeOutConductivityInverse=1/(nodeOut->getShapeFactorConstant()*pow(nodeOut->getRadius(),4)/(16*nodeOut->getShapeFactor())/(nodeOut->getViscosity()*p->getNodeOutLength()));

    p->setConductivity(1./(throatConductivityInverse+nodeInConductivityInverse+nodeOutConductivityInverse));
}

void network::updateElementaryFluidFlags(unordered_set<pore *> &poresToCheck, unordered_set<node *> &nodesToCheck)
{
    for(pore* p: poresToCheck)
    {
        node* n=nullptr;

        if(p->getPhaseFlag()==phase::water)
        {
            if(p->getNodeInOil())
                n=p->getNodeIn();
            if(p->getNodeOutOil())
                n=p->getNodeOut();

            p->setNodeInOil(false);
            p->setNodeOutOil(false);
            p->setNodeInWater(true);
            p->setNodeOutWater(true);
        }

        if(n!=nullptr && n->getPhaseFlag()==phase::water && n->getWaterTrapped())
        {
            for(node* nn: networkRange<node*>(this))
            {
                if(nn->getPhaseFlag()==phase::water && nn->getClusterWater()==n->getClusterWater())
                for(auto e : nn->getNeighboors())
                {
                    pore* nnn= static_cast<pore*>(e);
                    if(nnn->getPhaseFlag()==phase::oil)
                    {
                        if(nnn->getNodeIn()==nn)
                        {
                            nnn->setNodeInOil(false);
                            nnn->setNodeInWater(true);
                        }

                        if(nnn->getNodeOut()==nn)
                        {
                            nnn->setNodeOutOil(false);
                            nnn->setNodeOutWater(true);
                        }
                    }
                }
            }
        }
    }

    for(node* p : nodesToCheck)
    {
        if(p->getPhaseFlag()==phase::water)
        {
            for(auto e : p->getNeighboors())
            {
                pore* n= static_cast<pore*>(e);
                if(n->getPhaseFlag()==phase::oil)
                {
                    if(n->getNodeIn()==p)
                    {
                        n->setNodeInOil(false);
                        n->setNodeInWater(true);
                    }

                    if(n->getNodeOut()==p)
                    {
                        n->setNodeOutOil(false);
                        n->setNodeOutWater(true);
                    }
                }

                if(n->getPhaseFlag()==phase::water && n->getWaterTrapped())
                {
                    for(node* nn: networkRange<node*>(this))
                    {
                        if(nn->getPhaseFlag()==phase::water && nn->getClusterWater()==n->getClusterWater())
                        for(auto e : nn->getNeighboors())
                        {
                            pore* nnn= static_cast<pore*>(e);;
                            if(nnn->getPhaseFlag()==phase::oil)
                            {
                                if(nnn->getNodeIn()==nn)
                                {
                                    nnn->setNodeInOil(false);
                                    nnn->setNodeInWater(true);
                                }

                                if(nnn->getNodeOut()==nn)
                                {
                                    nnn->setNodeOutOil(false);
                                    nnn->setNodeOutWater(true);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

}
