/////////////////////////////////////////////////////////////////////////////
/// Name:        pore.h
/// Purpose:     attributes and methods of a pore (pore throat)
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef PORE_H
#define PORE_H

#include <iostream>
#include <vector>
#include <cmath>
#include "node.h"
#include "tools.h"
#include "cluster.h"
#include "element.h"

namespace PNM {

class pore : public element
{
public:
    pore(node *const&,node *const&);
    node *getNodeIn() const;
    void setNodeIn(node *value);
    node *getNodeOut() const;
    void setNodeOut(node *value);

    std::vector<pore *> getNeighboors() const;
    void setNeighboors(const std::vector<pore *> &value);

    double getDeltaPViscous() const;
    void setDeltaPViscous(double value);

    double getNodeInLength() const;
    void setNodeInLength(double value);

    double getNodeOutLength() const;
    void setNodeOutLength(double value);

    double getMinXCoordinate() const;
    double getMaxXCoordinate() const;

    double getMinYCoordinate() const;
    double getMaxYCoordinate() const;

    double getMinZCoordinate() const;
    double getMaxZCoordinate() const;

    double getXCoordinate() const;
    double getYCoordinate() const;
    double getZCoordinate() const;

    double getFullLength() const;
    void setFullLength(double value);

    double getCapillaryPressure() const;
    void setCapillaryPressure(double value);

    bool getNodeInOil() const;
    void setNodeInOil(bool value);

    bool getNodeOutWater() const;
    void setNodeOutWater(bool value);

    bool getNodeInWater() const;
    void setNodeInWater(bool value);

    bool getNodeOutOil() const;
    void setNodeOutOil(bool value);

    void assignConductivity();

protected:

    node *nodeIn;
    node *nodeOut;

    double fullLength;
    double nodeInLength;
    double nodeOutLength;

    std::vector<pore*> neighboors;

    double deltaPViscous;
    double capillaryPressure;

    bool nodeInOil;
    bool nodeOutWater;
    bool nodeInWater;
    bool nodeOutOil;
};

}

#endif // PORE_H
