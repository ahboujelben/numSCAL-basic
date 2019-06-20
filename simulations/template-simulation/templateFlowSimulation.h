/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef TEMPLATEFLOWSIMULATION_H
#define TEMPLATEFLOWSIMULATION_H

#include "simulations/simulation.h"

namespace PNM {

class templateFlowSimulation : public simulation {
 public:
  templateFlowSimulation();
  ~templateFlowSimulation() override;
  templateFlowSimulation(const templateFlowSimulation &) = delete;
  templateFlowSimulation(templateFlowSimulation &&) = delete;
  auto operator=(const templateFlowSimulation &)
      -> templateFlowSimulation & = delete;
  auto operator=(templateFlowSimulation &&)
      -> templateFlowSimulation & = delete;

  virtual void run() override;
  virtual std::string getNotification() override;
  virtual int getProgress() override;

 private:
  // methods
  ////////////////////////////////////////

  // attributes
  ///////////////////////////////////////
  double simulationTime;
  double timeSoFar;
  double timeStep;
};

}  // namespace PNM

#endif  // TEMPLATEFLOWSIMULATION_H
