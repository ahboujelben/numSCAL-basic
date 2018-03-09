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
    type=capillaryType::throat;
    nodeIn=pNodeIn;
    nodeOut=pNodeOut;
    fullLength=0;
    nodeInLength=0;
    nodeOutLength=0;
    capillaryPressure=0;
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
