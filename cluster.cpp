/////////////////////////////////////////////////////////////////////////////
/// Name:        cluster.cpp
/// Purpose:     attributes and methods of clusters (phase clusters,
///              wettability clusters) assigend to elements are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "cluster.h"

namespace PNM {

cluster::cluster(int pLabel)
{
    inlet=false;
    outlet=false;
    spanning=false;
    volume=0;
    size=0;
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

double cluster::getVolume() const
{
    return volume;
}

void cluster::setVolume(double value)
{
    volume = value;
}

int cluster::getId() const
{
    return id;
}

void cluster::setId(int value)
{
    id = value;
}

int cluster::getSize() const
{
    return size;
}

void cluster::setSize(int value)
{
    size = value;
}

int cluster::getPoreId() const
{
    return poreId;
}

void cluster::setPoreId(int value)
{
    poreId = value;
}

}
