/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "pnmSolver.h"
#include "operations/hkClustering.h"
#include "operations/pnmOperation.h"
#include "network/iterator.h"
#include "misc/userInput.h"

//Eigen library
#include <libs/Eigen/Sparse>
#include <libs/Eigen/IterativeLinearSolvers>
#include <libs/Eigen/SparseCholesky>
#include <fstream>
#include <iostream>

namespace PNM
{

using namespace Eigen;
using namespace std;

pnmSolver pnmSolver::instance;

pnmSolver &pnmSolver::get(std::shared_ptr<networkModel> network)
{
    instance.network = network;
    return instance;
}

double pnmSolver::solvePressuresConstantGradient(double pressureIn, double pressureOut, bool defaultSolver)
{
    SparseMatrix<double> conductivityMatrix(network->totalNodes, network->totalNodes);
    conductivityMatrix.reserve(VectorXi::Constant(network->totalNodes, network->maxConnectionNumber + 3));
    VectorXd b = VectorXd::Zero(network->totalNodes);
    VectorXd pressures = VectorXd::Zero(network->totalNodes);

    auto rank(0);
    for (node *n : pnmRange<node>(network))
        n->setRank(rank++);

    int row = 0;
    for (node *n : pnmRange<node>(network))
    {
        double conductivity(1e-200);
        for (element *e : n->getNeighboors())
        {
            pore *p = static_cast<pore *>(e);
            if (p->getActive())
            {
                if (p->getInlet())
                {
                    b(row) = -pressureIn * p->getConductivity();
                    conductivity -= p->getConductivity();
                }
                if (p->getOutlet())
                {
                    b(row) = -pressureOut * p->getConductivity();
                    conductivity -= p->getConductivity();
                }
                if (!p->getInlet() && !p->getOutlet())
                {
                    node *neighboor = p->getOtherNode(n);
                    conductivityMatrix.insert(row, neighboor->getRank()) = p->getConductivity();
                    conductivity -= p->getConductivity();

                    //Capillary Pressure
                    if (neighboor == p->getNodeOut())
                        b(row) += p->getCapillaryPressure() * p->getConductivity();
                    if (neighboor == p->getNodeIn())
                        b(row) -= p->getCapillaryPressure() * p->getConductivity();
                }
            }
        }
        conductivityMatrix.insert(row, n->getRank()) = conductivity;
        row++;
    }
    conductivityMatrix.makeCompressed();

    if (defaultSolver || userInput::get().solverChoice == solver::conjugateGradient)
    {
        ConjugateGradient<SparseMatrix<double>, Lower | Upper> solver;
        solver.setTolerance(1e-25);
        solver.setMaxIterations(2000);
        solver.compute(conductivityMatrix);
        pressures = solver.solve(b);
    }

    else if (userInput::get().solverChoice == solver::cholesky)
    {
        SimplicialLDLT<SparseMatrix<double>> solver;
        solver.compute(conductivityMatrix);
        pressures = solver.solve(b);
    }

    for (node *n : pnmRange<node>(network))
        n->setPressure(pressures[n->getRank()]);

    return updateFlowsConstantGradient(pressureIn, pressureOut);
}

double pnmSolver::solvePressuresConstantFlowRate()
{
    SparseMatrix<double> conductivityMatrix(network->totalNodes, network->totalNodes);
    conductivityMatrix.reserve(VectorXi::Constant(network->totalNodes, network->maxConnectionNumber + 3));
    VectorXd b = VectorXd::Zero(network->totalNodes);
    VectorXd pressures = VectorXd::Zero(network->totalNodes);

    auto rank(0);
    for (node *n : pnmRange<node>(network))
        n->setRank(rank++);

    double inletPoresVolume = pnmOperation::get(network).getInletPoresVolume();

    int row = 0;
    for (node *n : pnmRange<node>(network))
    {
        double conductivity(1e-200);
        for (element *e : n->getNeighboors())
        {
            pore *p = static_cast<pore *>(e);
            if (p->getActive())
            {
                if (p->getInlet())
                {
                    b(row) -= p->getVolume() / inletPoresVolume * userInput::get().flowRate;
                }
                if (p->getOutlet())
                {
                    conductivity -= p->getConductivity();
                }
                if (!p->getInlet() && !p->getOutlet())
                {
                    node *neighboor = p->getOtherNode(n);
                    conductivityMatrix.insert(row, neighboor->getRank()) = p->getConductivity();
                    conductivity -= p->getConductivity();

                    //Capillary Pressure
                    if (neighboor == p->getNodeOut())
                        b(row) += p->getCapillaryPressure() * p->getConductivity();
                    if (neighboor == p->getNodeIn())
                        b(row) -= p->getCapillaryPressure() * p->getConductivity();
                }
            }
        }
        conductivityMatrix.insert(row, n->getRank()) = conductivity;
        row++;
    }
    conductivityMatrix.makeCompressed();

    if (userInput::get().solverChoice == solver::conjugateGradient)
    {
        ConjugateGradient<SparseMatrix<double>, Lower | Upper> solver;
        solver.setTolerance(1e-25);
        solver.setMaxIterations(2000);
        solver.compute(conductivityMatrix);
        pressures = solver.solve(b);
    }

    else if (userInput::get().solverChoice == solver::cholesky)
    {
        SimplicialLDLT<SparseMatrix<double>> solver;
        solver.compute(conductivityMatrix);
        pressures = solver.solve(b);
    }

    for (node *n : pnmRange<node>(network))
        n->setPressure(pressures[n->getRank()]);

    return updateFlowsConstantFlowRate();
}

double pnmSolver::updateFlowsConstantGradient(double pressureIn, double pressureOut)
{
    double outletFlow(0);
    for (pore *p : pnmRange<pore>(network))
    {
        p->setFlow(0);
        if (p->getActive())
        {
            if (p->getOutlet())
            {
                node *activeNode = p->getNodeIn() == 0 ? p->getNodeOut() : p->getNodeIn();
                p->setFlow((activeNode->getPressure() - pressureOut) * p->getConductivity());
                outletFlow += p->getFlow();
            }
            if (p->getInlet())
            {
                node *activeNode = p->getNodeIn() == 0 ? p->getNodeOut() : p->getNodeIn();
                p->setFlow((pressureIn - activeNode->getPressure()) * p->getConductivity());
            }
            if (!p->getInlet() && !p->getOutlet())
            {
                p->setFlow((p->getNodeOut()->getPressure() - p->getNodeIn()->getPressure()) * p->getConductivity());
            }
        }
    }
    return outletFlow;
}

double pnmSolver::updateFlowsConstantFlowRate()
{
    double inletPoresVolume = pnmOperation::get(network).getInletPoresVolume();
    double outletFlow(0);
    for (pore *p : pnmRange<pore>(network))
    {
        p->setFlow(0);
        if (p->getActive())
        {
            if (p->getOutlet())
            {
                node *activeNode = p->getNodeIn() == 0 ? p->getNodeOut() : p->getNodeIn();
                p->setFlow((activeNode->getPressure()) * p->getConductivity());
                outletFlow += p->getFlow();
            }
            if (p->getInlet())
            {
                p->setFlow(p->getVolume() / inletPoresVolume * userInput::get().flowRate);
            }
            if (!p->getInlet() && !p->getOutlet())
            {
                p->setFlow((p->getNodeOut()->getPressure() - p->getNodeIn()->getPressure() - p->getCapillaryPressure()) * p->getConductivity());
            }
        }
    }

    for (node *n : pnmRange<node>(network))
        n->setFlow(0);

    for (pore *p : pnmRange<pore>(network))
    {
        if (p->getActive())
        {
            if (p->getFlow() > 1e-50)
            {
                node *n = p->getNodeIn();
                if (n != 0 && n->getActive())
                    n->setFlow((n->getFlow() + std::abs(p->getFlow())));
            }
            if (p->getFlow() < -1e-50)
            {
                node *n = p->getNodeOut();
                if (n != 0 && n->getActive())
                    n->setFlow((n->getFlow() + std::abs(p->getFlow())));
            }
        }
    }

    return outletFlow;
}

double pnmSolver::getDeltaP()
{
    double pInlet(0), pOutlet(0);
    double inletSize(0), outletSize(0);

    for (pore *p : pnmInlet(network))
    {
        if (p->getActive())
        {
            pInlet += p->getNodeIn()->getPressure();
            inletSize++;
        }
    }
    pInlet /= inletSize;

    for (pore *p : pnmOutlet(network))
    {
        if (p->getActive())
        {
            pOutlet += p->getNodeOut()->getPressure();
            outletSize++;
        }
    }
    pOutlet /= outletSize;

    return pInlet - pOutlet;
}

void pnmSolver::calculatePermeabilityAndPorosity()
{
    pnmOperation::get(network).assignConductivities();
    double outletFlow = solvePressuresConstantGradient(1, 0, true);

    network->normalisedFlow = outletFlow;

    network->absolutePermeability = (outletFlow * network->xEdgeLength) / (network->yEdgeLength * network->zEdgeLength);

    network->porosity = network->totalNetworkVolume / (network->xEdgeLength * network->yEdgeLength * network->zEdgeLength);
}

std::pair<double, double> pnmSolver::calculateRelativePermeabilities()
{
    double oilRelativePermeability(0), waterRelativePermeability(0);
    pnmOperation::get(network).assignViscosities();

    //Oil Rel Perm

    hkClustering::get(network).clusterOilConductorElements();

    if (hkClustering::get(network).isOilSpanningThroughFilms)
    {
        pnmOperation::get(network).assignOilConductivities();
        double oilFlow = solvePressuresConstantGradient();
        oilRelativePermeability = oilFlow * userInput::get().oilViscosity / network->normalisedFlow;
    }

    //Water Rel Perm

    hkClustering::get(network).clusterWaterConductorElements();

    if (hkClustering::get(network).isWaterSpanningThroughFilms)
    {
        pnmOperation::get(network).assignWaterConductivities();
        double waterFlow = solvePressuresConstantGradient();
        waterRelativePermeability = waterFlow * userInput::get().waterViscosity / network->normalisedFlow;
    }

    return std::make_pair(oilRelativePermeability, waterRelativePermeability);
}

} // namespace PNM
