/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "cluster.h"

namespace PNM {

cluster::cluster(int pLabel) {
  id = pLabel;
  inlet = false;
  outlet = false;
  spanning = false;
}

bool cluster::getInlet() const { return inlet; }

void cluster::setInlet(bool value) { inlet = value; }
bool cluster::getOutlet() const { return outlet; }

void cluster::setOutlet(bool value) { outlet = value; }
bool cluster::getSpanning() const { return spanning; }

void cluster::setSpanning(bool value) { spanning = value; }

int cluster::getId() const { return id; }

void cluster::setId(int value) { id = value; }

}  // namespace PNM
