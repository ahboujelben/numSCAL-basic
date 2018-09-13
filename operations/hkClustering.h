/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef HKCLUSTERING_H
#define HKCLUSTERING_H

#include <memory>
#include <vector>

namespace PNM
{

class networkModel;
class cluster;
class element;

using clusterPtr = std::shared_ptr<cluster>;

class hkClustering
{
  public:
    static hkClustering &get(std::shared_ptr<networkModel>);
    void clusterWaterWetElements();
    void clusterOilWetElements();
    void clusterWaterElements();
    void clusterOilElements();
    void clusterOilConductorElements();
    void clusterWaterConductorElements();
    void clusterActiveElements();
    bool isOilSpanning;
    bool isWaterSpanning;
    bool isOilSpanningThroughFilms;
    bool isWaterSpanningThroughFilms;
    bool isNetworkSpanning;
    std::vector<clusterPtr> waterClusters;
    std::vector<clusterPtr> oilClusters;
    std::vector<clusterPtr> waterWetClusters;
    std::vector<clusterPtr> oilWetClusters;
    std::vector<clusterPtr> oilFilmClusters;
    std::vector<clusterPtr> waterFilmClusters;
    std::vector<clusterPtr> oilLayerClusters;
    std::vector<clusterPtr> waterLayerClusters;
    std::vector<clusterPtr> activeClusters;

  protected:
    hkClustering() {}
    ~hkClustering() {}
    hkClustering(const hkClustering &) = delete;
    hkClustering(hkClustering &&) = delete;
    auto operator=(const hkClustering &) -> hkClustering & = delete;
    auto operator=(hkClustering &&) -> hkClustering & = delete;
    int hkFind(int, std::vector<int> &);
    int hkUnion(std::vector<int> &, std::vector<int> &);
    int hkMakeSet(std::vector<int> &);
    template <typename T>
    void clusterElements(cluster *(element::*)(void)const, void (element::*)(cluster *), T (element::*)(void) const, T, std::vector<clusterPtr> &);
    bool isSpanning(const std::vector<clusterPtr> &);

    std::shared_ptr<networkModel> network;
    static hkClustering instance;
};

} // namespace PNM

#endif // HKCLUSTERING_H
