/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "statoilNetworkBuilder.h"
#include "misc/maths.h"
#include "misc/userInput.h"
#include "network/iterator.h"

#include <QDir>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace PNM {

void statoilNetworkBuilder::make() {
  initiateNetworkProperties();
  importNode1();
  importNode2();
  importLink1();
  importLink2();
  assignNeighboors();
  cleanNetwork();
  assignShapeFactorConstants();
  assignWettabilities();
  calculateProperties();
}

std::string statoilNetworkBuilder::getNotification() {
  std::ostringstream ss;
  ss << userInput::get().rockPrefix
     << " Network / Total Nodes: " << network->totalNodes
     << " / Total Throats: " << network->totalPores << "\n"
     << std::fixed << std::setprecision(2)
     << "Perm.(mD): " << network->absolutePermeability / 0.987e-15
     << " / Porosity(%): " << network->porosity * 100
     << " / Dx(mm): " << network->xEdgeLength * 1e3
     << " / Dy(mm): " << network->yEdgeLength * 1e3
     << " / Dz(mm): " << network->zEdgeLength * 1e3;

  return ss.str();
}

void statoilNetworkBuilder::initiateNetworkProperties() {
  network = std::make_shared<networkModel>();
  network->maxConnectionNumber = 0;
  network->is2D = false;
}

void statoilNetworkBuilder::importNode1() {
  std::string filePath = userInput::get().extractedNetworkFolderPath +
                         userInput::get().rockPrefix + "_node1.dat";

  std::cout << "Importing data from " << QDir().absolutePath().toStdString()
            << "/" << filePath << " ..." << std::endl;

  std::ifstream file(filePath.c_str());
  if (!file.good()) throw std::domain_error("Missing data file. Aborting. \n");

  file >> network->totalNodes >> network->xEdgeLength >> network->yEdgeLength >>
      network->zEdgeLength;
  network->tableOfNodes.reserve(static_cast<unsigned>(network->totalNodes));

  std::string dummy;
  std::getline(file, dummy);

  for (auto i = 0; i < network->totalNodes; ++i) {
    int id;
    double x, y, z;
    int numberOfNeighboors;
    bool isInlet, isOutlet;

    file >> id >> x >> y >> z >> numberOfNeighboors;

    network->tableOfNodes.push_back(std::make_shared<node>(x, y, z));

    if (numberOfNeighboors > network->maxConnectionNumber)
      network->maxConnectionNumber = numberOfNeighboors;

    for (int j = 0; j < numberOfNeighboors; ++j) {
      file >> dummy;
    }

    file >> isInlet >> isOutlet;
    node *n = network->tableOfNodes[i].get();
    n->setInlet(isInlet);
    n->setOutlet(isOutlet);

    for (int j = 0; j < numberOfNeighboors; ++j) {
      file >> dummy;
    }
  }
  signalProgress(10);
}

void statoilNetworkBuilder::importNode2() {
  std::string filePath = userInput::get().extractedNetworkFolderPath +
                         userInput::get().rockPrefix + "_node2.dat";

  std::cout << "Importing data from " << QDir().absolutePath().toStdString()
            << "/" << filePath << " ..." << std::endl;

  std::ifstream file(filePath.c_str());
  if (!file.good()) throw std::domain_error("Missing data file. Aborting. \n");

  for (node *n : pnmRange<node>(network)) {
    int id;
    double volume, radius, shapeFactor;
    std::string dummy;

    file >> id >> volume >> radius >> shapeFactor >> dummy;

    n->setVolume(volume);
    n->setEffectiveVolume(volume);
    n->setRadius(radius);
    n->setShapeFactor(shapeFactor);
    n->setLength(volume / std::pow(radius, 2) * (4 * shapeFactor));
  }

  signalProgress(20);
}

void statoilNetworkBuilder::importLink1() {
  std::string filePath = userInput::get().extractedNetworkFolderPath +
                         userInput::get().rockPrefix + "_link1.dat";

  std::cout << "Importing data from " << QDir().absolutePath().toStdString()
            << "/" << filePath << " ..." << std::endl;

  std::ifstream file(filePath.c_str());
  if (!file.good()) throw std::domain_error("Missing data file. Aborting. \n");

  file >> network->totalPores;
  network->tableOfPores.reserve(network->totalPores);

  std::string dummy;
  getline(file, dummy);

  for (int i = 0; i < network->totalPores; ++i) {
    int id, nodeIndex1, nodeIndex2;
    double radius, shapeFactor, poreLength;

    file >> id >> nodeIndex1 >> nodeIndex2 >> radius >> shapeFactor >>
        poreLength;

    node *nodeIn = nullptr;
    node *nodeOut = nullptr;

    if (nodeIndex1 == 0 || nodeIndex2 == 0) {
      nodeOut = (nodeIndex1 == 0 ? network->getNode(nodeIndex2 - 1)
                                 : network->getNode(nodeIndex1 - 1));
    }

    else if (nodeIndex1 == -1 || nodeIndex2 == -1) {
      nodeIn = (nodeIndex1 == -1 ? network->getNode(nodeIndex2 - 1)
                                 : network->getNode(nodeIndex1 - 1));
    } else {
      nodeOut = network->getNode(nodeIndex1 - 1);
      nodeIn = network->getNode(nodeIndex2 - 1);
    }

    network->tableOfPores.push_back(std::make_shared<pore>(nodeIn, nodeOut));

    pore *p = network->tableOfPores[i].get();

    p->setRadius(radius);
    p->setShapeFactor(shapeFactor);
    p->setLength(poreLength);

    if (p->getNodeIn() != nullptr && p->getNodeOut() != nullptr) {
      double length = sqrt(pow(p->getNodeIn()->getXCoordinate() -
                                   p->getNodeOut()->getXCoordinate(),
                               2) +
                           pow(p->getNodeIn()->getYCoordinate() -
                                   p->getNodeOut()->getYCoordinate(),
                               2) +
                           pow(p->getNodeIn()->getZCoordinate() -
                                   p->getNodeOut()->getZCoordinate(),
                               2));
      p->setFullLength(length);
    }

    if (p->getNodeOut() == nullptr) {
      p->setInlet(true);
      network->inletPores.push_back(p);
    }
    if (p->getNodeIn() == nullptr) {
      p->setOutlet(true);
      network->outletPores.push_back(p);
    }
  }
  signalProgress(30);
}

void statoilNetworkBuilder::importLink2() {
  std::string filePath = userInput::get().extractedNetworkFolderPath +
                         userInput::get().rockPrefix + "_link2.dat";

  std::cout << "Importing data from " << QDir().absolutePath().toStdString()
            << "/" << filePath << " ..." << std::endl;

  std::ifstream file(filePath.c_str());
  if (!file.good()) throw std::domain_error("Missing data file. Aborting. \n");

  for (pore *p : pnmRange<pore>(network)) {
    double throatLength, volume;
    std::string dummy;

    file >> dummy >> dummy >> dummy >> dummy >> dummy >> throatLength >>
        volume >> dummy;

    p->setLength(throatLength);
    p->setVolume(volume);
    p->setEffectiveVolume(volume);
  }

  signalProgress(40);
}

}  // namespace PNM
