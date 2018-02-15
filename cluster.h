/////////////////////////////////////////////////////////////////////////////
/// Name:        cluster.h
/// Purpose:     attributes and methods of clusters (phase clusters,
///              wettability clusters) assigend to elements are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <map>

namespace PNM {

class cluster
{
public:
    cluster(int);

    bool getInlet() const;
    void setInlet(bool value);

    bool getOutlet() const;
    void setOutlet(bool value);

    bool getSpanning() const;
    void setSpanning(bool value);

    double getVolume() const;
    void setVolume(double value);

    int getId() const;
    void setId(int value);

    int getSize() const;
    void setSize(int value);

    int getPoreId() const;
    void setPoreId(int value);

private:
    int id;
    bool inlet;
    bool outlet;
    bool spanning;
    double volume;
    int size;
    int poreId;
};

}

#endif // CLUSTER_H
