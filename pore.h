/////////////////////////////////////////////////////////////////////////////
/// Name:        pore.h
/// Purpose:     attributes and methods of a pore (pore throat)
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
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
    node *getNodeIn() const {return nodeIn;}
    void setNodeIn(node *value) {nodeIn=value;}
    node *getNodeOut() const {return nodeOut;}
    void setNodeOut(node *value) {nodeOut=value;}
    node *getOtherNode(const node *other) const {return other==nodeIn ? nodeOut : nodeIn;}
    bool hasNode(const node *other) const {return other==nodeIn || other==nodeOut ? true : false;}

    double getNodeInLength() const {return nodeInLength;}
    void setNodeInLength(double value) {nodeInLength=value;}

    double getNodeOutLength() const {return nodeOutLength;}
    void setNodeOutLength(double value) {nodeOutLength=value;}

    double getFullLength() const {return fullLength;}
    void setFullLength(double value) {fullLength=value;}

    double getCapillaryPressure() const {return capillaryPressure;}
    void setCapillaryPressure(double value) {capillaryPressure=value;}

    bool getNodeInOil() const {return nodeInOil;}
    void setNodeInOil(bool value) {nodeInOil=value;}

    bool getNodeOutWater() const {return nodeOutWater;}
    void setNodeOutWater(bool value) {nodeOutWater=value;}

    bool getNodeInWater() const {return nodeInWater;}
    void setNodeInWater(bool value) {nodeInWater=value;}

    bool getNodeOutOil() const {return nodeOutOil;}
    void setNodeOutOil(bool value) {nodeOutOil=value;}

    double getXCoordinate() const;
    double getYCoordinate() const;
    double getZCoordinate() const;

    void assignConductivity(); //calculate pore conductivity

protected:

    double getMinXCoordinate() const;
    double getMaxXCoordinate() const;

    double getMinYCoordinate() const;
    double getMaxYCoordinate() const;

    double getMinZCoordinate() const;
    double getMaxZCoordinate() const;

    node *nodeIn; // node pointer at the first end of the pore
    node *nodeOut; // node pointer at the second end of the pore

    double fullLength; // distance (SI) between both connecting nodes center: fullLength=length+nodeInLength+nodeOutLength
    double nodeInLength; //distance (SI) between nodeIn center and pore extremity at nodeIn
    double nodeOutLength; //distance (SI) between nodeOut center and pore extremity at nodeOut

    // simulation related attributes

    double capillaryPressure; //capillary pressure across the pore

    bool nodeInOil; // flag on oil existence at nodeIn
    bool nodeOutWater; // flag on water existence at nodeOut
    bool nodeInWater; // flag on water existence at nodeIn
    bool nodeOutOil; // flag on oil existence at nodeOut
};

}

#endif // PORE_H
