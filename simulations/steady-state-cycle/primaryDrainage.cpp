/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "primaryDrainage.h"
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

namespace PNM
{

primaryDrainage::primaryDrainage(double _finalSwi)
{
    finalSwi = _finalSwi;
}

void primaryDrainage::run()
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
        checkTerminationCondition();
        updateGUI();

        if (simulationInterrupted)
            break;
    }
    finalise();
}

std::string primaryDrainage::getNotification()
{
    std::ostringstream ss;
    ss << "Primary Drainage Simulation \n"
       << std::fixed << std::setprecision(2)
       << "Current PC (psi): " << maths::PaToPsi(currentPc) << " / Sw: " << currentSw;
    return ss.str();
}

int primaryDrainage::getProgress()
{
    return step / userInput::get().twoPhaseSimulationSteps * 100;
}

void primaryDrainage::initialiseOutputFiles()
{
    tools::initialiseFolder("Results/SS_Simulation");

    tools::initialiseFolder("Network_State/Primary_Drainage");

    pcFilename = "Results/SS_Simulation/1-primaryDrainagePcCurve.txt";
    relPermFilename = "Results/SS_Simulation/1-primaryDrainageRelativePermeabilies.txt";

    std::ofstream file;

    file.open(pcFilename.c_str());
    file << "Sw\tPc\n";
    file.close();

    file.open(relPermFilename.c_str());
    file << "Sw\tKro\tKrw\n";
    file.close();
}

void primaryDrainage::initialiseSimulationAttributes()
{
    step = 0;
    currentSw = 1;

    outputCounter = 0;
    frameCount = 0;

    double effectiveMinRadius = 2 * userInput::get().OWSurfaceTension / getMaxPc();
    double effectiveMaxRadius = 2 * userInput::get().OWSurfaceTension / getMinPc();
    radiusStep = (effectiveMaxRadius - effectiveMinRadius) / userInput::get().twoPhaseSimulationSteps;
    currentRadius = effectiveMaxRadius - radiusStep;
    currentPc = 2 * userInput::get().OWSurfaceTension / currentRadius;

    elementsToInvade.clear();
    for (pore *e : pnmInlet(network))
        elementsToInvade.insert(e);
}

void primaryDrainage::initialiseCapillaries()
{
    pnmOperation::get(network).assignWWWettability();
    pnmOperation::get(network).fillWithWater();
    pnmOperation::get(network).assignHalfAngles();
    pnmOperation::get(network).assignFilmsStability();

    for (element *e : pnmRange<element>(network))
    {
        e->setConcentration(0);
        e->setOilFraction(e->getPhaseFlag() == phase::oil ? 1 : 0);
        e->setWaterFraction(e->getPhaseFlag() == phase::water ? 1 : 0);
        e->setOilConductor(e->getPhaseFlag() == phase::oil ? true : false);
        e->setWaterConductor(e->getPhaseFlag() == phase::water ? true : false);
        e->setWaterFilmVolume(0);
        e->setWaterFilmConductivity(0);
        e->setWaterCornerActivated(false);
        e->setOilFilmVolume(0);
        e->setOilFilmConductivity(0);
        e->setOilLayerActivated(false);
    }
}

double primaryDrainage::getMinPc()
{
    double minPc(1e20);
    for (element *e : pnmRange<element>(network))
    {
        double pc = e->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * std::cos(e->getTheta()) / e->getRadius();
        if (pc < minPc)
            minPc = pc;
    }
    return minPc;
}

double primaryDrainage::getMaxPc()
{
    double maxPc(-1e20);
    for (element *e : pnmRange<element>(network))
    {
        double pc = e->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * std::cos(e->getTheta()) / e->getRadius();
        if (pc > maxPc)
            maxPc = pc;
    }
    return maxPc;
}

void primaryDrainage::invadeCapillariesAtCurrentPc()
{
    bool stillMore = true;
    while (stillMore)
    {
        stillMore = false;
        hkClustering::get(network).clusterWaterConductorElements();

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
            addNeighboorsToElementsToInvade(e);

            stillMore = true;
        }

        updateOutputFiles();
        checkTerminationCondition();
        updateGUI();

        if (simulationInterrupted)
            break;
    }
}

void primaryDrainage::dismissTrappedElements()
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

void primaryDrainage::adjustCapillaryVolumes()
{
    double waterVolume(0);

    for (element *e : pnmRange<element>(network))
    {
        if (e->getPhaseFlag() == phase::oil)
        {
            if (e->getWaterCornerActivated() && e->getClusterWaterConductor()->getOutlet())
            {
                adjustVolumetrics(e);
            }
            waterVolume += e->getWaterFilmVolume();
        }

        if (e->getPhaseFlag() == phase::water)
            waterVolume += e->getVolume();
    }

    currentSw = waterVolume / network->totalNetworkVolume;
}

bool primaryDrainage::isInvadable(element *e)
{
    double entryPressure = e->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * std::cos(e->getTheta()) / e->getRadius();
    return currentPc + 1e-5 >= entryPressure && e->getClusterWaterConductor()->getOutlet();
}

void primaryDrainage::addNeighboorsToElementsToInvade(element *e)
{
    for (element *n : e->getNeighboors())
        if (n->getPhaseFlag() == phase::water && e->getClusterWaterConductor()->getOutlet())
            elementsToInvade.insert(n);
}

void primaryDrainage::fillWithOil(element *e)
{
    e->setPhaseFlag(phase::oil);
    e->setOilConductor(true);
    e->setOilFraction(1);
    e->setWaterFraction(0);
    if (e->getWaterCanFlowViaFilm())
    {
        e->setWaterCornerActivated(true);
        e->setWaterConductor(true);
    }
    else
        e->setWaterConductor(false);
}

void primaryDrainage::adjustVolumetrics(element *e)
{
    double rSquared = std::pow(userInput::get().OWSurfaceTension / currentPc, 2);
    double filmVolume = rSquared * e->getFilmAreaCoefficient() * e->getLength();
    double filmConductivity = rSquared * filmVolume / e->getLength() / (userInput::get().waterViscosity * e->getLength());

    e->setWaterFilmVolume(filmVolume);
    e->setWaterFilmConductivity(filmConductivity);
    e->setEffectiveVolume(e->getVolume() - e->getWaterFilmVolume());
}

void primaryDrainage::updateVariables()
{
    step++;

    if (step != userInput::get().twoPhaseSimulationSteps)
    {
        currentRadius -= radiusStep;
        currentPc = 2 * userInput::get().OWSurfaceTension / currentRadius;
    }
}

void primaryDrainage::checkTerminationCondition()
{
    if (currentSw < finalSwi)
        simulationInterrupted = true;
}

void primaryDrainage::updateOutputFiles()
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

void primaryDrainage::generateNetworkStateFiles()
{
    if (!userInput::get().extractDataSS)
        return;

    pnmOperation::get(network).generateNetworkState(frameCount, "Primary_Drainage");
    frameCount++;
}

void primaryDrainage::finalise()
{
    pnmOperation::get(network).restoreWettability();
    pnmOperation::get(network).assignFilmsStability();
}

} // namespace PNM
