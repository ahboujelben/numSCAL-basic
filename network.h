/////////////////////////////////////////////////////////////////////////////
/// Name:        network.h
/// Purpose:     attributes and methods of the loaded network
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
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

//Eigen library
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseCholesky>

//Boost library
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

namespace PNM {

class network : public QObject
{
    Q_OBJECT
public:
    explicit network(QObject *parent = 0);
    ~network();
    void destroy();
    void reset();
    void setupModel();


    ///////////// Methods for generating regular networks models
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
    void assignHalfAngles();
    void assignVolumes();
    void assignConductivities();
    void assignWettability();
    void assignElements();
    void setNeighs();
    void assignViscosities();


    ///////////// Methods for generating network models extracted from microCT images
    void setupExtractedModel();
    void loadExtractedNetwork();
    void setNeighboorsForExtractedModel();
    void cleanExtractedNetwork();
    //Methods for extending the size of microCT models
    void extendCustomModel(char);
    void stitchCustomModel(char);
    void setBoundaries();


    ///////////// Methods for solvers and permeability calculations
    void solvePressures();
    void solvePressuresForRegularModel();
    void solvePressuresWithCapillaryPressures();
    double updateFlows();
    double updateFlowsWithCapillaryPressure();
    void calculatePermeabilityAndPorosity();
    void calculateRelativePermeabilities();
    void calculateRelativePermeabilitiesUSS();


    ///////////// Methods for Quasi-steady state 2-Phase flow
    void runFluidInjectionSimulation();
    void runTwoPhaseSSModelPT();
    void initializeTwoPhaseSimulationPT();
    void primaryDrainagePT(double finalSaturation=0);
    void spontaneousImbibitionPT();
    void forcedWaterInjectionPT();
    void spontaneousOilInvasionPT();
    void secondaryOilDrainagePT();
    void backupWettabilityPT();
    void assignWWWettabilityPT(double theta=0);
    void restoreWettabilityPT();
    //Films 2 phases
    void assignFilmStability();


    ///////////// Methods for Unsteady-steady state 2-Phase flow
    void runUSSDrainageModelPT();
    void setInitialFlagsPT();
    void setAdvancedTrappingPT();
    void updateCapillaryPropertiesPT(set<pore*>&, set<node*>&);
    void solvePressureWithoutCounterImbibitionPT();
    void calculateTimeStepUSSPT(set<pore *> &, set<node *> &, bool);
    double updateElementaryFluidFractionsPT(set<pore*>&, set<node*>&, bool &);
    void updateElementaryFluidFlagsPT(set<pore*>&, set<node *> &nodesToCheck);
    void setConstantFlowRateAker();
    //Output data
    void initializeTwoPhaseOutputs();
    void outputTwoPhaseData(double, int &, double);

    ///////////// Methods for Unsteady-steady state 2-Phase flow
    void runTracerFlowPT();

    ///////////// Misc

    //Assigning viscosities
    void assignViscositiesWithMixedFluids();

    //Filling Network with fluids
    void fillWithPhasePT(char phase, double saturation=1,int distribution=1, char otherPhase='o');

    //Setting initial attributes
    void initialiseCapillaries();

    //Tools
    double getOutletFlow();
    double getWaterSaturation();
    double getWaterSaturationWithFilmsPT();

    //Video Recording
    void extractVideo();

    //Random generators
    int uniform_int(int a=0, int b=1);
    double uniform_real(double a=0, double b=1);
    double rayleigh(double, double, double);
    double triangular(double, double, double);
    double normal(double,double,double,double);
    double weibull(double,double,double,double);

    ///////////// Methods for clustering
    int hkFind(int, vector<int>&);
    int hkUnion(vector<int>&,vector<int>&);
    int hkMakeSet(vector<int>&);

    template<typename T>
    void clusterPores(cluster*(pore::*)(void) const, void(pore::*)(cluster*), T(pore::*)() const, T, vector<cluster *> &);
    void clusterOilPores();
    void clusterWaterPores();

    template<typename T>
    void clusterElements(cluster*(element::*)(void) const,void(element::*)(cluster*),T(element::*)(void) const,T,vector<cluster *> &);

    void clusterWaterWetElements();
    void clusterOilWetElements();
    void clusterWaterElements();
    void clusterOilElements();
    void clusterGasElements();
    void clusterOilFlowingElements();
    void clusterWaterFlowingElements();
    void clusterActiveElements();


    ///////////// Methods for loading data
    void loadNetworkData();
    void loadTwoPhaseData();


    ///////////// Access to pores/nodes/elements
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
    int getTotalOpenedNodes() const;


    ///////////// Getters/Setters
    //Getters for network attributes
    double getXEdgeLength() const;
    double getYEdgeLength() const;
    double getZEdgeLength() const;


    ////////////// ThreadManagement
    bool getReady() const;
    void setCancel(bool value);
    int getNetworkSource() const;
    bool getRecord() const;
    bool getVideoRecording() const;
    double getAbsolutePermeability() const;
    double getPorosity() const;
    bool getSimulationRunning() const;
    void setSimulationRunning(bool value);
    int getNz() const;
    void setNz(int value);


    ////////////// Display runtime notifications
    string getSimulationNotification() const;
    void setSimulationNotification(const string &value);


    ///////////// Emitting signals
    void emitPlotSignal();

signals:
    void plot();

private:
    ////////////// Network Attributes

    //Common attributes
    int networkSource;
    int Nx;
    int Ny;
    int Nz;
    vector<vector<vector<node*> > > tableOfNodes;
    vector<vector<vector<pore*> > > tableOfPoresX;
    vector<vector<vector<pore*> > > tableOfPoresY;
    vector<vector<vector<pore*> > > tableOfPoresZ;
    vector<pore*> tableOfAllPores;
    vector<node*> tableOfAllNodes;
    vector<element*> tableOfElements;
    vector<pore*> accessiblePores;
    vector<node*> accessibleNodes;
    vector<element*> accessibleElements;
    std::vector<pore*> inletPores;
    std::vector<pore*> outletPores;

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
    int seed;


    //Attributes for networks extracted from microCT images
    double xEdgeLength;
    double yEdgeLength;
    double zEdgeLength;
    int maxConnectionNumber;
    string extractedNetworkFolderPath;
    string rockPrefix;


    //Attributes for pressure solving
    double pressureIn;
    double pressureOut;
    double flow;
    double absolutePermeability;
    double porosity;
    bool absolutePermeabilityCalculation;
    int solverChoice;


    ////////////// Wettabilty Attributes
    int wettingTypeFlag;
    double maxWettingPhaseRadius;
    double minNWettingPhaseRadius;
    double minWaterWetTheta;
    double maxWaterWetTheta;
    double minOilWetTheta;
    double maxOilWetTheta;
    double oilWetFraction;
    map<element*,double> wettabiltyThetaBackup;


    ////////////// Fluid injection Simulations
    bool twoPhaseSS;
    bool drainageUSS;
    bool tracerFlow;

    ///////////// Steady 2 Phase Flow
    bool primaryDrainageSimulation;
    bool spontaneousImbibitionSimulation;
    bool forcedWaterInjectionSimulation;
    bool spontaneousOilInvasionSimulation;
    bool secondaryOilDrainageSimulation;
    int twoPhaseSimulationSteps;
    double criticalSaturationPD;
    double criticalPcPD;
    double finalSaturationPD;
    double finalPcPD;
    double finalSaturationPI;
    double finalPcPI;
    double finalSaturationSD;
    double finalPcSD;
    double finalSaturationSI;
    double finalPcSI;
    double finalSaturationTD;
    double finalPcTD;
    double filmConductanceResistivity;

    ///////////// Unsteady 2 Phase Flow
    double initialWaterSaturation;
    int waterDistribution;
    double flowRate;
    double deltaP;
    double timeStep;
    double simulationTime;
    bool overrideByInjectedPVs;
    double injectedPVs;
    bool enhancedWaterConnectivity;
    double tracerDiffusionCoef;


    ////////////// Misc Attributes
    bool relativePermeabilitiesCalculation;
    double oilRelativePermeability;
    double waterRelativePermeability;
    bool videoRecording;
    bool extractData;
    double extractionTimestep;
    int outputCount;
    bool record;
    string simulationNotification;


    ////////////// fluids properties
    double OGSurfaceTension;
    double WGSurfaceTension;
    double OWSurfaceTension;
    double gasViscosity;
    double oilViscosity;
    double waterViscosity;
    double gasDensity;
    double oilDensity;
    double waterDensity;
    double oilDiffusionCoef;
    double waterDiffusionCoef;
    double oilVolumeFactor;
    double waterVolumeFactor;
    double gasOilSolubility;
    double gasWaterSolubility;


    ////////////// Clustering Attributes
    vector<cluster*> waterClusters;
    vector<cluster*> oilClusters;
    vector<cluster*> waterWetClusters;
    vector<cluster*> oilWetClusters;
    vector<cluster*> oilFilmClusters;
    vector<cluster*> waterFilmClusters;
    vector<cluster*> oilLayerClusters;
    vector<cluster*> waterLayerClusters;
    vector<cluster*> activeClusters;
    bool isOilSpanning;
    bool isWaterSpanning;
    bool isGasSpanning;
    bool isNetworkSpanning;
    int totalClusters;


    ////////// Thread Management
    bool cancel;
    bool ready;
    bool simulationRunning;


    ////////// Random generator
    boost::random::mt19937 gen;
};

}

#endif // NETWORK_H
