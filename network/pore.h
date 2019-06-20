/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef PORE_H
#define PORE_H

#include "element.h"

namespace PNM {

class node;

class pore : public element {
 public:
  explicit pore(node *const &, node *const &);
  virtual ~pore();
  pore(const pore &) = delete;
  pore(pore &&) = delete;
  auto operator=(const pore &) -> pore & = delete;
  auto operator=(pore &&) -> pore & = delete;

  node *getNodeIn() const { return nodeIn; }
  void setNodeIn(node *value) { nodeIn = value; }
  node *getNodeOut() const { return nodeOut; }
  void setNodeOut(node *value) { nodeOut = value; }
  node *getOtherNode(const node *other) const {
    return other == nodeIn ? nodeOut : nodeIn;
  }
  bool hasNode(const node *other) const {
    return other == nodeIn || other == nodeOut ? true : false;
  }

  double getFullLength() const { return fullLength; }
  void setFullLength(double value) { fullLength = value; }

  bool getNodeInOil() const { return nodeInOil; }
  void setNodeInOil(bool value) { nodeInOil = value; }

  bool getNodeOutWater() const { return nodeOutWater; }
  void setNodeOutWater(bool value) { nodeOutWater = value; }

  bool getNodeInWater() const { return nodeInWater; }
  void setNodeInWater(bool value) { nodeInWater = value; }

  bool getNodeOutOil() const { return nodeOutOil; }
  void setNodeOutOil(bool value) { nodeOutOil = value; }

  // implemented methods

  double getXCoordinate() const;
  double getYCoordinate() const;
  double getZCoordinate() const;

  double getMinXCoordinate() const;
  double getMaxXCoordinate() const;

  double getMinYCoordinate() const;
  double getMaxYCoordinate() const;

  double getMinZCoordinate() const;
  double getMaxZCoordinate() const;

 protected:
  node *nodeIn;       // node pointer at the first end of the pore
  node *nodeOut;      // node pointer at the second end of the pore
  double fullLength;  // distance (SI) between both connecting nodes centers

  // simulation related attributes
  bool nodeInOil;     // flags oil existence at nodeIn
  bool nodeOutWater;  // flags water existence at nodeOut
  bool nodeInWater;   // flags water existence at nodeIn
  bool nodeOutOil;    // flags oil existence at nodeOut
};

}  // namespace PNM

#endif  // PORE_H
