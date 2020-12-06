/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#include "numscalNetworkBuilder.h"
#include "libs/csvParser/csv.h"
#include "misc/maths.h"
#include "misc/userInput.h"
#include "network/iterator.h"
#include "network/networkmodel.h"

#include <QDir>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace PNM {

void numscalNetworkBuilder::make() {
  initiateNetworkProperties();
  importNodes();
  importPores();
  assignNeighboors();
  cleanNetwork();
  assignMissingValues();
  assignShapeFactorConstants();
  assignVolumes();
  assignWettabilities();
  calculateProperties();
}

std::string PNM::numscalNetworkBuilder::getNotification() {
  std::ostringstream ss;
  ss << userInput::get().rockPrefix
     << " numSCAL Network / Total Nodes: " << network->totalNodes
     << " / Total Throats: " << network->totalPores << "\n"
     << std::fixed << std::setprecision(2)
     << "Perm.(mD): " << network->absolutePermeability / 0.987e-15
     << " / Porosity(%): " << network->porosity * 100
     << " / Dx(mm): " << network->xEdgeLength * 1e3
     << " / Dy(mm): " << network->yEdgeLength * 1e3
     << " / Dz(mm): " << network->zEdgeLength * 1e3;

  return ss.str();
}

void numscalNetworkBuilder::initiateNetworkProperties() {
  network = std::make_shared<networkModel>();
  network->maxConnectionNumber = 0;
}

void numscalNetworkBuilder::importNodes() {
  std::string filePath = userInput::get().extractedNetworkFolderPath +
                         userInput::get().rockPrefix + "_nodes.num";

  std::cout << "Importing data from " << QDir().absolutePath().toStdString()
            << "/" << filePath << " ..." << std::endl;

  double x, y, z, radius, length(-1), shapeFactor(0.03);
  int id(0);
  network->totalNodes = 0;
  network->xEdgeLength = 0;
  network->yEdgeLength = 0;
  network->zEdgeLength = 0;

  io::CSVReader<6> in(filePath);
  in.read_header(io::ignore_missing_column, "x(um)", "y(um)", "z(um)",
                 "radius(um)", "length(um)", "shapeFactor");
  while (in.read_row(x, y, z, radius, length, shapeFactor)) {
    x *= 1e-6;
    y *= 1e-6;
    z *= 1e-6;
    radius *= 1e-6;
    length *= 1e-6;

    auto n = std::make_shared<node>(x, y, z);
    network->tableOfNodes.push_back(n);

    n->setRadius(radius);
    n->setLength(length);
    n->setShapeFactor(shapeFactor);
    n->setId(++id);

    network->totalNodes++;

    if (x > network->xEdgeLength) network->xEdgeLength = x;
    if (y > network->yEdgeLength) network->yEdgeLength = y;
    if (z > network->zEdgeLength) network->zEdgeLength = z;
  }

  signalProgress(20);
}

void numscalNetworkBuilder::importPores() {
  std::string filePath = userInput::get().extractedNetworkFolderPath +
                         userInput::get().rockPrefix + "_throats.num";

  std::cout << "Importing data from " << QDir().absolutePath().toStdString()
            << "/" << filePath << " ..." << std::endl;

  int nodeInId, nodeOutId;
  double radius, length(-1), shapeFactor(0.03);
  network->totalPores = 0;

  io::CSVReader<5> in(filePath);
  in.read_header(io::ignore_missing_column, "nodeIn(id)", "nodeOut(id)",
                 "radius(um)", "length(um)", "shapeFactor");
  while (in.read_row(nodeInId, nodeOutId, radius, length, shapeFactor)) {
    radius *= 1e-6;
    length *= 1e-6;

    auto nodeIn = nodeInId == 0 ? nullptr : network->getNode(nodeInId - 1);
    auto nodeOut = nodeOutId == 0 ? nullptr : network->getNode(nodeOutId - 1);

    auto p = std::make_shared<pore>(nodeIn, nodeOut);

    network->tableOfPores.push_back(p);

    p->setRadius(radius);
    p->setLength(length);
    p->setShapeFactor(shapeFactor);

    if (p->getNodeOut() == nullptr) {
      p->setInlet(true);
      p->getNodeIn()->setInlet(true);
      network->inletPores.push_back(p.get());
    }

    if (p->getNodeIn() == nullptr) {
      p->setOutlet(true);
      p->getNodeOut()->setOutlet(true);
      network->outletPores.push_back(p.get());
    }

    network->totalPores++;
  }
  signalProgress(40);
}

void numscalNetworkBuilder::assignMissingValues() {
  for (node *n : pnmRange<node>(network)) {
    if (n->getLength() == -1) n->setLength(2 * n->getRadius());
  }

  for (pore *p : pnmRange<pore>(network)) {
    if (p->getNodeIn() == nullptr || p->getNodeOut() == nullptr) continue;

    double length = std::sqrt(std::pow(p->getNodeIn()->getXCoordinate() -
                                           p->getNodeOut()->getXCoordinate(),
                                       2) +
                              std::pow(p->getNodeIn()->getYCoordinate() -
                                           p->getNodeOut()->getYCoordinate(),
                                       2) +
                              std::pow(p->getNodeIn()->getZCoordinate() -
                                           p->getNodeOut()->getZCoordinate(),
                                       2));
    p->setFullLength(length);

    if (p->getLength() == -1) {
      double nodeInLength =
          p->getNodeIn() == nullptr ? 0 : p->getNodeIn()->getRadius();
      double nodeOutLength =
          p->getNodeOut() == nullptr ? 0 : p->getNodeOut()->getRadius();
      double newlength = length - nodeInLength - nodeOutLength;
      p->setLength(newlength > 0 ? newlength : length / 2);
    }
  }

  double averageLength = computeAverageLength();

  for (pore *p : pnmRange<pore>(network)) {
    if (p->getNodeIn() == nullptr || p->getNodeOut() == nullptr)
      if (p->getLength() == -1) p->setLength(averageLength);
  }

  network->is2D = network->zEdgeLength == 0 ? true : false;

  if (network->is2D) network->zEdgeLength = averageLength;
}

double numscalNetworkBuilder::computeAverageLength() {
  double averageLength(0), poresNumber(0);
  for (pore *p : pnmRange<pore>(network)) {
    if (p->getNodeIn() == nullptr || p->getNodeOut() == nullptr) continue;

    averageLength += p->getFullLength();
    poresNumber++;
  }

  averageLength /= poresNumber;

  return averageLength;
}

}  // namespace PNM
