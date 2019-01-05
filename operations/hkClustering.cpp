/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "hkClustering.h"
#include "network/iterator.h"
#include "network/cluster.h"

#include <set>
#include <algorithm>

namespace PNM
{

hkClustering hkClustering::instance;

hkClustering &hkClustering::get(std::shared_ptr<networkModel> network)
{
    instance.network = network;
    return instance;
}

void hkClustering::clusterWaterWetElements()
{
    cluster *(element::*getter)() const = &element::getClusterWaterWet;
    void (element::*setter)(cluster *) = &element::setClusterWaterWet;
    wettability (element::*status)(void) const = &element::getWettabilityFlag;
    clusterElements(getter, setter, status, wettability::waterWet, waterWetClusters);
}

void hkClustering::clusterOilWetElements()
{
    cluster *(element::*getter)() const = &element::getClusterOilWet;
    void (element::*setter)(cluster *) = &element::setClusterOilWet;
    wettability (element::*status)(void) const = &element::getWettabilityFlag;
    clusterElements(getter, setter, status, wettability::oilWet, oilWetClusters);
}

void hkClustering::clusterWaterElements()
{
    cluster *(element::*getter)() const = &element::getClusterWater;
    void (element::*setter)(cluster *) = &element::setClusterWater;
    phase (element::*status)(void) const = &element::getPhaseFlag;
    clusterElements(getter, setter, status, phase::water, waterClusters);

    isWaterSpanning = isSpanning(waterClusters);
}

void hkClustering::clusterOilElements()
{
    cluster *(element::*getter)() const = &element::getClusterOil;
    void (element::*setter)(cluster *) = &element::setClusterOil;
    phase (element::*status)(void) const = &element::getPhaseFlag;
    clusterElements(getter, setter, status, phase::oil, oilClusters);

    isOilSpanning = isSpanning(oilClusters);
}

void hkClustering::clusterOilConductorElements()
{
    cluster *(element::*getter)() const = &element::getClusterOilConductor;
    void (element::*setter)(cluster *) = &element::setClusterOilFilm;
    bool (element::*status)(void) const = &element::getOilConductor;
    clusterElements(getter, setter, status, true, oilFilmClusters);

    isOilSpanningThroughFilms = isSpanning(oilFilmClusters);
}

void hkClustering::clusterWaterConductorElements()
{
    cluster *(element::*getter)() const = &element::getClusterWaterConductor;
    void (element::*setter)(cluster *) = &element::setClusterWaterFilm;
    bool (element::*status)(void) const = &element::getWaterConductor;
    clusterElements(getter, setter, status, true, waterFilmClusters);

    isWaterSpanningThroughFilms = isSpanning(waterFilmClusters);
}

void hkClustering::clusterActiveElements()
{
    cluster *(element::*getter)() const = &element::getClusterActive;
    void (element::*setter)(cluster *) = &element::setClusterActive;
    bool (element::*status)(void) const = &element::getActive;
    clusterElements(getter, setter, status, true, activeClusters);

    isNetworkSpanning = isSpanning(activeClusters);
}

int hkClustering::hkFind(int x, std::vector<int> &labels)
{
    int y = x;
    while (labels[y] != y)
        y = labels[y];

    while (labels[x] != x)
    {
        int z = labels[x];
        labels[x] = y;
        x = z;
    }
    return y;
}

int hkClustering::hkUnion(std::vector<int> &v, std::vector<int> &labels)
{
    //joins equivalence classes and returns the canonical label of the resulting class.
    for (unsigned i = 1; i < v.size(); ++i)
        labels[hkFind(v[i], labels)] = hkFind(v[0], labels);
    return hkFind(v[0], labels);
}

int hkClustering::hkMakeSet(std::vector<int> &labels)
{
    //creates a new equivalence class and returns its label
    labels[0]++;
    labels.push_back(labels[0]);
    return labels[0];
}

bool hkClustering::isSpanning(const std::vector<clusterPtr> &clustersList)
{
    for (auto cls : clustersList)
    {
        if (cls->getSpanning())
        {
            return true;
        }
    }

    return false;
}

template <typename T>
void hkClustering::clusterElements(cluster *(element::*getter)() const, void (element::*setter)(cluster *), T (element::*status)() const, T flag, std::vector<clusterPtr> &clustersList)
{
    clustersList.clear();

    for (element *e : pnmRange<element>(network))
        if ((e->*status)() == flag)
            e->setClusterTemp(0);

    std::vector<int> labels;
    labels.push_back(0);

    for (element *e : pnmRange<element>(network))
    {
        if ((e->*status)() == flag)
        {
            std::vector<int> neighboorsClusters;
            std::vector<element *> &neighboors = e->getNeighboors();
            for (element *neigh : neighboors)
            {
                if ((neigh->*status)() == flag && neigh->getClusterTemp() != 0)
                    neighboorsClusters.push_back(neigh->getClusterTemp());
            }
            if (neighboorsClusters.empty())
                e->setClusterTemp(hkMakeSet(labels));
            else if (neighboorsClusters.size() == 1)
                e->setClusterTemp(neighboorsClusters[0]);
            else
                e->setClusterTemp(hkUnion(neighboorsClusters, labels));
        }
    }

    //Create a mapping from the canonical labels determined by union/find into a new set of canonical labels, which are guaranteed to be sequential.

    std::vector<int> new_labels(labels.size(), 0);

    for (element *e : pnmRange<element>(network))
    {
        if ((e->*status)() == flag)
        {
            int x = hkFind(e->getClusterTemp(), labels);
            if (new_labels[x] == 0)
            {
                new_labels[0]++;
                new_labels[x] = new_labels[0];
                clustersList.push_back(std::shared_ptr<cluster>(new cluster(new_labels[0])));
            }
            (e->*setter)(clustersList[new_labels[x] - 1].get());
        }
    }

    //Identify sepecial clusters
    std::set<cluster *> inletClusters, outletClusters, spanningClusters;

    for (pore *p : pnmInlet(network))
        if ((p->*status)() == flag)
            inletClusters.insert((p->*getter)());

    for (pore *p : pnmOutlet(network))
        if ((p->*status)() == flag)
            outletClusters.insert((p->*getter)());

    for (cluster *c : inletClusters)
        c->setInlet(true);

    for (cluster *c : outletClusters)
        c->setOutlet(true);

    std::set_intersection(inletClusters.begin(), inletClusters.end(), outletClusters.begin(), outletClusters.end(), std::inserter(spanningClusters, spanningClusters.begin()));

    for (cluster *c : spanningClusters)
        c->setSpanning(true);
}

} // namespace PNM
