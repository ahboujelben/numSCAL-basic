/////////////////////////////////////////////////////////////////////////////
/// Name:        pore.cpp
/// Purpose:     attributes and methods of a pore (pore throat)
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "pore.h"

namespace PNM {

using namespace std;

pore::pore(node *const &pNodeIn,node* const &pNodeOut)
{
    nodeIn=pNodeIn;
    nodeOut=pNodeOut;
    fullLength=0;
    nodeInLength=0;
    nodeOutLength=0;
    capillaryPressure=0;
}

std::vector<pore *> pore::getConnectedPores() const
{
    return connectedPores;
}

void pore::setConnectedPores(const std::vector<pore *> &value)
{
    connectedPores = value;
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

double pore::getCapillaryPressure() const
{
    return capillaryPressure;
}

void pore::setCapillaryPressure(double value)
{
    capillaryPressure = value;
}
bool pore::getNodeInOil() const
{
    return nodeInOil;
}

void pore::setNodeInOil(bool value)
{
    nodeInOil = value;
}
bool pore::getNodeOutWater() const
{
    return nodeOutWater;
}

void pore::setNodeOutWater(bool value)
{
    nodeOutWater = value;
}
bool pore::getNodeInWater() const
{
    return nodeInWater;
}

void pore::setNodeInWater(bool value)
{
    nodeInWater = value;
}
bool pore::getNodeOutOil() const
{
    return nodeOutOil;
}

void pore::setNodeOutOil(bool value)
{
    nodeOutOil = value;
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

void pore::assignConductivity()
{
    double throatConductivityInverse(0),nodeInConductivityInverse(0),nodeOutConductivityInverse(0);

    throatConductivityInverse=1/(shapeFactorConstant*pow(radius,4)/(16*shapeFactor)/(viscosity*length));

    if(nodeIn!=0)
        nodeInConductivityInverse=1/(nodeIn->getShapeFactorConstant()*pow(nodeIn->getRadius(),4)/(16*nodeIn->getShapeFactor())/(nodeIn->getViscosity()*nodeInLength));
    if(nodeOut!=0)
        nodeOutConductivityInverse=1/(nodeOut->getShapeFactorConstant()*pow(nodeOut->getRadius(),4)/(16*nodeOut->getShapeFactor())/(nodeOut->getViscosity()*nodeOutLength));

    conductivity=1./(throatConductivityInverse+nodeInConductivityInverse+nodeOutConductivityInverse);
}

}
