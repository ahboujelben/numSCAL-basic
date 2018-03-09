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

    int getIndexX() const;
    void setIndexX(int value);

    int getIndexY() const;
    void setIndexY(int value);

    int getIndexZ() const;
    void setIndexZ(int value);

    double getPressure() const;
    void setPressure(double value);

    double getXCoordinate() const;
    void setXCoordinate(double value);

    double getYCoordinate() const;
    void setYCoordinate(double value);

    double getZCoordinate() const;
    void setZCoordinate(double value);

    int getRank() const;
    void setRank(int value);

    std::vector<int> &getConnectedNodes();
    void setConnectedNodes(const std::vector<int> &value);

    std::vector<int> &getConnectedPores();
    void setConnectedPores(const std::vector<int> &value);

    int getConnectionNumber() const;
    void setConnectionNumber(int value);

private:
    int x; // relative x coordinate
    int y; // relative y coordinate
    int z; // relative z coordinate

    double xCoordinate; // absolute x coordinate
    double yCoordinate; // absolute y coordinate
    double zCoordinate; // absolute z coordinate

    int connectionNumber; //coordination number

    int rank; // solver ranking

    std::vector<int> connectedNodes; // table of connected nodes ID
    std::vector<int> connectedPores; // table of connected pores ID

    double pressure; // pressure (SI)
};

}

#endif // NODE_H
