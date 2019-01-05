/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "unsteadyStateSimulation.h"
#include "operations/pnmSolver.h"
#include "operations/hkClustering.h"
#include "operations/pnmOperation.h"
#include "network/iterator.h"
#include "network/cluster.h"
#include "misc/userInput.h"
#include "misc/tools.h"
#include "misc/maths.h"
#include "misc/scopedtimer.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

namespace PNM
{

void unsteadyStateSimulation::run()
{
    initialiseOutputFiles();
    initialiseCapillaries();
    initialiseSimulationAttributes();

    while (!simulationInterrupted && timeSoFar < simulationTime)
    {
        fetchTrappedCapillaries();
        updateCapillaryPropreties();
        solvePressureField();
        calculateTimeStep();
        updateFluidFractions();
        updateFluidTerminalFlags();

        updateVariables();
        updateOutputFiles();
        updateGUI();

        if (simulationInterrupted)
            break;
    }
}

std::string unsteadyStateSimulation::getNotification()
{
    std::ostringstream ss;
    ss << "Two-Phase Unsteady-State Simulation \n"
       << std::fixed << std::setprecision(2)
       << "Capillary Number (1e-5): " << capillaryNumber * 10e5 << " / Flow Velocity (m/day): " << flowVelocity << " / Injected PVs: " << injectedPVs << " / Sw: " << currentSw;
    return ss.str();
}

int unsteadyStateSimulation::getProgress()
{
    return timeSoFar / simulationTime * 100;
}

void unsteadyStateSimulation::initialiseOutputFiles()
{
    tools::initialiseFolder("Results/USS_Simulation");
    tools::initialiseFolder("Network_State/USS_Simulation");

    satFilename = "Results/USS_Simulation/saturations.txt";
    fractionalFilename = "Results/USS_Simulation/fractionalFlows.txt";
    pressureFilename = "Results/USS_Simulation/deltaP.txt";

    std::ofstream file;

    file.open(satFilename.c_str());
    file << "injectedPvs\tSw\n";
    file.close();

    file.open(fractionalFilename.c_str());
    file << "injectedPvs\tFo\tFw\n";
    file.close();

    file.open(pressureFilename.c_str());
    file << "injectedPvs\tdeltaP(psi)\n";
    file.close();
}

void unsteadyStateSimulation::initialiseCapillaries()
{
    pnmOperation::get(network).setSwi();
    addWaterChannel();
    setInitialTerminalFlags();
}

void unsteadyStateSimulation::initialiseSimulationAttributes()
{
    simulationTime = userInput::get().overrideByInjectedPVs ? network->totalNetworkVolume * userInput::get().injectedPVs / userInput::get().flowRate
                                                            : userInput::get().simulationTime;
    timeSoFar = 0;
    injectedPVs = 0;

    outputCounter = 0;
    frameCount = 0;

    currentSw = pnmOperation::get(network).getSw();

    auto inletFlux = userInput::get().flowRate / network->inletPoresArea;
    capillaryNumber = userInput::get().waterViscosity * inletFlux / userInput::get().OWSurfaceTension;
    flowVelocity = inletFlux * 86400;

    updatePressureCalculation = true;
}

void unsteadyStateSimulation::addWaterChannel()
{
    for (pore *p : pnmRange<pore>(network))
    {
        if (p->getInlet() || p->getNodeIn() != 0 && p->getNodeOut() != 0 && p->getNodeIn()->getInlet() && p->getNodeOut()->getInlet())
            p->setPhaseFlag(phase::water);
    };

    for (node *n : pnmRange<node>(network))
    {
        if (n->getInlet())
            n->setPhaseFlag(phase::water);
    };
}

void unsteadyStateSimulation::setInitialTerminalFlags()
{
    hkClustering::get(network).clusterWaterElements();

    for (element *e : pnmRange<element>(network))
    {
        e->setConcentration(0);
        e->setOilFraction(e->getPhaseFlag() == phase::oil ? 1 : 0);
        e->setWaterFraction(e->getPhaseFlag() == phase::water ? 1 : 0);
    }

    for (pore *p : pnmRange<pore>(network))
    {
        p->setNodeInOil(p->getPhaseFlag() == phase::oil ? true : false);
        p->setNodeOutOil(p->getPhaseFlag() == phase::oil ? true : false);
        p->setNodeInWater(p->getPhaseFlag() == phase::water ? true : false);
        p->setNodeOutWater(p->getPhaseFlag() == phase::water ? true : false);
    }

    //update terminal flags of oil-filled pores next to inlet-connected water
    for (node *p : pnmRange<node>(network))
    {
        if (p->getPhaseFlag() == phase::water)
            for (auto e : p->getNeighboors())
            {
                pore *neigh = static_cast<pore *>(e);
                if (neigh->getPhaseFlag() == phase::oil)
                {
                    if (p == neigh->getNodeIn())
                    {
                        neigh->setNodeInOil(false);
                        neigh->setNodeInWater(true);
                    }

                    if (p == neigh->getNodeOut())
                    {
                        neigh->setNodeOutOil(false);
                        neigh->setNodeOutWater(true);
                    }
                }
            }
    }
}

void unsteadyStateSimulation::fetchTrappedCapillaries()
{
    //MEASURE_FUNCTION(); //Profiling

    if (!updatePressureCalculation)
        return;

    hkClustering::get(network).clusterOilElements();
    hkClustering::get(network).clusterWaterElements();

    std::vector<pore *> partiallyFilled;
    for (pore *p : pnmRange<pore>(network))
    {
        if (p->getPhaseFlag() == phase::oil)
        {
            if (p->getNodeInWater() || p->getNodeOutWater())
            {
                p->setPhaseFlag(phase::temp);
                partiallyFilled.push_back(p);
            }
        }
        if (p->getPhaseFlag() == phase::water)
        {
            p->setWaterTrapped(true);
            if (p->getClusterWater()->getInlet())
                p->setWaterTrapped(false);
        }
    }

    for (node *p : pnmRange<node>(network))
    {
        if (p->getPhaseFlag() == phase::oil)
        {
            p->setOilTrapped(true);
            if (p->getPhaseFlag() == phase::oil && p->getClusterOil()->getOutlet())
                p->setOilTrapped(false);
        }
        if (p->getPhaseFlag() == phase::water)
        {
            p->setWaterTrapped(true);
            if (p->getClusterWater()->getInlet())
                p->setWaterTrapped(false);
        }
    }

    hkClustering::get(network).clusterOilElements();

    for (pore *p : pnmRange<pore>(network))
    {
        p->setOilTrapped(true);
        if (p->getPhaseFlag() == phase::oil && p->getClusterOil()->getOutlet())
            p->setOilTrapped(false);
    }

    //Identify oil-filled pores without bulk oil to outlet
    for (pore *p : partiallyFilled)
    {
        if (p->getOutlet())
            p->setOilTrapped(false);

        if (p->getNodeInOil() && p->getNodeIn() != 0 && p->getNodeIn()->getPhaseFlag() == phase::oil && p->getNodeIn()->getClusterOil()->getOutlet())
            p->setOilTrapped(false);

        if (p->getNodeOutOil() && p->getNodeOut() != 0 && p->getNodeOut()->getPhaseFlag() == phase::oil && p->getNodeOut()->getClusterOil()->getOutlet())
            p->setOilTrapped(false);
    }

    for (pore *p : partiallyFilled)
        p->setPhaseFlag(phase::oil);
}

void unsteadyStateSimulation::updateCapillaryPropreties()
{
    //MEASURE_FUNCTION(); //Profiling

    if (!updatePressureCalculation)
        return;

    pnmOperation::get(network).assignViscosities();
    pnmOperation::get(network).assignConductivities();

    poresToCheck.clear();
    nodesToCheck.clear();

    for (node *p : pnmRange<node>(network))
    {
        p->setActive(true);

        if (p->getPhaseFlag() == phase::oil && p->getOilTrapped())
            p->setActive(false);

        if (p->getPhaseFlag() == phase::water && p->getWaterTrapped())
            p->setActive(false);
    }

    for (pore *p : pnmRange<pore>(network))
    {
        p->setActive(true);
        p->setCapillaryPressure(0);

        if (p->getPhaseFlag() == phase::oil)
        {

            if (p->getOilTrapped())
                p->setActive(false);
            else
            {
                //Determine pores to check for phase changes
                if ((p->getNodeIn() != 0 && p->getNodeIn()->getPhaseFlag() == phase::water && !p->getNodeIn()->getWaterTrapped()) || (p->getNodeOut() != 0 && p->getNodeOut()->getPhaseFlag() == phase::water && !p->getNodeOut()->getWaterTrapped()))
                    poresToCheck.insert(p);

                //Update capilary pressures a pores with an oil/water interface
                if (!p->getInlet() && !p->getOutlet() && p->getNodeIn() != 0 && p->getNodeOut() != 0)
                {
                    if (p->getNodeOut()->getPhaseFlag() == phase::oil && p->getNodeIn()->getPhaseFlag() == phase::water)
                        p->setCapillaryPressure(p->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * cos(p->getTheta()) / p->getRadius());
                }
                if (!p->getInlet() && !p->getOutlet() && p->getNodeIn() != 0 && p->getNodeOut() != 0)
                {
                    if (p->getNodeOut()->getPhaseFlag() == phase::water && p->getNodeIn()->getPhaseFlag() == phase::oil)
                        p->setCapillaryPressure(-p->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * cos(p->getTheta()) / p->getRadius());
                }
            }
        }

        if (p->getPhaseFlag() == phase::water)
        {
            if (p->getWaterTrapped())
                p->setActive(false);
            else
            {
                //Determine nodes to check for phase changes
                node *nodeIn = p->getNodeIn();
                node *nodeOut = p->getNodeOut();
                if (nodeIn != 0 && nodeIn->getPhaseFlag() == phase::oil && !nodeIn->getOilTrapped())
                    nodesToCheck.insert(nodeIn);
                if (nodeOut != 0 && nodeOut->getPhaseFlag() == phase::oil && !nodeOut->getOilTrapped())
                    nodesToCheck.insert(nodeOut);

                //Update capilary pressures a nodes with an oil/water interfac
                if (!p->getInlet() && !p->getOutlet() && nodeIn != 0 && nodeOut != 0)
                {
                    if (nodeOut->getPhaseFlag() == phase::oil && nodeIn->getPhaseFlag() == phase::water)
                    {
                        //pore filling mechanism
                        int oilNeighboorsNumber(0);
                        for (element *n : nodeOut->getNeighboors())
                            if (n->getPhaseFlag() == phase::oil)
                                oilNeighboorsNumber++;

                        if (nodeOut->getTheta() > maths::pi() / 2) //drainage
                            p->setCapillaryPressure(nodeOut->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * cos(nodeOut->getTheta()) / nodeOut->getRadius());
                        if (nodeOut->getTheta() < maths::pi() / 2) //imbibition
                            p->setCapillaryPressure(nodeOut->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * cos(nodeOut->getTheta()) / nodeOut->getRadius() - oilNeighboorsNumber * userInput::get().OWSurfaceTension / nodeOut->getRadius());
                    }

                    if (nodeOut->getPhaseFlag() == phase::water && nodeIn->getPhaseFlag() == phase::oil)
                    {
                        //pore filling mechanism
                        int oilNeighboorsNumber(0);
                        for (element *n : nodeIn->getNeighboors())
                            if (n->getPhaseFlag() == phase::oil)
                                oilNeighboorsNumber++;

                        if (nodeIn->getTheta() > maths::pi() / 2) //drainage
                            p->setCapillaryPressure(-nodeIn->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * cos(nodeIn->getTheta()) / nodeIn->getRadius());
                        if (nodeIn->getTheta() < maths::pi() / 2) //imbibition
                            p->setCapillaryPressure(-nodeIn->getEntryPressureCoefficient() * userInput::get().OWSurfaceTension * cos(nodeIn->getTheta()) / nodeIn->getRadius() + oilNeighboorsNumber * userInput::get().OWSurfaceTension / nodeIn->getRadius());
                    }
                }
            }
        }
    }
}

void unsteadyStateSimulation::solvePressureField()
{
    //MEASURE_FUNCTION(); //Profiling

    if (!updatePressureCalculation)
        return;

    bool stillMorePoresToClose = true;

    while (stillMorePoresToClose)
    {
        hkClustering::get(network).clusterActiveElements();
        for (pore *p : pnmRange<pore>(network))
        {
            if (p->getActive() && p->getClusterActive()->getSpanning() == false)
            {
                p->setCapillaryPressure(0);
                p->setActive(false);
            }
        }

        stillMorePoresToClose = false;

        pnmSolver::get(network).solvePressuresConstantFlowRate();

        for (pore *p : pnmRange<pore>(network))
        {
            if (p->getActive() && p->getNodeIn() != 0 && p->getNodeOut() != 0 && ((p->getFlow() > 0 && p->getNodeOut()->getPhaseFlag() == phase::oil && p->getNodeIn()->getWaterFraction() > 1e-20) || (p->getFlow() < 0 && p->getNodeOut()->getWaterFraction() > 1e-20 && p->getNodeIn()->getPhaseFlag() == phase::oil)))
            {
                p->setCapillaryPressure(0);
                p->setActive(false);
                stillMorePoresToClose = true;
            }
            if (p->getActive() && p->getOutlet() && p->getFlow() < 0)
            {
                p->setCapillaryPressure(0);
                p->setActive(false);
                stillMorePoresToClose = true;
            }
        }
    }
}

void unsteadyStateSimulation::calculateTimeStep()
{
    //MEASURE_FUNCTION(); //Profiling

    timeStep = 1e50;
    for (pore *p : poresToCheck)
    {
        if (p->getActive() && std::abs(p->getFlow()) > 1e-50)
        {
            double step = p->getVolume() * p->getOilFraction() / std::abs(p->getFlow());
            if (step < timeStep)
                timeStep = step;
        }
    }

    for (node *p : nodesToCheck)
    {
        if (p->getActive() && std::abs(p->getFlow()) > 1e-50)
        {
            double step = p->getVolume() * p->getOilFraction() / std::abs(p->getFlow());
            if (step < timeStep)
                timeStep = step;
        }
    }

    if (hkClustering::get(network).isWaterSpanning) //if water is forming a spanning cluster
    {
        double timeRequiredToFillOneTenthPV = network->totalNetworkVolume / userInput::get().flowRate / 10;
        timeStep = std::min(timeStep, timeRequiredToFillOneTenthPV);
    }
}

void unsteadyStateSimulation::updateFluidFractions()
{
    //MEASURE_FUNCTION(); //Profiling

    for (pore *p : poresToCheck)
    {
        if (p->getActive() && std::abs(p->getFlow()) > 1e-50)
        {
            double incrementalWater = std::abs(p->getFlow()) * timeStep;
            currentSw += incrementalWater / network->totalNetworkVolume;

            p->setWaterFraction(p->getWaterFraction() + incrementalWater / p->getVolume());
            p->setOilFraction(1 - p->getWaterFraction());

            if (p->getWaterFraction() > 1 - 1e-8)
            {
                p->setPhaseFlag(phase::water);
                p->setWaterFraction(1);
                p->setOilFraction(0);
                updatePressureCalculation = true;
            }
        }
    }

    for (node *p : nodesToCheck)
    {
        if (p->getActive() && std::abs(p->getFlow()) > 1e-50)
        {
            double incrementalWater = std::abs(p->getFlow()) * timeStep;
            currentSw += incrementalWater / network->totalNetworkVolume;

            p->setWaterFraction(p->getWaterFraction() + incrementalWater / p->getVolume());
            p->setOilFraction(1 - p->getWaterFraction());

            if (p->getWaterFraction() > 1 - 1e-8)
            {
                p->setPhaseFlag(phase::water);
                p->setWaterFraction(1);
                p->setOilFraction(0);
                updatePressureCalculation = true;
            }
        }
    }
}

void unsteadyStateSimulation::updateFluidTerminalFlags()
{
    //MEASURE_FUNCTION(); //Profiling

    for (pore *p : poresToCheck)
    {
        node *n = nullptr;

        if (p->getPhaseFlag() == phase::water)
        {
            if (p->getNodeInOil())
                n = p->getNodeIn();
            if (p->getNodeOutOil())
                n = p->getNodeOut();

            p->setNodeInOil(false);
            p->setNodeOutOil(false);
            p->setNodeInWater(true);
            p->setNodeOutWater(true);
        }

        if (n != nullptr && n->getPhaseFlag() == phase::water && n->getWaterTrapped())
        {
            for (node *nn : pnmRange<node>(network))
            {
                if (nn->getPhaseFlag() == phase::water && nn->getClusterWater() == n->getClusterWater())
                    for (auto e : nn->getNeighboors())
                    {
                        pore *nnn = static_cast<pore *>(e);
                        if (nnn->getPhaseFlag() == phase::oil)
                        {
                            if (nnn->getNodeIn() == nn)
                            {
                                nnn->setNodeInOil(false);
                                nnn->setNodeInWater(true);
                            }

                            if (nnn->getNodeOut() == nn)
                            {
                                nnn->setNodeOutOil(false);
                                nnn->setNodeOutWater(true);
                            }
                        }
                    }
            }
        }
    }

    for (node *p : nodesToCheck)
    {
        if (p->getPhaseFlag() == phase::water)
        {
            for (auto e : p->getNeighboors())
            {
                pore *n = static_cast<pore *>(e);
                if (n->getPhaseFlag() == phase::oil)
                {
                    if (n->getNodeIn() == p)
                    {
                        n->setNodeInOil(false);
                        n->setNodeInWater(true);
                    }

                    if (n->getNodeOut() == p)
                    {
                        n->setNodeOutOil(false);
                        n->setNodeOutWater(true);
                    }
                }

                if (n->getPhaseFlag() == phase::water && n->getWaterTrapped())
                {
                    for (node *nn : pnmRange<node>(network))
                    {
                        if (nn->getPhaseFlag() == phase::water && nn->getClusterWater() == n->getClusterWater())
                            for (auto e : nn->getNeighboors())
                            {
                                pore *nnn = static_cast<pore *>(e);
                                ;
                                if (nnn->getPhaseFlag() == phase::oil)
                                {
                                    if (nnn->getNodeIn() == nn)
                                    {
                                        nnn->setNodeInOil(false);
                                        nnn->setNodeInWater(true);
                                    }

                                    if (nnn->getNodeOut() == nn)
                                    {
                                        nnn->setNodeOutOil(false);
                                        nnn->setNodeOutWater(true);
                                    }
                                }
                            }
                    }
                }
            }
        }
    }
}

void unsteadyStateSimulation::updateVariables()
{
    timeSoFar += timeStep;
    injectedPVs += timeStep * userInput::get().flowRate / network->totalNetworkVolume;
}

void unsteadyStateSimulation::updateOutputFiles()
{
    if (std::abs(outputCounter - injectedPVs) < 0.01)
        return;

    std::ofstream file;

    file.open(satFilename, std::ofstream::app);
    file << injectedPVs << "\t" << currentSw << std::endl;
    file.close();

    auto Fw = pnmOperation::get(network).getFlow(phase::water) / userInput::get().flowRate;
    auto Fo = 1 - Fw;
    file.open(fractionalFilename, std::ofstream::app);
    file << injectedPVs << "\t" << Fo << "\t" << Fw << std::endl;
    file.close();

    auto deltaP = pnmSolver::get(network).getDeltaP();
    file.open(pressureFilename, std::ofstream::app);
    file << injectedPVs << "\t" << maths::PaToPsi(deltaP) << std::endl;
    file.close();

    generateNetworkStateFiles();

    outputCounter = injectedPVs;
}

void unsteadyStateSimulation::generateNetworkStateFiles()
{
    if (!userInput::get().extractDataUSS)
        return;

    pnmOperation::get(network).generateNetworkState(frameCount, "USS_Simulation");
    frameCount++;
}

} // namespace PNM
