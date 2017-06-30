/////////////////////////////////////////////////////////////////////////////
/// Name:        cluster.h
/// Purpose:     attributes and methods of clusters (phase clusters,
///              wettability clusters) assigend to elements are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <map>

class cluster
{
public:
    cluster(int);
    int getLabel() const;
    void setLabel(int);

    bool getInlet() const;
    void setInlet(bool value);

    bool getOutlet() const;
    void setOutlet(bool value);

    bool getSpanning() const;
    void setSpanning(bool value);

    int getId() const;
    void setId(int value);

    char getPhase() const;
    void setPhase(char value);

private:
    int id;
    int label;
    bool inlet;
    bool outlet;
    bool spanning;
    char phase;
};

#endif // CLUSTER_H
