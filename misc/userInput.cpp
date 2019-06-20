/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "userInput.h"
#include "maths.h"

#include <libs/boost/property_tree/ini_parser.hpp>
#include <libs/boost/property_tree/ptree.hpp>

namespace PNM {

userInput::userInput() {}

userInput userInput::instance;

userInput &userInput::get() { return instance; }

void userInput::loadNetworkData() {
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini("Input_Data/Parameters.txt", pt);

  networkRegular = pt.get<bool>("NetworkGeneration_Source.networkRegular");
  networkStatoil = pt.get<bool>("NetworkGeneration_Source.networkStatoil");
  networkNumscal = pt.get<bool>("NetworkGeneration_Source.networkNumscal");
  extractedNetworkFolderPath =
      pt.get<std::string>("NetworkGeneration_Source.extractedNetworkPath");
  rockPrefix = pt.get<std::string>("NetworkGeneration_Source.rockPrefix");

  Nx = pt.get<int>("NetworkGeneration_Geometry.Nx");
  Ny = pt.get<int>("NetworkGeneration_Geometry.Ny");
  Nz = pt.get<int>("NetworkGeneration_Geometry.Nz");
  minRadius = pt.get<double>("NetworkGeneration_Geometry.minRadius") * 1e-6;
  maxRadius = pt.get<double>("NetworkGeneration_Geometry.maxRadius") * 1e-6;
  poreSizeDistribution =
      (psd)pt.get<int>("NetworkGeneration_Geometry.radiusDistribution");
  rayleighParameter =
      pt.get<double>("NetworkGeneration_Geometry.rayleighParameter") * 1e-6;
  triangularParameter =
      pt.get<double>("NetworkGeneration_Geometry.triangularParameter") * 1e-6;
  normalMuParameter =
      pt.get<double>("NetworkGeneration_Geometry.normalMuParameter") * 1e-6;
  normalSigmaParameter =
      pt.get<double>("NetworkGeneration_Geometry.normalSigmaParameter") * 1e-6;
  poreVolumeConstant =
      pt.get<double>("NetworkGeneration_Geometry.poreVolumeConstant");
  poreVolumeExponent =
      pt.get<double>("NetworkGeneration_Geometry.poreVolumeExponent");
  poreConductivityConstant =
      pt.get<double>("NetworkGeneration_Geometry.poreConductivityConstant");
  poreConductivityExponent =
      pt.get<double>("NetworkGeneration_Geometry.poreConductivityExponent");
  coordinationNumber =
      pt.get<double>("NetworkGeneration_Geometry.coordinationNumber");
  degreeOfDistortion =
      pt.get<double>("NetworkGeneration_Geometry.degreeOfDistortion");
  aspectRatio = pt.get<double>("NetworkGeneration_Geometry.aspectRatio");
  length = pt.get<double>("NetworkGeneration_Geometry.length") * 1e-6;
  seed = pt.get<int>("NetworkGeneration_Geometry.seed");

  wettability = (networkWettability)pt.get<int>(
      "NetworkGeneration_Wettability.wettabilityFlag");
  minWaterWetTheta =
      pt.get<double>("NetworkGeneration_Wettability.minWaterWetTheta") *
      (maths::pi() / 180.);
  maxWaterWetTheta =
      pt.get<double>("NetworkGeneration_Wettability.maxWaterWetTheta") *
      (maths::pi() / 180.);
  minOilWetTheta =
      pt.get<double>("NetworkGeneration_Wettability.minOilWetTheta") *
      (maths::pi() / 180.);
  maxOilWetTheta =
      pt.get<double>("NetworkGeneration_Wettability.maxOilWetTheta") *
      (maths::pi() / 180.);
  oilWetFraction =
      pt.get<double>("NetworkGeneration_Wettability.oilWetFraction");
  shapeFactor = pt.get<double>("NetworkGeneration_Wettability.shapeFactor");
}

void userInput::loadSimulationData() {
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini("Input_Data/Parameters.txt", pt);

  twoPhaseSS = pt.get<bool>("FluidInjection_Cycles.twoPhaseSS");
  drainageUSS = pt.get<bool>("FluidInjection_Cycles.drainageUSS");
  tracerFlow = pt.get<bool>("FluidInjection_Cycles.tracerFlow");
  templateFlow = pt.get<bool>("FluidInjection_Cycles.templateFlow");
  primaryDrainageSimulation =
      pt.get<bool>("FluidInjection_Cycles.primaryDrainageSimulation");
  spontaneousImbibitionSimulation =
      pt.get<bool>("FluidInjection_Cycles.primaryImbibitionSimulation");
  forcedWaterInjectionSimulation =
      pt.get<bool>("FluidInjection_Cycles.secondaryDrainageSimulation");
  spontaneousOilInvasionSimulation =
      pt.get<bool>("FluidInjection_Cycles.secondaryImbibitionSimulation");
  secondaryOilDrainageSimulation =
      pt.get<bool>("FluidInjection_Cycles.tertiaryDrainageSimulation");

  twoPhaseSimulationSteps =
      pt.get<int>("FluidInjection_SS.twoPhaseSimulationSteps");
  filmConductanceResistivity =
      pt.get<double>("FluidInjection_SS.filmConductanceResistivity");
  relativePermeabilitiesCalculation =
      pt.get<bool>("FluidInjection_SS.relativePermeabilitiesCalculation");
  extractDataSS = pt.get<bool>("FluidInjection_SS.extractDataSS");

  flowRate = pt.get<double>("FluidInjection_USS.flowRate");
  simulationTime = pt.get<double>("FluidInjection_USS.simulationTime");
  overrideByInjectedPVs =
      pt.get<bool>("FluidInjection_USS.overrideByInjectedPVs");
  injectedPVs = pt.get<double>("FluidInjection_USS.injectedPVs");
  tracerDiffusionCoef =
      pt.get<double>("FluidInjection_USS.tracerDiffusionCoef");
  extractDataUSS = pt.get<bool>("FluidInjection_USS.extractDataUSS");

  oilViscosity = pt.get<double>("FluidInjection_Fluids.oilViscosity") * 1e-3;
  waterViscosity =
      pt.get<double>("FluidInjection_Fluids.waterViscosity") * 1e-3;
  gasViscosity = pt.get<double>("FluidInjection_Fluids.gasViscosity") * 1e-3;
  OGSurfaceTension =
      pt.get<double>("FluidInjection_Fluids.OGSurfaceTension") * 1e-3;
  OWSurfaceTension =
      pt.get<double>("FluidInjection_Fluids.OWSurfaceTension") * 1e-3;
  WGSurfaceTension =
      pt.get<double>("FluidInjection_Fluids.WGSurfaceTension") * 1e-3;
  initialWaterSaturation =
      pt.get<double>("FluidInjection_Fluids.initialWaterSaturation");
  waterDistribution =
      (swi)pt.get<int>("FluidInjection_Fluids.waterDistribution");

  solverChoice = (solver)pt.get<int>("FluidInjection_Misc.solverChoice");

  pathToNetworkStateFiles = pt.get<std::string>(
      "FluidInjection_Postprocessing.pathToNetworkStateFiles");
  rendererFPS = pt.get<int>("FluidInjection_Postprocessing.rendererFPS");
  keepFrames = pt.get<bool>("FluidInjection_Postprocessing.keepFrames");
}

}  // namespace PNM
