/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "steadyStateSimulation.h"
#include "forcedWaterInjection.h"
#include "misc/userInput.h"
#include "primaryDrainage.h"
#include "secondaryOilDrainage.h"
#include "spontaneousImbibtion.h"
#include "spontaneousOilInvasion.h"

namespace PNM {

steadyStateSimulation::steadyStateSimulation() {}

steadyStateSimulation::~steadyStateSimulation() {}

void steadyStateSimulation::run() {
  if (userInput::get().primaryDrainageSimulation) {
    currentSimulation = std::make_shared<primaryDrainage>();
    runCurrentSimulation();
  }

  if (!simulationInterrupted &&
      userInput::get().spontaneousImbibitionSimulation) {
    currentSimulation = std::make_shared<spontaneousImbibtion>();
    runCurrentSimulation();
  }

  if (!simulationInterrupted &&
      userInput::get().forcedWaterInjectionSimulation) {
    currentSimulation = std::make_shared<forcedWaterInjection>();
    runCurrentSimulation();
  }

  if (!simulationInterrupted &&
      userInput::get().spontaneousOilInvasionSimulation) {
    currentSimulation = std::make_shared<spontaneousOilInvasion>();
    runCurrentSimulation();
  }

  if (!simulationInterrupted &&
      userInput::get().secondaryOilDrainageSimulation) {
    currentSimulation = std::make_shared<secondaryOilDrainage>();
    runCurrentSimulation();
  }
}

std::string steadyStateSimulation::getNotification() {
  return currentSimulation->getNotification();
}

int steadyStateSimulation::getProgress() {
  return currentSimulation->getProgress();
}

void steadyStateSimulation::interrupt() {
  simulationInterrupted = true;
  currentSimulation->interrupt();
}

void steadyStateSimulation::runCurrentSimulation() {
  currentSimulation->setNetwork(network);
  connect(currentSimulation.get(), SIGNAL(notifyGUI()), this,
          SLOT(updateGUI()));
  currentSimulation->execute();
}

}  // namespace PNM
