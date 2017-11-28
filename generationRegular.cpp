/////////////////////////////////////////////////////////////////////////////
/// Name:        generationRegular.cpp
/// Purpose:     methods related to the extraction of networks from microCT
///              network files are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

void network::setupRegularModel()
{
    cout<<"Creating Nodes..."<<endl;
    createNodes();
    cout<<"Creating Pores..."<<endl;
    createPores();
    cout<<"Applying Coordination Number..."<<endl;
    applyCoordinationNumber();
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
    cout<<"Assigning Elements..."<<endl;
    assignElements();
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
    tableOfNodes.resize(Nx);
    for (int i = 0; i < Nx; ++i)
    {
        tableOfNodes[i].resize(Ny);
        for (int j = 0; j < Ny; ++j)
            tableOfNodes[i][j].resize(Nz);
    }
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz; ++k)
            {
                tableOfNodes[i][j][k]=new node(i,j,k);
                tableOfAllNodes.push_back(tableOfNodes[i][j][k]);
            }

    totalNodes=Nx*Ny*Nz;

    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        n->setId(i+1);
        if(n->getIndexX()==0)n->setInlet(true);
        if(n->getIndexX()==Nx-1)n->setOutlet(true);
        n->setXCoordinate(n->getIndexX()*length);
        n->setYCoordinate(n->getIndexY()*length);
        n->setZCoordinate(n->getIndexZ()*length);
        tableOfElements.push_back(n);
    }

    xEdgeLength=Nx*length;
    yEdgeLength=Ny*length;
    zEdgeLength=Nz*length;
    minNodeRadius=0;
    maxNodeRadius=0;

}

void network::createPores()
{
    tableOfPoresX.resize(Nx+1);
    for (int i = 0; i < Nx+1; ++i)
    {
        tableOfPoresX[i].resize(Ny+1);
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

    totalPores=3*Nx*Ny*Nz+Ny*Nz+Nx*Nz+Nx*Ny;

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
        if(p->getNodeOut()==0)
            p->setInlet(true);
        if(p->getNodeIn()==0)
            p->setOutlet(true);
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
        n->setNeighboors(neighboors);
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
            zIn->setPhaseFlag('c');
            zIn->setWettabilityFlag('c');
            zIn->setClosed(true);
            zIn->setConductivity(1e-200);
            zIn->getNodeIn()->setConnectionNumber(zIn->getNodeIn()->getConnectionNumber()-1);
            zOut->setPhaseFlag('c');
            zOut->setWettabilityFlag('c');
            zOut->setClosed(true);
            zOut->setConductivity(1e-200);
            zOut->getNodeOut()->setConnectionNumber(zOut->getNodeOut()->getConnectionNumber()-1);
        }
    for (int i = 0; i < Nx; ++i)
        for (int k = 0; k < Nz; ++k)
        {
            pore* yIn=getPoreY(i,0,k);
            pore* yOut=getPoreY(i,Ny,k);
            yIn->setPhaseFlag('c');
            yIn->setWettabilityFlag('c');
            yIn->setClosed(true);
            yIn->setConductivity(1e-200);
            yIn->getNodeIn()->setConnectionNumber(yIn->getNodeIn()->getConnectionNumber()-1);
            yOut->setPhaseFlag('c');
            yOut->setWettabilityFlag('c');
            yOut->setClosed(true);
            yOut->setConductivity(1e-200);
            yOut->getNodeOut()->setConnectionNumber(yOut->getNodeOut()->getConnectionNumber()-1);
        }


    if (coordinationNumber<6 || (coordinationNumber<4 && Nz==1))
    {
        double totalOpenedPoresSoFar=totalPores-2*Nx*Ny-2*Nx*Nz;
        int closedPoresNumber=(Nz==1? int(totalOpenedPoresSoFar*(1-coordinationNumber/4.0)) :int(totalPores*(1-coordinationNumber/6.0)));
        int i=0;
        while(i<closedPoresNumber)
        {
            int index=uniform_int(0,totalPores-1);
            pore* p=getPore(index);
            if(!p->getClosed())
            {
                p->setClosed(true);
                p->setPhaseFlag('c');
                p->setWettabilityFlag('c');
                p->setConductivity(1e-200);
                node* in=p->getNodeIn();
                node* out=p->getNodeOut();
                if(in!=0)in->setConnectionNumber(in->getConnectionNumber()-1);
                if(out!=0)out->setConnectionNumber(out->getConnectionNumber()-1);
                ++i;
            }
        }

        //Clean Network from isolated pores

        clusterOilPores();
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(!p->getClosed() && !p->getClusterOil()->getSpanning())
            {
                p->setClosed(true);
                p->setPhaseFlag('c');
                p->setWettabilityFlag('c');
                p->setConductivity(1e-200);
                node* in=p->getNodeIn();
                node* out=p->getNodeOut();
                if(in!=0)in->setConnectionNumber(in->getConnectionNumber()-1);
                if(out!=0)out->setConnectionNumber(out->getConnectionNumber()-1);
            }
        }

        //delete isolated nodes
        for(int i=0;i<totalNodes;++i)
        {
            node* n=getNode(i);
            if(n->getConnectionNumber()==0)
            {
                n->setClosed(true);
                n->setPhaseFlag('c');
                n->setWettabilityFlag('c');
            }
        }

    }

    //Ranking for the solver
    int rank=0;
    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
        {
            n->setRank(rank);
            rank++;
        }
    }

    maxConnectionNumber=(Nz==1? 4: 6);
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
                e->setWettabilityFlag('w');
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
                e->setWettabilityFlag('o');
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
                p->setWettabilityFlag('w');
            }
        }

        int  oilWetSoFar=0;
        while((double(oilWetSoFar)/double(totalOpenedNodes))<oilWetFraction)
        {
            int index=uniform_int(0,totalNodes-1);
            node* p=getNode(index);
            if(!p->getClosed() && p->getWettabilityFlag()!='o')
                {
                    p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                    p->setWettabilityFlag('o');
                    oilWetSoFar++;
                }
        }

        for (int i = 0; i < totalPores; ++i)
        {
            pore* p=getPore(i);
            if(!p->getClosed())
            {
                if(p->getNeighs().size()==1)
                {
                    element* connectedNode=p->getNeighs()[0];
                    p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                    p->setTheta(connectedNode->getTheta());
                }
                else
                {
                    element* connectedNode1=p->getNeighs()[0];
                    element* connectedNode2=p->getNeighs()[1];
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
                    p->setWettabilityFlag('w');
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
                    if(!p->getClosed() && p->getRadius()<=currentRadius && p->getWettabilityFlag()!='o')
                    {
                        p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                        p->setWettabilityFlag('o');
                        oilWetSoFar++;
                    }
                }
            }

            for (int i = 0; i < totalPores; ++i)
            {
                pore* p=getPore(i);
                if(!p->getClosed())
                {
                    if(p->getNeighs().size()==1)
                    {
                        element* connectedNode=p->getNeighs()[0];
                        p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                        p->setTheta(connectedNode->getTheta());
                    }
                    else
                    {
                        element* connectedNode1=p->getNeighs()[0];
                        element* connectedNode2=p->getNeighs()[1];
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
                    p->setWettabilityFlag('w');
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
                    if(!p->getClosed() && p->getRadius()>=currentRadius && p->getWettabilityFlag()!='o')
                    {
                        p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                        p->setWettabilityFlag('o');
                        oilWetSoFar++;
                    }
                }
            }

            for (int i = 0; i < totalPores; ++i)
            {
                pore* p=getPore(i);
                if(!p->getClosed())
                {
                    if(p->getNeighs().size()==1)
                    {
                        element* connectedNode=p->getNeighs()[0];
                        p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                        p->setTheta(connectedNode->getTheta());
                    }
                    else
                    {
                        element* connectedNode1=p->getNeighs()[0];
                        element* connectedNode2=p->getNeighs()[1];
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
        if(!e->getClosed() && e->getPhaseFlag()=='o')
        {
            e->setTheta(wettabiltyThetaBackup[e]);
            if(e->getTheta()<tools::pi()/2)
            {
                e->setWettabilityFlag('w');
            }
            else
            {
                e->setWettabilityFlag('o');
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

void network::assignElements()
{
    tableOfElements.clear();
    totalElements=0;
    totalOpenedElements=0;
    totalOpenedPores=0;
    totalOpenedNodes=0;
    totalElementsVolume=0;
    totalPoresVolume=0;
    totalNodesVolume=0;
    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        tableOfElements.push_back(n);
        n->setType(0);
        if(!n->getClosed())
        {
            totalOpenedElements++;
            totalOpenedNodes++;
            totalElementsVolume+=n->getVolume();
            totalNodesVolume+=n->getVolume();
            accessibleNodes.push_back(n);
            accessibleElements.push_back(n);
        }
        totalElements++;
    }

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        tableOfElements.push_back(p);
        p->setType(1);
        if(!p->getClosed())
        {
            totalOpenedElements++;
            totalOpenedPores++;
            totalElementsVolume+=p->getVolume();
            totalPoresVolume+=p->getVolume();
            accessiblePores.push_back(p);
            accessibleElements.push_back(p);
        }
        totalElements++;
    }

    setNeighs();
}

void network::setNeighs()
{
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        vector<element*> neighs;
        if(p->getNodeIn()!=0)neighs.push_back(p->getNodeIn());
        if(p->getNodeOut()!=0)neighs.push_back(p->getNodeOut());
        p->setNeighs(neighs);
    }

    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        vector<element*> neighs;
        const vector<int>& neighboors=n->getConnectedPores();
        for(unsigned j=0;j<neighboors.size();++j)
           neighs.push_back(getPore(neighboors[j]-1));
        n->setNeighs(neighs);
    }
}
