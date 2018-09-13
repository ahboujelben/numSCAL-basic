/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "templateFlowSimulation.h"
#include "misc/userInput.h"

#include <sstream>
#include <iostream>
#include <iomanip>

namespace PNM
{

void templateFlowSimulation::run()
{
    //initialisation
    /////////////////////////////////////
    simulationTime = userInput::get().simulationTime;
    timeStep = 0.01;
    timeSoFar = 0;

    while (!simulationInterrupted && timeSoFar < simulationTime)
    {
        //algorithms
        ////////////////////////////////

        timeSoFar += timeStep;

        updateGUI();

        if (simulationInterrupted)
            break;
    }
}

std::string templateFlowSimulation::getNotification()
{
    std::ostringstream ss;
    ss << "Template Flow Simulation \n"
       << std::fixed << std::setprecision(2)
       << "Time: " << timeSoFar;
    return ss.str();
}

int templateFlowSimulation::getProgress()
{
    return timeSoFar / simulationTime * 100;
}

} // namespace PNM
