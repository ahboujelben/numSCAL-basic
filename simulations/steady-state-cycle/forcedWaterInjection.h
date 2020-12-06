/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef FORCEDWATERINJECTION_H
#define FORCEDWATERINJECTION_H

#include "simulations/simulation.h"

#include <unordered_set>

namespace PNM {
class element;

class forcedWaterInjection : public simulation {
 public:
  forcedWaterInjection();
  ~forcedWaterInjection() override;
  forcedWaterInjection(const forcedWaterInjection &) = delete;
  forcedWaterInjection(forcedWaterInjection &&) = delete;
  auto operator=(const forcedWaterInjection &)
      -> forcedWaterInjection & = delete;
  auto operator=(forcedWaterInjection &&) -> forcedWaterInjection & = delete;

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
  void fillWithWater(element *);
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

#endif  // FORCEDWATERINJECTION_H
