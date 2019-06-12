/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "secondaryOilDrainage.h"
#include "operations/pnmSolver.h"
#include "operations/hkClustering.h"
#include "operations/pnmOperation.h"
#include "network/iterator.h"
#include "network/cluster.h"
#include "misc/userInput.h"
#include "misc/tools.h"
#include "misc/maths.h"

#include <unordered_set>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace PNM
{

void secondaryOilDrainage::run()
{
    initialiseOutputFiles();
    initialiseCapillaries();
    initialiseSimulationAttributes();

    while (step < userInput::get().twoPhaseSimulationSteps)
    {
        invadeCapillariesAtCurrentPc();
        dismissTrappedElements();
        adjustCapillaryVolumes();

        updateOutputFiles();
        updateVariables();
        updateGUI();

        if (simulationInterrupted)
            break;
    }
    finalise();
}

std::string secondaryOilDrainage::getNotification()
{
    std::ostringstream ss;
    ss << "Secondary Oil Drainage Simulation \n"
       << std::fixed << std::setprecision(2)
       << "Current PC (psi): " << maths::PaToPsi(currentPc) << " / Sw: " << currentSw;
    return ss.str();
}

int secondaryOilDrainage::getProgress()
{
    return step / userInput::get().twoPhaseSimulationSteps * 100;
}

void secondaryOilDrainage::initialiseOutputFiles()
{
    tools::initialiseFolder("Network_State/Secondary_Oil_Drainage");

    pcFilename = "Results/SS_Simulation/5-secondaryOilDrainagePcCurve.txt";
    relPermFilename = "Results/SS_Simulation/5-secondaryOilDrainageRelativePermeabilies.txt";

    std::ofstream file;

    file.open(pcFilename.c_str());
    file << "Sw\tPc\n";
    file.close();

    file.open(relPermFilename.c_str());
    file << "Sw\tKro\tKrw\n";
    file.close();
}

void secondaryOilDrainage::initialiseSimulationAttributes()
{
    step = 0;
    currentSw = 0;

    outputCounter = 0;
    frameCount = 0;

    double effectiveMinRadius = 2 * userInput::get().OWSurfaceTension / getMaxPc();
    double effectiveMaxRadius = 2 * userInput::get().OWSurfaceTension / getMinPc();
    radiusStep = (effectiveMaxRadius - effectiveMinRadius) / userInput::get().twoPhaseSimulationSteps;
    currentRadius = effectiveMaxRadius - radiusStep;
    currentPc = 2 * userInput::get().OWSurfaceTension / currentRadius;

    elementsToInvade.clear();
    for (element *e : pnmRange<element>(network))
        if (e->getPhaseFlag() == phase::water)
            elementsToInvade.insert(e);
}

void secondaryOilDrainage::initialiseCapillaries()
{
}

double secondaryOilDrainage::getMinPc()
{
    double minPc(1e20);
    for (element *e : pnmRange<element>(network))
    {
        double pc = std::abs(e->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * std::cos(e->getTheta()) / e->getRadius());
        if (pc < minPc)
            minPc = pc;
    }
    return minPc;
}

double secondaryOilDrainage::getMaxPc()
{
    double maxPc(-1e20);
    for (element *e : pnmRange<element>(network))
    {
        double pc = std::abs(e->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * std::cos(e->getTheta()) / e->getRadius());
        if (pc > maxPc)
            maxPc = pc;
    }
    return maxPc;
}

void secondaryOilDrainage::invadeCapillariesAtCurrentPc()
{
    bool stillMore = true;
    while (stillMore)
    {
        stillMore = false;

        hkClustering::get(network).clusterWaterConductorElements();
        hkClustering::get(network).clusterOilConductorElements();

        std::vector<element *> invadedElements;
        for (element *e : elementsToInvade)
        {
            if (isInvadable(e))
                invadedElements.push_back(e);
        }

        for (element *e : invadedElements)
        {
            fillWithOil(e);
            elementsToInvade.erase(e);
            stillMore = true;
        }

        updateOutputFiles();
        updateGUI();

        if (simulationInterrupted)
            break;
    }
}

void secondaryOilDrainage::dismissTrappedElements()
{
    std::vector<element *> trappedElements;
    for (element *e : elementsToInvade)
    {
        if (!e->getClusterWaterConductor()->getOutlet())
            trappedElements.push_back(e);
    }

    for (element *e : trappedElements)
        elementsToInvade.erase(e);
}

void secondaryOilDrainage::adjustCapillaryVolumes()
{
    double waterVolume(0);

    for (element *e : pnmRange<element>(network))
    {
        if (e->getPhaseFlag() == phase::oil && e->getWettabilityFlag() == wettability::oilWet)
            waterVolume += e->getWaterFilmVolume();

        if (e->getPhaseFlag() == phase::oil && e->getWettabilityFlag() == wettability::waterWet)
        {
            if (e->getWaterCornerActivated() && e->getClusterOilConductor()->getInlet() && e->getClusterWaterConductor()->getOutlet())
                adjustVolumetrics(e);
            waterVolume += e->getWaterFilmVolume();
        }

        if (e->getPhaseFlag() == phase::water && e->getWettabilityFlag() == wettability::oilWet)
            waterVolume += e->getEffectiveVolume() + e->getWaterFilmVolume();

        if (e->getPhaseFlag() == phase::water && e->getWettabilityFlag() == wettability::waterWet)
            waterVolume += e->getVolume();
    }

    currentSw = waterVolume / network->totalNetworkVolume;
}

bool secondaryOilDrainage::isInvadable(element *e)
{
    bool isInvadable = false;

    if (e->getType() == capillaryType::throat && (e->getInlet() || isConnectedToInletCluster(e)) && e->getClusterWaterConductor()->getOutlet() ||
        e->getType() == capillaryType::poreBody && isConnectedToInletCluster(e) && e->getClusterWaterConductor()->getOutlet())
    {
        double entryPressure = e->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * std::cos(e->getTheta()) / e->getRadius();
        if (currentPc + 1e-5 >= entryPressure)
            isInvadable = true;
    }

    return isInvadable;
}

bool secondaryOilDrainage::isConnectedToInletCluster(element *e)
{
    bool connectedToInletCluster = false;
    for (element *n : e->getNeighboors())
        if (n->getPhaseFlag() == phase::oil && n->getClusterOilConductor()->getInlet())
        {
            connectedToInletCluster = true;
            break;
        }
    return connectedToInletCluster;
}

void secondaryOilDrainage::fillWithOil(element *e)
{
    e->setPhaseFlag(phase::oil);
    e->setOilConductor(true);
    e->setOilFraction(1);
    e->setWaterFraction(0);
    e->setOilLayerActivated(false);
    e->setOilFilmVolume(0);
    e->setOilFilmConductivity(1e-200);

    if (!e->getWaterCornerActivated() && e->getWaterCanFlowViaFilm())
    {
        e->setWaterCornerActivated(true);
        e->setWaterConductor(true);
    }

    if (!e->getWaterCornerActivated())
        e->setWaterConductor(false);
}

void secondaryOilDrainage::adjustVolumetrics(element *e)
{
    double rSquared = std::pow(userInput::get().OWSurfaceTension / currentPc, 2);
    double filmVolume = std::min(rSquared * e->getFilmAreaCoefficient() * e->getLength(), e->getWaterFilmVolume());
    double filmConductance = std::min(rSquared * filmVolume / e->getLength() / (userInput::get().waterViscosity * e->getLength()), e->getWaterFilmConductivity());

    e->setWaterFilmVolume(filmVolume);
    e->setWaterFilmConductivity(filmConductance);
    e->setEffectiveVolume(e->getVolume() - e->getWaterFilmVolume());
}

void secondaryOilDrainage::updateVariables()
{
    step++;

    if (step != userInput::get().twoPhaseSimulationSteps)
    {
        currentRadius -= radiusStep;
        currentPc = 2 * userInput::get().OWSurfaceTension / currentRadius;
    }
}

void secondaryOilDrainage::updateOutputFiles()
{
    if (std::abs(outputCounter - currentSw) < 0.01)
        return;

    std::ofstream file;

    file.open(pcFilename, std::ofstream::app);
    file << currentSw << "\t" << currentPc << std::endl;
    file.close();

    if (userInput::get().relativePermeabilitiesCalculation)
    {
        auto relPerms = pnmSolver::get(network).calculateRelativePermeabilities();

        file.open(relPermFilename, std::ofstream::app);
        file << currentSw << "\t" << relPerms.first << "\t" << relPerms.second << std::endl;
        file.close();
    }

    generateNetworkStateFiles();

    outputCounter = currentSw;
}

void secondaryOilDrainage::generateNetworkStateFiles()
{
    if (!userInput::get().extractDataSS)
        return;

    pnmOperation::get(network).generateNetworkState(frameCount, "Secondary_Oil_Drainage");
    frameCount++;
}

} // namespace PNM
