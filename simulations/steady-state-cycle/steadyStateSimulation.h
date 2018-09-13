/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef STEADYSTATESIMULATION_H
#define STEADYSTATESIMULATION_H

#include "simulations/simulation.h"

#include "memory"

namespace PNM
{
class steadyStateSimulation : public simulation
{
    Q_OBJECT
  public:
    steadyStateSimulation() {}
    ~steadyStateSimulation() {}
    steadyStateSimulation(const steadyStateSimulation &) = delete;
    steadyStateSimulation(steadyStateSimulation &&) = delete;
    auto operator=(const steadyStateSimulation &) -> steadyStateSimulation & = delete;
    auto operator=(steadyStateSimulation &&) -> steadyStateSimulation & = delete;
    virtual void run() override;
    virtual std::string getNotification() override;
    virtual int getProgress() override;
    virtual void interrupt() override;

  private:
    void runCurrentSimulation();
    std::shared_ptr<simulation> currentSimulation;
};

} // namespace PNM

#endif // STEADYSTATESIMULATION_H
