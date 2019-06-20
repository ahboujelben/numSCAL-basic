/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef SPONTANEOUSIMBIBTION_H
#define SPONTANEOUSIMBIBTION_H

#include "simulations/simulation.h"

#include <unordered_set>

namespace PNM {
class element;

class spontaneousImbibtion : public simulation {
 public:
  spontaneousImbibtion();
  ~spontaneousImbibtion() override;
  spontaneousImbibtion(const spontaneousImbibtion &) = delete;
  spontaneousImbibtion(spontaneousImbibtion &&) = delete;
  auto operator=(const spontaneousImbibtion &)
      -> spontaneousImbibtion & = delete;
  auto operator=(spontaneousImbibtion &&) -> spontaneousImbibtion & = delete;

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

#endif  // SPONTANEOUSIMBIBTION_H
