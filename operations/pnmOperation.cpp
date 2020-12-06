/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "pnmOperation.h"
#include "hkClustering.h"
#include "misc/maths.h"
#include "misc/randomGenerator.h"
#include "misc/userInput.h"
#include "network/cluster.h"
#include "network/iterator.h"
#include "simulations/steady-state-cycle/primaryDrainage.h"

#include "libs/boost/format.hpp"

#include <algorithm>
#include <fstream>

namespace PNM {

pnmOperation pnmOperation::instance;

pnmOperation &pnmOperation::get(std::shared_ptr<networkModel> network) {
  instance.network = network;
  return instance;
}

void pnmOperation::assignRadii() {
  randomGenerator gen(userInput::get().seed);
  for (pore *p : pnmRange<pore>(network)) {
    auto radiusDistribution = userInput::get().poreSizeDistribution;
    if (radiusDistribution == psd::uniform)
      p->setRadius(gen.uniform_real(userInput::get().minRadius,
                                    userInput::get().maxRadius));
    if (radiusDistribution == psd::rayleigh)
      p->setRadius(gen.rayleigh(userInput::get().minRadius,
                                userInput::get().maxRadius,
                                userInput::get().rayleighParameter));
    if (radiusDistribution == psd::triangular)
      p->setRadius(gen.triangular(userInput::get().minRadius,
                                  userInput::get().maxRadius,
                                  userInput::get().triangularParameter));
    if (radiusDistribution == psd::truncatedNormal)
      p->setRadius(gen.normal(userInput::get().minRadius,
                              userInput::get().maxRadius,
                              userInput::get().normalMuParameter,
                              userInput::get().normalSigmaParameter));
  }

  for (node *n : pnmRange<node>(network)) {
    double maxRadius(0), averageRadius(0);
    int neighboorsNumber(0);
    for (element *p : n->getNeighboors()) {
      if (p->getRadius() > maxRadius) maxRadius = p->getRadius();
      averageRadius += p->getRadius();
      neighboorsNumber++;
    }
    averageRadius =
        userInput::get().aspectRatio * averageRadius / neighboorsNumber;
    double radius = std::max(maxRadius, averageRadius);
    n->setRadius(radius);
  }
}

void pnmOperation::assignLengths() {
  double length = userInput::get().length;
  for (pore *p : pnmRange<pore>(network)) {
    p->setFullLength(length);
    double nodeInLength =
        p->getNodeIn() == nullptr ? 0 : p->getNodeIn()->getRadius();
    double nodeOutLength =
        p->getNodeOut() == nullptr ? 0 : p->getNodeOut()->getRadius();
    double newlength = length - nodeInLength - nodeOutLength;
    p->setLength(newlength > 0 ? newlength : length / 2);
  }

  for (node *n : pnmRange<node>(network)) n->setLength(2 * n->getRadius());
}

void pnmOperation::distortNetwork() {
  if (userInput::get().degreeOfDistortion <= 0) return;

  randomGenerator gen(userInput::get().seed);
  double length = userInput::get().length;
  double degreeOfDistortion = userInput::get().degreeOfDistortion;
  for (node *n : pnmRange<node>(network)) {
    n->setXCoordinate(n->getXCoordinate() + length * degreeOfDistortion *
                                                (-1 + 2 * gen.uniform_real()));
    n->setYCoordinate(n->getYCoordinate() + length * degreeOfDistortion *
                                                (-1 + 2 * gen.uniform_real()));
    if (!network->is2D)
      n->setZCoordinate(n->getZCoordinate() +
                        length * degreeOfDistortion *
                            (-1 + 2 * gen.uniform_real()));
  };

  for (pore *p : pnmRange<pore>(network)) {
    if (p->getNodeIn() == nullptr || p->getNodeOut() == nullptr) continue;
    double length = std::sqrt(std::pow(p->getNodeIn()->getXCoordinate() -
                                           p->getNodeOut()->getXCoordinate(),
                                       2) +
                              std::pow(p->getNodeIn()->getYCoordinate() -
                                           p->getNodeOut()->getYCoordinate(),
                                       2) +
                              std::pow(p->getNodeIn()->getZCoordinate() -
                                           p->getNodeOut()->getZCoordinate(),
                                       2));
    p->setFullLength(length);
    double nodeInLength =
        p->getNodeIn() == nullptr ? 0 : p->getNodeIn()->getRadius();
    double nodeOutLength =
        p->getNodeOut() == nullptr ? 0 : p->getNodeOut()->getRadius();
    double newlength = length - nodeInLength - nodeOutLength;
    p->setLength(newlength > 0 ? newlength : length / 2);
  }
}

void pnmOperation::assignShapeFactors() {
  for (element *e : pnmRange<element>(network))
    e->setShapeFactor(userInput::get().shapeFactor);
}

void pnmOperation::assignShapeFactorConstants() {
  auto circleThreshold = sqrt(3) / 36.0;
  auto squareThreshold = 1.0 / 16.0;
  for (element *e : pnmRange<element>(network)) {
    if (e->getShapeFactor() <= circleThreshold)
      e->setShapeFactorConstant(0.6);
    else if (e->getShapeFactor() <= squareThreshold)
      e->setShapeFactorConstant(0.5623);
    else
      e->setShapeFactorConstant(0.5);

    e->setEntryPressureCoefficient(1 +
                                   2 * sqrt(maths::pi() * e->getShapeFactor()));
  }
}

void pnmOperation::assignVolumes() {
  for (element *e : pnmRange<element>(network)) {
    double volume =
        userInput::get().poreVolumeConstant * e->getLength() *
        std::pow(e->getRadius(), userInput::get().poreVolumeExponent) /
        (4 * e->getShapeFactor()) *
        std::pow(10, (6 * userInput::get().poreVolumeExponent - 12));
    e->setVolume(volume);
    e->setEffectiveVolume(volume);
  }
}

void pnmOperation::assignViscosities() {
  for (element *e : pnmRange<element>(network)) {
    e->setViscosity(e->getOilFraction() * userInput::get().oilViscosity +
                    e->getWaterFraction() * userInput::get().waterViscosity);
  }
}

void pnmOperation::assignConductivities() {
  for (node *n : pnmRange<node>(network)) {
    n->setConductivity(
        userInput::get().poreConductivityConstant *
        n->getShapeFactorConstant() *
        pow(n->getRadius(), userInput::get().poreConductivityExponent) /
        (16 * n->getShapeFactor()) / (n->getLength() * n->getViscosity()) *
        pow(10, (6 * userInput::get().poreConductivityExponent - 24)));
  }

  for (pore *p : pnmRange<pore>(network)) {
    node *nodeIn = p->getNodeIn();
    node *nodeOut = p->getNodeOut();
    auto throatConductivityInverse(0.0), nodeInConductivityInverse(0.0),
        nodeOutConductivityInverse(0.0);

    auto throatConductivity =
        userInput::get().poreConductivityConstant *
        p->getShapeFactorConstant() *
        pow(p->getRadius(), userInput::get().poreConductivityExponent) /
        (16 * p->getShapeFactor()) / (p->getLength() * p->getViscosity()) *
        pow(10, (6 * userInput::get().poreConductivityExponent - 24));

    throatConductivityInverse = 1 / throatConductivity;

    if (nodeIn != nullptr)
      nodeInConductivityInverse = 1 / (nodeIn->getConductivity() * 2);

    if (nodeOut != nullptr)
      nodeOutConductivityInverse = 1 / (nodeOut->getConductivity() * 2);

    p->setConductivity(1. /
                       (throatConductivityInverse + nodeInConductivityInverse +
                        nodeOutConductivityInverse));
  }
}

void pnmOperation::calculateNetworkVolume() {
  network->totalNodesVolume = 0;
  network->totalPoresVolume = 0;
  network->inletPoresArea = 0;

  for (node *n : pnmRange<node>(network)) {
    network->totalNodesVolume += n->getVolume();
  }

  for (pore *p : pnmRange<pore>(network)) {
    network->totalPoresVolume += p->getVolume();
    if (p->getInlet())
      network->inletPoresArea += p->getVolume() / p->getLength();
  }

  network->totalNetworkVolume =
      network->totalNodesVolume + network->totalPoresVolume;
}

void pnmOperation::assignHalfAngles() {
  for (element *p : pnmRange<element>(network)) {
    double beta1(0), beta2(0), beta3(0);
    randomGenerator gen(userInput::get().seed);
    if (p->getShapeFactor() <= sqrt(3) / 36.) {
      double beta2Min = atan(2 / sqrt(3) *
                             cos(acos(-12 * sqrt(3) * p->getShapeFactor()) / 3 +
                                 4 * maths::pi() / 3));
      double beta2Max = atan(
          2 / sqrt(3) * cos(acos(-12 * sqrt(3) * p->getShapeFactor()) / 3));
      beta2 = gen.uniform_real(beta2Min, beta2Max);
      beta1 = -0.5 * beta2 +
              0.5 * asin((tan(beta2) + 4 * p->getShapeFactor()) /
                         (tan(beta2) - 4 * p->getShapeFactor()) * sin(beta2));
      beta3 = maths::pi() / 2 - beta1 - beta2;
    }
    p->setBeta1(beta1);
    p->setBeta2(beta2);
    p->setBeta3(beta3);
  }
}

void pnmOperation::assignFilmsStability() {
  for (element *p : pnmRange<element>(network)) {
    bool oilCanFlowViaFilm(false), waterCanFlowViaFilm(false);
    if (p->getShapeFactor() <= sqrt(3) / 36.) {
      double theta = p->getTheta();
      if (p->getWettabilityFlag() == wettability::waterWet) {
        double sum(0);
        if (theta < maths::pi() / 2 - p->getBeta1()) {
          waterCanFlowViaFilm = true;
          sum += cos(theta) * cos(theta + p->getBeta1()) / sin(p->getBeta1()) -
                 maths::pi() / 2 *
                     (1 - (theta + p->getBeta1()) / (maths::pi() / 2));
        }
        if (theta < maths::pi() / 2 - p->getBeta2()) {
          waterCanFlowViaFilm = true;
          sum += cos(theta) * cos(theta + p->getBeta2()) / sin(p->getBeta2()) -
                 maths::pi() / 2 *
                     (1 - (theta + p->getBeta2()) / (maths::pi() / 2));
        }
        if (theta < maths::pi() / 2 - p->getBeta3()) {
          waterCanFlowViaFilm = true;
          sum += cos(theta) * cos(theta + p->getBeta3()) / sin(p->getBeta3()) -
                 maths::pi() / 2 *
                     (1 - (theta + p->getBeta3()) / (maths::pi() / 2));
        }
        p->setFilmAreaCoefficient(sum);
      }

      if (p->getWettabilityFlag() == wettability::oilWet) {
        double sum(0);
        if (maths::pi() - theta < maths::pi() / 2 - p->getBeta1()) {
          oilCanFlowViaFilm = true;
          sum += cos(theta) * cos(theta - p->getBeta1()) / sin(p->getBeta1()) -
                 maths::pi() / 2 *
                     (-1 + (theta - p->getBeta1()) / (maths::pi() / 2));
        }
        if (maths::pi() - theta < maths::pi() / 2 - p->getBeta2()) {
          oilCanFlowViaFilm = true;
          sum += cos(theta) * cos(theta - p->getBeta2()) / sin(p->getBeta2()) -
                 maths::pi() / 2 *
                     (-1 + (theta - p->getBeta2()) / (maths::pi() / 2));
        }
        if (maths::pi() - theta < maths::pi() / 2 - p->getBeta3()) {
          oilCanFlowViaFilm = true;
          sum += cos(theta) * cos(theta - p->getBeta3()) / sin(p->getBeta3()) -
                 maths::pi() / 2 *
                     (-1 + (theta - p->getBeta3()) / (maths::pi() / 2));
        }
        p->setFilmAreaCoefficient(sum);
      }
    }
    p->setOilCanFlowViaFilm(oilCanFlowViaFilm);
    p->setWaterCanFlowViaFilm(waterCanFlowViaFilm);
  }
}

void pnmOperation::assignWettabilities() {
  randomGenerator gen(userInput::get().seed);

  if (userInput::get().wettability == networkWettability::oilWet) {
    for (element *e : pnmRange<element>(network)) {
      e->setTheta(gen.uniform_real(userInput::get().minOilWetTheta,
                                   userInput::get().maxOilWetTheta));
      e->setWettabilityFlag(wettability::oilWet);
    };
    pnmOperation::get(network).backupWettability();
    return;
  }

  for (element *e : pnmRange<element>(network)) {
    e->setTheta(gen.uniform_real(userInput::get().minWaterWetTheta,
                                 userInput::get().maxWaterWetTheta));
    e->setWettabilityFlag(wettability::waterWet);
  };

  if (userInput::get().wettability == networkWettability::waterWet) {
    pnmOperation::get(network).backupWettability();
    return;
  }

  if (userInput::get().wettability == networkWettability::fracionalWet)  // FW
  {
    auto shuffledNodes = network->tableOfNodes;
    std::shuffle(shuffledNodes.begin(), shuffledNodes.end(), gen.getGen());

    auto oilWetSoFar(0);
    while ((double(oilWetSoFar) / network->totalNodes) <
           userInput::get().oilWetFraction) {
      auto n = shuffledNodes.back();
      shuffledNodes.pop_back();
      if (n->getWettabilityFlag() != wettability::oilWet) {
        n->setTheta(gen.uniform_real(userInput::get().minOilWetTheta,
                                     userInput::get().maxOilWetTheta));
        n->setWettabilityFlag(wettability::oilWet);
        oilWetSoFar++;
      }
    }
  }

  if (userInput::get().wettability == networkWettability::mixdeWetLarge) {
    auto workingElements = network->tableOfNodes;
    std::sort(
        workingElements.begin(), workingElements.end(),
        [](auto e1, auto e2) { return e1->getRadius() > e2->getRadius(); });

    auto oilWetSoFar(0);

    while ((double(oilWetSoFar) / network->totalNodes) <
           userInput::get().oilWetFraction) {
      auto biggestElement = workingElements.back();
      biggestElement->setTheta(gen.uniform_real(
          userInput::get().minOilWetTheta, userInput::get().maxOilWetTheta));
      biggestElement->setWettabilityFlag(wettability::oilWet);
      oilWetSoFar++;
      workingElements.pop_back();
    }
  }

  if (userInput::get().wettability == networkWettability::mixedWetSmall) {
    auto workingElements = network->tableOfNodes;
    std::sort(
        workingElements.begin(), workingElements.end(),
        [](auto e1, auto e2) { return e1->getRadius() < e2->getRadius(); });

    auto oilWetSoFar(0);

    while ((double(oilWetSoFar) / network->totalNodes) <
           userInput::get().oilWetFraction) {
      auto smallestElement = workingElements.back();
      smallestElement->setTheta(gen.uniform_real(
          userInput::get().minOilWetTheta, userInput::get().maxOilWetTheta));
      smallestElement->setWettabilityFlag(wettability::oilWet);
      oilWetSoFar++;
      workingElements.pop_back();
    }
  }

  for (pore *p : pnmRange<pore>(network)) {
    if (p->getNodeIn() == nullptr) {
      auto connectedNode = p->getNodeOut();
      p->setTheta(connectedNode->getTheta());
      p->setWettabilityFlag(connectedNode->getWettabilityFlag());
    } else if (p->getNodeOut() == nullptr) {
      auto connectedNode = p->getNodeIn();
      p->setTheta(connectedNode->getTheta());
      p->setWettabilityFlag(connectedNode->getWettabilityFlag());
    } else {
      auto connectedNode1 = p->getNodeIn();
      auto connectedNode2 = p->getNodeOut();
      if (connectedNode1->getWettabilityFlag() ==
          connectedNode2->getWettabilityFlag()) {
        p->setTheta((connectedNode1->getTheta() + connectedNode2->getTheta()) /
                    2);
        p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
      } else {
        p->setTheta(gen.uniform_int() ? connectedNode1->getTheta()
                                      : connectedNode2->getTheta());
        if (p->getTheta() > maths::pi() / 2)
          p->setWettabilityFlag(wettability::oilWet);
      }
    }
  };
}

void pnmOperation::backupWettability() {
  for (element *e : pnmRange<element>(network)) {
    e->setOriginalTheta(e->getTheta());
  };
}

void pnmOperation::restoreWettability() {
  for (element *e : pnmRange<element>(network)) {
    e->setTheta(e->getOriginalTheta());
    e->setWettabilityFlag(e->getTheta() <= maths::pi() / 2
                              ? wettability::waterWet
                              : wettability::oilWet);
  }
}

void pnmOperation::assignWWWettability() {
  for (element *e : pnmRange<element>(network)) {
    e->setTheta(0);
    e->setWettabilityFlag(wettability::waterWet);
  }
}

void pnmOperation::assignOilConductivities() {
  assignConductivities();
  for (node *n : pnmRange<node>(network)) {
    n->setActive(true);
    if (n->getPhaseFlag() == phase::oil) {
      if (!n->getClusterOilConductor()->getSpanning()) n->setActive(false);
    }
    if (n->getPhaseFlag() == phase::water) {
      if (n->getOilLayerActivated() &&
          n->getClusterOilConductor()->getSpanning())
        n->setConductivity(n->getOilFilmConductivity() /
                           userInput::get().filmConductanceResistivity);
      else
        n->setActive(false);
    }
  }

  for (pore *p : pnmRange<pore>(network)) {
    p->setActive(true);

    node *nodeIn = p->getNodeIn();
    node *nodeOut = p->getNodeOut();

    if ((nodeIn != nullptr && !nodeIn->getActive()) ||
        (nodeOut != nullptr && !nodeOut->getActive())) {
      p->setActive(false);
      continue;
    }

    double throatConductivity(0.0);

    if (p->getPhaseFlag() == phase::oil) {
      if (p->getClusterOilConductor()->getSpanning())
        throatConductivity =
            userInput::get().poreConductivityConstant *
            p->getShapeFactorConstant() *
            pow(p->getRadius(), userInput::get().poreConductivityExponent) /
            (16 * p->getShapeFactor()) / (p->getLength() * p->getViscosity()) *
            pow(10, (6 * userInput::get().poreConductivityExponent - 24));
      else {
        p->setActive(false);
        continue;
      }
    }
    if (p->getPhaseFlag() == phase::water) {
      if (p->getOilLayerActivated() &&
          p->getClusterOilConductor()->getSpanning())
        throatConductivity = p->getOilFilmConductivity() /
                             userInput::get().filmConductanceResistivity;
      else {
        p->setActive(false);
        continue;
      }
    }

    double throatConductivityInverse = 1 / throatConductivity;
    double nodeInConductivityInverse =
        nodeIn != nullptr ? 1 / (nodeIn->getConductivity() * 2) : 0;
    double nodeOutConductivityInverse =
        nodeOut != nullptr ? 1 / (nodeOut->getConductivity() * 2) : 0;

    p->setConductivity(1. /
                       (throatConductivityInverse + nodeInConductivityInverse +
                        nodeOutConductivityInverse));
  }
}

void pnmOperation::assignWaterConductivities() {
  assignConductivities();
  for (node *n : pnmRange<node>(network)) {
    n->setActive(true);
    if (n->getPhaseFlag() == phase::water) {
      if (!n->getClusterWaterConductor()->getSpanning()) n->setActive(false);
    }
    if (n->getPhaseFlag() == phase::oil) {
      if (n->getWaterCornerActivated() &&
          n->getClusterWaterConductor()->getSpanning())
        n->setConductivity(n->getWaterFilmConductivity() /
                           userInput::get().filmConductanceResistivity);
      else
        n->setActive(false);
    }
  }

  for (pore *p : pnmRange<pore>(network)) {
    p->setActive(true);

    node *nodeIn = p->getNodeIn();
    node *nodeOut = p->getNodeOut();

    if ((nodeIn != nullptr && !nodeIn->getActive()) ||
        (nodeOut != nullptr && !nodeOut->getActive())) {
      p->setActive(false);
      continue;
    }

    double throatConductivity(0.0);

    if (p->getPhaseFlag() == phase::water) {
      if (p->getClusterWaterConductor()->getSpanning())
        throatConductivity =
            userInput::get().poreConductivityConstant *
            p->getShapeFactorConstant() *
            pow(p->getRadius(), userInput::get().poreConductivityExponent) /
            (16 * p->getShapeFactor()) / (p->getLength() * p->getViscosity()) *
            pow(10, (6 * userInput::get().poreConductivityExponent - 24));
      else {
        p->setActive(false);
        continue;
      }
    }
    if (p->getPhaseFlag() == phase::oil) {
      if (p->getWaterCornerActivated() &&
          p->getClusterWaterConductor()->getSpanning())
        throatConductivity = p->getWaterFilmConductivity() /
                             userInput::get().filmConductanceResistivity;
      else {
        p->setActive(false);
        continue;
      }
    }

    double throatConductivityInverse = 1 / throatConductivity;
    double nodeInConductivityInverse =
        nodeIn != nullptr ? 1 / (nodeIn->getConductivity() * 2) : 0;
    double nodeOutConductivityInverse =
        nodeOut != nullptr ? 1 / (nodeOut->getConductivity() * 2) : 0;

    p->setConductivity(1. /
                       (throatConductivityInverse + nodeInConductivityInverse +
                        nodeOutConductivityInverse));
  }
}

void pnmOperation::setSwi() {
  randomGenerator gen(userInput::get().seed);

  if (std::equal_to<>()(userInput::get().initialWaterSaturation, 1)) {
    std::for_each(pnmRange<element>(network).begin(),
                  pnmRange<element>(network).end(),
                  [](element *e) { e->setPhaseFlag(phase::water); });
    return;
  }

  std::for_each(pnmRange<element>(network).begin(),
                pnmRange<element>(network).end(),
                [](element *e) { e->setPhaseFlag(phase::oil); });

  if (std::equal_to<>()(userInput::get().initialWaterSaturation, 0)) {
    return;
  }

  if (userInput::get().waterDistribution == swi::random) {
    std::vector<node *> shuffledNodes;
    shuffledNodes.reserve(network->totalNodes);
    for (node *n : pnmRange<node>(network)) shuffledNodes.push_back(n);
    std::shuffle(shuffledNodes.begin(), shuffledNodes.end(), gen.getGen());

    auto actualWaterVolume(0.0);
    while ((actualWaterVolume / network->totalNodesVolume) <
           userInput::get().initialWaterSaturation) {
      node *p = shuffledNodes.back();
      shuffledNodes.pop_back();
      if (p->getPhaseFlag() != phase::water) {
        p->setPhaseFlag(phase::water);
        actualWaterVolume += p->getVolume();
      }
    }
  }

  if (userInput::get().waterDistribution == swi::waterInSmallCapillaries) {
    std::vector<node *> workingElements;
    for (node *n : pnmRange<node>(network)) workingElements.push_back(n);

    std::sort(
        workingElements.begin(), workingElements.end(),
        [](node *e1, node *e2) { return e1->getRadius() > e2->getRadius(); });

    auto actualWaterVolume(0.0);

    while (actualWaterVolume / network->totalNetworkVolume <
           userInput::get().initialWaterSaturation) {
      auto smallestElement = workingElements.back();
      smallestElement->setPhaseFlag(phase::water);
      actualWaterVolume += smallestElement->getVolume();
      workingElements.pop_back();
    }
  }

  if (userInput::get().waterDistribution == swi::waterInBigCapillaries) {
    std::vector<node *> workingElements;
    for (node *n : pnmRange<node>(network)) workingElements.push_back(n);

    std::sort(
        workingElements.begin(), workingElements.end(),
        [](node *e1, node *e2) { return e1->getRadius() < e2->getRadius(); });

    auto actualWaterVolume(0.0);

    while (actualWaterVolume / network->totalNetworkVolume <
           userInput::get().initialWaterSaturation) {
      auto biggestElement = workingElements.back();
      biggestElement->setPhaseFlag(phase::water);
      actualWaterVolume += biggestElement->getVolume();
      workingElements.pop_back();
    }
  }

  if (userInput::get().waterDistribution == swi::afterPrimaryDrainage) {
    auto sim = std::make_shared<primaryDrainage>(
        userInput::get().initialWaterSaturation);
    sim->setNetwork(network);
    sim->execute();
  }

  std::for_each(pnmRange<pore>(network).begin(), pnmRange<pore>(network).end(),
                [&gen](pore *p) {
                  if (p->getNodeIn() == nullptr) {
                    auto connectedNode = p->getNodeOut();
                    p->setPhaseFlag(connectedNode->getPhaseFlag());
                  } else if (p->getNodeOut() == nullptr) {
                    auto connectedNode = p->getNodeIn();
                    p->setPhaseFlag(connectedNode->getPhaseFlag());
                  } else {
                    if (p->getNodeIn()->getPhaseFlag() ==
                        p->getNodeOut()->getPhaseFlag()) {
                      p->setPhaseFlag(p->getNodeIn()->getPhaseFlag());
                    } else {
                      p->setPhaseFlag(gen.uniform_int()
                                          ? p->getNodeIn()->getPhaseFlag()
                                          : p->getNodeOut()->getPhaseFlag());
                    }
                  }
                });
}

void pnmOperation::fillWithWater() {
  std::for_each(pnmRange<element>(network).begin(),
                pnmRange<element>(network).end(),
                [](element *e) { e->setPhaseFlag(phase::water); });
}

double pnmOperation::getSw() {
  double waterVolume(0);
  for (element *e : pnmRange<element>(network)) {
    waterVolume += e->getVolume() * e->getWaterFraction();
  };

  return waterVolume / network->totalNetworkVolume;
}

double pnmOperation::getFlow(phase phaseFlag) {
  double outletFlow(0);
  for (pore *p : pnmOutlet(network))
    if (p->getActive() && p->getPhaseFlag() == phaseFlag)
      outletFlow += p->getFlow();
  return outletFlow;
}

double pnmOperation::getInletPoresVolume() {
  double inletPoresVolume(0.0);
  for (pore *p : pnmInlet(network))
    if (p->getActive()) inletPoresVolume += p->getVolume();

  return inletPoresVolume;
}

void pnmOperation::exportToNumcalFormat() {
  std::ofstream file;

  file.open("numSCAL_Networks/_nodes.num");
  file << std::fixed << "x(um)"
       << ","
       << "y(um)"
       << ","
       << "z(um)"
       << ","
       << "radius(um)"
       << ","
       << "length(um)"
       << ","
       << "shapeFactor" << std::endl;

  for (node *n : pnmRange<node>(network)) {
    file << n->getXCoordinate() * 1e6 << "," << n->getYCoordinate() * 1e6 << ","
         << n->getZCoordinate() * 1e6 << "," << n->getRadius() * 1e6 << ","
         << n->getLength() * 1e6 << "," << n->getShapeFactor() << std::endl;
  }
  file.close();

  file.open("numSCAL_Networks/_throats.num");
  file << std::fixed << "nodeIn(id)"
       << ","
       << "nodeOut(id)"
       << ","
       << "radius(um)"
       << ","
       << "length(um)"
       << ","
       << "shapeFactor" << std::endl;

  for (pore *p : pnmRange<pore>(network)) {
    file << (p->getNodeIn() ? p->getNodeIn()->getId() : 0) << ","
         << (p->getNodeOut() ? p->getNodeOut()->getId() : 0) << ","
         << p->getRadius() * 1e6 << "," << p->getLength() * 1e6 << ","
         << p->getShapeFactor() << std::endl;
  }
  file.close();
}

void pnmOperation::generateNetworkState(int frame, std::string folderPath) {
  std::string path = "Network_State/" + folderPath + "/network_state_" +
                     boost::str(boost::format("%07d") % frame) + ".nums";

  std::ofstream file;
  file.open(path.c_str());

  file << "phase,concentration" << std::endl;

  for (element *e : pnmRange<element>(network))
    file << int(e->getPhaseFlag()) << "," << e->getConcentration() << std::endl;

  file.close();
}

}  // namespace PNM
