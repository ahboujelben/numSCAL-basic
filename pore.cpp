/////////////////////////////////////////////////////////////////////////////
/// Name:        pore.cpp
/// Purpose:     attributes and methods of a pore (pore throat)
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "pore.h"
using namespace std;

pore::pore(node *const &pNodeIn,node* const &pNodeOut)
{
    nodeIn=pNodeIn;
    nodeOut=pNodeOut;

    fullLength=0;
    nodeInLength=0;
    nodeOutLength=0;
    flow=0;
    throatConductivity=0;
    capillaryPressure=0;
}

std::vector<pore *> pore::getNeighboors() const
{
    return neighboors;
}

void pore::setNeighboors(const std::vector<pore *> &value)
{
    neighboors = value;
}

double pore::getFlow() const
{
    return flow;
}

void pore::setFlow(double value)
{
    flow = value;
}

double pore::getDeltaPViscous() const
{
    return deltaPViscous;
}

void pore::setDeltaPViscous(double value)
{
    deltaPViscous = value;
}

double pore::getNodeInLength() const
{
    return nodeInLength;
}

void pore::setNodeInLength(double value)
{
    nodeInLength = value;
}
double pore::getNodeOutLength() const
{
    return nodeOutLength;
}

void pore::setNodeOutLength(double value)
{
    nodeOutLength = value;
}

double pore::getMinXCoordinate() const
{
    if(nodeIn==0)
        return nodeOut->getXCoordinate();
    if(nodeOut==0)
        return nodeIn->getXCoordinate();
    return min(nodeIn->getXCoordinate(),nodeOut->getXCoordinate());
}

double pore::getMaxXCoordinate() const
{
    if(nodeIn==0)
        return nodeOut->getXCoordinate();
    if(nodeOut==0)
        return nodeIn->getXCoordinate();
    return max(nodeIn->getXCoordinate(),nodeOut->getXCoordinate());
}

double pore::getMinYCoordinate() const
{
    if(nodeIn==0)
        return nodeOut->getYCoordinate();
    if(nodeOut==0)
        return nodeIn->getYCoordinate();
    return min(nodeIn->getYCoordinate(),nodeOut->getYCoordinate());
}

double pore::getMaxYCoordinate() const
{
    if(nodeIn==0)
        return nodeOut->getYCoordinate();
    if(nodeOut==0)
        return nodeIn->getYCoordinate();
    return max(nodeIn->getYCoordinate(),nodeOut->getYCoordinate());
}

double pore::getMinZCoordinate() const
{
    if(nodeIn==0)
        return nodeOut->getZCoordinate();
    if(nodeOut==0)
        return nodeIn->getZCoordinate();
    return min(nodeIn->getZCoordinate(),nodeOut->getZCoordinate());
}

double pore::getMaxZCoordinate() const
{
    if(nodeIn==0)
        return nodeOut->getZCoordinate();
    if(nodeOut==0)
        return nodeIn->getZCoordinate();
    return max(nodeIn->getZCoordinate(),nodeOut->getZCoordinate());
}

double pore::getXCoordinate() const
{
    return (getMaxXCoordinate()+getMinXCoordinate())/2;
}

double pore::getYCoordinate() const
{
    return (getMaxYCoordinate()+getMinYCoordinate())/2;
}

double pore::getZCoordinate() const
{
    return (getMaxZCoordinate()+getMinZCoordinate())/2;
}

double pore::getFullLength() const
{
    return fullLength;
}

void pore::setFullLength(double value)
{
    fullLength = value;
}
double pore::getThroatConductivity() const
{
    return throatConductivity;
}

void pore::setThroatConductivity(double value)
{
    throatConductivity = value;
}

double pore::getCapillaryPressure() const
{
    return capillaryPressure;
}

void pore::setCapillaryPressure(double value)
{
    capillaryPressure = value;
}

std::map<pore *, double> &pore::getNeighboorsArea()
{
    return neighboorsArea;
}

void pore::setNeighboorsArea(const std::map<pore *, double> &value)
{
    neighboorsArea = value;
}

node *pore::getNodeIn() const
{
    return nodeIn;
}

void pore::setNodeIn(node *value)
{
    nodeIn = value;
}
node *pore::getNodeOut() const
{
    return nodeOut;
}

void pore::setNodeOut(node *value)
{
    nodeOut = value;
}



