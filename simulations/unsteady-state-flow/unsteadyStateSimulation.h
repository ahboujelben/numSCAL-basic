/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef UNSTEADYSTATESIMULATION_H
#define UNSTEADYSTATESIMULATION_H

#include "simulations/simulation.h"

#include <unordered_set>

namespace PNM {

class pore;
class node;

class unsteadyStateSimulation : public simulation {
 public:
  unsteadyStateSimulation();
  ~unsteadyStateSimulation() override;
  unsteadyStateSimulation(const unsteadyStateSimulation &) = delete;
  unsteadyStateSimulation(unsteadyStateSimulation &&) = delete;
  auto operator=(const unsteadyStateSimulation &)
      -> unsteadyStateSimulation & = delete;
  auto operator=(unsteadyStateSimulation &&)
      -> unsteadyStateSimulation & = delete;

  virtual void run() override;
  virtual std::string getNotification() override;
  virtual int getProgress() override;

 private:
  void initialiseOutputFiles();
  void initialiseCapillaries();
  void initialiseSimulationAttributes();
  void addWaterChannel();
  void setInitialTerminalFlags();
  void fetchTrappedCapillaries();
  void updateCapillaryPropreties();
  void solvePressureField();
  void calculateTimeStep();
  void updateFluidFractions();
  void updateFluidTerminalFlags();
  void updateOutputFiles();
  void generateNetworkStateFiles();
  void updateVariables();

  double simulationTime;
  double timeSoFar;
  double timeStep;
  double injectedPVs;
  double currentSw;
  double capillaryNumber;
  double flowVelocity;
  double outputCounter;
  int frameCount;
  bool updatePressureCalculation;
  std::string satFilename;
  std::string fractionalFilename;
  std::string pressureFilename;
  std::unordered_set<pore *> poresToCheck;
  std::unordered_set<node *> nodesToCheck;
};

}  // namespace PNM

#endif  // UNSTEADYSTATESIMULATION_H
