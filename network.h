/////////////////////////////////////////////////////////////////////////////
/// Name:        network.h
/// Purpose:     attributes and methods of the loaded network
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <QObject>

namespace PNM {

class element;
class pore;
class node;
class cluster;
enum class phase;

template<typename T>
struct pointerHash;

class network : public QObject
{
    Q_OBJECT
public:
    explicit network(QObject *parent = 0);
    ~network();
    void destroy();
    void reset();
    void setupModel();
    void runSimulation();


    ///////////// Methods for generating regular networks models
    void setupRegularModel();
    void calculateRegularNetworkAttributes();
    void createNodes();
    void createPores();
    void setNeighboors(); 
    void setBoundaryConditions();
    void applyCoordinationNumber();
    void cleanNetwork();
    void calculateNetworkAttributes();
    void assignRadii();
    void assignLengths();
    void distortNetwork();
    void assignShapeFactors();
    void assignVolumes();
    void assignConductivities();
    void assignWettability();
    void assignViscosities();
    void displayNetworkInfo();


    ///////////// Methods for generating network models extracted from microCT images
    void setupExtractedModel();
    void loadExtractedNetwork();


    ///////////// Methods for solvers and permeability calculations
    void solvePressures();
    void solvePressuresWithCapillaryPressures();
    double updateFlows();
    double updateFlowsWithCapillaryPressure();
    void calculatePermeabilityAndPorosity();
    void calculateRelativePermeabilities();


    ///////////// Methods for Quasi-steady state 2-Phase flow
    void runTwoPhaseSSModel();
    void initialiseTwoPhaseSSModel();
    void primaryDrainage(double finalSaturation=0);
    void spontaneousImbibition();
    void forcedWaterInjection();
    void spontaneousOilInvasion();
    void secondaryOilDrainage();
    void backupWettability();
    void assignWWWettability(double theta=0);
    void restoreWettability();
    //Films 2 phases
    void assignHalfAngles();
    void assignFilmStability();


    ///////////// Methods for Unsteady-steady state 2-Phase flow
    void runUSSDrainageModel();
    void initialiseUSSDrainageModel();
    void addWaterChannel();
    void setInitialFlags();
    void setAdvancedTrapping();
    void updateCapillaryProperties(std::unordered_set<pore*, pointerHash<pore>>&, std::unordered_set<node*,pointerHash<node>>&);
    void updateConductivity(pore*);
    void solvePressureWithoutCounterImbibition();
    void calculateTimeStepUSS(std::unordered_set<pore*, pointerHash<pore>>&, std::unordered_set<node*,pointerHash<node>>&, bool);
    double updateElementaryFluidFractions(std::unordered_set<pore*, pointerHash<pore>>&, std::unordered_set<node*,pointerHash<node>>&, bool &);
    void updateElementaryFluidFlags(std::unordered_set<pore*, pointerHash<pore>>&, std::unordered_set<node*,pointerHash<node>>&);
    //Output data
    void initializeTwoPhaseOutputs();
    void outputTwoPhaseData(double, int &, double);


    ///////////// Methods for Unsteady-steady state 2-Phase flow
    void runTracerModel();
    void initialiseTracerModel();
    void solvePressureFieldInOil();
    void calculateTracerTimeStep();
    void updateConcentrationValues(std::unordered_map<element *, double, pointerHash<element> > &newConcentration);


    ///////////// Misc

    //Constants and conversion
    double pi(){return 3.14159265358979323846264;}
    double PsiToPa(double const &pressure){return pressure/14.50377*1e5;}
    double PaToPsi(double const &pressure){return pressure*14.50377/1e5;}

    //Setting initial attributes
    void initialiseCapillaries();

    //Filling Network with fluids
    void fillWithPhase(PNM::phase phase, double saturation=1, int distribution=1);

    //Getting flow properties
    double getOutletFlow();
    double getWaterSaturation();
    double getWaterSaturationWithFilms();

    //Video Recording
    void extractVideo();

    ///////////// Methods for clustering
    int hkFind(int, std::vector<int>&);
    int hkUnion(std::vector<int>&,std::vector<int>&);
    int hkMakeSet(std::vector<int>&);
    template<typename T>
    void clusterElements(cluster*(element::*)(void) const,void(element::*)(cluster*),T(element::*)(void) const,T,std::vector<cluster *> &);
    void clusterWaterWetElements();
    void clusterOilWetElements();
    void clusterWaterElements();
    void clusterOilElements();
    void clusterGasElements();
    void clusterOilFlowingElements();
    void clusterWaterFlowingElements();
    void clusterActiveElements();


    ///////////// Methods for loading input data
    void loadNetworkData();
    void loadSimulationData();


    ///////////// Access to pores/nodes
    pore *getPoreX(int,int,int) const;
    pore *getPoreY(int,int,int) const;
    pore *getPoreZ(int,int,int) const;
    pore *getPoreXout(int,int,int) const;
    pore *getPoreYout(int,int,int) const;
    pore *getPoreZout(int,int,int) const;
    pore *getPore(int) const;
    node *getNode(int,int,int) const;
    node *getNode(int) const;
    int getTotalPores() const;
    int getTotalNodes() const;
    int getTotalEnabledPores() const;
    int getTotalEnabledNodes() const;


    ///////////// Getters/Setters
    //Getters for network attributes
    double getXEdgeLength() const;
    double getYEdgeLength() const;
    double getZEdgeLength() const;


    ////////////// Thread Management
    bool isLoaded() const;
    void setCancel(bool value);
    int getNetworkSource() const;
    bool getRecord() const;
    bool getVideoRecording() const;
    double getAbsolutePermeability() const;
    double getPorosity() const;
    bool getSimulationRunning() const;
    void setSimulationRunning(bool value);
    int getNz() const;


    ////////////// Display runtime notifications
    std::string getSimulationNotification() const;
    void setSimulationNotification(const std::string &value);


    ///////////// Interacting with main window
    void emitPlotSignal();
    void emitNetworkLoadedSignal();
    void emitSimulationDoneSignal();
    void emitUpdateNotificationSignal();

signals:
    void plot();
    void networkLoaded();
    void simulationDone();
    void updateNotification();

private:
    ////////////// Network Attributes

    //Common attributes
    int networkSource;
    int Nx;
    int Ny;
    int Nz;
    std::vector<pore*> tableOfPores;
    std::vector<node*> tableOfNodes;
    std::vector<pore*> inletPores;
    std::vector<pore*> outletPores;

    int totalPores;
    int totalEnabledPores;
    int totalNodes;
    int totaEnabledNodes;
    double totalPoresVolume;
    double totalNodesVolume;
    double totalNetworkVolume;
    double inletPoresVolume;
    double coordinationNumber;
    double minRadius;
    double maxRadius;
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
    std::string extractedNetworkFolderPath;
    std::string rockPrefix;


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
    bool extractData;
    double extractionTimestep;
    int outputCount;
    bool videoRecording;
    bool record;
    std::string simulationNotification;


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
    std::vector<cluster*> waterClusters;
    std::vector<cluster*> oilClusters;
    std::vector<cluster*> waterWetClusters;
    std::vector<cluster*> oilWetClusters;
    std::vector<cluster*> oilFilmClusters;
    std::vector<cluster*> waterFilmClusters;
    std::vector<cluster*> oilLayerClusters;
    std::vector<cluster*> waterLayerClusters;
    std::vector<cluster*> activeClusters;
    bool isOilSpanning;
    bool isWaterSpanning;
    bool isGasSpanning;
    bool isNetworkSpanning;
    int totalClusters;


    ////////// Thread Management
    bool networkIsLoaded;
    bool simulationInterrupted;
    bool simulationRunning;
};

}

#endif // NETWORK_H
