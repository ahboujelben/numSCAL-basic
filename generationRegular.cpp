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

namespace PNM {

void network::setupRegularModel()
{
    totalNodes=Nx*Ny*Nz;
    totalPores=3*Nx*Ny*Nz+Ny*Nz+Nx*Nz+Nx*Ny;
    totalElements=totalNodes+totalPores;

    xEdgeLength=Nx*length;
    yEdgeLength=Ny*length;
    zEdgeLength=Nz*length;

    tableOfAllNodes.reserve(totalNodes); 
    tableOfAllPores.reserve(totalPores);
    tableOfElements.reserve(totalPores+totalNodes);

    cout<<"Creating Nodes..."<<endl;
    createNodes();
    cout<<"Creating Pores..."<<endl;
    createPores();
    cout<<"Applying Coordination Number..."<<endl;
    applyCoordinationNumber();
    cout<<"Defining accessible elements..."<<endl;
    defineAccessibleElements();
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

    if(absolutePermeabilityCalculation){
        cout<<"Absolute Permeabilty Calculation..."<<endl;
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
        tableOfElements.push_back(e);
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
        tableOfElements.push_back(p);
    });

    setNeighboors();
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

void network::applyCoordinationNumber()
{
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
        {
            pore* zIn=getPoreZ(i,j,0);
            pore* zOut=getPoreZ(i,j,Nz);
            zIn->setClosed(true);
            zIn->getNodeIn()->setConnectionNumber(zIn->getNodeIn()->getConnectionNumber()-1);
            zOut->setClosed(true);
            zOut->getNodeOut()->setConnectionNumber(zOut->getNodeOut()->getConnectionNumber()-1);
        }
    for (int i = 0; i < Nx; ++i)
        for (int k = 0; k < Nz; ++k)
        {
            pore* yIn=getPoreY(i,0,k);
            pore* yOut=getPoreY(i,Ny,k);
            yIn->setClosed(true);
            yIn->getNodeIn()->setConnectionNumber(yIn->getNodeIn()->getConnectionNumber()-1);
            yOut->setClosed(true);
            yOut->getNodeOut()->setConnectionNumber(yOut->getNodeOut()->getConnectionNumber()-1);
        }


    if (coordinationNumber<6 || (coordinationNumber<4 && Nz==1))
    {
        auto totalOpenedPoresSoFar=totalPores-2*Nx*Ny-2*Nx*Nz;
        auto closedPoresNumber=(Nz==1? int(totalOpenedPoresSoFar*(1-coordinationNumber/4.0)) :int(totalPores*(1-coordinationNumber/6.0)));

        auto shuffledPores=tableOfAllPores;
        shuffle(shuffledPores.begin(), shuffledPores.end(), gen);

        while(closedPoresNumber>0)
        {
            pore* p=shuffledPores.back();
            shuffledPores.pop_back();
            if(!p->getClosed())
            {
                p->setClosed(true);
                node* in=p->getNodeIn();
                node* out=p->getNodeOut();
                if(in!=0)in->setConnectionNumber(in->getConnectionNumber()-1);
                if(out!=0)out->setConnectionNumber(out->getConnectionNumber()-1);
                --closedPoresNumber;
            }
        }

        //Clean Network from isolated pores
        clusterOilElements();

        for_each(tableOfAllPores.begin(),tableOfAllPores.end(),[this](pore* e){
            if(!e->getClosed() && !e->getClusterOil()->getSpanning()){
                e->setClosed(true);
                node* in=e->getNodeIn();
                node* out=e->getNodeOut();
                if(in!=0)in->setConnectionNumber(in->getConnectionNumber()-1);
                if(out!=0)out->setConnectionNumber(out->getConnectionNumber()-1);
            }
        });

        //Delete isolated nodes
        for_each(tableOfAllNodes.begin(),tableOfAllNodes.end(),[this](node* e){
            if(e->getConnectionNumber()==0 || !e->getClusterOil()->getSpanning()){
                e->setClosed(true);
            }
        });
    }

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

    //Ranking for the solver
    auto rank(0);
    for_each(tableOfAllNodes.begin(),tableOfAllNodes.end(),[this, &rank](node* e){
        if(!e->getClosed()){
            e->setRank(rank);
            ++rank;
        }
    });

    maxConnectionNumber=(Nz==1? 4: 6);
}

void network::defineAccessibleElements()
{
    totalOpenedElements=0;
    totalOpenedPores=0;
    totalOpenedNodes=0;

    accessibleNodes.reserve(totalNodes);
    accessiblePores.reserve(totalPores);
    accessibleElements.reserve(totalNodes+totalPores);

    for(node* n:tableOfAllNodes){
        if(!n->getClosed()){
            totalOpenedElements++;
            totalOpenedNodes++;
            accessibleNodes.push_back(n);
            accessibleElements.push_back(n);
        }
    }

    for(pore* p:tableOfAllPores){
        if(!p->getClosed()){
            totalOpenedElements++;
            totalOpenedPores++;
            accessiblePores.push_back(p);
            accessibleElements.push_back(p);
        }
    }
}

void network::assignRadii()
{
    for_each(accessiblePores.begin(),accessiblePores.end(),[this](pore* p){
        if(radiusDistribution==1)
            p->setRadius(uniform_real(minRadius,maxRadius));
        if(radiusDistribution==2)
            p->setRadius(rayleigh(minRadius,maxRadius,rayleighParameter));
        if(radiusDistribution==3)
            p->setRadius(triangular(minRadius,maxRadius,triangularParameter));
        if(radiusDistribution==4)
            p->setRadius(normal(minRadius,maxRadius,normalMuParameter,normalSigmaParameter));
    });

    maxNodeRadius=0;
    minNodeRadius=1e10;

    for_each(accessibleNodes.begin(),accessibleNodes.end(),[this](node* n){
        double maxRadius(0),averageRadius(0);
        int neighboorsNumber(0);
        for(element* p : n->getNeighboors())
        {
            if(!p->getClosed())
            {
                if(p->getRadius()>maxRadius)maxRadius=p->getRadius();
                averageRadius+=p->getRadius();
                neighboorsNumber++;
            }
        }
        averageRadius=aspectRatio*averageRadius/neighboorsNumber;
        double radius=max(maxRadius,averageRadius);
        if(radius>maxNodeRadius)maxNodeRadius=radius;
        if(radius<minNodeRadius)minNodeRadius=radius;
        n->setRadius(radius);
    });
}

void network::assignLengths()
{
    for_each(accessiblePores.begin(),accessiblePores.end(),[this](pore* p){
        p->setFullLength(length);
        p->setNodeInLength(p->getNodeIn()==0 ?0:p->getNodeIn()->getRadius());
        p->setNodeOutLength(p->getNodeOut()==0 ?0:p->getNodeOut()->getRadius());
        p->setLength(length-p->getNodeInLength()-p->getNodeOutLength()>0?length-p->getNodeInLength()-p->getNodeOutLength():length/2);
    });

    for_each(accessibleNodes.begin(),accessibleNodes.end(),[this](node* n){
        n->setLength(2*n->getRadius());
    });
}

void network::distortNetwork()
{
    if(degreeOfDistortion>0)
    {
        for_each(accessibleNodes.begin(),accessibleNodes.end(),[this](node* n){
            n->setXCoordinate(n->getXCoordinate()+length*degreeOfDistortion*(-1+2*uniform_real()));
            n->setYCoordinate(n->getYCoordinate()+length*degreeOfDistortion*(-1+2*uniform_real()));
            if(Nz!=1)
            n->setZCoordinate(n->getZCoordinate()+length*degreeOfDistortion*(-1+2*uniform_real()));
        });

        //update pores' lengths
        for_each(accessiblePores.begin(),accessiblePores.end(),[this](pore* p){
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
    for_each(accessibleElements.begin(),accessibleElements.end(),[=,this](element* e){
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
    for_each(accessibleElements.begin(),accessibleElements.end(),[this](element* e){
        double volume=pow(poreVolumeConstant,2-poreVolumeExponent)*e->getLength()*pow(e->getRadius(),poreVolumeExponent)/(4*e->getShapeFactor())*pow(10,(6*poreVolumeExponent-12));
        e->setVolume(volume);
        e->setEffectiveVolume(volume);
    });

    totalNodesVolume=accumulate(accessibleNodes.begin(), accessibleNodes.end(), 0.0, [](double sum, const node* n){
        return sum+n->getVolume();
    });

    totalPoresVolume=accumulate(accessiblePores.begin(), accessiblePores.end(), 0.0, [](double sum, const pore* p){
        return sum+p->getVolume();
    });

    inletPoresVolume=accumulate(inletPores.begin(), inletPores.end(), 0.0, [](double sum, const pore* p){
        return sum+p->getVolume();
    });

    totalElementsVolume=totalNodesVolume+totalPoresVolume;
}

}
