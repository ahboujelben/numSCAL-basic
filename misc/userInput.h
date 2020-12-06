/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef USERINPUT_H
#define USERINPUT_H

#include <string>

namespace PNM {

enum class networkWettability {
  waterWet = 1,
  oilWet = 2,
  fracionalWet = 3,
  mixedWetSmall = 4,
  mixdeWetLarge = 5
};

enum class psd {
  uniform = 1,
  rayleigh = 2,
  triangular = 3,
  truncatedNormal = 4
};

enum class swi {
  random = 1,
  waterInSmallCapillaries = 2,
  waterInBigCapillaries = 3,
  afterPrimaryDrainage = 4
};

enum class solver { cholesky = 1, conjugateGradient = 2 };

class userInput {
 public:
  static userInput &get();
  void loadNetworkData();
  void loadSimulationData();

  // Network Data
  double minRadius;
  double maxRadius;
  double rayleighParameter;
  double triangularParameter;
  double normalMuParameter;
  double normalSigmaParameter;
  double poreVolumeConstant;
  double poreVolumeExponent;
  double poreConductivityConstant;
  double poreConductivityExponent;
  double coordinationNumber;
  double degreeOfDistortion;
  double aspectRatio;
  double length;
  double minWaterWetTheta;
  double maxWaterWetTheta;
  double minOilWetTheta;
  double maxOilWetTheta;
  double oilWetFraction;
  double shapeFactor;
  int seed;
  int Nx;
  int Ny;
  int Nz;
  psd poreSizeDistribution;
  solver solverChoice;
  networkWettability wettability;
  bool networkRegular;
  bool networkStatoil;
  bool networkNumscal;
  std::string extractedNetworkFolderPath;
  std::string rockPrefix;

  // Simulation Data

  // Type
  bool twoPhaseSS;
  bool drainageUSS;
  bool tracerFlow;
  bool templateFlow;

  // SS
  bool primaryDrainageSimulation;
  bool spontaneousImbibitionSimulation;
  bool forcedWaterInjectionSimulation;
  bool spontaneousOilInvasionSimulation;
  bool secondaryOilDrainageSimulation;
  bool relativePermeabilitiesCalculation;
  bool extractDataSS;
  int twoPhaseSimulationSteps;
  double filmConductanceResistivity;

  // USS / Tracer
  double flowRate;
  double simulationTime;
  bool overrideByInjectedPVs;
  double injectedPVs;
  bool enhancedWaterConnectivity;
  double tracerDiffusionCoef;
  bool extractDataUSS;
  double oilViscosity;
  double waterViscosity;
  double gasViscosity;
  double OGSurfaceTension;
  double OWSurfaceTension;
  double WGSurfaceTension;
  double initialWaterSaturation;
  swi waterDistribution;

  // Template
  ///////////////////////////////////

  // Post-processing
  std::string pathToNetworkStateFiles;
  int rendererFPS;
  bool keepFrames;

 private:
  userInput();
  static userInput instance;
};

}  // namespace PNM
#endif  // USERINPUT_H
