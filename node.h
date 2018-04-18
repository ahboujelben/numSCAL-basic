/////////////////////////////////////////////////////////////////////////////
/// Name:        node.h
/// Purpose:     attributes and methods of a node (pore body)
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef NODE_H
#define NODE_H

#include <vector>
#include "element.h"

namespace PNM {

class node : public element
{
public:
    node(double, double, double);

    int getIndexX() const {return x;}
    void setIndexX(int value) {x=value;}

    int getIndexY() const {return y;}
    void setIndexY(int value) {y=value;}

    int getIndexZ() const {return z;}
    void setIndexZ(int value) {z=value;}

    double getXCoordinate() const override {return xCoordinate;}
    void setXCoordinate(double value) {xCoordinate=value;}

    double getYCoordinate() const override {return yCoordinate;}
    void setYCoordinate(double value) {yCoordinate=value;}

    double getZCoordinate() const override {return zCoordinate;}
    void setZCoordinate(double value) {zCoordinate=value;}

    int getConnectionNumber() const {return connectionNumber;}
    void setConnectionNumber(int value) {connectionNumber=value;}

    double getPressure() const {return pressure;}
    void setPressure(double value) {pressure=value;}

    int getRank() const {return rank;}
    void setRank(int value) {rank=value;}

    std::vector<int> &getConnectedNodes() {return connectedNodes;}
    void setConnectedNodes(const std::vector<int> &value) {connectedNodes=value;}

    std::vector<int> &getConnectedPores() {return connectedPores;}
    void setConnectedPores(const std::vector<int> &value) {connectedPores=value;}

private:
    int x; // relative x coordinate
    int y; // relative y coordinate
    int z; // relative z coordinate

    double xCoordinate; // absolute x coordinate
    double yCoordinate; // absolute y coordinate
    double zCoordinate; // absolute z coordinate

    int connectionNumber; //coordination number

    double pressure; // pressure (SI)
    int rank; // solver ranking

    std::vector<int> connectedNodes; // table of connected nodes ID
    std::vector<int> connectedPores; // table of connected pores ID
};

}

#endif // NODE_H
