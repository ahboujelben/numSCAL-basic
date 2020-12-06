/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "templateFlowSimulation.h"
#include "misc/userInput.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace PNM {

templateFlowSimulation::templateFlowSimulation() {}

templateFlowSimulation::~templateFlowSimulation() {}

void templateFlowSimulation::run() {
  // initialisation
  /////////////////////////////////////
  simulationTime = userInput::get().simulationTime;
  timeStep = 0.01;
  timeSoFar = 0;

  while (!simulationInterrupted && timeSoFar < simulationTime) {
    // algorithms
    ////////////////////////////////

    timeSoFar += timeStep;

    updateGUI();

    if (simulationInterrupted) break;
  }
}

std::string templateFlowSimulation::getNotification() {
  std::ostringstream ss;
  ss << "Template Flow Simulation \n"
     << std::fixed << std::setprecision(2) << "Time: " << timeSoFar;
  return ss.str();
}

int templateFlowSimulation::getProgress() {
  return static_cast<int>(timeSoFar / simulationTime * 100);
}

}  // namespace PNM
