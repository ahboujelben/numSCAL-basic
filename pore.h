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

#include <vector>
#include <cmath>
#include "node.h"

namespace PNM {

class pore : public element
{
public:
    pore(node *const&,node *const&);
    node *getNodeIn() const;
    void setNodeIn(node *value);
    node *getNodeOut() const;
    void setNodeOut(node *value);

    std::vector<pore *> getConnectedPores() const;
    void setConnectedPores(const std::vector<pore *> &value);

    double getNodeInLength() const;
    void setNodeInLength(double value);

    double getNodeOutLength() const;
    void setNodeOutLength(double value);

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

    //defined methods

    double getMinXCoordinate() const;
    double getMaxXCoordinate() const;

    double getMinYCoordinate() const;
    double getMaxYCoordinate() const;

    double getMinZCoordinate() const;
    double getMaxZCoordinate() const;

    double getXCoordinate() const;
    double getYCoordinate() const;
    double getZCoordinate() const;

    void assignConductivity(); //calculate pore conductivity

protected:

    node *nodeIn; // node pointer at the first end of the pore
    node *nodeOut; // node pointer at the second end of the pore

    double fullLength; // distance (SI) between both connecting nodes center: fullLength=length+nodeInLength+nodeOutLength
    double nodeInLength; //distance (SI) between nodeIn center and pore extremity at nodeIn
    double nodeOutLength; //distance (SI) between nodeOut center and pore extremity at nodeOut

    std::vector<pore*> connectedPores; // table of connected pores pointers

    // simulation related attributes

    double capillaryPressure; //capillary pressure across the pore

    bool nodeInOil; // flag on oil existence at nodeIn
    bool nodeOutWater; // flag on water existence at nodeOut
    bool nodeInWater; // flag on water existence at nodeIn
    bool nodeOutOil; // flag on oil existence at nodeOut
};

}

#endif // PORE_H
