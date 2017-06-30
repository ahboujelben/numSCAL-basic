/////////////////////////////////////////////////////////////////////////////
/// Name:        hoshenKopelmann.cpp
/// Purpose:     methods related to Hoshen-Kopelmann clustering algorithms
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

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

void network::clusterPores(cluster*(pore::*getter)(void) const,void(pore::*setter)(cluster*),char(pore::*status)(void) const,char flag, vector<cluster*> &clustersList)
{
    if(!clustersList.empty())
        for (unsigned i = 0; i < clustersList.size(); ++i)
            delete clustersList[i];
    clustersList.clear();

    for(int i=0; i<totalPores; ++i)
    {
        pore* p=getPore(i);
        if((p->*status)()==flag)
            p->setClusterTemp(0);
    }

    vector<int> labels;
    labels.push_back(0);


    for(int i=0; i<totalPores;++i)
    {
        pore* p=getPore(i);
        if((p->*status)()==flag)
        {
            vector<int> neighboorsClusters;
            vector<pore*> neighboors=p->getNeighboors();
            for(unsigned j=0;j<neighboors.size();j++)
            {
                if((neighboors[j]->*status)()==flag && neighboors[j]->getClusterTemp()!=0)
                    neighboorsClusters.push_back(neighboors[j]->getClusterTemp());
            }
            if(neighboorsClusters.empty())
                p->setClusterTemp(hkMakeSet(labels));
            else if(neighboorsClusters.size()==1)
                p->setClusterTemp(neighboorsClusters[0]);
            else
                p->setClusterTemp(hkUnion(neighboorsClusters,labels));
        }
    }
      /* apply the relabeling to the matrix */
      /* Create a mapping from the canonical labels determined by union/find into a new set of canonical labels, which are guaranteed to be sequential. */

        vector<int> new_labels(labels.size(),0);

        for(int i=0; i<totalPores; ++i)
        {
            pore* p=getPore(i);
            if((p->*status)()==flag)
            {
                int x=hkFind(p->getClusterTemp(),labels);
                if (new_labels[x] == 0)
                {
                    new_labels[0]++;
                    new_labels[x] = new_labels[0];
                    clustersList.push_back(new cluster(new_labels[0]));
                }
                (p->*setter)(clustersList[new_labels[x]-1]);
            }
        }

        //Identify sepecial clusters
        set<cluster*> inletClusters,outletClusters,spanningClusters;

        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if((p->*status)()==flag && p->getInlet())
                inletClusters.insert((p->*getter)());
            if((p->*status)()==flag && p->getOutlet())
                outletClusters.insert((p->*getter)());
        }
        for (set<cluster*>::iterator iterator = inletClusters.begin(); iterator != inletClusters.end(); ++iterator) {
            (*iterator)->setInlet(true);
        }
        for (set<cluster*>::iterator iterator = outletClusters.begin(); iterator != outletClusters.end(); ++iterator) {
            (*iterator)->setOutlet(true);
        }
        std::set_intersection( inletClusters.begin(), inletClusters.end(), outletClusters.begin(), outletClusters.end(), std::inserter( spanningClusters, spanningClusters.begin() ) );
        for (set<cluster*>::iterator iterator = spanningClusters.begin(); iterator != spanningClusters.end(); ++iterator) {
            (*iterator)->setSpanning(true);
        }
}

void network::clusterWaterPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterWater;
    void (pore::*setter)(cluster*) =&pore::setClusterWater;
    char (pore::*status)(void) const=&pore::getPhaseFlag;
    clusterPores(getter,setter,status,'w',waterClusters);

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

void network::clusterOilPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterOil;
    void (pore::*setter)(cluster*) =&pore::setClusterOil;
    char (pore::*status)(void) const=status=&pore::getPhaseFlag;
    clusterPores(getter,setter,status,'o',oilClusters);

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

void network::clusterGasPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterGas;
    void (pore::*setter)(cluster*) =&pore::setClusterGas;
    char (pore::*status)(void) const=&pore::getPhaseFlag;
    clusterPores(getter,setter,status,'g',gasClusters);
    isGasSpanning=false;
    for(unsigned i=0;i<gasClusters.size();++i)
    {
        cluster* cls=gasClusters[i];
        if(cls->getSpanning())
        {
            isGasSpanning=true;
            break;
        }
    }
}

void network::clusterOilGasPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterOilOrGas;
    void (pore::*setter)(cluster*) =&pore::setClusterOilOrGas;
    char (pore::*status)(void) const=status=&pore::getOilOrGas;
    clusterPores(getter,setter,status,'t',oilGasClusters);
}

void network::clusterWaterGasPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterWaterOrGas;
    void (pore::*setter)(cluster*) =&pore::setClusterWaterOrGas;
    char (pore::*status)(void) const=status=&pore::getWaterOrGas;
    clusterPores(getter,setter,status,'t',waterGasClusters);
}

void network::clusterWaterWetPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterWaterWet;
    void (pore::*setter)(cluster*) =&pore::setClusterWaterWet;
    char (pore::*status)(void) const=&pore::getWettabilityFlag;
    clusterPores(getter,setter,status,'w',waterWetClusters);
}

void network::clusterOilWetPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterOilWet;
    void (pore::*setter)(cluster*) =&pore::setClusterOilWet;
    char (pore::*status)(void) const=&pore::getWettabilityFlag;
    clusterPores(getter,setter,status,'o',oilWetClusters);
}

void network::clusterWaterFilmPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterWaterFilm;
    void (pore::*setter)(cluster*) =&pore::setClusterWaterFilm;
    char (pore::*status)(void) const=&pore::getWaterFlowingViaFilm;
    clusterPores(getter,setter,status,'t',waterFilmClusters);
}

void network::clusterOilFilmPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterOilFilm;
    void (pore::*setter)(cluster*) =&pore::setClusterOilFilm;
    char (pore::*status)(void) const=&pore::getOilFlowingViaFilm;
    clusterPores(getter,setter,status,'t',oilFilmClusters);
}

void network::clusterWaterLayerPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterWaterLayer;
    void (pore::*setter)(cluster*) =&pore::setClusterWaterLayer;
    char (pore::*status)(void) const=&pore::getWaterFlowingViaLayer;
    clusterPores(getter,setter,status,'t',waterLayerClusters);
}

void network::clusterOilLayerPores()
{
    cluster* (pore::*getter)() const =&pore::getClusterOilLayer;
    void (pore::*setter)(cluster*) =&pore::setClusterOilLayer;
    char (pore::*status)(void) const=&pore::getOilFlowingViaLayer;
    clusterPores(getter,setter,status,'t',oilLayerClusters);
}

void network::clusterEverything()
{
    cluster* (pore::*getter)() const =&pore::getClusterExist;
    void (pore::*setter)(cluster*) =&pore::setClusterExist;
    char (pore::*status)(void) const=&pore::getExist;
    clusterPores(getter,setter,status,'t',existClusters);

    isNetworkSpanning=false;
    for(unsigned i=0;i<existClusters.size();++i)
    {
        cluster* cls=existClusters[i];
        if(cls->getSpanning())
        {
            isNetworkSpanning=true;
            break;
        }
    }
}

void network::clusterEverythingEverything()
{
    cluster* (element::*getter)() const =&element::getClusterExist;
    void (element::*setter)(cluster*) =&element::setClusterExist;
    char (element::*status)(void) const=&element::getExist;
    clusterElements(getter,setter,status,'t',existClusters);

    isNetworkSpanning=false;
    for(unsigned i=0;i<existClusters.size();++i)
    {
        cluster* cls=existClusters[i];
        if(cls->getSpanning())
        {
            isNetworkSpanning=true;
            break;
        }
    }
}

void network::clusterElements(cluster *(element::*getter)() const, void (element::*setter)(cluster *), char (element::*status)() const, char flag, std::vector<cluster *> &clustersList)
{
    if(!clustersList.empty())
        for (unsigned i = 0; i < clustersList.size(); ++i)
            delete clustersList[i];
    clustersList.clear();

    for(int i=0; i<totalElements; ++i)
       if((getElement(i)->*status)()==flag)
           getElement(i)->setClusterTemp(0);

    vector<int> labels;
    labels.push_back(0);


    for(int i=0; i<totalElements;++i)
    {

        element* e=getElement(i);
        if((e->*status)()==flag)
        {
            vector<int> neighboorsClusters;
            vector<element*> neighboors=e->getNeighs();
            for(unsigned j=0;j<neighboors.size();j++)
            {
                if((neighboors[j]->*status)()==flag && neighboors[j]->getClusterTemp()!=0)
                    neighboorsClusters.push_back(neighboors[j]->getClusterTemp());
            }
            if(neighboorsClusters.empty())
                e->setClusterTemp(hkMakeSet(labels));
            else if(neighboorsClusters.size()==1)
                e->setClusterTemp(neighboorsClusters[0]);
            else
                e->setClusterTemp(hkUnion(neighboorsClusters,labels));
        }
    }
      /* apply the relabeling to the matrix */
      /* Create a mapping from the canonical labels determined by union/find into a new set of canonical labels, which are guaranteed to be sequential. */

        vector<int> new_labels(labels.size(),0);

        for(int i=0; i<totalElements; ++i)
        {
            if((getElement(i)->*status)()==flag)
            {
                int x=hkFind(getElement(i)->getClusterTemp(),labels);
                if (new_labels[x] == 0)
                {
                    new_labels[0]++;
                    new_labels[x] = new_labels[0];
                    clustersList.push_back(new cluster(new_labels[0]));
                }
                (getElement(i)->*setter)(clustersList[new_labels[x]-1]);
                //getPore(i)->setClusterTemp(new_labels[x]);
            }
        }

        //Identify sepecial clusters
        set<cluster*> inletClusters,outletClusters,spanningClusters;

        for(int i=0;i<totalElements;++i)
        {
            element* e=getElement(i);

            if(e->getType()==1)
            {
                if((e->*status)()==flag && e->getInlet())
                    inletClusters.insert((e->*getter)());
                if((e->*status)()==flag && e->getOutlet())
                    outletClusters.insert((e->*getter)());
            }
        }
        for (set<cluster*>::iterator iterator = inletClusters.begin(); iterator != inletClusters.end(); ++iterator) {
            (*iterator)->setInlet(true);
        }
        for (set<cluster*>::iterator iterator = outletClusters.begin(); iterator != outletClusters.end(); ++iterator) {
            (*iterator)->setOutlet(true);
        }
        std::set_intersection( inletClusters.begin(), inletClusters.end(), outletClusters.begin(), outletClusters.end(), std::inserter( spanningClusters, spanningClusters.begin() ) );
        for (set<cluster*>::iterator iterator = spanningClusters.begin(); iterator != spanningClusters.end(); ++iterator) {
            (*iterator)->setSpanning(true);
        }
}

void network::clusterWaterWetElements()
{
    cluster* (element::*getter)() const =&element::getClusterWaterWet;
    void (element::*setter)(cluster*) =&element::setClusterWaterWet;
    char (element::*status)(void) const=&element::getWettabilityFlag;
    clusterElements(getter,setter,status,'w',waterWetClusters);
}

void network::clusterOilWetElements()
{
    cluster* (element::*getter)() const =&element::getClusterOilWet;
    void (element::*setter)(cluster*) =&element::setClusterOilWet;
    char (element::*status)(void) const=&element::getWettabilityFlag;
    clusterElements(getter,setter,status,'o',oilWetClusters);
}

void network::clusterWaterElements()
{
    cluster* (element::*getter)() const =&element::getClusterWater;
    void (element::*setter)(cluster*) =&element::setClusterWater;
    char (element::*status)(void) const=&element::getPhaseFlag;
    clusterElements(getter,setter,status,'w',waterClusters);
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
    char (element::*status)(void) const=&element::getPhaseFlag;
    clusterElements(getter,setter,status,'o',oilClusters);
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

void network::clusterGasElements()
{
    cluster* (element::*getter)() const =&element::getClusterGas;
    void (element::*setter)(cluster*) =&element::setClusterGas;
    char (element::*status)(void) const=&element::getPhaseFlag;
    clusterElements(getter,setter,status,'g',gasClusters);
    isGasSpanning=false;
    for(unsigned i=0;i<gasClusters.size();++i)
    {
        cluster* cls=gasClusters[i];
        if(cls->getSpanning())
        {
            isGasSpanning=true;
            break;
        }
    }
}

void network::clusterOilGasElements()
{
    cluster* (element::*getter)() const =&element::getClusterOilOrGas;
    void (element::*setter)(cluster*) =&element::setClusterOilOrGas;
    char (element::*status)(void) const=status=&element::getOilOrGas;
    clusterElements(getter,setter,status,'t',oilGasClusters);
}

void network::clusterWaterGasElements()
{
    cluster* (element::*getter)() const =&element::getClusterWaterOrGas;
    void (element::*setter)(cluster*) =&element::setClusterWaterOrGas;
    char (element::*status)(void) const=status=&element::getWaterOrGas;
    clusterElements(getter,setter,status,'t',waterGasClusters);
}

void network::clusterWaterFilmElements()
{
    cluster* (element::*getter)() const =&element::getClusterWaterFilm;
    void (element::*setter)(cluster*) =&element::setClusterWaterFilm;
    char (element::*status)(void) const=&element::getWaterFlowingViaFilm;
    clusterElements(getter,setter,status,'t',waterFilmClusters);
}

void network::clusterOilFilmElements()
{
    cluster* (element::*getter)() const =&element::getClusterOilFilm;
    void (element::*setter)(cluster*) =&element::setClusterOilFilm;
    char (element::*status)(void) const=&element::getOilFlowingViaFilm;
    clusterElements(getter,setter,status,'t',oilFilmClusters);
}

void network::clusterWaterLayerElements()
{
    cluster* (element::*getter)() const =&element::getClusterWaterLayer;
    void (element::*setter)(cluster*) =&element::setClusterWaterLayer;
    char (element::*status)(void) const=&element::getWaterFlowingViaLayer;
    clusterElements(getter,setter,status,'t',waterLayerClusters);
}

void network::clusterOilLayerElements()
{
    cluster* (element::*getter)() const =&element::getClusterOilLayer;
    void (element::*setter)(cluster*) =&element::setClusterOilLayer;
    char (element::*status)(void) const=&element::getOilFlowingViaLayer;
    clusterElements(getter,setter,status,'t',oilLayerClusters);
}

void network::clusterAllPores()
{
    clusterOilPores();
    clusterWaterPores();
    clusterGasPores();
    clusterOilFilmPores();
    clusterWaterFilmPores();
    if(oilSpreading)clusterOilLayerPores();
    if(waterSpreading)clusterWaterLayerPores();

    allClusters.clear();

    int clsNum=0;
    for (unsigned i=0; i<oilClusters.size();i++)
    {
        clsNum++;
        cluster* cls=oilClusters[i];
        allClusters.push_back(cls);
        cls->setId(clsNum);
        cls->setPhase('o');
    }

    for (unsigned i=0; i<waterClusters.size();i++)
    {
        clsNum++;
        cluster* cls=waterClusters[i];
        allClusters.push_back(cls);
        cls->setId(clsNum);
        cls->setPhase('w');
    }

    for (unsigned i=0; i<gasClusters.size();i++)
    {
        clsNum++;
        cluster* cls=gasClusters[i];
        allClusters.push_back(cls);
        cls->setId(clsNum);
        cls->setPhase('g');
    }
    totalClusters=clsNum;
}

void network::clusterAllElements()
{
    clusterOilElements();
    clusterWaterElements();
    clusterGasElements();
    clusterOilFilmElements();
    clusterWaterFilmElements();
    if(oilSpreading)clusterOilLayerElements();
    if(waterSpreading)clusterWaterLayerElements();

    allClusters.clear();

    int clsNum=0;
    for (unsigned i=0; i<oilClusters.size();i++)
    {
        clsNum++;
        cluster* cls=oilClusters[i];
        allClusters.push_back(cls);
        cls->setId(clsNum);
        cls->setPhase('o');
    }

    for (unsigned i=0; i<waterClusters.size();i++)
    {
        clsNum++;
        cluster* cls=waterClusters[i];
        allClusters.push_back(cls);
        cls->setId(clsNum);
        cls->setPhase('w');
    }

    for (unsigned i=0; i<gasClusters.size();i++)
    {
        clsNum++;
        cluster* cls=gasClusters[i];
        allClusters.push_back(cls);
        cls->setId(clsNum);
        cls->setPhase('g');
    }
    totalClusters=clsNum;
}


