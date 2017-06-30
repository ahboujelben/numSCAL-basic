/////////////////////////////////////////////////////////////////////////////
/// Name:        network.h
/// Purpose:     attributes and methods of the loaded network
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_H
#define NETWORK_H

#include "node.h"
#include "pore.h"
#include "cluster.h"
#include "tools.h"

#include <vector>
#include <queue>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <fstream>
#include <unistd.h>
#include <math.h>
#include <chrono>
#include <thread>

//Eigen library
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>

//boost library
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/heap/fibonacci_heap.hpp>

#include <QObject>
#include <QMessageBox>
class network : public QObject
{
    Q_OBJECT
public:
    explicit network(QObject *parent = 0);
    ~network();
    void destroy();
    void reset();
    void setupModel();

    ////Regular Model
    void setupRegularModel();
    void createNodes();
    void createPores();
    void setNeighboors(); 
    void applyCoordinationNumber();
    void assignRadii();
    void assignLengths();
    void distortNetwork();
    void assignShapeFactors();
    void assignShapeFactorConstants();
    void assignMaxFilmFraction();
    void assignVolumes();
    void assignConductivities();
    void assignPoresConductivities();
    void assignElementsConductivities();
    void assignWettability();
    void assignPoresWettability();
    void assignElementsWettability();
    void assignElements();
    void setNeighs();
    void assignViscosities();

    //// Extracted Model
    void setupExtractedModel();
    void loadExtractedNetwork();
    void loadMouseData();
    void loadArtificialNetwork();
    void setNeighboorsForExtractedModel();
    void cleanExtractedNetwork();
    //Model Extension
    void extendCustomModel(char);
    void stitchCustomModel(char);
    void setBoundaries();

    ////Solvers and Permeabilities
    void solvePressuresForRegularModel();
    void solvePressuresForExtractedModel();
    void solvePressuresWithCapillaryPressures();
    double updateFlows();
    double updateFlowsWithCapillaryPressure();
    void calculatePermeabilityAndPorosity();
    void calculateRelativePermeabilitiesOW();
    void calculateRelativePermeabilitiesOG();
    void calculateRelativePermeabilitiesWG();
    void calculate3PhaseRelativePermeabilities();
    void solvePressures();

    ////2-Phase Flow
    void runTwoPhaseFlowSimulation();
    //Regular
    void runTwoPhaseSSModel();
    void initializeTwoPhaseSimulation();
    void primaryDrainage(double finalSaturation=0);
    void primaryImbibition();
    void secondaryDrainage();
    void secondaryImbibition();
    void tertiaryDrainage();
    void backupWettability();
    void assignWWWettability();
    void restoreWettability();

    //Pore Throat
    void runTwoPhaseSSModelPT();
    void initializeTwoPhaseSimulationPT();
    void primaryDrainagePT(double finalSaturation=0);
    void primaryImbibitionPT();
    void secondaryDrainagePT();
    void secondaryImbibitionPT();
    void tertiaryDrainagePT();
    void backupWettabilityPT();
    void assignWWWettabilityPT();
    void restoreWettabilityPT();

    //Films
    void assign3PhaseTheta();
    void assignHalfAngles();
    void assignFilmStability();
    void assignFd();
    void updateFilmProperties(double, char);
    void updateFilmPropertiesPT(double, char);
    void updateFilmStabilityPT();
    void updateFilmsPT(double);

    //tools
    double getVolume(char);
    double getVolumePT(char);
    int getNumberOfPores(char);
    double getOutletFlow();
    double getInletFlow();
    double getWaterSaturation();
    double getOilSaturation();
    double getWaterSaturationPT();
    double getOilSaturationPT();

    //Trapping
    void setWaterTrapped(int type=0);
    void setOilTrapped(int type=0);
    void setGasTrapped(int type=0);
    void setWaterTrappedPT(int type=0);
    void setOilTrappedPT(int type=0);
    void setGasTrappedPT(int type=0);

    //mixing fluids
    void assignViscositiesWithMixedFluids();

    //filling with fluids
    void fillWithPhase(char phase, double saturation=1,int distribution=1, char otherPhase='o');
    void fillWithPhasePT(char phase, double saturation=1,int distribution=1, char otherPhase='o');

    //Video Recording
    void extractVideo();

    //randomness
    int uniform_int(int a=0, int b=1);
    double uniform_real(double a=0, double b=1);
    double rayleigh(double, double, double);
    double triangular(double, double, double);
    double normal(double,double,double,double);
    double weibull(double,double,double,double);

    ////clustering
    int hkFind(int, std::vector<int>&);
    int hkUnion(std::vector<int>&,std::vector<int>&);
    int hkMakeSet(std::vector<int>&);
    //Regular
    void clusterPores(cluster*(pore::*)(void) const,void(pore::*)(cluster*),char(pore::*)(void) const,char,std::vector<cluster *> &);
    void clusterWaterWetPores();
    void clusterOilWetPores();
    void clusterWaterPores();
    void clusterOilPores();
    void clusterGasPores();
    void clusterOilGasPores();
    void clusterWaterGasPores();
    void clusterOilFilmPores();
    void clusterWaterFilmPores();
    void clusterOilLayerPores();
    void clusterWaterLayerPores();
    void clusterEverything();
    void clusterEverythingEverything();

    //Pore Throat
    void clusterElements(cluster*(element::*)(void) const,void(element::*)(cluster*),char(element::*)(void) const,char,std::vector<cluster *> &);
    void clusterWaterWetElements();
    void clusterOilWetElements();
    void clusterWaterElements();
    void clusterOilElements();
    void clusterGasElements();
    void clusterOilGasElements();
    void clusterWaterGasElements();
    void clusterOilFilmElements();
    void clusterWaterFilmElements();
    void clusterOilLayerElements();
    void clusterWaterLayerElements();

    void clusterAllPores();
    void clusterAllElements();

    ////loading data
    void loadData();
    void loadNetworkData();
    void loadTwoPhaseData();
    void loadDepletionData();
    void loadPressureDependentData(std::string, double&, double&);
    void loadPressureDependentData();

    ////Access to pores/nodes/elements
    pore *getPoreX(int,int,int) const;
    pore *getPoreY(int,int,int) const;
    pore *getPoreZ(int,int,int) const;
    pore *getPoreXout(int,int,int) const;
    pore *getPoreYout(int,int,int) const;
    pore *getPoreZout(int,int,int) const;
    pore *getPore(int) const;
    node *getNode(int,int,int) const;
    node *getNode(int) const;
    element *getElement(int) const;
    int getTotalPores() const;
    int getTotalNodes() const;
    int getTotalOpenedPores() const;

    ////Access to clusters
    cluster *getCluster(int) const;

    ////Getters/Setters
    //Getters for network attributes
    double getSystemMass() const;
    double getXEdgeLength() const;
    double getYEdgeLength() const;
    double getZEdgeLength() const;
    bool isBasicNetwork() const;
    double minConc,maxConc,minConcWater,maxConcWater;

    //ThreadManagement
    bool getReady() const;
    void setReady(bool value);
    bool getCancel() const;
    void setCancel(bool value);

    bool getRecord() const;
    void setRecord(bool value);
    bool getVideoRecording() const;
    void setVideoRecording(bool value);
    bool getStartRecording() const;
    void setStartRecording(bool value);

    bool getFinished() const;
    void setFinished(bool value);

    int getNx() const;
    void setNx(int value);

    int getNy() const;
    void setNy(int value);

    int getNz() const;
    void setNz(int value);

    int getNetworkSource() const;
    void setNetworkSource(int value);

    std::string getExtractedNetworkFolderPath() const;
    void setExtractedNetworkFolderPath(const std::string &value);

    std::string getRockPrefix() const;
    void setRockPrefix(const std::string &value);

    int getRadiusDistribution() const;
    void setRadiusDistribution(int value);

    double getLength() const;
    void setLength(double value);

    double getDegreeOfDistortion() const;
    void setDegreeOfDistortion(double value);

    double getMinRadius() const;
    void setMinRadius(double value);

    double getMaxRadius() const;
    void setMaxRadius(double value);

    double getCoordinationNumber() const;
    void setCoordinationNumber(double value);

    int getWettingTypeFlag() const;
    void setWettingTypeFlag(int value);

    double getMinWaterWetTheta() const;
    void setMinWaterWetTheta(double value);

    double getMaxWaterWetTheta() const;
    void setMaxWaterWetTheta(double value);

    double getMinOilWetTheta() const;
    void setMinOilWetTheta(double value);

    double getMaxOilWetTheta() const;
    void setMaxOilWetTheta(double value);

    bool getWaterFilms() const;
    void setWaterFilms(bool value);

    bool getOilFilms() const;
    void setOilFilms(bool value);

    double getOilWetFraction() const;
    void setOilWetFraction(double value);

    bool getSimulationRunning() const;
    void setSimulationRunning(bool value);

    double getAbsolutePermeability() const;
    void setAbsolutePermeability(double value);

    double getPorosity() const;
    void setPorosity(double value);

    int getTotalOpenedNodes() const;
    void setTotalOpenedNodes(int value);

    int getTwoPhaseSystem() const;
    void setTwoPhaseSystem(int value);

    bool getPrimaryDrainageSimulation() const;
    void setPrimaryDrainageSimulation(bool value);

    bool getPrimaryImbibitionSimulation() const;
    void setPrimaryImbibitionSimulation(bool value);

    bool getSecondaryDrainageSimulation() const;
    void setSecondaryDrainageSimulation(bool value);

    bool getSecondaryImbibitionSimulation() const;
    void setSecondaryImbibitionSimulation(bool value);

    bool getTertiaryDrainageSimulation() const;
    void setTertiaryDrainageSimulation(bool value);

    double getSwiAfterPD() const;
    void setSwiAfterPD(double value);

    bool getRelativePermeabilitiesCalculation() const;
    void setRelativePermeabilitiesCalculation(bool value);

    bool getUnsteadyState() const;
    void setUnsteadyState(bool value);

    double getUnsteadyStateFlowRate() const;
    void setUnsteadyStateFlowRate(double value);

    bool getMercuryInjectionSimulation() const;
    void setMercuryInjectionSimulation(bool value);

    int getTwoPhaseSimulationSteps() const;
    void setTwoPhaseSimulationSteps(int value);

    double getCriticalSaturationPD() const;
    void setCriticalSaturationPD(double value);

    double getCriticalPcPD() const;
    void setCriticalPcPD(double value);

    double getFinalSaturationPD() const;
    void setFinalSaturationPD(double value);

    double getFinalPcPD() const;
    void setFinalPcPD(double value);

    double getSwiAfterPI() const;
    void setSwiAfterPI(double value);

    double getSwiAfterSD() const;
    void setSwiAfterSD(double value);

    double getSwiAfterSI() const;
    void setSwiAfterSI(double value);

    double getSwiAfterTD() const;
    void setSwiAfterTD(double value);

    double getOGSurfaceTension(double p=0) const;
    void setOGSurfaceTension(double value);

    double getWGSurfaceTension(double p=0) const;
    void setWGSurfaceTension(double value);

    double getOWSurfaceTension(double p=0) const;
    void setOWSurfaceTension(double value);

    double getGasViscosity(double p=0) const;
    void setGasViscosity(double value);

    double getOilViscosity(double p=0) const;
    void setOilViscosity(double value);

    double getWaterViscosity(double p=0) const;
    void setWaterViscosity(double value);

    double getGasDensity(double p=0) const;
    void setGasDensity(double value);

    double getOilDensity(double p=0) const;
    void setOilDensity(double value);

    double getWaterDensity(double p=0) const;
    void setWaterDensity(double value);

    double getAverageDensity() const;
    void setAverageDensity(double value);

    double getOilDiffusionCoef(double p=0) const;
    void setOilDiffusionCoef(double value);

    double getWaterDiffusionCoef(double p=0) const;
    void setWaterDiffusionCoef(double value);

    double getOilVolumeFactor(double p=0) const;
    void setOilVolumeFactor(double value);

    double getWaterVolumeFactor(double p=0) const;
    void setWaterVolumeFactor(double value);

    double getGOR() const;
    void setGOR(double value);

    double getGWR() const;
    void setGWR(double value);

    double getGasOilConcentration(double p=0) const;
    void setGasOilConcentration(double value);

    double getGasWaterConcentration(double p=0) const;
    void setGasWaterConcentration(double value);

    bool getGravity() const;
    void setGravity(bool value);

    bool getViscousForces() const;
    void setViscousForces(bool value);

    double getBPPressure() const;
    void setBPPressure(double value);

    double getDepletionRate() const;
    void setDepletionRate(double value);

    double getDepletionTime() const;
    void setDepletionTime(double value);

    double getDepletionSteps() const;
    void setDepletionSteps(double value);

    double getInitialWaterSaturation() const;
    void setInitialWaterSaturation(double value);

    bool getProduction() const;
    void setProduction(bool value);

    double getBufferSize() const;
    void setBufferSize(double value);

    bool getInstantaneousNucleation() const;
    void setInstantaneousNucleation(bool value);

    bool getProgressiveNucleation() const;
    void setProgressiveNucleation(bool value);

    int getBubblesToNucleateIN() const;
    void setBubblesToNucleateIN(int value);

    double getMinCavityRadius() const;
    void setMinCavityRadius(double value);

    double getMaxCavityRadius() const;
    void setMaxCavityRadius(double value);

    int getCavityDensity() const;
    void setCavityDensity(int value);

    bool getWaterNucleation() const;
    void setWaterNucleation(bool value);

    double getPinViscous() const;
    void setPinViscous(double value);

    double getPoutViscous() const;
    void setPoutViscous(double value);

    bool getPressureDependentData() const;
    void setPressureDependentData(bool value);

    std::string getPressureDependentDataPath() const;
    void setPressureDependentDataPath(const std::string &value);

    bool getNucleationSimulation() const;
    void setNucleationSimulation(bool value);

    bool getDiffusionSimulation() const;
    void setDiffusionSimulation(bool value);

    bool getOptimalDepletionStep() const;
    void setOptimalDepletionStep(bool value);

    int getNucleationType() const;
    void setNucleationType(int value);

    bool getGrowthSimulation() const;
    void setGrowthSimulation(bool value);

    bool getMigrationSimulation() const;
    void setMigrationSimulation(bool value);

    double getCriticalGasSaturation() const;
    void setCriticalGasSaturation(double value);

    double getCriticalPressure() const;
    void setCriticalPressure(double value);

    double getCriticalSupersaturation() const;
    void setCriticalSupersaturation(double value);

    int getSeed() const;
    void setSeed(int value);

    double getWaterWetThetaIncrementAfterAging() const;
    void setWaterWetThetaIncrementAfterAging(double value);

    double getOilWetThetaIncrementAfterAging() const;
    void setOilWetThetaIncrementAfterAging(double value);

    bool getSnapOffOnly() const;
    void setSnapOffOnly(bool value);

    bool getExtension() const;
    void setExtension(bool value);

    int getExtensionNumber() const;
    void setExtensionNumber(int value);

    bool getAbsolutePermeabilityCalculation() const;
    void setAbsolutePermeabilityCalculation(bool value);

    int getSolverChoice() const;
    void setSolverChoice(int value);

    bool getUpdateGraphics() const;
    void setUpdateGraphics(bool value);

    double getTotalPoresVolume() const;
    void setTotalPoresVolume(double value);

    double getFlow() const;
    void setFlow(double value);

    bool getOverrideWithFixedData() const;
    void setOverrideWithFixedData(bool value);

    double getMinNodeRadius() const;
    void setMinNodeRadius(double value);

    double getMaxNodeRadius() const;
    void setMaxNodeRadius(double value);

    double getAspectRatio() const;
    void setAspectRatio(double value);

    bool getUnsteadyStateRegime() const;
    void setUnsteadyStateRegime(bool value);

    bool getUnsteadyStateRegimeFast() const;
    void setUnsteadyStateRegimeFast(bool value);

    double getMaxConc() const;
    void setMaxConc(double value);

    double getMaxConcWater() const;
    void setMaxConcWater(double value);

    bool getThreePhaseDepletion() const;
    void setThreePhaseDepletion(bool value);

    int getSimulationType() const;
    void setSimulationType(int value);

    bool getTwoPhaseSS() const;
    void setTwoPhaseSS(bool value);

signals:
    void plot();

private:
    ////////////// Network Attributes //////////////
    int networkSource;
    //Basic
    int Nx;
    int Ny;
    int Nz;
    std::vector<std::vector<std::vector<node*> > > tableOfNodes;
    std::vector<std::vector<std::vector<pore*> > > tableOfPoresX;
    std::vector<std::vector<std::vector<pore*> > > tableOfPoresY;
    std::vector<std::vector<std::vector<pore*> > > tableOfPoresZ;
    std::vector<pore*> tableOfAllPores;
    std::vector<node*> tableOfAllNodes;
    std::vector<element*> tableOfElements;


    int totalPores;
    int totalOpenedPores;
    int totalNodes;
    int totalOpenedNodes;
    int totalElements;
    int totalOpenedElements;
    double totalPoresVolume;
    double totalNodesVolume;
    double totalElementsVolume;
    double coordinationNumber;
    double minRadius;
    double maxRadius;
    double minNodeRadius;
    double maxNodeRadius;
    int radiusDistribution;
    double length;
    double degreeOfDistortion;
    double aspectRatio;
    double shapeFactor;

    double poreVolumeConstant;
    double poreVolumeExponent;
    double poreConductivityConstant;
    double poreConductivityExponent;

    double rayleighParameter;
    double triangularParameter;
    double normalMuParameter;
    double normalSigmaParameter;

    //Extracted Network
    double xEdgeLength;
    double yEdgeLength;
    double zEdgeLength;
    int maxConnectionNumber;
    std::string extractedNetworkFolderPath;
    std::string rockPrefix;
    bool extension;
    int extensionNumber;
    bool extensionOneDirection;

    double pressureIn;
    double pressureOut;
    double flow;
    double absolutePermeability;
    double absolutePermeabilityFracture;
    double porosity;

    //seed
    int seed;
    int twoPhaseSeed;
    int depletionSeed;

    //solver
    int solverChoice;

    //perm Calc
    bool absolutePermeabilityCalculation;

    ////////////// Wettabilty Attributes //////////////
    int wettingTypeFlag;
    double maxWettingPhaseRadius;
    double minNWettingPhaseRadius;
    double minWaterWetTheta;
    double maxWaterWetTheta;
    double minOilWetTheta;
    double maxOilWetTheta;
    double oilWetFraction;
    double waterWetThetaIncrementAfterAging;
    double oilWetThetaIncrementAfterAging;
    bool distributedThetaFlag;
    bool waterFilms;
    bool oilFilms;

    ////////////// Fluid injection Cycles //////////////
    bool twoPhaseSS;
    bool threePhaseSS;
    bool ganglionUSS;
    bool drainageUSS;

    ///////////// 2 Phase SS ////////////////////////
    bool primaryDrainageSimulation;
    bool primaryImbibitionSimulation;
    bool secondaryDrainageSimulation;
    bool secondaryImbibitionSimulation;
    bool tertiaryDrainageSimulation;
    bool mercuryInjectionSimulation;
    int twoPhaseSystem;
    int twoPhaseSimulationSteps;
    double swiAfterPD;
    double swiAfterPI;
    double swiAfterSD;
    double swiAfterSI;
    double swiAfterTD;
    double criticalSaturationPD;
    double criticalPcPD;
    double finalSaturationPD;
    double finalPcPD;
    double criticalSaturationPI;
    double criticalPcPI;
    double finalSaturationPI;
    double finalPcPI;
    double criticalSaturationSD;
    double criticalPcSD;
    double finalSaturationSD;
    double finalPcSD;
    double criticalSaturationSI;
    double criticalPcSI;
    double finalSaturationSI;
    double finalPcSI;
    double criticalSaturationTD;
    double criticalPcTD;
    double finalSaturationTD;
    double finalPcTD;
    bool snapOffOnly;
    std::map<element*,double>wettabiltyThetaBackup;

    double initialWaterSaturation;
    double claySaturation;
    int waterDistribution;
    bool waterWetSwi;

    ////////////// Misc Attributes //////////////
    bool gravity;
    double g;
    bool viscousForces;
    bool relativePermeabilitiesCalculation;
    double oilRelativePermeability;
    double waterRelativePermeability;
    double gasRelativePermeability;
    bool videoRecording;
    bool extractData;
    double extractionTimestep;
    int outputCount;
    bool record;

    ////////////// fluids properties //////////////
    double OGSurfaceTension;
    double WGSurfaceTension;
    double OWSurfaceTension;
    bool oilSpreading;
    bool waterSpreading;
    double gasViscosity;
    double oilViscosity;
    double waterViscosity;
    double gasDensity;
    double oilDensity;
    double waterDensity;
    double oilDiffusionCoef;
    double waterDiffusionCoef;
    double oilWaterDiffusionCoef;
    double averageDiffusionCoef;
    double gasOilConcentration;
    double gasWaterConcentration;
    double oilVolumeFactor,oilVolumeFactorOld;
    double waterVolumeFactor,waterVolumeFactorOld;
    double GOR,GORold,GORinit;
    double GWR,GWRold,GWRinit;

    double gasOilSolubility;
    double gasWaterSolubility;

    ////////////// Clustering Attributes //////////////

    std::vector<cluster*> waterClusters;
    std::vector<cluster*> oilClusters;
    std::vector<cluster*> gasClusters;
    std::vector<cluster*> waterWetClusters;
    std::vector<cluster*> oilWetClusters;
    std::vector<cluster*> oilGasClusters;
    std::vector<cluster*> waterGasClusters;
    std::vector<cluster*> oilFilmClusters;
    std::vector<cluster*> waterFilmClusters;
    std::vector<cluster*> oilLayerClusters;
    std::vector<cluster*> waterLayerClusters;
    std::vector<cluster*> allClusters;
    std::vector<cluster*> existClusters;
    bool isOilSpanning;
    bool isWaterSpanning;
    bool isGasSpanning;
    bool isNetworkSpanning;
    int totalClusters;

    ////////// Thread Management ///////////////
    int simulationType;
    bool cancel;
    bool ready;
    bool simulationRunning;

    ////////// Random generator ////////////////
    boost::random::mt19937 gen;
};

#endif // NETWORK_H
