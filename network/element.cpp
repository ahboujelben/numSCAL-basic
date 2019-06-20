/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "element.h"
#include "cluster.h"

namespace PNM {

element::element() {
  radius = 0;
  length = 0;
  theta = 0;
  wettabilityFlag = wettability::oilWet;
  phaseFlag = phase::oil;
  volume = 0;
  capillaryPressure = 0;
  viscosity = 1;
  conductivity = 0;
  concentration = 0;
  flow = 0;
  closed = false;
  active = true;
  inlet = false;
  outlet = false;

  clusterTemp = 0;
  clusterOil = nullptr;
  clusterWater = nullptr;
  clusterOilWet = nullptr;
  clusterWaterWet = nullptr;

  oilFraction = 1;
  waterFraction = 0;

  beta1 = 0;
  beta2 = 0;
  beta3 = 0;
}

element::~element() {}

}  // namespace PNM
