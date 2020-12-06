/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "simulation.h"
#include "misc/scopedtimer.h"
#include "misc/tools.h"
#include "misc/userInput.h"
#include "simulations/renderer/renderer.h"
#include "simulations/steady-state-cycle/steadyStateSimulation.h"
#include "simulations/template-simulation/templateFlowSimulation.h"
#include "simulations/tracer-flow/tracerFlowSimulation.h"
#include "simulations/unsteady-state-flow/unsteadyStateSimulation.h"

#include <iostream>
#include <thread>

namespace PNM {
simulation::simulation(QObject *parent) : QObject(parent) {
  simulationInterrupted = false;
}

std::shared_ptr<simulation> simulation::createSimulation() {
  if (userInput::get().twoPhaseSS)
    return std::make_shared<steadyStateSimulation>();

  if (userInput::get().drainageUSS)
    return std::make_shared<unsteadyStateSimulation>();

  if (userInput::get().tracerFlow)
    return std::make_shared<tracerFlowSimulation>();

  if (userInput::get().templateFlow)
    return std::make_shared<templateFlowSimulation>();

  throw std::invalid_argument("Invalid simulation type.\n");
}

std::shared_ptr<simulation> simulation::createRenderer() {
  return std::make_shared<renderer>();
}

void simulation::execute() {
  initialise();
  run();
  finalise();
}

void simulation::setNetwork(const std::shared_ptr<networkModel> &value) {
  network = value;
}

void simulation::initialise() { tools::createRequiredFolders(); }

void simulation::finalise() {
  ScopedTimer::printProfileData();
  emit finished();
}

void simulation::updateGUI() { emit notifyGUI(); }

void simulation::interrupt() { simulationInterrupted = true; }

}  // namespace PNM
