/////////////////////////////////////////////////////////////////////////////
/// Name:        pore.h
/// Purpose:     attributes and methods of a pore (pore throat)
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
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

    double getFlow() const;
    void setFlow(double value);

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

    double getThroatConductivity() const;
    void setThroatConductivity(double value);

    double getCapillaryPressure() const;
    void setCapillaryPressure(double value);

    std::map<pore *, double> &getNeighboorsArea();
    void setNeighboorsArea(const std::map<pore *, double> &value);

protected:

    node *nodeIn;
    node *nodeOut;

    double fullLength;
    double nodeInLength;
    double nodeOutLength;
    double throatConductivity;

    double flow;

    std::vector<pore*> neighboors;
    std::map<pore*,double> neighboorsArea;

    double deltaPViscous;
    double capillaryPressure;
};

#endif // PORE_H
