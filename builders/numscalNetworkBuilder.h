/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef NUMSCALNETWORKBUILDER_H
#define NUMSCALNETWORKBUILDER_H

#include "regularNetworkBuilder.h"

namespace PNM
{

class numscalNetworkBuilder : public regularNetworkBuilder
{
    Q_OBJECT
  public:
    numscalNetworkBuilder() {}
    ~numscalNetworkBuilder() {}
    numscalNetworkBuilder(const numscalNetworkBuilder &) = delete;
    numscalNetworkBuilder(numscalNetworkBuilder &&) = delete;
    auto operator=(const numscalNetworkBuilder &) -> numscalNetworkBuilder & = delete;
    auto operator=(numscalNetworkBuilder &&) -> numscalNetworkBuilder & = delete;
    void make() override;
    std::string getNotification() override;

  protected:
    void initiateNetworkProperties() override;
    void importNodes();
    void importPores();
    void assignMissingValues();
    double computeAverageLength();
};

} // namespace PNM

#endif // NUMSCALNETWORKBUILDER_H
