/////////////////////////////////////////////////////////////////////////////
/// Name:        cluster.cpp
/// Purpose:     attributes and methods of clusters (phase clusters,
///              wettability clusters) assigend to elements are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "cluster.h"

cluster::cluster(int pLabel)
{
    label=pLabel;
    inlet=false;
    outlet=false;
    spanning=false;
}
int cluster::getId() const
{
    return id;
}
void cluster::setId(int pId)
{
    id=pId;
}

int cluster::getLabel() const
{
    return label;
}
void cluster::setLabel(int pLabel)
{
    label=pLabel;
}
bool cluster::getInlet() const
{
    return inlet;
}

void cluster::setInlet(bool value)
{
    inlet = value;
}
bool cluster::getOutlet() const
{
    return outlet;
}

void cluster::setOutlet(bool value)
{
    outlet = value;
}
bool cluster::getSpanning() const
{
    return spanning;
}

void cluster::setSpanning(bool value)
{
    spanning = value;
}

char cluster::getPhase() const
{
    return phase;
}

void cluster::setPhase(char value)
{
    phase = value;
}


































