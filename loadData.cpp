/////////////////////////////////////////////////////////////////////////////
/// Name:        loadData.cpp
/// Purpose:     methods related to populating network attributes from INI
///              input data files are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

void network::loadNetworkData()
{
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("Input Data/network_data.txt", pt);

    networkSource=pt.get<int>("Network_Source.source");
    extractedNetworkFolderPath=pt.get<std::string>("Network_Source.extractedNetworkPath");
    rockPrefix=pt.get<std::string>("Network_Source.rockPrefix");
    extension=pt.get<bool>("Network_Source.extension");
    extensionNumber=pt.get<int>("Network_Source.extensionNumber");
    extensionOneDirection=pt.get<bool>("Network_Source.extensionOneDirection");

    Nx=pt.get<int>("Geometry.Nx");
    Ny=pt.get<int>("Geometry.Ny");
    Nz=pt.get<int>("Geometry.Nz");
    minRadius=pt.get<double>("Geometry.minRadius")*1e-6;
    maxRadius=pt.get<double>("Geometry.maxRadius")*1e-6;
    radiusDistribution=pt.get<int>("Geometry.radiusDistribution");
    rayleighParameter=pt.get<double>("Geometry.rayleighParameter")*1e-6;
    triangularParameter=pt.get<double>("Geometry.triangularParameter")*1e-6;
    normalMuParameter=pt.get<double>("Geometry.normalMuParameter")*1e-6;
    normalSigmaParameter=pt.get<double>("Geometry.normalSigmaParameter")*1e-6;
    poreVolumeConstant=pt.get<double>("Geometry.poreVolumeConstant");
    poreVolumeExponent=pt.get<double>("Geometry.poreVolumeExponent");
    poreConductivityConstant=pt.get<double>("Geometry.poreConductivityConstant");
    poreConductivityExponent=pt.get<double>("Geometry.poreConductivityExponent");
    coordinationNumber=pt.get<double>("Geometry.coordinationNumber");
    degreeOfDistortion=pt.get<double>("Geometry.degreeOfDistortion");
    aspectRatio=pt.get<double>("Geometry.aspectRatio");
    length=pt.get<double>("Geometry.length")*1e-6;
    seed=pt.get<int>("Geometry.seed");
    solverChoice=pt.get<int>("Geometry.solverChoice");
    absolutePermeabilityCalculation=pt.get<bool>("Geometry.absolutePermeabilityCalculation");
    extractData=pt.get<bool>("Geometry.extractData");

    wettingTypeFlag=pt.get<int>("Wettability.wettabilityFlag");
    minWaterWetTheta=pt.get<double>("Wettability.minWaterWetTheta")*(tools::pi()/180);
    maxWaterWetTheta=pt.get<double>("Wettability.maxWaterWetTheta")*(tools::pi()/180);
    minOilWetTheta=pt.get<double>("Wettability.minOilWetTheta")*(tools::pi()/180);
    maxOilWetTheta=pt.get<double>("Wettability.maxOilWetTheta")*(tools::pi()/180);
    oilWetFraction=pt.get<double>("Wettability.oilWetFraction");
    shapeFactor=pt.get<double>("Wettability.shapeFactor");
}

void network::loadTwoPhaseData()
{
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("Input Data/twoPhaseFlow_data.txt", pt);

    twoPhaseSS=pt.get<bool>("Cycles.twoPhaseSS");
    threePhaseSS=pt.get<bool>("Cycles.threePhaseSS");
    ganglionUSS=pt.get<bool>("Cycles.ganglionUSS");
    drainageUSS=pt.get<bool>("Cycles.drainageUSS");
    primaryDrainageSimulation=pt.get<bool>("Cycles.primaryDrainageSimulation");
    primaryImbibitionSimulation=pt.get<bool>("Cycles.primaryImbibitionSimulation");
    secondaryDrainageSimulation=pt.get<bool>("Cycles.secondaryDrainageSimulation");
    secondaryImbibitionSimulation=pt.get<bool>("Cycles.secondaryImbibitionSimulation");
    tertiaryDrainageSimulation=pt.get<bool>("Cycles.tertiaryDrainageSimulation");

    twoPhaseSimulationSteps=pt.get<int>("Parameters.twoPhaseSimulationSteps");
    twoPhaseSeed=pt.get<int>("Parameters.twoPhaseSeed");
    waterWetThetaIncrementAfterAging=pt.get<double>("Parameters.waterWetThetaIncrementAfterAging")*(tools::pi()/180);
    oilWetThetaIncrementAfterAging=pt.get<double>("Parameters.oilWetThetaIncrementAfterAging")*(tools::pi()/180);
    waterFilms=pt.get<bool>("Parameters.waterFilms");
    oilFilms=pt.get<bool>("Parameters.oilFilms");
    gravity=pt.get<bool>("Parameters.gravity");
    g=pt.get<double>("Parameters.g");
    viscousForces=pt.get<bool>("Parameters.viscousForces");
    relativePermeabilitiesCalculation=pt.get<bool>("Parameters.relativePermeabilitiesCalculation");
    videoRecording=pt.get<bool>("Parameters.videoRecording");

    oilViscosity=pt.get<double>("Fluids.oilViscosity")*1e-3;
    waterViscosity=pt.get<double>("Fluids.waterViscosity")*1e-3;
    gasViscosity=pt.get<double>("Fluids.gasViscosity")*1e-3;
    OGSurfaceTension=pt.get<double>("Fluids.OGSurfaceTension")*1e-3;
    OWSurfaceTension=pt.get<double>("Fluids.OWSurfaceTension")*1e-3;
    WGSurfaceTension=pt.get<double>("Fluids.WGSurfaceTension")*1e-3;
    initialWaterSaturation=pt.get<double>("Fluids.initialWaterSaturation");
    waterDistribution=pt.get<int>("Fluids.waterDistribution");
    waterWetSwi=pt.get<bool>("Fluids.waterWetSwi");
}

void network::loadDepletionData()
{

}
