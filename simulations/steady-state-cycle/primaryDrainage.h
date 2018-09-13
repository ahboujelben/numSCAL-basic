/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef PRIMARYDRAINAGE_H
#define PRIMARYDRAINAGE_H

#include "simulations/simulation.h"

#include <unordered_set>

namespace PNM
{
class element;

class primaryDrainage : public simulation
{
public:
  primaryDrainage(double _finalSwi = 0);
  ~primaryDrainage() {}
  primaryDrainage(const primaryDrainage &) = delete;
  primaryDrainage(primaryDrainage &&) = delete;
  auto operator=(const primaryDrainage &) -> primaryDrainage & = delete;
  auto operator=(primaryDrainage &&) -> primaryDrainage & = delete;
  virtual void run() override;
  virtual std::string getNotification() override;
  virtual int getProgress() override;

private:
  void initialiseOutputFiles();
  void initialiseSimulationAttributes();
  void initialiseCapillaries();
  double getMinPc();
  double getMaxPc();
  void invadeCapillariesAtCurrentPc();
  void dismissTrappedElements();
  void adjustCapillaryVolumes();
  bool isInvadable(element *);
  void addNeighboorsToElementsToInvade(element *);
  void fillWithOil(element *);
  void adjustVolumetrics(element *);
  void updateOutputFiles();
  void generateNetworkStateFiles();
  void updateVariables();
  void checkTerminationCondition();
  void finalise();

  double finalSwi;
  double step;
  double radiusStep;
  double currentRadius;
  double currentPc;
  double currentSw;
  double outputCounter;
  int frameCount;
  std::string pcFilename;
  std::string relPermFilename;
  std::unordered_set<element *> elementsToInvade;
};

} // namespace PNM

#endif // PRIMARYDRAINAGE_H
