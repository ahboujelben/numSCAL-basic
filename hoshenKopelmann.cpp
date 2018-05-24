/////////////////////////////////////////////////////////////////////////////
/// Name:        hoshenKopelmann.cpp
/// Purpose:     methods related to Hoshen-Kopelmann clustering algorithms
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"
#include "iterator.h"

namespace PNM {

//clustering

int network::hkFind(int x, vector<int>& labels)
{
    int y = x;
    while (labels[y] != y)
      y = labels[y];

    while (labels[x] != x) {
      int z = labels[x];
      labels[x] = y;
      x = z;
    }
    return y;
}

int network::hkUnion(vector<int>& v,vector<int>& labels)
{
    //joins equivalence classes and returns the canonical label of the resulting class.
    for(unsigned i=1;i<v.size();++i)
        labels[hkFind(v[i],labels)] = hkFind(v[0],labels);
    return hkFind(v[0],labels);
}

int network::hkMakeSet(vector<int>& labels)
{
    //creates a new equivalence class and returns its label
    labels[0] ++;
    labels.push_back(labels[0]);
    return labels[0];
}

template<typename T>
void network::clusterElements(cluster *(element::*getter)() const, void (element::*setter)(cluster *), T(element::*status)() const, T flag, std::vector<cluster *> &clustersList)
{
    for (cluster* c : clustersList)
        delete c;
    clustersList.clear();

    for(element* e: networkRange<element*, filter::all>(this))
       if((e->*status)()==flag)
           e->setClusterTemp(0);

    vector<int> labels;
    labels.push_back(0);

    for(element* e: networkRange<element*, filter::all>(this)){
        if((e->*status)()==flag){
            vector<int> neighboorsClusters;
            vector<element*>& neighboors=e->getNeighboors();
            for(element* neigh: neighboors){
                if((neigh->*status)()==flag && neigh->getClusterTemp()!=0)
                    neighboorsClusters.push_back(neigh->getClusterTemp());
            }
            if(neighboorsClusters.empty())
                e->setClusterTemp(hkMakeSet(labels));
            else if(neighboorsClusters.size()==1)
                e->setClusterTemp(neighboorsClusters[0]);
            else
                e->setClusterTemp(hkUnion(neighboorsClusters,labels));
        }
    }

    //Create a mapping from the canonical labels determined by union/find into a new set of canonical labels, which are guaranteed to be sequential.

    vector<int> new_labels(labels.size(),0);

    for(element* e: networkRange<element*, filter::all>(this)){
        if((e->*status)()==flag)
        {
            int x=hkFind(e->getClusterTemp(),labels);
            if (new_labels[x] == 0){
                new_labels[0]++;
                new_labels[x] = new_labels[0];
                clustersList.push_back(new cluster(new_labels[0]));
            }
            (e->*setter)(clustersList[new_labels[x]-1]);
        }
    }

    //Identify sepecial clusters
    set<cluster*> inletClusters,outletClusters,spanningClusters;

    for(pore* p: inletPores)
        if((p->*status)()==flag)
            inletClusters.insert((p->*getter)());

    for(pore* p: outletPores)
        if((p->*status)()==flag)
            outletClusters.insert((p->*getter)());


    for (cluster* c: inletClusters)
        c->setInlet(true);

    for (cluster* c: outletClusters)
        c->setOutlet(true);

    set_intersection( inletClusters.begin(), inletClusters.end(), outletClusters.begin(), outletClusters.end(), std::inserter( spanningClusters, spanningClusters.begin() ) );

    for (cluster* c: spanningClusters)
        c->setSpanning(true);
}

void network::clusterWaterWetElements()
{
    cluster* (element::*getter)() const =&element::getClusterWaterWet;
    void (element::*setter)(cluster*) =&element::setClusterWaterWet;
    wettability (element::*status)(void) const=&element::getWettabilityFlag;
    clusterElements(getter,setter,status,wettability::waterWet,waterWetClusters);
}

void network::clusterOilWetElements()
{
    cluster* (element::*getter)() const =&element::getClusterOilWet;
    void (element::*setter)(cluster*) =&element::setClusterOilWet;
    wettability (element::*status)(void) const=&element::getWettabilityFlag;
    clusterElements(getter,setter,status,wettability::oilWet,oilWetClusters);
}

void network::clusterWaterElements()
{
    cluster* (element::*getter)() const =&element::getClusterWater;
    void (element::*setter)(cluster*) =&element::setClusterWater;
    phase (element::*status)(void) const=&element::getPhaseFlag;
    clusterElements(getter,setter,status,phase::water,waterClusters);
    isWaterSpanning=false;
    for(unsigned i=0;i<waterClusters.size();++i)
    {
        cluster* cls=waterClusters[i];
        if(cls->getSpanning())
        {
            isWaterSpanning=true;
            break;
        }
    }
}

void network::clusterOilElements()
{
    cluster* (element::*getter)() const =&element::getClusterOil;
    void (element::*setter)(cluster*) =&element::setClusterOil;
    phase (element::*status)(void) const=&element::getPhaseFlag;
    clusterElements(getter,setter,status,phase::oil,oilClusters);
    isOilSpanning=false;
    for(unsigned i=0;i<oilClusters.size();++i)
    {
        cluster* cls=oilClusters[i];
        if(cls->getSpanning())
        {
            isOilSpanning=true;
            break;
        }
    }
}

void network::clusterOilFlowingElements()
{
    for_each(accessibleElements.begin(), accessibleElements.end(), [this] (element* e){
        if(e->getPhaseFlag()==phase::oil || e->getOilLayerActivated())
            e->setOilConductor(true);
        else
            e->setOilConductor(false);
    });

    cluster* (element::*getter)() const =&element::getClusterOilFilm;
    void (element::*setter)(cluster*) =&element::setClusterOilFilm;
    bool (element::*status)(void) const=&element::getOilConductor;
    clusterElements(getter,setter,status,true,oilFilmClusters);

    isOilSpanning=false;
    for(unsigned i=0;i<oilFilmClusters.size();++i)
    {
        cluster* cls=oilFilmClusters[i];
        if(cls->getSpanning())
        {
            isOilSpanning=true;
            break;
        }
    }

}

void network::clusterWaterFlowingElements()
{
    for_each(accessibleElements.begin(), accessibleElements.end(), [this] (element* e){
        if(e->getPhaseFlag()==phase::water || e->getWaterCornerActivated())
            e->setWaterConductor(true);
        else
            e->setWaterConductor(false);
    });

    cluster* (element::*getter)() const =&element::getClusterWaterFilm;
    void (element::*setter)(cluster*) =&element::setClusterWaterFilm;
    bool (element::*status)(void) const=&element::getWaterConductor;
    clusterElements(getter,setter,status,true,waterFilmClusters);

    isWaterSpanning=false;
    for(unsigned i=0;i<waterFilmClusters.size();++i)
    {
        cluster* cls=waterFilmClusters[i];
        if(cls->getSpanning())
        {
            isWaterSpanning=true;
            break;
        }
    }
}

void network::clusterActiveElements()
{
    cluster* (element::*getter)() const =&element::getClusterActive;
    void (element::*setter)(cluster*) =&element::setClusterActive;
    bool (element::*status)(void) const=&element::getActive;
    clusterElements(getter,setter,status,true,activeClusters);

    isNetworkSpanning=false;
    for(unsigned i=0;i<activeClusters.size();++i)
    {
        cluster* cls=activeClusters[i];
        if(cls->getSpanning())
        {
            isNetworkSpanning=true;
            break;
        }
    }
}

}
