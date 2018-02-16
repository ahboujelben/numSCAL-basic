/////////////////////////////////////////////////////////////////////////////
/// Name:        generationExtracted.cpp
/// Purpose:     methods related to regular network construction (simple bond
///              or pore/throat netorks) are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

namespace PNM {

void network::setupExtractedModel()
{
    cout<<"Reading Extracted Network.."<<endl;
    loadExtractedNetwork();
    cout<<"Cleaning Network..."<<endl;
    cleanExtractedNetwork();
    cout<<"Assigning Half Angles..."<<endl;
    assignHalfAngles();
    cout<<"Assigning Elements..."<<endl;
    assignElements();
    cout<<"Setting Wettability..."<<endl;
    assignWettability();

    if(absolutePermeabilityCalculation)
    {
        cout<<"Absolute permeabilty calculation.."<<endl;
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

void network::loadExtractedNetwork()
{
    string line;
    maxConnectionNumber=0;
    xEdgeLength=0;
    yEdgeLength=0;
    zEdgeLength=0;
    maxNodeRadius=0;
    minNodeRadius=1e10;

    ofstream filep("Results/Network_Description/pores_radii.txt");
    ofstream filen("Results/Network_Description/nodes_radii.txt");

    // Creating Nodes
    string pathNode1=extractedNetworkFolderPath+rockPrefix+"_node1.dat";
    ifstream node1(pathNode1.c_str());

    node1>>totalNodes>>xEdgeLength>>yEdgeLength>>zEdgeLength;

    totalOpenedNodes=totalNodes;

    tableOfAllNodes.resize(totalNodes);

    getline(node1,line);

    double averageConnectionNumber(0);

    for(int i=0;i<totalNodes;++i)
    {
        int id;
        double x,y,z;
        int numberOfNeighboors;
        bool isInlet,isOutlet;



        node1>>id>>x>>y>>z>>numberOfNeighboors;

        if(numberOfNeighboors>maxConnectionNumber)maxConnectionNumber=numberOfNeighboors;

        tableOfAllNodes[i]= new node(x,y,z);
        node* n=tableOfAllNodes[i];
        n->setId(id);
        n->setAbsId(i);
        n->setConnectionNumber(numberOfNeighboors);
        averageConnectionNumber+=numberOfNeighboors;

        vector<int> neighboors;
        for(int j=0;j<numberOfNeighboors;++j)
        {
            int neighboor;
            node1>>neighboor;
            neighboors.push_back(neighboor);
        }
        n->setConnectedNodes(neighboors);

        node1>>isInlet>>isOutlet;
        n->setInlet(isInlet);
        n->setOutlet(isOutlet);

        vector<int> connectedPores;
        for(int j=0;j<numberOfNeighboors;++j)
        {
            int poreId;
            node1>>poreId;
            connectedPores.push_back(poreId);
        }
        n->setConnectedPores(connectedPores);
    }

    cout<<"Average Connection Number: "<<averageConnectionNumber/totalNodes<<endl;

    string pathNode2=extractedNetworkFolderPath+rockPrefix+"_node2.dat";
    ifstream node2(pathNode2.c_str());

    for(int i=0;i<totalNodes;++i)
    {
        int id;
        double volume,radius,shapeFactor;

        node2>>id>>volume>>radius>>shapeFactor>>line;

        if(radius>maxNodeRadius)maxNodeRadius=radius;
        if(radius<minNodeRadius)minNodeRadius=radius;

        node* n=getNode(id-1);
        n->setVolume(volume);
        n->setEffectiveVolume(volume);
        n->setRadius(radius);
        n->setShapeFactor(shapeFactor);
        n->setLength(volume/pow(radius,2)*(4*shapeFactor));

        filen<<n->getId()<<" "<<n->getRadius()<<endl;
    }

    //Creating Pores

    //override max/min radius and average length
    maxRadius=0;
    minRadius=1e10;
    length=0;

    string pathLink1=extractedNetworkFolderPath+rockPrefix+"_link1.dat";
    ifstream link1(pathLink1.c_str());

    link1>>totalPores;

    totalOpenedPores=totalPores;

    tableOfAllPores.resize(totalPores);

    getline(link1,line);

    for(int i=0;i<totalPores;++i)
    {
        int id,nodeIndex1,nodeIndex2;
        double radius,shapeFactor,poreLength;

        link1>>id>>nodeIndex1>>nodeIndex2>>radius>>shapeFactor>>poreLength;

        if(radius>maxRadius)maxRadius=radius;
        if(radius<minRadius)minRadius=radius;

        node* nodeIn;
        node* nodeOut;

        if(nodeIndex1==0 || nodeIndex2==0)
        {
            nodeIn=0;
            nodeOut=(nodeIndex1==0? getNode(nodeIndex2-1) : getNode(nodeIndex1-1));
        }

        else if(nodeIndex1==-1 || nodeIndex2==-1)
        {
            nodeOut=0;
            nodeIn=(nodeIndex1==-1? getNode(nodeIndex2-1) : getNode(nodeIndex1-1));
        }
        else
        {
            nodeOut=getNode(nodeIndex1-1);
            nodeIn=getNode(nodeIndex2-1);
        }

        tableOfAllPores[i]=new pore(nodeIn,nodeOut);
        pore* p=tableOfAllPores[i];

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
        p->setId(id);
        p->setAbsId(i+totalNodes);
        p->setShapeFactor(shapeFactor);
        p->setFullLength(poreLength);
        if(p->getNodeIn()!=0 && p->getNodeOut()!=0)
        {
            double length=sqrt(pow(p->getNodeIn()->getXCoordinate()-p->getNodeOut()->getXCoordinate(),2)+pow(p->getNodeIn()->getYCoordinate()-p->getNodeOut()->getYCoordinate(),2)+pow(p->getNodeIn()->getZCoordinate()-p->getNodeOut()->getZCoordinate(),2));
            p->setFullLength(length);
        }
        p->setRadius(radius);
        length+=poreLength;

        filep<<p->getId()<<" "<<p->getRadius()<<endl;
    }
    length=length/totalPores;

    string pathLink2=extractedNetworkFolderPath+rockPrefix+"_link2.dat";
    ifstream link2(pathLink2.c_str());

    for(int i=0;i<totalPores;++i)
    {
        int id,nodeIndex1,nodeIndex2;;
        double node1Length,node2Length,throatLength,volume;

        link2>>id>>nodeIndex1>>nodeIndex2>>node1Length>>node2Length>>throatLength>>volume>>line;

        pore* p=getPore(i);
        node* nodeIn=p->getNodeIn();

        if(nodeIndex1==0 || nodeIndex1==-1)
        {
            if(nodeIn==0)
                p->setNodeOutLength(node2Length);
            else
                p->setNodeInLength(node2Length);
        }
        else if(nodeIndex2==0 || nodeIndex2==-1)
        {
            if(nodeIn==0)
                p->setNodeOutLength(node1Length);
            else
                p->setNodeInLength(node1Length);
        }
        else
        {
            p->setNodeOutLength(node1Length);
            p->setNodeInLength(node2Length);
        }

        p->setLength(throatLength);
        p->setVolume(volume);
        p->setEffectiveVolume(volume);
    }

    assignShapeFactorConstants();
    assignConductivities();

    //setting neighboors
    setNeighboorsForExtractedModel();
}

void network::setNeighboorsForExtractedModel()
{
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

void network::cleanExtractedNetwork()
{
    //delete isolated clusters
    clusterOilPores();

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClusterOil()->getSpanning())
        {
            node* nodeIn=p->getNodeIn();
            node* nodeOut=p->getNodeOut();
            if(nodeIn!=0)nodeIn->setConnectionNumber(0);
            if(nodeOut!=0)nodeOut->setConnectionNumber(0);
            p->setClosed(true);
            p->setPhaseFlag('c');
            p->setWettabilityFlag('c');
            p->setConductivity(1e-100);
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

    //ranking for the solver
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
}

}
