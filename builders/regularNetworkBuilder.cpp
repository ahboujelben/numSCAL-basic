/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "regularNetworkBuilder.h"
#include "operations/pnmSolver.h"
#include "operations/hkClustering.h"
#include "operations/pnmOperation.h"
#include "network/iterator.h"
#include "network/cluster.h"
#include "misc/userInput.h"
#include "misc/tools.h"
#include "misc/randomGenerator.h"
#include "misc/maths.h"

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace PNM
{

void regularNetworkBuilder::make()
{
    initiateNetworkProperties();
    createNodes();
    createPores();
    assignNeighboors();
    setBoundaryConditions();
    applyCoordinationNumber();
    cleanNetwork();
    assignRadii();
    assignLengths();
    distortNetwork();
    assignShapeFactors();
    assignShapeFactorConstants();
    assignVolumes();
    assignWettabilities();
    calculateProperties();
}

std::string regularNetworkBuilder::getNotification()
{
    std::ostringstream ss;
    ss << "Regular Network: " << userInput::get().Nx << "x" << userInput::get().Ny << "x" << userInput::get().Nz << " / Total Nodes: " << network->totalNodes << " / Total Throats: " << network->totalPores << "\n"
       << std::fixed << std::setprecision(2)
       << "Perm.(mD): " << network->absolutePermeability / 0.987e-15
       << " / Porosity(%): " << network->porosity * 100
       << " / Dx(mm): " << network->xEdgeLength * 1e3
       << " / Dy(mm): " << network->yEdgeLength * 1e3
       << " / Dz(mm): " << network->zEdgeLength * 1e3;

    return ss.str();
}

void regularNetworkBuilder::initiateNetworkProperties()
{
    network = std::make_shared<networkModel>();

    int Nx = userInput::get().Nx;
    int Ny = userInput::get().Ny;
    int Nz = userInput::get().Nz;

    network->totalNodes = Nx * Ny * Nz;
    network->totalPores = 3 * Nx * Ny * Nz + Ny * Nz + Nx * Nz + Nx * Ny;

    network->tableOfNodes.reserve(network->totalNodes);
    network->tableOfPores.reserve(network->totalPores);

    network->xEdgeLength = Nx == 1 ? userInput::get().length : (Nx - 1) * userInput::get().length;
    network->yEdgeLength = Ny == 1 ? userInput::get().length : (Ny - 1) * userInput::get().length;
    network->zEdgeLength = Nz == 1 ? userInput::get().length : (Nz - 1) * userInput::get().length;

    network->maxConnectionNumber = 6;

    network->is2D = Nz == 1 ? true : false;
}

void regularNetworkBuilder::createNodes()
{
    std::cout << "Creating Nodes..." << std::endl;

    int Nx = userInput::get().Nx;
    int Ny = userInput::get().Ny;
    int Nz = userInput::get().Nz;

    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz; ++k)
            {
                network->tableOfNodes.push_back(std::shared_ptr<node>(new node(i, j, k)));
            }

    for (node *n : pnmRange<node>(network))
    {
        n->setXCoordinate(n->getIndexX() * userInput::get().length);
        n->setYCoordinate(n->getIndexY() * userInput::get().length);
        n->setZCoordinate(n->getIndexZ() * userInput::get().length);
    }

    signalProgress(20);
}

void regularNetworkBuilder::createPores()
{
    std::cout << "Creating Throats..." << std::endl;
    int Nx = userInput::get().Nx;
    int Ny = userInput::get().Ny;
    int Nz = userInput::get().Nz;

    for (int i = 0; i < Nx + 1; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz; ++k)
                network->tableOfPores.push_back(std::shared_ptr<pore>(new pore(getNode(i, j, k), getNode(i - 1, j, k))));
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny + 1; ++j)
            for (int k = 0; k < Nz; ++k)
                network->tableOfPores.push_back(std::shared_ptr<pore>(new pore(getNode(i, j, k), getNode(i, j - 1, k))));
    for (int i = 0; i < Nx; ++i)
        for (int j = 0; j < Ny; ++j)
            for (int k = 0; k < Nz + 1; ++k)
                network->tableOfPores.push_back(std::shared_ptr<pore>(new pore(getNode(i, j, k), getNode(i, j, k - 1))));

    signalProgress(40);
}

void regularNetworkBuilder::assignNeighboors()
{
    std::cout << "Setting neighboors..." << std::endl;

    for (pore *p : pnmRange<pore>(network))
    {
        if (p->getNodeIn() != 0)
        {
            p->getNodeIn()->getNeighboors().push_back(p);
        }

        if (p->getNodeOut() != 0)
        {
            p->getNodeOut()->getNeighboors().push_back(p);
        }
    }

    for (node *n : pnmRange<node>(network))
    {
        int connectionNumber = n->getNeighboors().size();
        if (connectionNumber > network->maxConnectionNumber)
            network->maxConnectionNumber = connectionNumber;
    }

    for (pore *p : pnmRange<pore>(network))
    {
        std::vector<element *> neighboors;
        if (p->getNodeIn() != 0)
            neighboors.push_back(p->getNodeIn());
        if (p->getNodeOut() != 0)
            neighboors.push_back(p->getNodeOut());
        p->setNeighboors(neighboors);
    }
}

void regularNetworkBuilder::setBoundaryConditions()
{
    std::cout << "Setting boundary conditions..." << std::endl;

    //Define inlet/outlet elements
    for (pore *p : pnmRange<pore>(network))
    {
        if (p->getNodeOut() == 0)
        {
            p->setInlet(true);
            network->inletPores.push_back(p);
        }
        if (p->getNodeIn() == 0)
        {
            p->setOutlet(true);
            network->outletPores.push_back(p);
        }
    }

    for (node *n : pnmRange<node>(network))
    {
        if (n->getIndexX() == 0)
            n->setInlet(true);
        if (n->getIndexX() == userInput::get().Nx - 1)
            n->setOutlet(true);
    }

    //Remove throats at both sides
    for (int i = 0; i < userInput::get().Nx; ++i)
        for (int j = 0; j < userInput::get().Ny; ++j)
        {
            pore *zIn = getPoreZ(i, j, 0);
            pore *zOut = getPoreZ(i, j, userInput::get().Nz);
            zIn->setActive(false);
            zOut->setActive(false);
        }
    for (int i = 0; i < userInput::get().Nx; ++i)
        for (int k = 0; k < userInput::get().Nz; ++k)
        {
            pore *yIn = getPoreY(i, 0, k);
            pore *yOut = getPoreY(i, userInput::get().Ny, k);
            yIn->setActive(false);
            yOut->setActive(false);
        }
}

void regularNetworkBuilder::applyCoordinationNumber()
{
    std::cout << "Setting coordination number..." << std::endl;

    double coordinationNumber = userInput::get().coordinationNumber;

    if (coordinationNumber < 6 || (coordinationNumber < 4 && network->is2D))
    {
        double totalEnabledPores = network->totalPores - 2 * userInput::get().Nx * userInput::get().Ny - 2 * userInput::get().Nx * userInput::get().Nz;

        auto closedPoresNumber = (network->is2D ? int(totalEnabledPores * (1 - coordinationNumber / 4.0)) : int(totalEnabledPores * (1 - coordinationNumber / 6.0)));

        auto shuffledPores = network->tableOfPores;
        std::shuffle(shuffledPores.begin(), shuffledPores.end(), randomGenerator(userInput::get().seed).getGen());

        while (closedPoresNumber > 0)
        {
            pore *p = shuffledPores.back().get();
            shuffledPores.pop_back();
            if (p->getActive() && !p->getInlet() && !p->getOutlet())
            {
                p->setActive(false);
                --closedPoresNumber;
            }
        }
    }
}

void regularNetworkBuilder::cleanNetwork()
{
    std::cout << "Cleaning up..." << std::endl;

    hkClustering::get(network).clusterActiveElements();

    for (element *e : pnmRange<element>(network))
    {
        if (e->getActive() && !e->getClusterActive()->getSpanning())
            e->setActive(false);
    };

    // delete closed pores from inlet/ outlet containers
    network->inletPores.erase(std::remove_if(network->inletPores.begin(), network->inletPores.end(), [this](pore *p) -> bool {
                                  return !p->getActive();
                              }),
                              network->inletPores.end());
    network->outletPores.erase(remove_if(network->outletPores.begin(), network->outletPores.end(), [this](pore *p) -> bool {
                                   return !p->getActive();
                               }),
                               network->outletPores.end());

    // delete closed elements from neighboor containers
    for (element *e : pnmRange<element>(network))
    {
        std::vector<element *> &neighboors = e->getNeighboors();
        neighboors.erase(remove_if(neighboors.begin(), neighboors.end(), [this](element *e) -> bool {
                             return !e->getActive();
                         }),
                         neighboors.end());
    }

    network->tableOfNodes.erase(remove_if(network->tableOfNodes.begin(), network->tableOfNodes.end(), [this](std::shared_ptr<node> n) -> bool {
                                    return !n->getActive();
                                }),
                                network->tableOfNodes.end());

    network->tableOfPores.erase(remove_if(network->tableOfPores.begin(), network->tableOfPores.end(), [this](std::shared_ptr<pore> p) -> bool {
                                    return !p->getActive();
                                }),
                                network->tableOfPores.end());

    network->totalPores = network->tableOfPores.size();
    network->totalNodes = network->tableOfNodes.size();

    int pid(0);
    for (pore *p : pnmRange<pore>(network))
    {
        p->setId(++pid);
    };

    int nid(0);
    for (node *n : pnmRange<node>(network))
    {
        n->setId(++nid);
    };

    signalProgress(60);
}

void regularNetworkBuilder::assignRadii()
{
    std::cout << "Setting radii..." << std::endl;

    pnmOperation::get(network).assignRadii();
}

void regularNetworkBuilder::assignLengths()
{
    std::cout << "Setting lengths..." << std::endl;

    pnmOperation::get(network).assignLengths();
}

void regularNetworkBuilder::distortNetwork()
{
    std::cout << "Distorting network..." << std::endl;

    pnmOperation::get(network).distortNetwork();
}

void regularNetworkBuilder::assignShapeFactors()
{
    std::cout << "Setting shape factors..." << std::endl;

    pnmOperation::get(network).assignShapeFactors();
}

void regularNetworkBuilder::assignShapeFactorConstants()
{
    std::cout << "Setting shape factor constants..." << std::endl;

    pnmOperation::get(network).assignShapeFactorConstants();
}

void regularNetworkBuilder::assignVolumes()
{
    std::cout << "Setting volumes..." << std::endl;

    pnmOperation::get(network).assignVolumes();
}

void regularNetworkBuilder::assignWettabilities()
{
    std::cout << "Setting wettabilities..." << std::endl;

    pnmOperation::get(network).assignWettabilities();
    pnmOperation::get(network).backupWettability();

    signalProgress(80);
}

void regularNetworkBuilder::calculateProperties()
{
    std::cout << "Calculating network properties..." << std::endl;

    pnmOperation::get(network).calculateNetworkVolume();
    pnmSolver::get(network).calculatePermeabilityAndPorosity();

    signalProgress(100);
}

node *regularNetworkBuilder::getNode(int i, int j, int k)
{
    auto Nx = userInput::get().Nx;
    auto Ny = userInput::get().Ny;
    auto Nz = userInput::get().Nz;

    if (i < 0 || i > Nx - 1 || j < 0 || j > Ny - 1 || k < 0 || k > Nz - 1)
        return 0;
    return network->getNode(i * Ny * Nz + j * Nz + k);
}

pore *regularNetworkBuilder::getPoreX(int i, int j, int k)
{
    auto Nx = userInput::get().Nx;
    auto Ny = userInput::get().Ny;
    auto Nz = userInput::get().Nz;

    if (i < 0 || i > Nx || j < 0 || j > Ny - 1 || k < 0 || k > Nz - 1)
        return 0;
    return network->getPore(i * Ny * Nz + j * Nz + k);
}

pore *regularNetworkBuilder::getPoreY(int i, int j, int k)
{
    auto Nx = userInput::get().Nx;
    auto Ny = userInput::get().Ny;
    auto Nz = userInput::get().Nz;

    if (i < 0 || i > Nx - 1 || j < 0 || j > Ny || k < 0 || k > Nz - 1)
        return 0;
    return network->getPore((Nx + 1) * Ny * Nz + i * (Ny + 1) * Nz + j * Nz + k);
}

pore *regularNetworkBuilder::getPoreZ(int i, int j, int k)
{
    auto Nx = userInput::get().Nx;
    auto Ny = userInput::get().Ny;
    auto Nz = userInput::get().Nz;

    if (i < 0 || i > Nx - 1 || j < 0 || j > Ny - 1 || k < 0 || k > Nz)
        return 0;
    return network->getPore((Nx + 1) * Ny * Nz + Nx * (Ny + 1) * Nz + i * Ny * (Nz + 1) + j * (Nz + 1) + k);
}

} // namespace PNM
