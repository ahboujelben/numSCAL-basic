/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "networkmodel.h"

namespace PNM {

pore *networkModel::getPore(int i) const {
  if (i < 0 || i > totalPores - 1) return nullptr;
  return tableOfPores[i].get();
}

node *networkModel::getNode(int i) const {
  if (i < 0 || i > totalNodes - 1) return nullptr;
  return tableOfNodes[i].get();
}

}  // namespace PNM
