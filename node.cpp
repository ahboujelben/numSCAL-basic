/////////////////////////////////////////////////////////////////////////////
/// Name:        node.cpp
/// Purpose:     attributes and methods of a node (pore body)
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "node.h"
using namespace std;

node::node(double X, double Y, double Z)
{
    x=X;
    y=Y;
    z=Z;
    xCoordinate=X;
    yCoordinate=Y;
    zCoordinate=Z;
    connectionNumber=6;
    pressure=0;
    inletY=false;
    outletY=false;
    inletZ=false;
    outletZ=false;
}

int node::getIndexX() const
{
    return x;
}

void node::setIndexX(int value)
{
    x = value;
}
int node::getIndexY() const
{
    return y;
}

void node::setIndexY(int value)
{
    y = value;
}
int node::getIndexZ() const
{
    return z;
}

void node::setIndexZ(int value)
{
    z= value;
}

double node::getPressure() const
{
    return pressure;
}

void node::setPressure(double value)
{
    pressure = value;
}

double node::getXCoordinate() const
{
    return xCoordinate;
}

void node::setXCoordinate(double value)
{
    xCoordinate = value;
}
double node::getYCoordinate() const
{
    return yCoordinate;
}

void node::setYCoordinate(double value)
{
    yCoordinate = value;
}
double node::getZCoordinate() const
{
    return zCoordinate;
}

void node::setZCoordinate(double value)
{
    zCoordinate = value;
}
int node::getRank() const
{
    return rank;
}

void node::setRank(int value)
{
    rank = value;
}
std::vector<int> &node::getNeighboors()
{
    return neighboors;
}

void node::setNeighboors(const std::vector<int> &value)
{
    neighboors = value;
}

int node::getConnectionNumber() const
{
    return connectionNumber;
}

void node::setConnectionNumber(int value)
{
    connectionNumber = value;
}

std::vector<int> &node::getConnectedPores()
{
    return connectedPores;
}

void node::setConnectedPores(const std::vector<int> &value)
{
    connectedPores = value;
}

bool node::getInletY() const
{
    return inletY;
}

void node::setInletY(bool value)
{
    inletY = value;
}
bool node::getOutletY() const
{
    return outletY;
}

void node::setOutletY(bool value)
{
    outletY = value;
}
bool node::getInletZ() const
{
    return inletZ;
}

void node::setInletZ(bool value)
{
    inletZ = value;
}
bool node::getOutletZ() const
{
    return outletZ;
}

void node::setOutletZ(bool value)
{
    outletZ = value;
}
double node::getNodeConductivity() const
{
    return nodeConductivity;
}

void node::setNodeConductivity(double value)
{
    nodeConductivity = value;
}


























