/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef PNMSOLVER_H
#define PNMSOLVER_H

#include <memory>

namespace PNM
{

class networkModel;

class pnmSolver
{
  public:
    static pnmSolver &get(std::shared_ptr<networkModel>);
    double solvePressuresConstantGradient(double pressureIn = 1, double pressureOut = 0, bool defaultSolver = false);
    double solvePressuresConstantFlowRate();
    double updateFlowsConstantGradient(double pressureIn = 1, double pressureOut = 0);
    double updateFlowsConstantFlowRate();
    double getDeltaP();
    void calculatePermeabilityAndPorosity();
    std::pair<double, double> calculateRelativePermeabilities();

  protected:
    pnmSolver() {}
    ~pnmSolver() {}
    pnmSolver(const pnmSolver &) = delete;
    pnmSolver(pnmSolver &&) = delete;
    auto operator=(const pnmSolver &) -> pnmSolver & = delete;
    auto operator=(pnmSolver &&) -> pnmSolver & = delete;

    std::shared_ptr<networkModel> network;
    static pnmSolver instance;
};

} // namespace PNM

#endif // PNMSOLVER_H
