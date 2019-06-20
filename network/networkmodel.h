/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <memory>
#include <vector>

namespace PNM {

class pore;
class node;

struct networkModel {
  using porePtr = std::shared_ptr<pore>;
  using nodePtr = std::shared_ptr<node>;

  ///////////// Access to pores/nodes

  pore *getPore(int) const;
  node *getNode(int) const;

  ///////////// Attributes

  int totalPores;
  int totalNodes;
  int maxConnectionNumber;
  double xEdgeLength;
  double yEdgeLength;
  double zEdgeLength;
  double totalPoresVolume;
  double totalNodesVolume;
  double totalNetworkVolume;
  double inletPoresArea;
  double absolutePermeability;
  double porosity;
  double normalisedFlow;
  bool is2D;

  std::vector<porePtr> tableOfPores;
  std::vector<nodePtr> tableOfNodes;
  std::vector<pore *> inletPores;
  std::vector<pore *> outletPores;
};

}  // namespace PNM
#endif  // NETWORKMODEL_H
