/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef SECONDARYOILDRAINAGE_H
#define SECONDARYOILDRAINAGE_H

#include "simulations/simulation.h"

#include <unordered_set>

namespace PNM {
class element;

class secondaryOilDrainage : public simulation {
 public:
  secondaryOilDrainage();
  ~secondaryOilDrainage() override;
  secondaryOilDrainage(const secondaryOilDrainage &) = delete;
  secondaryOilDrainage(secondaryOilDrainage &&) = delete;
  auto operator=(const secondaryOilDrainage &)
      -> secondaryOilDrainage & = delete;
  auto operator=(secondaryOilDrainage &&) -> secondaryOilDrainage & = delete;

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
  void invadeCapillariesViaSnapOff();
  void invadeCapillariesViaBulk();
  void dismissTrappedElements();
  void adjustCapillaryVolumes();
  bool isInvadableViaSnapOff(element *);
  bool isInvadable(element *);
  bool isConnectedToInletCluster(element *);
  void fillWithOil(element *);
  void adjustVolumetrics(element *);
  void updateOutputFiles();
  void generateNetworkStateFiles();
  void updateVariables();

  int step;
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

}  // namespace PNM

#endif  // SECONDARYOILDRAINAGE_H
