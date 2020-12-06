/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "pore.h"
#include "node.h"

#include <cmath>

namespace PNM {

using namespace std;

pore::pore(node *const &pNodeIn, node *const &pNodeOut) {
  type = capillaryType::throat;
  nodeIn = pNodeIn;
  nodeOut = pNodeOut;
  fullLength = 0;
}

pore::~pore() {}

double pore::getMinXCoordinate() const {
  if (nodeIn == nullptr) return nodeOut->getXCoordinate();
  if (nodeOut == nullptr) return nodeIn->getXCoordinate();
  return min(nodeIn->getXCoordinate(), nodeOut->getXCoordinate());
}

double pore::getMaxXCoordinate() const {
  if (nodeIn == nullptr) return nodeOut->getXCoordinate();
  if (nodeOut == nullptr) return nodeIn->getXCoordinate();
  return max(nodeIn->getXCoordinate(), nodeOut->getXCoordinate());
}

double pore::getMinYCoordinate() const {
  if (nodeIn == nullptr) return nodeOut->getYCoordinate();
  if (nodeOut == nullptr) return nodeIn->getYCoordinate();
  return min(nodeIn->getYCoordinate(), nodeOut->getYCoordinate());
}

double pore::getMaxYCoordinate() const {
  if (nodeIn == nullptr) return nodeOut->getYCoordinate();
  if (nodeOut == nullptr) return nodeIn->getYCoordinate();
  return max(nodeIn->getYCoordinate(), nodeOut->getYCoordinate());
}

double pore::getMinZCoordinate() const {
  if (nodeIn == nullptr) return nodeOut->getZCoordinate();
  if (nodeOut == nullptr) return nodeIn->getZCoordinate();
  return min(nodeIn->getZCoordinate(), nodeOut->getZCoordinate());
}

double pore::getMaxZCoordinate() const {
  if (nodeIn == nullptr) return nodeOut->getZCoordinate();
  if (nodeOut == nullptr) return nodeIn->getZCoordinate();
  return max(nodeIn->getZCoordinate(), nodeOut->getZCoordinate());
}

double pore::getXCoordinate() const {
  return (getMaxXCoordinate() + getMinXCoordinate()) / 2;
}

double pore::getYCoordinate() const {
  return (getMaxYCoordinate() + getMinYCoordinate()) / 2;
}

double pore::getZCoordinate() const {
  return (getMaxZCoordinate() + getMinZCoordinate()) / 2;
}

}  // namespace PNM
