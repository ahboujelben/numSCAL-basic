/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef TRACERFLOWSIMULATION_H
#define TRACERFLOWSIMULATION_H

#include "simulations/simulation.h"

namespace PNM {

class tracerFlowSimulation : public simulation {
 public:
  tracerFlowSimulation();
  ~tracerFlowSimulation() override;
  tracerFlowSimulation(const tracerFlowSimulation &) = delete;
  tracerFlowSimulation(tracerFlowSimulation &&) = delete;
  auto operator=(const tracerFlowSimulation &)
      -> tracerFlowSimulation & = delete;
  auto operator=(tracerFlowSimulation &&) -> tracerFlowSimulation & = delete;

  virtual void run() override;
  virtual std::string getNotification() override;
  virtual int getProgress() override;

 private:
  void initialiseOutputFiles();
  void initialiseSimulationAttributes();
  void initialiseCapillaries();
  void setInitialAttributes();
  void fetchNonFlowingCapillaries();
  void solvePressureField();
  void calculateTimeStep();
  void updateConcentrations();
  void updateVariables();
  void updateOutputFiles();
  void generateNetworkStateFiles();

  double simulationTime;
  double timeSoFar;
  double timeStep;
  double injectedPVs;
  double flowVelocity;
  double outputCounter;
  int frameCount;
};

}  // namespace PNM

#endif  // TRACERFLOWSIMULATION_H
