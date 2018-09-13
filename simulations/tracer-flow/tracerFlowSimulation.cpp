/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "tracerFlowSimulation.h"
#include "operations/pnmSolver.h"
#include "operations/hkClustering.h"
#include "operations/pnmOperation.h"
#include "network/iterator.h"
#include "network/cluster.h"
#include "misc/userInput.h"
#include "misc/tools.h"

#include <unordered_map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

namespace PNM
{

void tracerFlowSimulation::run()
{
    initialiseOutputFiles();
    initialiseCapillaries();
    initialiseSimulationAttributes();

    fetchNonFlowingCapillaries();
    solvePressureField();
    calculateTimeStep();

    while (!simulationInterrupted && timeSoFar < simulationTime)
    {
        updateConcentrations();
        updateVariables();
        updateOutputFiles();
        updateGUI();

        if (simulationInterrupted)
            break;
    }
}

std::string tracerFlowSimulation::getNotification()
{
    std::ostringstream ss;
    ss << "Tracer Flow Simulation \n"
       << std::fixed << std::setprecision(2)
       << "Flow Velocity (m/day): " << flowVelocity << " / Injected PVs: " << injectedPVs;
    return ss.str();
}

int tracerFlowSimulation::getProgress()
{
    return timeSoFar / simulationTime * 100;
}

void tracerFlowSimulation::initialiseOutputFiles()
{
    tools::initialiseFolder("Results/Tracer_Simulation");
    tools::initialiseFolder("Network_State/Tracer_Simulation");
}

void tracerFlowSimulation::initialiseSimulationAttributes()
{
    simulationTime = userInput::get().overrideByInjectedPVs ? network->totalNetworkVolume * userInput::get().injectedPVs / userInput::get().flowRate
                                                            : userInput::get().simulationTime;

    timeSoFar = 0;
    injectedPVs = 0;

    outputCounter = 0;
    frameCount = 0;

    auto inletFlux = userInput::get().flowRate / network->inletPoresArea;
    flowVelocity = inletFlux * 86400;
}

void tracerFlowSimulation::initialiseCapillaries()
{
    pnmOperation::get(network).setSwi();
    setInitialAttributes();
}

void tracerFlowSimulation::setInitialAttributes()
{
    for (element *e : pnmRange<element>(network))
    {
        e->setConcentration(0);
        e->setCapillaryPressure(0);
        e->setOilFraction(e->getPhaseFlag() == phase::oil ? 1 : 0);
        e->setWaterFraction(e->getPhaseFlag() == phase::water ? 1 : 0);
    }
}

void tracerFlowSimulation::fetchNonFlowingCapillaries()
{
    hkClustering::get(network).clusterOilElements();

    for (element *e : pnmRange<element>(network))
    {
        e->setActive(true);

        if (e->getPhaseFlag() == phase::water)
            e->setActive(false);
        if (e->getPhaseFlag() == phase::oil && !e->getClusterOil()->getSpanning())
            e->setActive(false);

        if (e->getType() == capillaryType::throat)
        {
            pore *p = static_cast<pore *>(e);
            if ((p->getNodeIn() != 0 && p->getNodeIn()->getPhaseFlag() == phase::water) || (p->getNodeOut() != 0 && p->getNodeOut()->getPhaseFlag() == phase::water))
                e->setActive(false);
        }
    }
}

void tracerFlowSimulation::solvePressureField()
{
    pnmOperation::get(network).assignViscosities();
    pnmOperation::get(network).assignConductivities();
    pnmSolver::get(network).solvePressuresConstantFlowRate();
}

void tracerFlowSimulation::calculateTimeStep()
{
    hkClustering::get(network).clusterOilElements();

    timeStep = 1e50;

    for (pore *p : pnmRange<pore>(network))
    {
        if (p->getPhaseFlag() == phase::oil && p->getClusterOil()->getSpanning())
        {
            //Diffusion
            double sumDiffusionSource = 0;
            for (element *e : p->getNeighboors())
            {
                if (e->getPhaseFlag() == phase::oil)
                {
                    double area = std::min(e->getVolume() / e->getLength(), p->getVolume() / p->getLength());
                    sumDiffusionSource += userInput::get().tracerDiffusionCoef / area;
                }
            }

            //Convection
            if ((std::abs(p->getFlow()) / p->getVolume() + sumDiffusionSource) > 1e-30)
            {
                double step = 1. / (std::abs(p->getFlow()) / p->getVolume() + sumDiffusionSource);
                if (step < timeStep)
                {
                    timeStep = step;
                }
            }
        }
    }

    for (node *p : pnmRange<node>(network))
    {
        if (p->getPhaseFlag() == phase::oil && p->getClusterOil()->getSpanning())
        {
            //Diffusion
            double sumDiffusionSource = 0;
            for (element *e : p->getNeighboors())
            {
                if (e->getPhaseFlag() == phase::oil)
                {
                    double area = std::min(e->getVolume() / e->getLength(), p->getVolume() / p->getLength());
                    sumDiffusionSource += userInput::get().tracerDiffusionCoef / area;
                }
            }

            //Convection
            if ((std::abs(p->getFlow()) / p->getVolume() + sumDiffusionSource) > 1e-30)
            {
                double step = 1. / (std::abs(p->getFlow()) / p->getVolume() + sumDiffusionSource);
                if (step < timeStep)
                {
                    timeStep = step;
                }
            }
        }
    }
}

void tracerFlowSimulation::updateConcentrations()
{
    std::unordered_map<element *, double> newConcentration;

    for (node *n : pnmRange<node>(network))
    {
        if (n->getPhaseFlag() == phase::oil && n->getClusterOil()->getSpanning())
        {
            //Convection
            double massIn = 0;
            for (element *e : n->getNeighboors())
            {
                pore *p = static_cast<pore *>(e);
                if (p->getPhaseFlag() == phase::oil && p->getActive())
                {
                    if ((p->getNodeIn() == n && p->getFlow() > 1e-30) || (p->getNodeOut() == n && p->getFlow() < -1e-30))
                    {
                        massIn += p->getConcentration() * std::abs(p->getFlow());
                    }
                }
            }
            n->setMassFlow(massIn);

            //Diffusion
            double sumDiffusionIn = 0;
            double sumDiffusionOut = 0;
            for (element *e : n->getNeighboors())
            {
                if (e->getPhaseFlag() == phase::oil)
                {
                    double area = std::min(e->getVolume() / e->getLength(), n->getVolume() / n->getLength());
                    sumDiffusionIn += e->getConcentration() * userInput::get().tracerDiffusionCoef / area;
                    sumDiffusionOut += n->getConcentration() * userInput::get().tracerDiffusionCoef / area;
                }
            }

            //Load new concentration in a temporary vector
            newConcentration[n] = (n->getConcentration() + (massIn - std::abs(n->getFlow()) * n->getConcentration()) * timeStep / n->getVolume() + sumDiffusionIn * timeStep - sumDiffusionOut * timeStep);
        }
    }

    for (pore *p : pnmRange<pore>(network))
    {
        if (p->getPhaseFlag() == phase::oil && p->getClusterOil()->getSpanning())
        {
            double massIn = 0;
            double flowIn = 0;
            double sumDiffusionIn = 0;
            double sumDiffusionOut = 0;

            //Convection
            if (p->getInlet())
            {
                if (std::abs(p->getFlow()) > 1e-30 && p->getActive())
                {
                    massIn = std::abs(p->getFlow());
                    flowIn = std::abs(p->getFlow());
                }
            }
            else if (p->getOutlet())
            {
                if (std::abs(p->getFlow()) > 1e-30 && p->getActive())
                {
                    node *activeNode = p->getNodeIn() == 0 ? p->getNodeOut() : p->getNodeIn();
                    massIn = activeNode->getMassFlow();
                    flowIn = activeNode->getFlow();
                }
            }
            else
            {
                if (p->getFlow() > 1e-30 && p->getActive() && p->getNodeOut()->getPhaseFlag() == phase::oil)
                {
                    massIn = p->getNodeOut()->getMassFlow();
                    flowIn = p->getNodeOut()->getFlow();
                }
                if (p->getFlow() < 1e-30 && p->getActive() && p->getNodeIn()->getPhaseFlag() == phase::oil)
                {
                    massIn = p->getNodeIn()->getMassFlow();
                    flowIn = p->getNodeIn()->getFlow();
                }
            }

            if (std::abs(p->getFlow()) < 1e-30 || flowIn < 1e-30 || !p->getActive())
            {
                massIn = 0;
                flowIn = 1;
            }

            //Diffusion
            for (element *e : p->getNeighboors())
            {
                if (e->getPhaseFlag() == phase::oil)
                {
                    double area = std::min(e->getVolume() / e->getLength(), p->getVolume() / p->getLength());
                    sumDiffusionIn += e->getConcentration() * userInput::get().tracerDiffusionCoef / area;
                    sumDiffusionOut += p->getConcentration() * userInput::get().tracerDiffusionCoef / area;
                }
            }

            //Load new concentration in a temporary vector
            newConcentration[p] = (p->getConcentration() + (std::abs(p->getFlow()) / flowIn * massIn - std::abs(p->getFlow()) * p->getConcentration()) * timeStep / p->getVolume() + sumDiffusionIn * timeStep - sumDiffusionOut * timeStep);
        }
    }

    //Update concentrations
    for (element *e : pnmRange<element>(network))
    {
        if (e->getPhaseFlag() == phase::oil && e->getClusterOil()->getSpanning())
        {
            e->setConcentration(newConcentration[e]);
            //  std::cout << e->getConcentration() <<" "<<e->getFlow() << std::endl;
            if (e->getConcentration() < -0.00001 || e->getConcentration() > 1.0001)
            {
                simulationInterrupted = true;
                std::cout << "ERROR: Concentration out of range: " << e->getConcentration() << std::endl;
            }
        }
    }
}

void tracerFlowSimulation::updateVariables()
{
    timeSoFar += timeStep;
    injectedPVs += timeStep * userInput::get().flowRate / network->totalNetworkVolume;
}

void tracerFlowSimulation::updateOutputFiles()
{
    if (std::abs(outputCounter - injectedPVs) < 0.01)
        return;

    generateNetworkStateFiles();

    outputCounter = injectedPVs;
}

void tracerFlowSimulation::generateNetworkStateFiles()
{
    if (!userInput::get().extractDataUSS)
        return;

    pnmOperation::get(network).generateNetworkState(frameCount, "Tracer_Simulation");
    frameCount++;
}

} // namespace PNM
