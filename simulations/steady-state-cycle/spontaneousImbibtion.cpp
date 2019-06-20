/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "spontaneousImbibtion.h"
#include "misc/maths.h"
#include "misc/tools.h"
#include "misc/userInput.h"
#include "network/cluster.h"
#include "network/iterator.h"
#include "operations/hkClustering.h"
#include "operations/pnmOperation.h"
#include "operations/pnmSolver.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>

namespace PNM {

spontaneousImbibtion::spontaneousImbibtion() {}

spontaneousImbibtion::~spontaneousImbibtion() {}

void spontaneousImbibtion::run() {
  initialiseOutputFiles();
  initialiseCapillaries();
  initialiseSimulationAttributes();

  while (step < userInput::get().twoPhaseSimulationSteps) {
    invadeCapillariesAtCurrentPc();
    dismissTrappedElements();
    adjustCapillaryVolumes();

    updateOutputFiles();
    updateVariables();
    updateGUI();

    if (simulationInterrupted) break;
  }
  finalise();
}

std::string spontaneousImbibtion::getNotification() {
  std::ostringstream ss;
  ss << "Spontaneous Imbibition Simulation \n"
     << std::fixed << std::setprecision(2)
     << "Current PC (psi): " << maths::PaToPsi(currentPc)
     << " / Sw: " << currentSw;
  return ss.str();
}

int spontaneousImbibtion::getProgress() {
  return step / userInput::get().twoPhaseSimulationSteps * 100;
}

void spontaneousImbibtion::initialiseOutputFiles() {
  tools::initialiseFolder("Network_State/Spontaneous_Imbibition");

  pcFilename = "Results/SS_Simulation/2-spontaneousImbibtionPcCurve.txt";
  relPermFilename =
      "Results/SS_Simulation/2-spontaneousImbibtionRelativePermeabilies.txt";

  std::ofstream file;

  file.open(pcFilename.c_str());
  file << "Sw\tPc\n";
  file.close();

  file.open(relPermFilename.c_str());
  file << "Sw\tKro\tKrw\n";
  file.close();
}

void spontaneousImbibtion::initialiseSimulationAttributes() {
  step = 0;
  currentSw = 0;

  outputCounter = 0;
  frameCount = 0;

  double effectiveMinRadius = userInput::get().OWSurfaceTension / getMaxPc();
  double effectiveMaxRadius = userInput::get().OWSurfaceTension / getMinPc();
  radiusStep = (effectiveMaxRadius - effectiveMinRadius) /
               userInput::get().twoPhaseSimulationSteps;
  currentRadius = effectiveMinRadius + radiusStep;
  currentPc = userInput::get().OWSurfaceTension / currentRadius;

  elementsToInvade.clear();
  for (element *e : pnmRange<element>(network))
    if (e->getPhaseFlag() == phase::oil &&
        e->getWettabilityFlag() == wettability::waterWet)
      elementsToInvade.insert(e);
}

void spontaneousImbibtion::initialiseCapillaries() {}

double spontaneousImbibtion::getMinPc() {
  double minPc(1e20);
  for (element *e : pnmRange<element>(network)) {
    double pc = std::abs(userInput::get().OWSurfaceTension *
                         std::cos(e->getTheta()) / e->getRadius());
    if (pc < minPc) minPc = pc;
  }
  return minPc;
}

double spontaneousImbibtion::getMaxPc() {
  double maxPc(-1e20);
  for (element *e : pnmRange<element>(network)) {
    double pc = std::abs(e->getEntryPressureCoefficient() *
                         userInput::get().OWSurfaceTension *
                         std::cos(e->getTheta()) / e->getRadius());
    if (pc > maxPc) maxPc = pc;
  }
  return maxPc;
}

void spontaneousImbibtion::invadeCapillariesAtCurrentPc() {
  invadeCapillariesViaSnapOff();
  invadeCapillariesViaBulk();
}

void spontaneousImbibtion::invadeCapillariesViaSnapOff() {
  hkClustering::get(network).clusterWaterConductorElements();
  hkClustering::get(network).clusterOilConductorElements();

  std::vector<element *> invadedElements;
  for (element *e : elementsToInvade) {
    if (isInvadableViaSnapOff(e)) invadedElements.push_back(e);
  }

  for (element *e : invadedElements) {
    fillWithWater(e);
    elementsToInvade.erase(e);
  }

  updateOutputFiles();
  updateGUI();
}

void spontaneousImbibtion::invadeCapillariesViaBulk() {
  bool stillMore = true;
  while (stillMore) {
    stillMore = false;

    hkClustering::get(network).clusterWaterConductorElements();
    hkClustering::get(network).clusterOilConductorElements();

    std::vector<element *> invadedElements;
    for (element *e : elementsToInvade) {
      if (isInvadableViaBulk(e)) invadedElements.push_back(e);
    }

    for (element *e : invadedElements) {
      fillWithWater(e);
      elementsToInvade.erase(e);
      stillMore = true;
    }

    updateOutputFiles();
    updateGUI();

    if (simulationInterrupted) break;
  }
}

void spontaneousImbibtion::dismissTrappedElements() {
  std::vector<element *> trappedElements;
  for (element *e : elementsToInvade) {
    if (!e->getClusterOilConductor()->getOutlet()) trappedElements.push_back(e);
  }

  for (element *e : trappedElements) elementsToInvade.erase(e);
}

void spontaneousImbibtion::adjustCapillaryVolumes() {
  double waterVolume(0);

  for (element *e : pnmRange<element>(network)) {
    if (e->getPhaseFlag() == phase::oil &&
        e->getWettabilityFlag() == wettability::waterWet) {
      if (e->getWaterCornerActivated() &&
          e->getClusterWaterConductor()->getInlet() &&
          e->getClusterOilConductor()->getOutlet())
        adjustVolumetrics(e);

      waterVolume += e->getWaterFilmVolume();
    }

    if (e->getPhaseFlag() == phase::oil &&
        e->getWettabilityFlag() == wettability::oilWet)
      waterVolume += e->getWaterFilmVolume();

    if (e->getPhaseFlag() == phase::water &&
        e->getWettabilityFlag() == wettability::oilWet)
      waterVolume += e->getEffectiveVolume() + e->getWaterFilmVolume();

    if (e->getPhaseFlag() == phase::water &&
        e->getWettabilityFlag() == wettability::waterWet)
      waterVolume += e->getVolume();
  }

  currentSw = waterVolume / network->totalNetworkVolume;
}

bool spontaneousImbibtion::isInvadableViaSnapOff(element *e) {
  bool isInvadable = false;
  double entryPressure =
      userInput::get().OWSurfaceTension * cos(e->getTheta()) / e->getRadius();
  if (currentPc - 1e-5 <= entryPressure && e->getWaterCornerActivated() &&
      e->getClusterWaterConductor()->getInlet() &&
      e->getClusterOilConductor()->getOutlet())
    isInvadable = true;
  return isInvadable;
}

bool spontaneousImbibtion::isInvadableViaBulk(element *e) {
  bool isInvadable = false;

  if (e->getType() == capillaryType::throat &&
      (e->getInlet() || isConnectedToInletCluster(e)) &&
      e->getClusterOilConductor()->getOutlet()) {
    double entryPressure = e->getEntryPressureCoefficient() *
                           userInput::get().OWSurfaceTension *
                           std::cos(e->getTheta()) / e->getRadius();
    if (currentPc - 1e-5 <= entryPressure) isInvadable = true;
  }

  else if (e->getType() == capillaryType::poreBody &&
           isConnectedToInletCluster(e) &&
           e->getClusterOilConductor()->getOutlet()) {
    int oilNeighboorsNumber(0);
    for (element *n : e->getNeighboors()) {
      if (n->getPhaseFlag() == phase::oil) oilNeighboorsNumber++;
    }

    double entryPressureBodyFilling = 0;
    if (oilNeighboorsNumber == 1)
      entryPressureBodyFilling = e->getEntryPressureCoefficient() *
                                 userInput::get().OWSurfaceTension *
                                 std::cos(e->getTheta()) / e->getRadius();
    if (oilNeighboorsNumber > 1)
      entryPressureBodyFilling = e->getEntryPressureCoefficient() *
                                 userInput::get().OWSurfaceTension *
                                 std::cos(e->getTheta()) / e->getRadius() /
                                 double(oilNeighboorsNumber);

    if (currentPc - 1e-5 <= entryPressureBodyFilling) isInvadable = true;
  }

  return isInvadable;
}

bool spontaneousImbibtion::isConnectedToInletCluster(element *e) {
  bool connectedToInletCluster = false;
  for (element *n : e->getNeighboors())
    if (n->getPhaseFlag() == phase::water &&
        n->getClusterWaterConductor()->getInlet()) {
      connectedToInletCluster = true;
      break;
    }
  return connectedToInletCluster;
}

void spontaneousImbibtion::fillWithWater(element *e) {
  e->setPhaseFlag(phase::water);
  e->setWaterConductor(true);
  e->setOilFraction(0);
  e->setWaterFraction(1);
  e->setWaterCornerActivated(false);
  e->setWaterFilmVolume(0);
  e->setWaterFilmConductivity(1e-200);

  e->setOilConductor(false);
}

void spontaneousImbibtion::adjustVolumetrics(element *e) {
  double rSquared = std::pow(userInput::get().OWSurfaceTension / currentPc, 2);
  double filmVolume =
      std::min(rSquared * e->getFilmAreaCoefficient() * e->getLength(),
               (1 - 4 * maths::pi() * e->getShapeFactor()) * e->getVolume());
  double filmConductance = rSquared * filmVolume / e->getLength() /
                           (userInput::get().waterViscosity * e->getLength());

  e->setWaterFilmVolume(filmVolume);
  e->setWaterFilmConductivity(filmConductance);
  e->setEffectiveVolume(e->getVolume() - e->getWaterFilmVolume());
}

void spontaneousImbibtion::updateVariables() {
  step++;

  if (step != userInput::get().twoPhaseSimulationSteps) {
    currentRadius += radiusStep;
    currentPc = userInput::get().OWSurfaceTension / currentRadius;
  }
}

void spontaneousImbibtion::updateOutputFiles() {
  if (std::abs(outputCounter - currentSw) < 0.01) return;

  std::ofstream file;

  file.open(pcFilename, std::ofstream::app);
  file << currentSw << "\t" << currentPc << std::endl;
  file.close();

  if (userInput::get().relativePermeabilitiesCalculation) {
    auto relPerms = pnmSolver::get(network).calculateRelativePermeabilities();

    file.open(relPermFilename, std::ofstream::app);
    file << currentSw << "\t" << relPerms.first << "\t" << relPerms.second
         << std::endl;
    file.close();
  }

  generateNetworkStateFiles();

  outputCounter = currentSw;
}

void spontaneousImbibtion::generateNetworkStateFiles() {
  if (!userInput::get().extractDataSS) return;

  pnmOperation::get(network).generateNetworkState(frameCount,
                                                  "Spontaneous_Imbibition");
  frameCount++;
}

}  // namespace PNM
