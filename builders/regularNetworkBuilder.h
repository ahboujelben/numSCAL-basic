/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef REGULARNETWORKBUILDER_H
#define REGULARNETWORKBUILDER_H

#include "networkbuilder.h"

namespace PNM {

class node;
class pore;

class regularNetworkBuilder : public networkBuilder {
  Q_OBJECT
 public:
  regularNetworkBuilder() {}
  regularNetworkBuilder(const regularNetworkBuilder &) = delete;
  regularNetworkBuilder(regularNetworkBuilder &&) = delete;
  auto operator=(const regularNetworkBuilder &)
      -> regularNetworkBuilder & = delete;
  auto operator=(regularNetworkBuilder &&) -> regularNetworkBuilder & = delete;
  virtual void make() override;
  virtual std::string getNotification() override;

 protected:
  virtual void initiateNetworkProperties();
  void createNodes();
  void createPores();
  void assignNeighboors();
  void setBoundaryConditions();
  void applyCoordinationNumber();
  void cleanNetwork();
  void assignRadii();
  void assignLengths();
  void assignPoreLengths();
  void assignNodeLengths();
  void distortNetwork();
  void assignShapeFactors();
  void assignShapeFactorConstants();
  void assignVolumes();
  void assignWettabilities();
  void calculateProperties();
  node *getNode(int, int, int);
  pore *getPoreX(int, int, int);
  pore *getPoreY(int, int, int);
  pore *getPoreZ(int, int, int);
};

}  // namespace PNM

#endif  // REGULARNETWORKBUILDER_H
