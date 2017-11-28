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

using namespace std;

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
        n->setConnectionNumber(numberOfNeighboors);
        averageConnectionNumber+=numberOfNeighboors;

        vector<int> neighboors;
        for(int j=0;j<numberOfNeighboors;++j)
        {
            int neighboor;
            node1>>neighboor;
            neighboors.push_back(neighboor);
        }
        n->setNeighboors(neighboors);

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

        if(p->getNodeOut()==0)p->setInlet(true);
        if(p->getNodeIn()==0)p->setOutlet(true);
        p->setId(id);
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

    //extension
    if(extension)
    {
        setBoundaries();
        for(int i=0;i<extensionNumber-1;++i)
        {
            extendCustomModel('x');
            if(!extensionOneDirection)
            {
                extendCustomModel('y');
                extendCustomModel('z');
            }
        }
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
        p->setNeighboors(neighboors);
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


void network::extendCustomModel(char direction)
{
    double shiftX=xEdgeLength;
    double shiftY=yEdgeLength;
    double shiftZ=zEdgeLength;

    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);

        double newXCoordinate(n->getXCoordinate());
        double newYCoordinate(n->getYCoordinate());
        double newZCoordinate(n->getZCoordinate());

        if (direction=='x')
            newXCoordinate=2*shiftX-n->getXCoordinate();
        if (direction=='y')
            newYCoordinate=2*shiftY-n->getYCoordinate();
        if (direction=='z')
            newZCoordinate=2*shiftZ-n->getZCoordinate();

        tableOfAllNodes.push_back(new node(newXCoordinate,newYCoordinate,newZCoordinate));
        node* newNode=tableOfAllNodes[totalNodes+i];
        newNode->setId(n->getId()+totalNodes);
        newNode->setConnectionNumber(n->getConnectionNumber());

        vector<int> neighboors;
        for(unsigned j=0;j<n->getNeighboors().size();++j)
        {
            int neighboor=n->getNeighboors()[j];
            if(neighboor!=0 && neighboor!=-1)
                neighboors.push_back(neighboor+totalNodes);
            else
                neighboors.push_back(neighboor);
        }
        newNode->setNeighboors(neighboors);

        vector<int> poreNeighboors;
        for(unsigned j=0;j<n->getConnectedPores().size();++j)
        {
            int poreNeighboor=n->getConnectedPores()[j];
            poreNeighboors.push_back(poreNeighboor+totalPores);
        }
        newNode->setConnectedPores(poreNeighboors);

        newNode->setInlet(n->getInlet());
        newNode->setOutlet(n->getOutlet());
        newNode->setInletY(n->getInletY());
        newNode->setOutletY(n->getOutletY());
        newNode->setInletZ(n->getInletZ());
        newNode->setOutletZ(n->getOutletZ());
        newNode->setVolume(n->getVolume()*(1+uniform_real(-0.001,0.001)));
        newNode->setEffectiveVolume(newNode->getVolume());
        newNode->setRadius(n->getRadius()*(1+uniform_real(-0.001,0.001)));
        newNode->setLength(n->getLength()*(1+uniform_real(-0.001,0.001)));
        newNode->setShapeFactor(n->getShapeFactor()*(1+uniform_real(-0.001,0.001)));
    }

    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        node* nodeIn=p->getNodeIn();
        node* nodeOut=p->getNodeOut();
        node* newNodeIn=(nodeIn==0 ? 0 : tableOfAllNodes[nodeIn->getId()-1+totalNodes]);
        node* newNodeOut=(nodeOut==0 ? 0 : tableOfAllNodes[nodeOut->getId()-1+totalNodes]);

        tableOfAllPores.push_back(new pore(newNodeIn,newNodeOut));
        pore* newPore=tableOfAllPores[totalPores+i];

        newPore->setId(p->getId()+totalPores);
        newPore->setInlet(p->getInlet());
        newPore->setOutlet(p->getOutlet());
        newPore->setShapeFactor(p->getShapeFactor());
        newPore->setLength(p->getLength()*(1+uniform_real(-0.001,0.001)));
        newPore->setRadius(p->getRadius()*(1+uniform_real(-0.001,0.001)));
        newPore->setFullLength(p->getFullLength()*(1+uniform_real(-0.001,0.001)));
        newPore->setNodeInLength(p->getNodeInLength()*(1+uniform_real(-0.001,0.001)));
        newPore->setNodeOutLength(p->getNodeOutLength()*(1+uniform_real(-0.001,0.001)));
        newPore->setVolume(p->getVolume()*(1+uniform_real(-0.001,0.001)));
        newPore->setEffectiveVolume(newPore->getVolume());
        newPore->setClosed(p->getClosed());
    }

    stitchCustomModel(direction);
}

void network::stitchCustomModel(char direction)
{
    int newAddedPoresNumber(0);

    if(direction=='x')
    {
        for(int i=0;i<totalNodes;++i)
        {
            node* n=getNode(i);
            node* newNode=tableOfAllNodes[totalNodes+i];

            if(n->getOutlet())
            {
                n->setOutlet(false);
                newNode->setOutlet(false);
                for(unsigned j=0;j<n->getNeighboors().size();++j)
                    if(n->getNeighboors()[j]==0)
                    {
                        n->getNeighboors()[j]=newNode->getId();
                        newNode->getNeighboors()[j]=n->getId();

                        double averageRadius(0);
                        double averageShapeFactor(0);
                        double averageNodeInLength(0);
                        double averageNodeOutLength(0);
                        double averageVolume(0);
                        double neighboorsNumber(0);
                        for(unsigned k=0;k<n->getConnectedPores().size();++k)
                        {
                            pore* p=getPore(n->getConnectedPores()[k]-1);
                            averageRadius+=p->getRadius();
                            averageShapeFactor+=p->getShapeFactor();
                            averageNodeInLength+=p->getNodeInLength();
                            averageNodeOutLength+=p->getNodeOutLength();
                            averageVolume+=p->getVolume();
                            neighboorsNumber++;
                        }

                        if(neighboorsNumber!=0)
                        {
                            tableOfAllPores.push_back(new pore(newNode,n));
                            pore* poreToBeAdded=tableOfAllPores[2*totalPores+newAddedPoresNumber];

                            n->getConnectedPores()[j]=2*totalPores+newAddedPoresNumber+1;
                            newNode->getConnectedPores()[j]=2*totalPores+newAddedPoresNumber+1;

                            poreToBeAdded->setId(2*totalPores+newAddedPoresNumber+1);
                            poreToBeAdded->setRadius(averageRadius/neighboorsNumber);
                            poreToBeAdded->setShapeFactor(averageShapeFactor/neighboorsNumber);
                            poreToBeAdded->setFullLength(sqrt(pow(poreToBeAdded->getNodeIn()->getXCoordinate()-poreToBeAdded->getNodeOut()->getXCoordinate(),2)+pow(poreToBeAdded->getNodeIn()->getYCoordinate()-poreToBeAdded->getNodeOut()->getYCoordinate(),2)+pow(poreToBeAdded->getNodeIn()->getZCoordinate()-poreToBeAdded->getNodeOut()->getZCoordinate(),2)));
                            poreToBeAdded->setNodeInLength(averageNodeInLength/neighboorsNumber);
                            poreToBeAdded->setNodeOutLength(averageNodeOutLength/neighboorsNumber);
                            poreToBeAdded->setLength(poreToBeAdded->getFullLength()/3);
                            poreToBeAdded->setVolume(averageVolume/neighboorsNumber);
                            newAddedPoresNumber++;
                        }

                        break;
                    }
            }

            if(n->getInlet())
            {
                newNode->setInlet(false);
                newNode->setOutlet(true);
                for(unsigned j=0;j<n->getNeighboors().size();++j)
                    if(n->getNeighboors()[j]==-1)
                    {
                        newNode->getNeighboors()[j]=0;
                    }
            }
        }

        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            pore* newPore=tableOfAllPores[totalPores+i];

            if(p->getOutlet())
            {
                p->setOutlet(false);
                p->setClosed(true);
                p->setNodeOut(0);
                newPore->setOutlet(false);
                newPore->setNodeOut(0);
                newPore->setClosed(true);
            }

            node* tmp=newPore->getNodeIn();
            newPore->setNodeIn(newPore->getNodeOut());
            newPore->setNodeOut(tmp);
        }

        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            pore* newPore=tableOfAllPores[totalPores+i];
            if(p->getInlet())
            {
                newPore->setInlet(false);
                newPore->setOutlet(true);
            }
        }
    }

    if(direction=='y')
    {
        for(int i=0;i<totalNodes;++i)
        {
            node* n=getNode(i);
            node* newNode=tableOfAllNodes[totalNodes+i];

            if(n->getOutletY())
            {
                n->setOutletY(false);
                newNode->setOutletY(false);

                n->getNeighboors().push_back(newNode->getId());
                newNode->getNeighboors().push_back(n->getId());

                double averageRadius(0);
                double averageShapeFactor(0);
                double averageNodeInLength(0);
                double averageNodeOutLength(0);
                double averageVolume(0);
                int neighboorsNumber(0);

                for(unsigned j=0;j<n->getConnectedPores().size();++j)
                {
                    pore* p=getPore(n->getConnectedPores()[j]-1);
                    averageRadius+=p->getRadius();
                    averageShapeFactor+=p->getShapeFactor();
                    averageNodeInLength+=p->getNodeInLength();
                    averageNodeOutLength+=p->getNodeOutLength();
                    averageVolume+=p->getVolume();
                    neighboorsNumber++;
                }

                if(neighboorsNumber!=0)
                {
                    tableOfAllPores.push_back(new pore(newNode,n));
                    pore* poreToBeAdded=tableOfAllPores[2*totalPores+newAddedPoresNumber];

                    n->getConnectedPores().push_back(2*totalPores+newAddedPoresNumber+1);
                    newNode->getConnectedPores().push_back(2*totalPores+newAddedPoresNumber+1);
                    n->setConnectionNumber(n->getConnectionNumber()+1);
                    newNode->setConnectionNumber(newNode->getConnectionNumber()+1);

                    poreToBeAdded->setId(2*totalPores+newAddedPoresNumber+1);
                    poreToBeAdded->setRadius(averageRadius/neighboorsNumber);
                    poreToBeAdded->setShapeFactor(averageShapeFactor/neighboorsNumber);
                    poreToBeAdded->setFullLength(sqrt(pow(poreToBeAdded->getNodeIn()->getXCoordinate()-poreToBeAdded->getNodeOut()->getXCoordinate(),2)+pow(poreToBeAdded->getNodeIn()->getYCoordinate()-poreToBeAdded->getNodeOut()->getYCoordinate(),2)+pow(poreToBeAdded->getNodeIn()->getZCoordinate()-poreToBeAdded->getNodeOut()->getZCoordinate(),2)));
                    poreToBeAdded->setNodeInLength(averageNodeInLength/neighboorsNumber);
                    poreToBeAdded->setNodeOutLength(averageNodeOutLength/neighboorsNumber);
                    poreToBeAdded->setLength(poreToBeAdded->getFullLength()/3);
                    poreToBeAdded->setVolume(averageVolume/neighboorsNumber);

                    newAddedPoresNumber++;
                }

            }

            if(n->getInletY())
            {
                newNode->setInletY(false);
                newNode->setOutletY(true);
            }
        }

    }

    if(direction=='z')
    {
        for(int i=0;i<totalNodes;++i)
        {
            node* n=getNode(i);
            node* newNode=tableOfAllNodes[totalNodes+i];

            if(n->getOutletZ())
            {
                n->setOutletZ(false);
                newNode->setOutletZ(false);

                n->getNeighboors().push_back(newNode->getId());
                newNode->getNeighboors().push_back(n->getId());

                double averageRadius(0);
                double averageShapeFactor(0);
                double averageNodeInLength(0);
                double averageNodeOutLength(0);
                double averageVolume(0);
                int neighboorsNumber(0);

                for(unsigned j=0;j<n->getConnectedPores().size();++j)
                {
                    pore* p=getPore(n->getConnectedPores()[j]-1);
                    averageRadius+=p->getRadius();
                    averageShapeFactor+=p->getShapeFactor();
                    averageNodeInLength+=p->getNodeInLength();
                    averageNodeOutLength+=p->getNodeOutLength();
                    averageVolume+=p->getVolume();
                    neighboorsNumber++;
                }

                if(neighboorsNumber!=0)
                {
                    tableOfAllPores.push_back(new pore(newNode,n));
                    pore* poreToBeAdded=tableOfAllPores[2*totalPores+newAddedPoresNumber];

                    n->getConnectedPores().push_back(2*totalPores+newAddedPoresNumber+1);
                    newNode->getConnectedPores().push_back(2*totalPores+newAddedPoresNumber+1);
                    n->setConnectionNumber(n->getConnectionNumber()+1);
                    newNode->setConnectionNumber(newNode->getConnectionNumber()+1);

                    poreToBeAdded->setId(2*totalPores+newAddedPoresNumber+1);
                    poreToBeAdded->setRadius(averageRadius/neighboorsNumber);
                    poreToBeAdded->setShapeFactor(averageShapeFactor/neighboorsNumber);
                    poreToBeAdded->setFullLength(sqrt(pow(poreToBeAdded->getNodeIn()->getXCoordinate()-poreToBeAdded->getNodeOut()->getXCoordinate(),2)+pow(poreToBeAdded->getNodeIn()->getYCoordinate()-poreToBeAdded->getNodeOut()->getYCoordinate(),2)+pow(poreToBeAdded->getNodeIn()->getZCoordinate()-poreToBeAdded->getNodeOut()->getZCoordinate(),2)));
                    poreToBeAdded->setNodeInLength(averageNodeInLength/neighboorsNumber);
                    poreToBeAdded->setNodeOutLength(averageNodeOutLength/neighboorsNumber);
                    poreToBeAdded->setLength(poreToBeAdded->getFullLength()/3);
                    poreToBeAdded->setVolume(averageVolume/neighboorsNumber);
                    poreToBeAdded->setEffectiveVolume(poreToBeAdded->getVolume());

                    newAddedPoresNumber++;
                }
            }

            if(n->getInletZ())
            {
                newNode->setInletZ(false);
                newNode->setOutletZ(true);
            }
        }
    }

    totalPores=2*totalPores+newAddedPoresNumber;
    totalOpenedPores=totalPores;
    totalNodes*=2;
    totalOpenedNodes=totalNodes;
    if(direction=='x')xEdgeLength*=2;
    if(direction=='y')yEdgeLength*=2;
    if(direction=='z')zEdgeLength*=2;
}

void network::setBoundaries()
{
    int averageBoundryNodesNumber(0);

    for(int i=0;i<totalNodes;++i)
    {
        node* n=getNode(i);
        if(n->getInlet() || n->getOutlet())
            averageBoundryNodesNumber++;
    }

    int inletYSoFar(0),outletYSoFar(0);
    double currentY=0;
    double yStep=yEdgeLength/(cbrt(totalNodes)*10);

    while(inletYSoFar<averageBoundryNodesNumber/2)
    {
        currentY+=yStep;
        for (int i = 0; i < totalNodes; ++i)
        {
            node* n=getNode(i);
            if(n->getYCoordinate()<=currentY && !n->getInletY() && n->getNeighboors().size()!=0)
            {
                n->setInletY(true);
                inletYSoFar++;
                if(inletYSoFar>=averageBoundryNodesNumber/2)break;
            }
        }
    }

    currentY=0;
    while(outletYSoFar<averageBoundryNodesNumber/2)
    {
        currentY+=yStep;
        for (int i = 0; i < totalNodes; ++i)
        {
            node* n=getNode(i);
            if(n->getYCoordinate()>=yEdgeLength-currentY && !n->getOutletY() && n->getNeighboors().size()!=0)
            {
                n->setOutletY(true);
                outletYSoFar++;
                if(outletYSoFar>=averageBoundryNodesNumber/2)break;
            }
        }
    }

    int inletZSoFar(0),outletZSoFar(0);
    double currentZ=0;
    double zStep=zEdgeLength/(cbrt(totalNodes)*10);

    while(inletZSoFar<averageBoundryNodesNumber/2)
    {
        currentZ+=zStep;
        for (int i = 0; i < totalNodes; ++i)
        {
            node* n=getNode(i);
            if(n->getZCoordinate()<=currentZ && !n->getInletZ() && n->getNeighboors().size()!=0)
            {
                n->setInletZ(true);
                inletZSoFar++;
                if(inletZSoFar>=averageBoundryNodesNumber/2)break;
            }
        }
    }

    currentZ=0;
    while(outletZSoFar<averageBoundryNodesNumber/2)
    {
        currentZ+=zStep;
        for (int i = 0; i < totalNodes; ++i)
        {
            node* n=getNode(i);

            if(n->getZCoordinate()>=zEdgeLength-currentZ && !n->getOutletZ() && n->getNeighboors().size()!=0)
            {
                n->setOutletZ(true);
                outletZSoFar++;
                if(outletZSoFar>=averageBoundryNodesNumber/2)break;
            }
        }
    }
}
