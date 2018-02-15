/////////////////////////////////////////////////////////////////////////////
/// Name:        network.cpp
/// Purpose:     attributes and methods of the loaded network
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

namespace PNM{

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
    accessiblePores.clear();
    accessibleNodes.clear();
    accessibleElements.clear();

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

    totalPores=totalOpenedPores=0;
    totalNodes=totalOpenedNodes=0;
    totalElements=totalOpenedElements=0;

    simulationNotification="";

    record=false;
    ready=false;
    cancel=false;
    simulationRunning=false;

    twoPhaseSS=true;
    drainageUSS=false;
}

void network::setupModel()
{
    tools::createRequiredFolders();
    tools::cleanNetworkDescriptionFolder();

    if(ready)
    {
        ready=false;
        destroy();
    }

    reset();
    loadNetworkData();

    gen.seed(seed);

    if(networkSource==2)
        setupRegularModel();
    if(networkSource==3)
        setupExtractedModel();

    ready=true;
    emitPlotSignal();
}

void network::runFluidInjectionSimulation()
{
    tools::createRequiredFolders();
    tools::cleanResultsFolder();

    loadTwoPhaseData();

    if(twoPhaseSS)
        runTwoPhaseSSModelPT();
    if(drainageUSS)
        runUSSDrainageModelPT();
    if(tracerFlow)
        runTracerFlowPT();
}

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

bool network::getSimulationRunning() const
{
    return simulationRunning;
}

void network::setSimulationRunning(bool value)
{
    simulationRunning = value;
}
int network::getNz() const
{
    return Nz;
}

void network::setNz(int value)
{
    Nz = value;
}
std::string network::getSimulationNotification() const
{
    return simulationNotification;
}

void network::setSimulationNotification(const std::string &value)
{
    simulationNotification = value;
}

int network::getTotalOpenedNodes() const
{
    return totalOpenedNodes;
}

bool network::getRecord() const
{
    return record;
}

bool network::getVideoRecording() const
{
    return videoRecording;
}

int network::getNetworkSource() const
{
    return networkSource;
}

double network::getAbsolutePermeability() const
{
    return absolutePermeability;
}

double network::getPorosity() const
{
    return porosity;
}


bool network::getReady() const
{
    return ready;
}

void network::setCancel(bool value)
{
    cancel = value;
}

}
