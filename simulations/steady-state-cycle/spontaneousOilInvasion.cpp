/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "spontaneousOilInvasion.h"
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

spontaneousOilInvasion::spontaneousOilInvasion() {}

spontaneousOilInvasion::~spontaneousOilInvasion() {}

void spontaneousOilInvasion::run() {
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

std::string spontaneousOilInvasion::getNotification() {
  std::ostringstream ss;
  ss << "Spontaneous Oil Invasion Simulation \n"
     << std::fixed << std::setprecision(2)
     << "Current PC (psi): " << maths::PaToPsi(currentPc)
     << " / Sw: " << currentSw;
  return ss.str();
}

int spontaneousOilInvasion::getProgress() {
  return step / userInput::get().twoPhaseSimulationSteps * 100;
}

void spontaneousOilInvasion::initialiseOutputFiles() {
  tools::initialiseFolder("Network_State/Spontaneous_Oil_Invasion");

  pcFilename = "Results/SS_Simulation/4-spontaneousOilInvasionPcCurve.txt";
  relPermFilename =
      "Results/SS_Simulation/4-spontaneousOilInvasionRelativePermeabilies.txt";

  std::ofstream file;

  file.open(pcFilename.c_str());
  file << "Sw\tPc\n";
  file.close();

  file.open(relPermFilename.c_str());
  file << "Sw\tKro\tKrw\n";
  file.close();
}

void spontaneousOilInvasion::initialiseSimulationAttributes() {
  step = 0;
  currentSw = 0;

  outputCounter = 0;
  frameCount = 0;

  double effectiveMinRadius = userInput::get().OWSurfaceTension / getMaxPc();
  double effectiveMaxRadius = userInput::get().OWSurfaceTension / getMinPc();
  radiusStep = (effectiveMaxRadius - effectiveMinRadius) /
               userInput::get().twoPhaseSimulationSteps;
  currentRadius = effectiveMinRadius + radiusStep;
  currentPc = -userInput::get().OWSurfaceTension / currentRadius;

  elementsToInvade.clear();
  for (element *e : pnmRange<element>(network))
    if (e->getPhaseFlag() == phase::water &&
        e->getWettabilityFlag() == wettability::oilWet)
      elementsToInvade.insert(e);
}

void spontaneousOilInvasion::initialiseCapillaries() {}

double spontaneousOilInvasion::getMinPc() {
  double minPc(1e20);
  for (element *e : pnmRange<element>(network)) {
    double pc = std::abs(userInput::get().OWSurfaceTension *
                         std::cos(e->getTheta()) / e->getRadius());
    if (pc < minPc) minPc = pc;
  }
  return minPc;
}

double spontaneousOilInvasion::getMaxPc() {
  double maxPc(-1e20);
  for (element *e : pnmRange<element>(network)) {
    double pc = std::abs(e->getEntryPressureCoefficient() *
                         userInput::get().OWSurfaceTension *
                         std::cos(e->getTheta()) / e->getRadius());
    if (pc > maxPc) maxPc = pc;
  }
  return maxPc;
}

void spontaneousOilInvasion::invadeCapillariesAtCurrentPc() {
  invadeCapillariesViaSnapOff();
  invadeCapillariesViaBulk();
}

void spontaneousOilInvasion::invadeCapillariesViaSnapOff() {
  hkClustering::get(network).clusterWaterConductorElements();
  hkClustering::get(network).clusterOilConductorElements();

  std::vector<element *> invadedElements;
  for (element *e : elementsToInvade) {
    if (isInvadableViaSnapOff(e)) invadedElements.push_back(e);
  }

  for (element *e : invadedElements) {
    fillWithOil(e);
    elementsToInvade.erase(e);
  }

  updateOutputFiles();
  updateGUI();
}

void spontaneousOilInvasion::invadeCapillariesViaBulk() {
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
      fillWithOil(e);
      elementsToInvade.erase(e);
      stillMore = true;
    }

    updateOutputFiles();
    updateGUI();

    if (simulationInterrupted) break;
  }
}

void spontaneousOilInvasion::dismissTrappedElements() {
  std::vector<element *> trappedElements;
  for (element *e : elementsToInvade) {
    if (!e->getClusterWaterConductor()->getOutlet())
      trappedElements.push_back(e);
  }

  for (element *e : trappedElements) elementsToInvade.erase(e);
}

void spontaneousOilInvasion::adjustCapillaryVolumes() {
  double waterVolume(0);

  for (element *e : pnmRange<element>(network)) {
    if (e->getPhaseFlag() == phase::oil &&
        e->getWettabilityFlag() == wettability::waterWet)
      waterVolume += e->getWaterFilmVolume();

    if (e->getPhaseFlag() == phase::oil &&
        e->getWettabilityFlag() == wettability::oilWet)
      waterVolume += e->getWaterFilmVolume();

    if (e->getPhaseFlag() == phase::water &&
        e->getWettabilityFlag() == wettability::oilWet) {
      if (e->getOilLayerActivated() &&
          e->getClusterOilConductor()->getInlet() &&
          e->getClusterWaterConductor()->getOutlet())
        adjustVolumetrics(e);

      waterVolume += e->getEffectiveVolume() + e->getWaterFilmVolume();
    }

    if (e->getPhaseFlag() == phase::water &&
        e->getWettabilityFlag() == wettability::waterWet)
      waterVolume += e->getVolume();
  }

  currentSw = waterVolume / network->totalNetworkVolume;
}

bool spontaneousOilInvasion::isInvadableViaSnapOff(element *e) {
  bool isInvadable = false;
  double entryPressure =
      userInput::get().OWSurfaceTension * cos(e->getTheta()) / e->getRadius();
  if (currentPc + 1e-5 >= entryPressure && e->getOilLayerActivated() &&
      e->getClusterOilConductor()->getInlet() &&
      e->getClusterWaterConductor()->getOutlet())
    isInvadable = true;
  return isInvadable;
}

bool spontaneousOilInvasion::isInvadableViaBulk(element *e) {
  bool isInvadable = false;

  if (e->getType() == capillaryType::throat &&
      (e->getInlet() || isConnectedToInletCluster(e)) &&
      e->getClusterWaterConductor()->getOutlet()) {
    double entryPressure = e->getEntryPressureCoefficient() *
                           userInput::get().OWSurfaceTension *
                           std::cos(e->getTheta()) / e->getRadius();
    if (currentPc + 1e-5 >= entryPressure) isInvadable = true;
  }

  else if (e->getType() == capillaryType::poreBody &&
           isConnectedToInletCluster(e) &&
           e->getClusterWaterConductor()->getOutlet()) {
    int waterNeighboorsNumber(0);
    for (element *n : e->getNeighboors()) {
      if (n->getPhaseFlag() == phase::water) waterNeighboorsNumber++;
    }

    double entryPressureBodyFilling = 0;
    if (waterNeighboorsNumber == 1)
      entryPressureBodyFilling = e->getEntryPressureCoefficient() *
                                 userInput::get().OWSurfaceTension *
                                 std::cos(e->getTheta()) / e->getRadius();
    if (waterNeighboorsNumber > 1)
      entryPressureBodyFilling = e->getEntryPressureCoefficient() *
                                 userInput::get().OWSurfaceTension *
                                 std::cos(e->getTheta()) / e->getRadius() /
                                 double(waterNeighboorsNumber);

    if (currentPc + 1e-5 >= entryPressureBodyFilling) isInvadable = true;
  }

  return isInvadable;
}

bool spontaneousOilInvasion::isConnectedToInletCluster(element *e) {
  bool connectedToInletCluster = false;
  for (element *n : e->getNeighboors())
    if (n->getPhaseFlag() == phase::oil &&
        n->getClusterOilConductor()->getInlet()) {
      connectedToInletCluster = true;
      break;
    }
  return connectedToInletCluster;
}

void spontaneousOilInvasion::fillWithOil(element *e) {
  e->setPhaseFlag(phase::oil);
  e->setOilConductor(true);
  e->setOilFraction(1);
  e->setWaterFraction(0);
  e->setOilLayerActivated(false);
  e->setOilFilmVolume(0);
  e->setOilFilmConductivity(1e-200);

  if (!e->getWaterCornerActivated()) e->setWaterConductor(false);
}

void spontaneousOilInvasion::adjustVolumetrics(element *e) {
  double rSquared = std::pow(userInput::get().OWSurfaceTension / currentPc, 2);
  double filmVolume =
      std::min(rSquared * e->getFilmAreaCoefficient() * e->getLength(),
               (1 - 4 * maths::pi() * e->getShapeFactor()) * e->getVolume() -
                   e->getWaterFilmVolume());
  double filmConductance = rSquared * filmVolume / e->getLength() /
                           (userInput::get().oilViscosity * e->getLength());

  e->setOilFilmVolume(filmVolume);
  e->setOilFilmConductivity(filmConductance);
  e->setEffectiveVolume(e->getVolume() - e->getOilFilmVolume() -
                        e->getWaterFilmVolume());
}

void spontaneousOilInvasion::updateVariables() {
  step++;

  if (step != userInput::get().twoPhaseSimulationSteps) {
    currentRadius += radiusStep;
    currentPc = -userInput::get().OWSurfaceTension / currentRadius;
  }
}

void spontaneousOilInvasion::updateOutputFiles() {
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

void spontaneousOilInvasion::generateNetworkStateFiles() {
  if (!userInput::get().extractDataSS) return;

  pnmOperation::get(network).generateNetworkState(frameCount,
                                                  "Spontaneous_Oil_Invasion");
  frameCount++;
}

}  // namespace PNM
