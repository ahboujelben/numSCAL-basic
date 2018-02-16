/////////////////////////////////////////////////////////////////////////////
/// Name:        node.cpp
/// Purpose:     attributes and methods of a node (pore body)
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "node.h"

namespace PNM {

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
    rank=0;
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
std::vector<int> &node::getConnectedNodes()
{
    return connectedNodes;
}

void node::setConnectedNodes(const std::vector<int> &value)
{
    connectedNodes = value;
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

}

