/////////////////////////////////////////////////////////////////////////////
/// Name:        generationRegular.cpp
/// Purpose:     methods related to the extraction of networks from microCT
///              network files are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"
#include "randomGenerator.h"
#include "iterator.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace PNM {

void network::setupRegularModel()
{
    cout<<"Loading Global Attributes..."<<endl;
    calculateRegularNetworkAttributes();
    cout<<"Creating Nodes..."<<endl;
    createNodes();
    cout<<"Creating Pores..."<<endl;
    createPores();
    cout<<"Assigning Neighboors..."<<endl;
    setNeighboors();
    cout<<"Setting Boundary Conditions..."<<endl;
    setBoundaryConditions();
    cout<<"Applying Coordination Number..."<<endl;
    applyCoordinationNumber();
    cout<<"Cleaning Network..."<<endl;
    cleanNetwork();
    cout<<"Assigning Radii..."<<endl;
    assignRadii();
    cout<<"Assigning Lengths..."<<endl;
    assignLengths();
    cout<<"Distorting Network..."<<endl;
    distortNetwork();
    cout<<"Assigning Shape Factors..."<<endl;
    assignShapeFactors();
    cout<<"Assigning Volumes..."<<endl;
    assignVolumes();
    cout<<"Assigning Conductivities..."<<endl;
    assignConductivities();
    cout<<"Assigning Wettability..."<<endl;
    assignWettability();
    cout<<"Calculating Network Attributes.."<<endl;
    calculateNetworkAttributes();

    if(absolutePermeabilityCalculation){
        cout<<"Calculating Absolute Permeabilty..."<<endl;
        solvePressures();
        updateFlows();
        calculatePermeabilityAndPorosity();
    }

    //Display notification
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "Perm.(mD): " << absolutePermeability/0.987e-15<<" / Porosity(%): "<<porosity*100<<" / Dx(mm): "<< xEdgeLength*1e3 << " / Dy(mm): "  << yEdgeLength*1e3<<" / Dz(mm): "<<zEdgeLength*1e3;
    simulationNotification = ss.str();
}

void network::calculateRegularNetworkAttributes()
{
    totalNodes=Nx*Ny*Nz;
    totalPores=3*Nx*Ny*Nz+Ny*Nz+Nx*Nz+Nx*Ny;
    tableOfAllNodes.reserve(totalNodes);
    tableOfAllPores.reserve(totalPores);

    xEdgeLength=Nx*length;
    yEdgeLength=Ny*length;
    zEdgeLength=Nz*length;

    maxConnectionNumber = 6;
}

void network::createNodes()
{
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz; ++k){
                tableOfAllNodes.push_back(new node(i,j,k));
            }

    auto key(0);
    for_each(tableOfAllNodes.begin(),tableOfAllNodes.end(),[this, &key](node* e){
        e->setId(key+1);
        e->setAbsId(key);
        ++key;
        if(e->getIndexX()==0)e->setInlet(true);
        if(e->getIndexX()==Nx-1)e->setOutlet(true);
        e->setXCoordinate(e->getIndexX()*length);
        e->setYCoordinate(e->getIndexY()*length);
        e->setZCoordinate(e->getIndexZ()*length);
    });

}

void network::createPores()
{
    for (int i = 0; i < Nx+1; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz; ++k)
                tableOfAllPores.push_back(new pore(getNode(i,j,k),getNode(i-1,j,k)));
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny+1; ++j)
            for (int k = 0; k < Nz; ++k)
                tableOfAllPores.push_back(new pore(getNode(i,j,k),getNode(i,j-1,k)));
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz+1; ++k)
                tableOfAllPores.push_back(new pore(getNode(i,j,k),getNode(i,j,k-1)));

    auto key(0);
    for_each(tableOfAllPores.begin(),tableOfAllPores.end(),[this, &key](pore* p){
        p->setId(key+1);
        p->setAbsId(totalNodes+key);
        ++key;
        if(p->getNodeOut()==0)
        {
            p->setInlet(true);
            inletPores.push_back(p);
        }
        if(p->getNodeIn()==0)
        {
            p->setOutlet(true);
            outletPores.push_back(p);
        }
    });
}

void network::setNeighboors()
{
    for(node* n : tableOfAllNodes)
    {
        vector<element*> neighboors;
        neighboors.reserve(6);

        pore *x=getPoreX(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *xout=getPoreXout(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *y=getPoreY(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *yout=getPoreYout(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *z=getPoreZ(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *zout=getPoreZout(n->getIndexX(),n->getIndexY(),n->getIndexZ());

        neighboors.push_back(x);
        neighboors.push_back(xout);
        neighboors.push_back(y);
        neighboors.push_back(yout);
        neighboors.push_back(z);
        neighboors.push_back(zout);
        n->setNeighboors(neighboors);
    }

    for(pore* p : tableOfAllPores)
    {
        vector<element*> neighboors;
        if(p->getNodeIn()!=0)
            neighboors.push_back(p->getNodeIn());
        if(p->getNodeOut()!=0)
            neighboors.push_back(p->getNodeOut());
        p->setNeighboors(neighboors);
    }
}

void network::setBoundaryConditions()
{
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
        {
            pore* zIn=getPoreZ(i,j,0);
            pore* zOut=getPoreZ(i,j,Nz);
            zIn->setClosed(true);
            zOut->setClosed(true);
        }
    for (int i = 0; i < Nx; ++i)
        for (int k = 0; k < Nz; ++k)
        {
            pore* yIn=getPoreY(i,0,k);
            pore* yOut=getPoreY(i,Ny,k);
            yIn->setClosed(true);
            yOut->setClosed(true);
        }

    totalOpenedPores = totalPores - 2*Nx*Ny - 2*Nx*Nz;

}

void network::applyCoordinationNumber()
{
    if (coordinationNumber<6 || (coordinationNumber<4 && Nz==1))
    {
        auto closedPoresNumber=(Nz==1? int(totalOpenedPores*(1-coordinationNumber/4.0)) :int(totalPores*(1-coordinationNumber/6.0)));

        auto shuffledPores=tableOfAllPores;
        shuffle(shuffledPores.begin(), shuffledPores.end(), randomGenerator(seed).getGen());

        while(closedPoresNumber>0)
        {
            pore* p=shuffledPores.back();
            shuffledPores.pop_back();
            if(!p->getClosed()){
                p->setClosed(true);
                --closedPoresNumber;
            }
        }
    }
}

void network::cleanNetwork()
{
    //Clean Network from isolated pores
    clusterOilElements();

    for_each(tableOfAllPores.begin(),tableOfAllPores.end(),[this](pore* e){
        if(!e->getClosed() && !e->getClusterOil()->getSpanning()){
            e->setClosed(true);
        }
    });

    //Delete isolated nodes
    for_each(tableOfAllNodes.begin(),tableOfAllNodes.end(),[this](node* e){
        if(!e->getClusterOil()->getSpanning()){
            e->setClosed(true);
        }
    });

    // delete closed pores from inlet/ outlet containers
    inletPores.erase(remove_if(inletPores.begin(), inletPores.end(), [this](pore* p)->bool{
                         return p->getClosed();
                     }), inletPores.end());
    outletPores.erase(remove_if(outletPores.begin(), outletPores.end(), [this](pore* p)->bool{
                         return p->getClosed();
                     }), outletPores.end());

    // delete closed elements from neighboor containers
    for_each(tableOfAllPores.begin(),tableOfAllPores.end(),[this](pore* e){
        vector<element*>& neighboors = e->getNeighboors();
        neighboors.erase(remove_if(neighboors.begin(), neighboors.end(), [this](element* e)->bool{
                             return e->getClosed();
                         }), neighboors.end());
    });

    for_each(tableOfAllNodes.begin(),tableOfAllNodes.end(),[this](node* e){
        vector<element*>& neighboors = e->getNeighboors();
        neighboors.erase(remove_if(neighboors.begin(), neighboors.end(), [this](element* e)->bool{
                             return e->getClosed();
                         }), neighboors.end());
    });
}

void network::assignRadii()
{
    randomGenerator gen(seed);
    for_each(networkRange<pore*>(this).begin(),networkRange<pore*>(this).end(),[this, &gen](pore* p){
        if(radiusDistribution==1)
            p->setRadius(gen.uniform_real(minRadius,maxRadius));
        if(radiusDistribution==2)
            p->setRadius(gen.rayleigh(minRadius,maxRadius,rayleighParameter));
        if(radiusDistribution==3)
            p->setRadius(gen.triangular(minRadius,maxRadius,triangularParameter));
        if(radiusDistribution==4)
            p->setRadius(gen.normal(minRadius,maxRadius,normalMuParameter,normalSigmaParameter));
    });

    for_each(networkRange<node*>(this).begin(),networkRange<node*>(this).end(),[this](node* n){
        double maxRadius(0),averageRadius(0);
        int neighboorsNumber(0);
        for(element* p : n->getNeighboors()){
            if(p->getRadius()>maxRadius)
                maxRadius=p->getRadius();
            averageRadius+=p->getRadius();
            neighboorsNumber++;
        }
        averageRadius=aspectRatio*averageRadius/neighboorsNumber;
        double radius=max(maxRadius,averageRadius);
        n->setRadius(radius);
    });
}

void network::assignLengths()
{
    for_each(networkRange<pore*>(this).begin(),networkRange<pore*>(this).end(),[this](pore* p){
        p->setFullLength(length);
        p->setNodeInLength(p->getNodeIn()==0 ?0:p->getNodeIn()->getRadius());
        p->setNodeOutLength(p->getNodeOut()==0 ?0:p->getNodeOut()->getRadius());
        p->setLength(length-p->getNodeInLength()-p->getNodeOutLength()>0?length-p->getNodeInLength()-p->getNodeOutLength():length/2);
    });

    for_each(networkRange<node*>(this).begin(),networkRange<node*>(this).end(),[this](node* n){
        n->setLength(2*n->getRadius());
    });
}

void network::distortNetwork()
{
    if(degreeOfDistortion>0)
    {
        randomGenerator gen(seed);
        for_each(networkRange<node*>(this).begin(),networkRange<node*>(this).end(),[this, &gen](node* n){
            n->setXCoordinate(n->getXCoordinate()+length*degreeOfDistortion*(-1+2*gen.uniform_real()));
            n->setYCoordinate(n->getYCoordinate()+length*degreeOfDistortion*(-1+2*gen.uniform_real()));
            if(Nz!=1)
            n->setZCoordinate(n->getZCoordinate()+length*degreeOfDistortion*(-1+2*gen.uniform_real()));
        });

        //update pores' lengths
        for_each(networkRange<pore*>(this).begin(),networkRange<pore*>(this).end(),[this](pore* p){
            if(p->getNodeIn()==0 || p->getNodeOut()==0) return;
            double length=sqrt(pow(p->getNodeIn()->getXCoordinate()-p->getNodeOut()->getXCoordinate(),2)+pow(p->getNodeIn()->getYCoordinate()-p->getNodeOut()->getYCoordinate(),2)+pow(p->getNodeIn()->getZCoordinate()-p->getNodeOut()->getZCoordinate(),2));
            p->setFullLength(length);
            p->setLength(length-p->getNodeInLength()-p->getNodeOutLength()>0?length-p->getNodeInLength()-p->getNodeOutLength():length/2);
        });
    }
}

void network::assignShapeFactors()
{
    auto circleThreshold=sqrt(3)/36.0;
    auto squareThreshold=1.0/16.0;
    for_each(networkRange<element*>(this).begin(),networkRange<element*>(this).end(),[=,this](element* e){
        e->setShapeFactor(shapeFactor);
        if(shapeFactor<=circleThreshold)
            e->setShapeFactorConstant(0.6);
        else if (shapeFactor<=squareThreshold)
            e->setShapeFactorConstant(0.5623);
        else
            e->setShapeFactorConstant(0.5);
        e->setEntryPressureCoefficient(1+2*sqrt(pi()*e->getShapeFactor()));
    });
}

void network::assignVolumes()
{
    for_each(networkRange<element*>(this).begin(),networkRange<element*>(this).end(),[this](element* e){
        double volume=pow(poreVolumeConstant,2-poreVolumeExponent)*e->getLength()*pow(e->getRadius(),poreVolumeExponent)/(4*e->getShapeFactor())*pow(10,(6*poreVolumeExponent-12));
        e->setVolume(volume);
        e->setEffectiveVolume(volume);
    });
}

void network::calculateNetworkAttributes()
{
    totalOpenedPores=0;
    totalOpenedNodes=0;
    totalNodesVolume=0;
    totalPoresVolume=0;
    inletPoresVolume=0;

    for_each(networkRange<node*>(this).begin() , networkRange<node*>(this).end(),[&, this](node* n){
        totalOpenedNodes++;
        totalNodesVolume+=n->getVolume();
    });

    for_each(networkRange<pore*>(this).begin() , networkRange<pore*>(this).end(),[&, this](pore* p){
        totalOpenedPores++;
        totalPoresVolume+=p->getVolume();
        if(p->getInlet())
             inletPoresVolume+=p->getVolume();
    });

    totalNetworkVolume=totalNodesVolume+totalPoresVolume;
}

}
