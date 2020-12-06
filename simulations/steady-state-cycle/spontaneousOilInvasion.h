/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef SPONTANEOUSOILINVASION_H
#define SPONTANEOUSOILINVASION_H

#include "simulations/simulation.h"

#include <unordered_set>

namespace PNM {
class element;

class spontaneousOilInvasion : public simulation {
 public:
  spontaneousOilInvasion();
  ~spontaneousOilInvasion() override;
  spontaneousOilInvasion(const spontaneousOilInvasion &) = delete;
  spontaneousOilInvasion(spontaneousOilInvasion &&) = delete;
  auto operator=(const spontaneousOilInvasion &)
      -> spontaneousOilInvasion & = delete;
  auto operator=(spontaneousOilInvasion &&)
      -> spontaneousOilInvasion & = delete;

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
  bool isInvadableViaBulk(element *);
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

#endif  // SPONTANEOUSOILINVASION_H
