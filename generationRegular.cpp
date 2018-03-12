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
    tableOfAllNodes.reserve(totalNodes);
    totalPores=3*Nx*Ny*Nz+Ny*Nz+Nx*Nz+Nx*Ny;
    tableOfAllPores.reserve(totalPores);
    totalElements=totalNodes+totalPores;
    tableOfElements.reserve(totalPores+totalNodes);
    xEdgeLength=Nx*length;
    yEdgeLength=Ny*length;
    zEdgeLength=Nz*length;

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
    cout<<"Assigning Half Angles..."<<endl;
    assignHalfAngles();
    cout<<"Assigning Volumes..."<<endl;
    assignVolumes();
    cout<<"Assigning Conductivities..."<<endl;
    assignConductivities();
    cout<<"Assigning General properties..."<<endl;
    assignGeneralProperties();
    cout<<"Assigning Wettability..."<<endl;
    assignWettability();

    if(absolutePermeabilityCalculation)
    {
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
    tableOfPoresX.resize(Nx+1);
    for (int i = 0; i < Nx+1; ++i)
    {
        tableOfPoresX[i].resize(Ny);
        for (int j = 0; j < Ny; ++j)
            tableOfPoresX[i][j].resize(Nz);
    }

    tableOfPoresY.resize(Nx);
    for (int i = 0; i < Nx; ++i)
    {
        tableOfPoresY[i].resize(Ny+1);
        for (int j = 0; j < Ny+1; ++j)
            tableOfPoresY[i][j].resize(Nz);
    }

    tableOfPoresZ.resize(Nx);
    for (int i = 0; i < Nx; ++i)
    {
        tableOfPoresZ[i].resize(Ny);
        for (int j = 0; j < Ny; ++j)
            tableOfPoresZ[i][j].resize(Nz+1);
    }

    for (int i = 0; i < Nx+1; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz; ++k)
                tableOfPoresX[i][j][k]=new pore(getNode(i,j,k),getNode(i-1,j,k));
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny+1; ++j)
            for (int k = 0; k < Nz; ++k)
                tableOfPoresY[i][j][k]=new pore(getNode(i,j,k),getNode(i,j-1,k));
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz+1; ++k)
                tableOfPoresZ[i][j][k]=new pore(getNode(i,j,k),getNode(i,j,k-1));

    for (int k = 0; k < Nz; ++k)
        for (int j = 0; j < Ny; ++j)
            for (int i = 0; i < Nx+1; ++i)
                tableOfAllPores.push_back(tableOfPoresX[i][j][k]);
    for (int k = 0; k < Nz; ++k)
        for (int j = 0; j < Ny+1; ++j)
            for (int i = 0; i < Nx; ++i)
                tableOfAllPores.push_back(tableOfPoresY[i][j][k]);
    for (int k = 0; k < Nz+1; ++k)
        for (int j = 0; j < Ny; ++j)
            for (int i = 0; i < Nx; ++i)
                tableOfAllPores.push_back(tableOfPoresZ[i][j][k]);

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        p->setId(i+1);
        p->setAbsId(totalNodes+i);
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
    }
    setNeighboors();
}

void network::setNeighboors()
{
    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        vector<int> connectedPores;
        vector<int> neighboors;
        pore *x=getPoreX(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *xout=getPoreXout(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *y=getPoreY(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *yout=getPoreYout(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *z=getPoreZ(n->getIndexX(),n->getIndexY(),n->getIndexZ());
        pore *zout=getPoreZout(n->getIndexX(),n->getIndexY(),n->getIndexZ());

        connectedPores.push_back(x->getId());
        connectedPores.push_back(xout->getId());
        connectedPores.push_back(y->getId());
        connectedPores.push_back(yout->getId());
        connectedPores.push_back(z->getId());
        connectedPores.push_back(zout->getId());
        n->setConnectedPores(connectedPores);

        neighboors.push_back(x->getNodeOut()==0?0:x->getNodeOut()->getId());
        neighboors.push_back(xout->getNodeIn()==0?0:xout->getNodeIn()->getId());
        neighboors.push_back(y->getNodeOut()==0?0:y->getNodeOut()->getId());
        neighboors.push_back(yout->getNodeIn()==0?0:yout->getNodeIn()->getId());
        neighboors.push_back(z->getNodeOut()==0?0:z->getNodeOut()->getId());
        neighboors.push_back(zout->getNodeIn()==0?0:zout->getNodeIn()->getId());
        n->setConnectedNodes(neighboors);
    }

    for(int i=0;i<totalPores;++i)
    {
        pore*p=getPore(i);
        vector<pore*> neighboors;
        if(p->getNodeIn()!=0)
        {
            const vector<int>& neighboorsIn=p->getNodeIn()->getConnectedPores();
            for(unsigned j=0;j<neighboorsIn.size();++j)
                if(neighboorsIn[j]!=p->getId())
                    neighboors.push_back(getPore(neighboorsIn[j]-1));
        }
        if(p->getNodeOut()!=0)
        {
            const vector<int>& neighboorsOut=p->getNodeOut()->getConnectedPores();
            for(unsigned j=0;j<neighboorsOut.size();++j)
                if(neighboorsOut[j]!=p->getId())
                    neighboors.push_back(getPore(neighboorsOut[j]-1));
        }
        p->setConnectedPores(neighboors);
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
        clusterOilPores();
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
            if(e->getConnectionNumber()==0){
                e->setClosed(true);
            }
        });
    }

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

    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
        {
            totalOpenedElements++;
            totalOpenedNodes++;
            accessibleNodes.push_back(n);
            accessibleElements.push_back(n);
        }
    }

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            totalOpenedElements++;
            totalOpenedPores++;
            accessiblePores.push_back(p);
            accessibleElements.push_back(p);
        }
    }

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        vector<element*> neighs;
        if(p->getNodeIn()!=0)neighs.push_back(p->getNodeIn());
        if(p->getNodeOut()!=0)neighs.push_back(p->getNodeOut());
        p->setNeighboors(neighs);
    }

    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        vector<element*> neighs;
        const vector<int>& neighboors=n->getConnectedPores();
        for(unsigned j=0;j<neighboors.size();++j)
           neighs.push_back(getPore(neighboors[j]-1));
        n->setNeighboors(neighs);
    }
}

void network::assignRadii()
{
    ofstream filep("Results/Network_Description/pores_radii.txt");
    ofstream filen("Results/Network_Description/nodes_radii.txt");

    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            if(radiusDistribution==1)
                p->setRadius(uniform_real(minRadius,maxRadius));
            if(radiusDistribution==2)
                p->setRadius(rayleigh(minRadius,maxRadius,rayleighParameter));
            if(radiusDistribution==3)
                p->setRadius(triangular(minRadius,maxRadius,triangularParameter));
            if(radiusDistribution==4)
                p->setRadius(normal(minRadius,maxRadius,normalMuParameter,normalSigmaParameter));
            filep<<p->getId()<<" "<<p->getRadius()<<endl;
        }
    }

    if(networkSource==2)
    {
        maxNodeRadius=0;
        minNodeRadius=1e10;
        for (int i = 0; i < totalNodes; ++i)
        {
            node* n=getNode(i);
            if(!n->getClosed())
            {
                double maxRadius(0),averageRadius(0);
                int neighboorsNumber(0);
                for(unsigned j=0;j<n->getConnectedPores().size();++j)
                {
                    pore* p=getPore(n->getConnectedPores()[j]-1);
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
                filen<<n->getId()<<" "<<n->getRadius()<<endl;
            }
        }
    }

    //createGanglia();
}

void network::assignLengths()
{
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            p->setFullLength(length);
            p->setNodeInLength(p->getNodeIn()==0 ?0:p->getNodeIn()->getRadius());
            p->setNodeOutLength(p->getNodeOut()==0 ?0:p->getNodeOut()->getRadius());
            p->setLength(length-p->getNodeInLength()-p->getNodeOutLength()>0?length-p->getNodeInLength()-p->getNodeOutLength():length/2);
        }
    }

    if(networkSource==2)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
            n->setLength(2*n->getRadius());
    }
}

void network::distortNetwork()
{
    if(degreeOfDistortion>0)
    {
        for (int i = 0; i < Nx; ++i)
            for (int j = 0; j < Ny; ++j)
                for (int k = 0; k < Nz; ++k)
                {
                    node* n=getNode(i,j,k);
                    n->setXCoordinate(n->getXCoordinate()+length*degreeOfDistortion*(-1+2*uniform_real()));
                    n->setYCoordinate(n->getYCoordinate()+length*degreeOfDistortion*(-1+2*uniform_real()));
                    if(Nz!=1)
                    n->setZCoordinate(n->getZCoordinate()+length*degreeOfDistortion*(-1+2*uniform_real()));
                }

        //update pores' lengths
        for (int i = 0; i < totalPores; ++i)
        {
            pore* p=getPore(i);
            if(!p->getClosed())
            {
                if(p->getNodeIn()==0 || p->getNodeOut()==0)continue;
                double length=sqrt(pow(p->getNodeIn()->getXCoordinate()-p->getNodeOut()->getXCoordinate(),2)+pow(p->getNodeIn()->getYCoordinate()-p->getNodeOut()->getYCoordinate(),2)+pow(p->getNodeIn()->getZCoordinate()-p->getNodeOut()->getZCoordinate(),2));
                p->setFullLength(length);
                p->setLength(length-p->getNodeInLength()-p->getNodeOutLength()>0?length-p->getNodeInLength()-p->getNodeOutLength():length/2);//if(p->getLength()<0)cout<<"sdsads"<<endl;
            }
        }
    }
}

void network::assignShapeFactors()
{
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
            p->setShapeFactor(shapeFactor);
    }

    if(networkSource==2)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
            n->setShapeFactor(shapeFactor);

    }

    assignShapeFactorConstants();
}

void network::assignShapeFactorConstants()
{
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            if(p->getShapeFactor()<=sqrt(3)/36.)
                p->setShapeFactorConstant(0.6);
            else if (p->getShapeFactor()<=1./16.)
                p->setShapeFactorConstant(0.5623);
            else
                p->setShapeFactorConstant(0.5);
        }
    }

    if(networkSource==2 || networkSource==3)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
        {
            if(n->getShapeFactor()<=sqrt(3)/36.)
                n->setShapeFactorConstant(0.6);
            else if (n->getShapeFactor()<=1./16.)
                n->setShapeFactorConstant(0.5623);
            else
                n->setShapeFactorConstant(0.5);
        }
    }
}

void network::assignVolumes()
{
    totalPoresVolume=0;
    totalNodesVolume=0;
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            double volume=pow(poreVolumeConstant,2-poreVolumeExponent)*p->getLength()*pow(p->getRadius(),poreVolumeExponent)/(4*p->getShapeFactor())*pow(10,(6*poreVolumeExponent-12));
            p->setVolume(volume);
            p->setEffectiveVolume(volume);
            totalPoresVolume+=volume;
        }
    }

    if(networkSource==2 || networkSource==3)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
        {
            double volume=pow(poreVolumeConstant,2-poreVolumeExponent)*n->getLength()*pow(n->getRadius(),poreVolumeExponent)/(4*n->getShapeFactor())*pow(10,(6*poreVolumeExponent-12));
            n->setVolume(volume);
            n->setEffectiveVolume(volume);
            totalNodesVolume+=volume;
        }
    }
}

void network::assignConductivities()
{
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            node* nodeIn=p->getNodeIn();
            node* nodeOut=p->getNodeOut();
            double throatConductivityInverse(0),nodeInConductivityInverse(0),nodeOutConductivityInverse(0);

            throatConductivityInverse=1/(p->getShapeFactorConstant()*pow(p->getRadius(),4)/(16*p->getShapeFactor())/(p->getViscosity()*p->getLength()));

            if(nodeIn!=0)
                nodeInConductivityInverse=1/(nodeIn->getShapeFactorConstant()*pow(nodeIn->getRadius(),4)/(16*nodeIn->getShapeFactor())/(nodeIn->getViscosity()*p->getNodeInLength()));
            if(nodeOut!=0)
                nodeOutConductivityInverse=1/(nodeOut->getShapeFactorConstant()*pow(nodeOut->getRadius(),4)/(16*nodeOut->getShapeFactor())/(nodeOut->getViscosity()*p->getNodeOutLength()));

            p->setConductivity(1./(throatConductivityInverse+nodeInConductivityInverse+nodeOutConductivityInverse));

        }
    }
}

void network::assignWettability()
{
    if(wettingTypeFlag==1) //WW
    {
        for (int i = 0; i < totalElements; ++i)
        {
            element* e=getElement(i);
            if(!e->getClosed())
            {
                e->setTheta(uniform_real(minWaterWetTheta,maxWaterWetTheta));
                e->setWettabilityFlag(wettability::waterWet);
            }
        }
    }

    if(wettingTypeFlag==2) //OW
    {
        for (int i = 0; i < totalElements; ++i)
        {
            element* e=getElement(i);
            if(!e->getClosed())
            {
                e->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                e->setWettabilityFlag(wettability::oilWet);
            }
        }
    }

    if(wettingTypeFlag==3) //FW
    {
        for (int i = 0; i < totalElements; ++i)
        {
            element* p=getElement(i);
            if(!p->getClosed())
            {
                p->setTheta(uniform_real(minWaterWetTheta,maxWaterWetTheta));
                p->setWettabilityFlag(wettability::waterWet);
            }
        }

        int  oilWetSoFar=0;
        while((double(oilWetSoFar)/double(totalOpenedNodes))<oilWetFraction)
        {
            int index=uniform_int(0,totalNodes-1);
            node* p=getNode(index);
            if(!p->getClosed() && p->getWettabilityFlag()!=wettability::oilWet)
                {
                    p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                    p->setWettabilityFlag(wettability::oilWet);
                    oilWetSoFar++;
                }
        }

        for (int i = 0; i < totalPores; ++i)
        {
            pore* p=getPore(i);
            if(!p->getClosed())
            {
                if(p->getNeighboors().size()==1)
                {
                    element* connectedNode=p->getNeighboors()[0];
                    p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                    p->setTheta(connectedNode->getTheta());
                }
                else
                {
                    element* connectedNode1=p->getNeighboors()[0];
                    element* connectedNode2=p->getNeighboors()[1];
                    if(connectedNode1->getWettabilityFlag()==connectedNode2->getWettabilityFlag())
                    {
                        p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                        p->setTheta(connectedNode1->getTheta());
                    }
                    else
                    {
                        int dice=uniform_int();
                        if(dice==0)
                        {
                            p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                            p->setTheta(connectedNode1->getTheta());
                        }
                        else
                        {
                            p->setWettabilityFlag(connectedNode2->getWettabilityFlag());
                            p->setTheta(connectedNode2->getTheta());
                        }
                    }

                }
            }
        }
    }

    if(wettingTypeFlag==4) //MWS
        {
            for (int i = 0; i < totalElements; ++i)
            {
                element* p=getElement(i);
                if(!p->getClosed())
                {
                    p->setTheta(uniform_real(minWaterWetTheta,maxWaterWetTheta));
                    p->setWettabilityFlag(wettability::waterWet);
                }
            }

            int  oilWetSoFar=0;
            double currentRadius=minNodeRadius;
            double radiusStep=(maxNodeRadius-minNodeRadius)/100;

            while((double(oilWetSoFar)/double(totalOpenedNodes))<oilWetFraction)
            {
                currentRadius+=radiusStep;
                for (int i = 0; i < totalNodes; ++i)
                {
                    element* p=getNode(i);
                    if(!p->getClosed() && p->getRadius()<=currentRadius && p->getWettabilityFlag()!=wettability::oilWet)
                    {
                        p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                        p->setWettabilityFlag(wettability::oilWet);
                        oilWetSoFar++;
                    }
                }
            }

            for (int i = 0; i < totalPores; ++i)
            {
                pore* p=getPore(i);
                if(!p->getClosed())
                {
                    if(p->getNeighboors().size()==1)
                    {
                        element* connectedNode=p->getNeighboors()[0];
                        p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                        p->setTheta(connectedNode->getTheta());
                    }
                    else
                    {
                        element* connectedNode1=p->getNeighboors()[0];
                        element* connectedNode2=p->getNeighboors()[1];
                        if(connectedNode1->getWettabilityFlag()==connectedNode2->getWettabilityFlag())
                        {
                            p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                            p->setTheta(connectedNode1->getTheta());
                        }
                        else
                        {
                            int dice=uniform_int();
                            if(dice==0)
                            {
                                p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                                p->setTheta(connectedNode1->getTheta());
                            }
                            else
                            {
                                p->setWettabilityFlag(connectedNode2->getWettabilityFlag());
                                p->setTheta(connectedNode2->getTheta());
                            }
                        }

                    }
                }
            }
        }

        if(wettingTypeFlag==5) //MWL
        {
            for (int i = 0; i < totalElements; ++i)
            {
                element* p=getElement(i);
                if(!p->getClosed())
                {
                    p->setTheta(uniform_real(minWaterWetTheta,maxWaterWetTheta));
                    p->setWettabilityFlag(wettability::waterWet);
                }
            }

            int  oilWetSoFar=0;
            double currentRadius=maxNodeRadius;
            double radiusStep=(maxNodeRadius-minNodeRadius)/100;

            while((double(oilWetSoFar)/double(totalOpenedNodes))<oilWetFraction)
            {
                currentRadius-=radiusStep;
                for (int i = 0; i < totalNodes; ++i)
                {
                    element* p=getNode(i);
                    if(!p->getClosed() && p->getRadius()>=currentRadius && p->getWettabilityFlag()!=wettability::oilWet)
                    {
                        p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                        p->setWettabilityFlag(wettability::oilWet);
                        oilWetSoFar++;
                    }
                }
            }

            for (int i = 0; i < totalPores; ++i)
            {
                pore* p=getPore(i);
                if(!p->getClosed())
                {
                    if(p->getNeighboors().size()==1)
                    {
                        element* connectedNode=p->getNeighboors()[0];
                        p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                        p->setTheta(connectedNode->getTheta());
                    }
                    else
                    {
                        element* connectedNode1=p->getNeighboors()[0];
                        element* connectedNode2=p->getNeighboors()[1];
                        if(connectedNode1->getWettabilityFlag()==connectedNode2->getWettabilityFlag())
                        {
                            p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                            p->setTheta(connectedNode1->getTheta());
                        }
                        else
                        {
                            int dice=uniform_int();
                            if(dice==0)
                            {
                                p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                                p->setTheta(connectedNode1->getTheta());
                            }
                            else
                            {
                                p->setWettabilityFlag(connectedNode2->getWettabilityFlag());
                                p->setTheta(connectedNode2->getTheta());
                            }
                        }

                    }
                }
            }
        }
        backupWettabilityPT();
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

void network::assignWWWettabilityPT(double theta)
{
    for(int i=0;i<totalElements;++i)
    {
        element* e=getElement(i);
        if(!e->getClosed())
        {
            e->setTheta(theta);
            e->setWettabilityFlag(wettability::waterWet);
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
        if(!e->getClosed() && e->getPhaseFlag()==phase::oil)
        {
            e->setTheta(wettabiltyThetaBackup[e]);
            if(e->getTheta()<tools::pi()/2)
            {
                e->setWettabilityFlag(wettability::waterWet);
            }
            else
            {
                e->setWettabilityFlag(wettability::oilWet);
                e->setWaterCornerActivated(false);
            }
        }
    }
    clusterWaterWetElements();
    clusterOilWetElements();
}

void network::assignHalfAngles()
{
    ofstream file("Results/halfAngles.txt");
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            double beta1(0), beta2(0),beta3(0);
            if(p->getShapeFactor()<=sqrt(3)/36.)
            {
                double beta2Min=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3+4*tools::pi()/3));
                double beta2Max=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3));
                beta2=uniform_real(beta2Min,beta2Max);
                beta1=-0.5*beta2+0.5*asin((tan(beta2)+4*p->getShapeFactor())/(tan(beta2)-4*p->getShapeFactor())*sin(beta2));
                beta3=tools::pi()/2-beta1-beta2;
            }
            p->setBeta1(beta1);
            p->setBeta2(beta2);
            p->setBeta3(beta3);
            file<<p->getId()<<" "<<p->getBeta1()<<" "<<p->getBeta2()<<" "<<p->getBeta3()<<endl;
        }
    }

    if(networkSource==2 || networkSource==3)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* p=getNode(i);
        if(!p->getClosed())
        {
            double beta1(0), beta2(0),beta3(0);
            if(p->getShapeFactor()<=sqrt(3)/36.)
            {
                double beta2Min=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3+4*tools::pi()/3));
                double beta2Max=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3));
                beta2=uniform_real(beta2Min,beta2Max);
                beta1=-0.5*beta2+0.5*asin((tan(beta2)+4*p->getShapeFactor())/(tan(beta2)-4*p->getShapeFactor())*sin(beta2));
                beta3=tools::pi()/2-beta1-beta2;
            }
            p->setBeta1(beta1);
            p->setBeta2(beta2);
            p->setBeta3(beta3);
            file<<p->getId()<<" "<<p->getBeta1()<<" "<<p->getBeta2()<<" "<<p->getBeta3()<<endl;
        }
    }
}

void network::assignGeneralProperties()
{
    totalElementsVolume=0;
    totalPoresVolume=0;
    totalNodesVolume=0;
    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
        {
            totalElementsVolume+=n->getVolume();
            totalNodesVolume+=n->getVolume();
        }
    }

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            totalElementsVolume+=p->getVolume();
            totalPoresVolume+=p->getVolume();
        }
    }
}

}
