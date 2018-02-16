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

namespace PNM {

class cluster
{
public:
    cluster(int);

    int getId() const;
    void setId(int value);

    bool getInlet() const;
    void setInlet(bool value);

    bool getOutlet() const;
    void setOutlet(bool value);

    bool getSpanning() const;
    void setSpanning(bool value);

private:
    int id; //cluster id
    bool inlet; // flag on whether the cluster is connected to the inlet
    bool outlet; // flag on whether the cluster is connected to the outlet
    bool spanning; // flag on whether the cluster is spanning
};

}

#endif // CLUSTER_H
