/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "forcedWaterInjection.h"
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

void forcedWaterInjection::run()
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

std::string forcedWaterInjection::getNotification()
{
    std::ostringstream ss;
    ss << "Forced Water Injection Simulation \n"
       << std::fixed << std::setprecision(2)
       << "Current PC (psi): " << maths::PaToPsi(currentPc) << " / Sw: " << currentSw;
    return ss.str();
}

int forcedWaterInjection::getProgress()
{
    return step / userInput::get().twoPhaseSimulationSteps * 100;
}

void forcedWaterInjection::initialiseOutputFiles()
{
    tools::initialiseFolder("Network_State/Forced_Water_Injection");

    pcFilename = "Results/SS_Simulation/3-forcedWaterInjectionPcCurve.txt";
    relPermFilename = "Results/SS_Simulation/3-forcedWaterInjectionRelativePermeabilies.txt";

    std::ofstream file;

    file.open(pcFilename.c_str());
    file << "Sw\tPc\n";
    file.close();

    file.open(relPermFilename.c_str());
    file << "Sw\tKro\tKrw\n";
    file.close();
}

void forcedWaterInjection::initialiseSimulationAttributes()
{
    step = 0;
    currentSw = 0;

    outputCounter = 0;
    frameCount = 0;

    double effectiveMinRadius = 2 * userInput::get().OWSurfaceTension / getMaxPc();
    double effectiveMaxRadius = 2 * userInput::get().OWSurfaceTension / getMinPc();
    radiusStep = (effectiveMaxRadius - effectiveMinRadius) / userInput::get().twoPhaseSimulationSteps;
    currentRadius = effectiveMaxRadius - radiusStep;
    currentPc = -2 * userInput::get().OWSurfaceTension / currentRadius;

    elementsToInvade.clear();
    for (element *e : pnmRange<element>(network))
        if (e->getPhaseFlag() == phase::oil)
            elementsToInvade.insert(e);
}

void forcedWaterInjection::initialiseCapillaries()
{
}

double forcedWaterInjection::getMinPc()
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

double forcedWaterInjection::getMaxPc()
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

void forcedWaterInjection::invadeCapillariesAtCurrentPc()
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
            fillWithWater(e);
            elementsToInvade.erase(e);
            stillMore = true;
        }

        updateOutputFiles();
        updateGUI();

        if (simulationInterrupted)
            break;
    }
}

void forcedWaterInjection::dismissTrappedElements()
{
    std::vector<element *> trappedElements;
    for (element *e : elementsToInvade)
    {
        if (!e->getClusterOilConductor()->getOutlet())
            trappedElements.push_back(e);
    }

    for (element *e : trappedElements)
        elementsToInvade.erase(e);
}

void forcedWaterInjection::adjustCapillaryVolumes()
{
    double waterVolume(0);

    for (element *e : pnmRange<element>(network))
    {
        if (e->getPhaseFlag() == phase::oil && e->getWettabilityFlag() == wettability::waterWet)
            waterVolume += e->getWaterFilmVolume();

        if (e->getPhaseFlag() == phase::oil && e->getWettabilityFlag() == wettability::oilWet)
            waterVolume += e->getWaterFilmVolume();

        if (e->getPhaseFlag() == phase::water && e->getWettabilityFlag() == wettability::oilWet)
        {
            if (e->getOilLayerActivated() && e->getClusterWaterConductor()->getInlet() && e->getClusterOilConductor()->getOutlet())
                adjustVolumetrics(e);
            waterVolume += e->getEffectiveVolume() + e->getWaterFilmVolume();
        }

        if (e->getPhaseFlag() == phase::water && e->getWettabilityFlag() == wettability::waterWet)
            waterVolume += e->getVolume();
    }

    currentSw = waterVolume / network->totalNetworkVolume;
}

bool forcedWaterInjection::isInvadable(element *e)
{
    bool isInvadable = false;

    if (e->getType() == capillaryType::throat && (e->getInlet() || isConnectedToInletCluster(e)) && e->getClusterOilConductor()->getOutlet() ||
        e->getType() == capillaryType::poreBody && isConnectedToInletCluster(e) && e->getClusterOilConductor()->getOutlet())
    {
        double entryPressure = e->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * std::cos(e->getTheta()) / e->getRadius();
        if (currentPc - 1e-5 <= entryPressure)
            isInvadable = true;
    }

    return isInvadable;
}

bool forcedWaterInjection::isConnectedToInletCluster(element *e)
{
    bool connectedToInletCluster = false;
    for (element *n : e->getNeighboors())
        if (n->getPhaseFlag() == phase::water && n->getClusterWaterConductor()->getInlet())
        {
            connectedToInletCluster = true;
            break;
        }
    return connectedToInletCluster;
}

void forcedWaterInjection::fillWithWater(element *e)
{
    e->setPhaseFlag(phase::water);
    e->setWaterConductor(true);
    e->setOilFraction(0);
    e->setWaterFraction(1);

    if (e->getOilCanFlowViaFilm())
    {
        e->setOilLayerActivated(true);
        e->setOilConductor(true);
    }
    else
    {
        e->setOilConductor(false);
        e->setWaterCornerActivated(false);
        e->setWaterFilmVolume(0);
        e->setWaterFilmConductivity(1e-200);
    }
}

void forcedWaterInjection::adjustVolumetrics(element *e)
{
    double rSquared = std::pow(userInput::get().OWSurfaceTension / currentPc, 2);
    double filmVolume = std::min(rSquared * e->getFilmAreaCoefficient() * e->getLength(), (1 - 4 * maths::pi() * e->getShapeFactor()) * e->getVolume());
    double effectiveOilFilmVolume = std::max(0.0, filmVolume - e->getWaterFilmVolume());
    double filmConductance = rSquared * effectiveOilFilmVolume / e->getLength() / (userInput::get().oilViscosity * e->getLength());

    e->setOilFilmVolume(effectiveOilFilmVolume);
    e->setOilFilmConductivity(filmConductance);
    e->setEffectiveVolume(e->getVolume() - e->getOilFilmVolume() - e->getWaterFilmVolume());
}

void forcedWaterInjection::updateVariables()
{
    step++;

    if (step != userInput::get().twoPhaseSimulationSteps)
    {
        currentRadius -= radiusStep;
        currentPc = -2 * userInput::get().OWSurfaceTension / currentRadius;
    }
}

void forcedWaterInjection::updateOutputFiles()
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

void forcedWaterInjection::generateNetworkStateFiles()
{
    if (!userInput::get().extractDataSS)
        return;

    pnmOperation::get(network).generateNetworkState(frameCount, "Forced_Water_Injection");
    frameCount++;
}

} // namespace PNM
