/////////////////////////////////////////////////////////////////////////////
/// Name:        network.cpp
/// Purpose:     attributes and methods of the loaded network
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

network::network(QObject *parent):
    QObject(parent)
{
    reset();
}

network::~network()
{
    for (int i = 0; i < totalPores; ++i)
        delete tableOfAllPores[i];

    for (int i = 0; i < totalNodes; ++i)
        delete tableOfAllNodes[i];

    if(!waterClusters.empty())
        for (unsigned i = 0; i < waterClusters.size(); ++i)
            delete waterClusters[i];
    if(!oilClusters.empty())
        for (unsigned i = 0; i < oilClusters.size(); ++i)
            delete oilClusters[i];
    if(!gasClusters.empty())
        for (unsigned i = 0; i < gasClusters.size(); ++i)
            delete gasClusters[i];
    if(!waterWetClusters.empty())
        for (unsigned i = 0; i < waterWetClusters.size(); ++i)
            delete waterWetClusters[i];
    if(!oilWetClusters.empty())
        for (unsigned i = 0; i < oilWetClusters.size(); ++i)
            delete oilWetClusters[i];
    if(!waterFilmClusters.empty())
        for (unsigned i = 0; i < waterFilmClusters.size(); ++i)
            delete waterFilmClusters[i];
    if(!oilFilmClusters.empty())
        for (unsigned i = 0; i < oilFilmClusters.size(); ++i)
            delete oilFilmClusters[i];
    if(!waterLayerClusters.empty())
        for (unsigned i = 0; i < waterLayerClusters.size(); ++i)
            delete waterLayerClusters[i];
    if(!oilLayerClusters.empty())
        for (unsigned i = 0; i < oilLayerClusters.size(); ++i)
            delete oilLayerClusters[i];
    if(!existClusters.empty())
        for (unsigned i = 0; i < existClusters.size(); ++i)
            delete existClusters[i];

    tools::cleanVideosFolder();
}

void network::destroy()
{
    for (int i = 0; i < totalPores; ++i)
        delete tableOfAllPores[i];

    for (int i = 0; i < totalNodes; ++i)
        delete tableOfAllNodes[i];

    totalPores=0;
    totalNodes=0;
    tableOfAllPores.clear();
    tableOfAllNodes.clear();
    tableOfNodes.clear();
    tableOfPoresX.clear();
    tableOfPoresY.clear();
    tableOfPoresZ.clear();

    if(!waterClusters.empty())
        for (unsigned i = 0; i < waterClusters.size(); ++i)
            delete waterClusters[i];
    if(!oilClusters.empty())
        for (unsigned i = 0; i < oilClusters.size(); ++i)
            delete oilClusters[i];
    if(!gasClusters.empty())
        for (unsigned i = 0; i < gasClusters.size(); ++i)
            delete gasClusters[i];
    if(!waterWetClusters.empty())
        for (unsigned i = 0; i < waterWetClusters.size(); ++i)
            delete waterWetClusters[i];
    if(!oilWetClusters.empty())
        for (unsigned i = 0; i < oilWetClusters.size(); ++i)
            delete oilWetClusters[i];
    if(!waterFilmClusters.empty())
        for (unsigned i = 0; i < waterFilmClusters.size(); ++i)
            delete waterFilmClusters[i];
    if(!oilFilmClusters.empty())
        for (unsigned i = 0; i < oilFilmClusters.size(); ++i)
            delete oilFilmClusters[i];
    if(!waterLayerClusters.empty())
        for (unsigned i = 0; i < waterLayerClusters.size(); ++i)
            delete waterLayerClusters[i];
    if(!oilLayerClusters.empty())
        for (unsigned i = 0; i < oilLayerClusters.size(); ++i)
            delete oilLayerClusters[i];
    if(!existClusters.empty())
        for (unsigned i = 0; i < existClusters.size(); ++i)
            delete existClusters[i];

    waterClusters.clear();
    oilClusters.clear();
    gasClusters.clear();
    waterWetClusters.clear();
    oilWetClusters.clear();
    waterFilmClusters.clear();
    oilFilmClusters.clear();
    waterLayerClusters.clear();
    oilLayerClusters.clear();
    existClusters.clear();


    tools::cleanVideosFolder();
}

void network::reset()
{
    pressureIn=1;
    pressureOut=0;

    totalPores=0;
    totalNodes=0;

    record=false;
    videoRecording=false;
    ready=false;
    cancel=false;
    simulationRunning=false;

    oilSpreading=false;

    extractionTimestep=1;
}

void network::setupModel()
{
    if(ready)
    {
        ready=false;
        destroy();
    }

    reset();
    loadNetworkData();

    gen.seed(seed);

    if(networkSource==1 || networkSource==2)
        setupRegularModel();
    if(networkSource==3)
        setupExtractedModel();

    ready=true;
    emit plot();
}

void network::runTwoPhaseFlowSimulation()
{
    tools::cleanResultsFolder();

    loadTwoPhaseData();
    gen.seed(twoPhaseSeed);

    if(networkSource==1)
    {
        if(twoPhaseSS)
            runTwoPhaseSSModel();
//        if(threePhaseSS)
//            runThreePhaseSSModel();
//        if(drainageUSS)
//            runUSSDrainageModel();
//        if(ganglionUSS)
//            runUSSGanglionModel();

    }
    if(networkSource==2 || networkSource==3)
    {
        if(twoPhaseSS)
            runTwoPhaseSSModelPT();
//        if(threePhaseSS)
//            runThreePhaseSSModelPT();
//        if(drainageUSS)
//            runUSSDrainageModelPT();
//        if(ganglionUSS)
//            runUSSGanglionModelPT();
    }
}

//void network::runDepletionSimulation()
//{
//    tools::cleanResultsFolder();

//    loadDepletionData();
//    gen.seed(depletionSeed);

//    if(networkSource==1)
//        runDepletionModel();
//    if(networkSource==2 || networkSource==3)
//        runDepletionModelPT();
//}

//getters
pore *network::getPoreX(int i, int j, int k) const
{
    if(i<0 || i>Nx || j<0 || j>Ny-1 || k<0 || k>Nz-1)
        return 0;
    return tableOfPoresX[i][j][k];
}

pore *network::getPoreY(int i, int j, int k) const
{
    if(i<0 || i>Nx-1 || j<0 || j>Ny || k<0 || k>Nz-1)
        return 0;
    return tableOfPoresY[i][j][k];
}

pore *network::getPoreZ(int i, int j, int k) const
{
    if(i<0 || i>Nx-1 || j<0 || j>Ny-1 || k<0 || k>Nz)
        return 0;
    return tableOfPoresZ[i][j][k];
}

pore *network::getPoreXout(int i, int j, int k) const
{
    if(i<-1 || i>Nx-1 || j<0 || j>Ny-1 || k<0 || k>Nz-1)
        return 0;
    return tableOfPoresX[i+1][j][k];
}

pore *network::getPoreYout(int i, int j, int k) const
{
    if(i<0 || i>Nx-1 || j<-1 || j>Ny-1 || k<0 || k>Nz-1)
        return 0;
    return tableOfPoresY[i][j+1][k];
}

pore *network::getPoreZout(int i, int j, int k) const
{
    if(i<0 || i>Nx-1 || j<0 || j>Ny-1 || k<-1 || k>Nz-1)
        return 0;
    return tableOfPoresZ[i][j][k+1];
}

pore *network::getPore(int i) const
{
    if(i<0 || i>totalPores-1)
        return 0;
    return tableOfAllPores[i];
}

node *network::getNode(int i,int j, int k) const
{
    if(i<0 || i>Nx-1 || j<0 || j>Ny-1 || k<0 || k>Nz-1)
        return 0;
    return tableOfNodes[i][j][k];
}

node *network::getNode(int i) const
{
    if(i<0 || i>totalNodes-1)
        return 0;
    return tableOfAllNodes[i];
}

element *network::getElement(int i) const
{
if(i<0 || i>totalElements-1)
    return 0;
return tableOfElements[i];
}

int network::getTotalPores() const
{
    return totalPores;
}

int network::getTotalNodes() const
{
    return totalNodes;
}

int network::getTotalOpenedPores() const
{
    return totalOpenedPores;
}

cluster *network::getCluster(int i) const
{
    if(i<0 || i>totalClusters-1)
        return 0;
    return allClusters[i];
}

double network::getXEdgeLength() const
{
    return xEdgeLength;
}

double network::getYEdgeLength() const
{
    return yEdgeLength;
}

double network::getZEdgeLength() const
{
    return zEdgeLength;
}
double network::getMaxConcWater() const
{
    return maxConcWater;
}

void network::setMaxConcWater(double value)
{
    maxConcWater = value;
}

int network::getSimulationType() const
{
    return simulationType;
}

void network::setSimulationType(int value)
{
    simulationType = value;
}
bool network::getTwoPhaseSS() const
{
    return twoPhaseSS;
}

void network::setTwoPhaseSS(bool value)
{
    twoPhaseSS = value;
}



double network::getMaxConc() const
{
    return maxConc;
}

void network::setMaxConc(double value)
{
    maxConc = value;
}


bool network::getRecord() const
{
    return record;
}

void network::setRecord(bool value)
{
    record = value;
}
bool network::getVideoRecording() const
{
    return videoRecording;
}

void network::setVideoRecording(bool value)
{
    videoRecording = value;
}

int network::getNx() const
{
    return Nx;
}

void network::setNx(int value)
{
    Nx = value;
}
int network::getNy() const
{
    return Ny;
}

void network::setNy(int value)
{
    Ny = value;
}
int network::getNz() const
{
    return Nz;
}

void network::setNz(int value)
{
    Nz = value;
}
int network::getNetworkSource() const
{
    return networkSource;
}

void network::setNetworkSource(int value)
{
    networkSource = value;
}
std::string network::getExtractedNetworkFolderPath() const
{
    return extractedNetworkFolderPath;
}

void network::setExtractedNetworkFolderPath(const std::string &value)
{
    extractedNetworkFolderPath = value;
}
std::string network::getRockPrefix() const
{
    return rockPrefix;
}

void network::setRockPrefix(const std::string &value)
{
    rockPrefix = value;
}
int network::getRadiusDistribution() const
{
    return radiusDistribution;
}

void network::setRadiusDistribution(int value)
{
    radiusDistribution = value;
}
double network::getLength() const
{
    return length;
}

void network::setLength(double value)
{
    length = value;
}
double network::getDegreeOfDistortion() const
{
    return degreeOfDistortion;
}

void network::setDegreeOfDistortion(double value)
{
    degreeOfDistortion = value;
}
double network::getMinRadius() const
{
    return minRadius;
}

void network::setMinRadius(double value)
{
    minRadius = value;
}
double network::getMaxRadius() const
{
    return maxRadius;
}

void network::setMaxRadius(double value)
{
    maxRadius = value;
}
double network::getCoordinationNumber() const
{
    return coordinationNumber;
}

void network::setCoordinationNumber(double value)
{
    coordinationNumber = value;
}
int network::getWettingTypeFlag() const
{
    return wettingTypeFlag;
}

void network::setWettingTypeFlag(int value)
{
    wettingTypeFlag = value;
}

double network::getMinWaterWetTheta() const
{
    return minWaterWetTheta;
}

void network::setMinWaterWetTheta(double value)
{
    minWaterWetTheta = value;
}
double network::getMaxWaterWetTheta() const
{
    return maxWaterWetTheta;
}

void network::setMaxWaterWetTheta(double value)
{
    maxWaterWetTheta = value;
}
double network::getMinOilWetTheta() const
{
    return minOilWetTheta;
}

void network::setMinOilWetTheta(double value)
{
    minOilWetTheta = value;
}
double network::getMaxOilWetTheta() const
{
    return maxOilWetTheta;
}

void network::setMaxOilWetTheta(double value)
{
    maxOilWetTheta = value;
}
bool network::getWaterFilms() const
{
    return waterFilms;
}

void network::setWaterFilms(bool value)
{
    waterFilms = value;
}
bool network::getOilFilms() const
{
    return oilFilms;
}

void network::setOilFilms(bool value)
{
    oilFilms = value;
}
double network::getOilWetFraction() const
{
    return oilWetFraction;
}

void network::setOilWetFraction(double value)
{
    oilWetFraction = value;
}
bool network::getSimulationRunning() const
{
    return simulationRunning;
}

void network::setSimulationRunning(bool value)
{
    simulationRunning = value;
}
double network::getAbsolutePermeability() const
{
    return absolutePermeability;
}

void network::setAbsolutePermeability(double value)
{
    absolutePermeability = value;
}
double network::getPorosity() const
{
    return porosity;
}

void network::setPorosity(double value)
{
    porosity = value;
}
int network::getTotalOpenedNodes() const
{
    return totalOpenedNodes;
}

void network::setTotalOpenedNodes(int value)
{
    totalOpenedNodes = value;
}
int network::getTwoPhaseSystem() const
{
    return twoPhaseSystem;
}

void network::setTwoPhaseSystem(int value)
{
    twoPhaseSystem = value;
}
bool network::getPrimaryDrainageSimulation() const
{
    return primaryDrainageSimulation;
}

void network::setPrimaryDrainageSimulation(bool value)
{
    primaryDrainageSimulation = value;
}
bool network::getPrimaryImbibitionSimulation() const
{
    return primaryImbibitionSimulation;
}

void network::setPrimaryImbibitionSimulation(bool value)
{
    primaryImbibitionSimulation = value;
}
bool network::getSecondaryDrainageSimulation() const
{
    return secondaryDrainageSimulation;
}

void network::setSecondaryDrainageSimulation(bool value)
{
    secondaryDrainageSimulation = value;
}
bool network::getSecondaryImbibitionSimulation() const
{
    return secondaryImbibitionSimulation;
}

void network::setSecondaryImbibitionSimulation(bool value)
{
    secondaryImbibitionSimulation = value;
}
bool network::getTertiaryDrainageSimulation() const
{
    return tertiaryDrainageSimulation;
}

void network::setTertiaryDrainageSimulation(bool value)
{
    tertiaryDrainageSimulation = value;
}
double network::getSwiAfterPD() const
{
    return swiAfterPD;
}

void network::setSwiAfterPD(double value)
{
    swiAfterPD = value;
}
bool network::getRelativePermeabilitiesCalculation() const
{
    return relativePermeabilitiesCalculation;
}

void network::setRelativePermeabilitiesCalculation(bool value)
{
    relativePermeabilitiesCalculation = value;
}

bool network::getMercuryInjectionSimulation() const
{
    return mercuryInjectionSimulation;
}

void network::setMercuryInjectionSimulation(bool value)
{
    mercuryInjectionSimulation = value;
}
int network::getTwoPhaseSimulationSteps() const
{
    return twoPhaseSimulationSteps;
}

void network::setTwoPhaseSimulationSteps(int value)
{
    twoPhaseSimulationSteps = value;
}
double network::getCriticalSaturationPD() const
{
    return criticalSaturationPD;
}

void network::setCriticalSaturationPD(double value)
{
    criticalSaturationPD = value;
}
double network::getCriticalPcPD() const
{
    return criticalPcPD;
}

void network::setCriticalPcPD(double value)
{
    criticalPcPD = value;
}
double network::getFinalSaturationPD() const
{
    return finalSaturationPD;
}

void network::setFinalSaturationPD(double value)
{
    finalSaturationPD = value;
}
double network::getFinalPcPD() const
{
    return finalPcPD;
}

void network::setFinalPcPD(double value)
{
    finalPcPD = value;
}
double network::getSwiAfterPI() const
{
    return swiAfterPI;
}

void network::setSwiAfterPI(double value)
{
    swiAfterPI = value;
}
double network::getSwiAfterSD() const
{
    return swiAfterSD;
}

void network::setSwiAfterSD(double value)
{
    swiAfterSD = value;
}
double network::getSwiAfterSI() const
{
    return swiAfterSI;
}

void network::setSwiAfterSI(double value)
{
    swiAfterSI = value;
}
double network::getSwiAfterTD() const
{
    return swiAfterTD;
}

void network::setSwiAfterTD(double value)
{
    swiAfterTD = value;
}


//Fluids getters and setters


void network::setOGSurfaceTension(double value)
{
    OGSurfaceTension = value;
}

void network::setWGSurfaceTension(double value)
{
    WGSurfaceTension = value;
}

void network::setOWSurfaceTension(double value)
{
    OWSurfaceTension = value;
}

void network::setGasViscosity(double value)
{
    gasViscosity = value;
}

void network::setOilViscosity(double value)
{
    oilViscosity = value;
}

void network::setWaterViscosity(double value)
{
    waterViscosity = value;
}

void network::setGasDensity(double value)
{
    gasDensity = value;
}

void network::setOilDensity(double value)
{
    oilDensity = value;
}
void network::setWaterDensity(double value)
{
    waterDensity = value;
}

void network::setOilDiffusionCoef(double value)
{
    oilDiffusionCoef = value;
}

void network::setWaterDiffusionCoef(double value)
{
    waterDiffusionCoef = value;
}

void network::setOilVolumeFactor(double value)
{
    oilVolumeFactor = value;
}

void network::setWaterVolumeFactor(double value)
{
    waterVolumeFactor = value;
}

void network::setGasOilConcentration(double value)
{
    gasOilConcentration = value;
}

void network::setGasWaterConcentration(double value)
{
    gasWaterConcentration = value;
}
bool network::getGravity() const
{
    return gravity;
}

void network::setGravity(bool value)
{
    gravity = value;
}
bool network::getViscousForces() const
{
    return viscousForces;
}

void network::setViscousForces(bool value)
{
    viscousForces = value;
}

double network::getInitialWaterSaturation() const
{
    return initialWaterSaturation;
}

void network::setInitialWaterSaturation(double value)
{
    initialWaterSaturation = value;
}

int network::getSeed() const
{
    return seed;
}

void network::setSeed(int value)
{
    seed = value;
}
double network::getWaterWetThetaIncrementAfterAging() const
{
    return waterWetThetaIncrementAfterAging;
}

void network::setWaterWetThetaIncrementAfterAging(double value)
{
    waterWetThetaIncrementAfterAging = value;
}
double network::getOilWetThetaIncrementAfterAging() const
{
    return oilWetThetaIncrementAfterAging;
}

void network::setOilWetThetaIncrementAfterAging(double value)
{
    oilWetThetaIncrementAfterAging = value;
}
bool network::getSnapOffOnly() const
{
    return snapOffOnly;
}

void network::setSnapOffOnly(bool value)
{
    snapOffOnly = value;
}
bool network::getExtension() const
{
    return extension;
}

void network::setExtension(bool value)
{
    extension = value;
}
int network::getExtensionNumber() const
{
    return extensionNumber;
}

void network::setExtensionNumber(int value)
{
    extensionNumber = value;
}

bool network::getAbsolutePermeabilityCalculation() const
{
    return absolutePermeabilityCalculation;
}

void network::setAbsolutePermeabilityCalculation(bool value)
{
    absolutePermeabilityCalculation = value;
}
int network::getSolverChoice() const
{
    return solverChoice;
}

void network::setSolverChoice(int value)
{
    solverChoice = value;
}

double network::getTotalPoresVolume() const
{
    return totalPoresVolume;
}

void network::setTotalPoresVolume(double value)
{
    totalPoresVolume = value;
}
double network::getFlow() const
{
    return flow;
}

void network::setFlow(double value)
{
    flow = value;
}

double network::getMinNodeRadius() const
{
    return minNodeRadius;
}

void network::setMinNodeRadius(double value)
{
    minNodeRadius = value;
}
double network::getMaxNodeRadius() const
{
    return maxNodeRadius;
}

void network::setMaxNodeRadius(double value)
{
    maxNodeRadius = value;
}
double network::getAspectRatio() const
{
    return aspectRatio;
}

void network::setAspectRatio(double value)
{
    aspectRatio = value;
}
bool network::getUnsteadyStateRegime() const
{
    return ganglionUSS;
}

void network::setUnsteadyStateRegime(bool value)
{
    ganglionUSS = value;
}

bool network::getUnsteadyStateRegimeFast() const
{
    return drainageUSS;
}

void network::setUnsteadyStateRegimeFast(bool value)
{
    drainageUSS = value;
}

double network::getGOR() const
{
    return GOR;
}

void network::setGOR(double value)
{
    GOR = value;
}
double network::getGWR() const
{
    return GWR;
}

void network::setGWR(double value)
{
    GWR = value;
}

bool network::getReady() const
{
    return ready;
}

void network::setReady(bool value)
{
    ready = value;
}

bool network::getCancel() const
{
    return cancel;
}

void network::setCancel(bool value)
{
    cancel = value;
}
