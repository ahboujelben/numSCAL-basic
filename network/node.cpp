/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "node.h"

namespace PNM
{

node::node(double X, double Y, double Z)
{
    type = capillaryType::poreBody;
    x = X;
    y = Y;
    z = Z;
    xCoordinate = X;
    yCoordinate = Y;
    zCoordinate = Z;
    connectionNumber = 6;
    pressure = 0;
    rank = 0;
}

} // namespace PNM
