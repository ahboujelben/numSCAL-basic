/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "node.h"

namespace PNM {

node::node(double X, double Y, double Z) {
  type = capillaryType::poreBody;
  x = X;
  y = Y;
  z = Z;
  xCoordinate = X;
  yCoordinate = Y;
  zCoordinate = Z;
  connectionNumber = 6;
  pressure = 0;
  rank = 0;
}

node::~node() {}

}  // namespace PNM
