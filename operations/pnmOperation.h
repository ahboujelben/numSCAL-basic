/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef PNMOPERATION_H
#define PNMOPERATION_H

#include <memory>

namespace PNM
{

class networkModel;
enum class phase;

class pnmOperation
{
  public:
    static pnmOperation &get(std::shared_ptr<networkModel>);
    void assignRadii();
    void assignLengths();
    void distortNetwork();
    void assignShapeFactors();
    void assignShapeFactorConstants();
    void assignVolumes();
    void assignViscosities();
    void assignConductivities();
    void calculateNetworkVolume();
    void assignHalfAngles();
    void assignFilmsStability();
    void assignWettabilities();
    void backupWettability();
    void restoreWettability();
    void assignWWWettability();
    void assignOilConductivities();
    void assignWaterConductivities();
    void setSwi();
    void fillWithWater();
    double getSw();
    double getFlow(phase);
    void exportToNumcalFormat();
    void generateNetworkState(int frame, std::string folderPath = "");

  protected:
    pnmOperation() {}
    ~pnmOperation() {}
    pnmOperation(const pnmOperation &) = delete;
    pnmOperation(pnmOperation &&) = delete;
    auto operator=(const pnmOperation &) -> pnmOperation & = delete;
    auto operator=(pnmOperation &&) -> pnmOperation & = delete;

    std::shared_ptr<networkModel> network;
    static pnmOperation instance;
};

} // namespace PNM

#endif // PNMOPERATION_H
