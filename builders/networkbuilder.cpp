/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "networkbuilder.h"
#include "regularNetworkBuilder.h"
#include "statoilNetworkBuilder.h"
#include "numscalNetworkBuilder.h"
#include "operations/pnmOperation.h"
#include "misc/userInput.h"
#include "misc/tools.h"

#include <iostream>

namespace PNM
{

std::shared_ptr<networkBuilder> networkBuilder::createBuilder()
{
    if (userInput::get().networkRegular)
        return std::make_shared<regularNetworkBuilder>();
    if (userInput::get().networkStatoil)
        return std::make_shared<statoilNetworkBuilder>();
    if (userInput::get().networkNumscal)
        return std::make_shared<numscalNetworkBuilder>();
}

std::shared_ptr<networkModel> networkBuilder::build()
{
    std::cout << "########## Building network ##########" << std::endl;

    //Create necessary folders
    initialise();

    //Make the network : a virtual function to be redefined in each subclass
    make();

    //Output properties
    finalise();

    std::cout << "########## Network Built ##########\n"
              << std::endl;

    return network;
}

std::shared_ptr<networkModel> networkBuilder::getNetwork() const
{
    return network;
}

void networkBuilder::initialise()
{
    tools::createRequiredFolders();
}

void networkBuilder::finalise()
{
    pnmOperation::get(network).exportToNumcalFormat();
    emit finished();
}

void networkBuilder::signalProgress(int _progress)
{
    progress = _progress;
    emit notifyGUI();
}

int networkBuilder::getProgress()
{
    return progress;
}

} // namespace PNM
