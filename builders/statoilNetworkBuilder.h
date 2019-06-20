/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef STATOILNETWORKBUILDER_H
#define STATOILNETWORKBUILDER_H

#include "regularNetworkBuilder.h"

namespace PNM {

class statoilNetworkBuilder : public regularNetworkBuilder {
  Q_OBJECT
 public:
  statoilNetworkBuilder() {}
  statoilNetworkBuilder(const statoilNetworkBuilder &) = delete;
  statoilNetworkBuilder(statoilNetworkBuilder &&) = delete;
  auto operator=(const statoilNetworkBuilder &)
      -> statoilNetworkBuilder & = delete;
  auto operator=(statoilNetworkBuilder &&) -> statoilNetworkBuilder & = delete;
  void make() override;
  std::string getNotification() override;

 protected:
  void initiateNetworkProperties() override;
  void importNode1();
  void importNode2();
  void importLink1();
  void importLink2();
};

}  // namespace PNM

#endif  // STATOILNETWORKBUILDER_H
