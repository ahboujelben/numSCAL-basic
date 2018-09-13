/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef NODE_H
#define NODE_H

#include "element.h"

namespace PNM
{

class node : public element
{
public:
  explicit node(double, double, double);
  virtual ~node() {}
  node(const node &) = delete;
  node(node &&) = delete;
  auto operator=(const node &) -> node & = delete;
  auto operator=(node &&) -> node & = delete;

  int getIndexX() const { return x; }
  void setIndexX(int value) { x = value; }

  int getIndexY() const { return y; }
  void setIndexY(int value) { y = value; }

  int getIndexZ() const { return z; }
  void setIndexZ(int value) { z = value; }

  double getXCoordinate() const { return xCoordinate; }
  void setXCoordinate(double value) { xCoordinate = value; }

  double getYCoordinate() const { return yCoordinate; }
  void setYCoordinate(double value) { yCoordinate = value; }

  double getZCoordinate() const { return zCoordinate; }
  void setZCoordinate(double value) { zCoordinate = value; }

  int getConnectionNumber() const { return connectionNumber; }
  void setConnectionNumber(int value) { connectionNumber = value; }

  double getPressure() const { return pressure; }
  void setPressure(double value) { pressure = value; }

  int getRank() const { return rank; }
  void setRank(int value) { rank = value; }

private:
  int x; // relative x coordinate
  int y; // relative y coordinate
  int z; // relative z coordinate

  double xCoordinate; // absolute x coordinate
  double yCoordinate; // absolute y coordinate
  double zCoordinate; // absolute z coordinate

  int connectionNumber; //coordination number

  double pressure; // pressure (SI)
  int rank;        // solver ranking
};

} // namespace PNM

#endif // NODE_H
