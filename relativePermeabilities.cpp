/////////////////////////////////////////////////////////////////////////////
/// Name:        solver.cpp
/// Purpose:     methods related to the calculation of relative permeabilties
///              are defined.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

namespace PNM {

void network::calculateRelativePermeabilities()
{
    waterRelativePermeability=0;
    oilRelativePermeability=0;

    pressureIn=1;
    pressureOut=0;

    double waterFlow(0),oilFlow(0);

    clusterOilFlowingElements();

    if (isOilSpanning)
    {
        assignViscosities();
        assignConductivities();

        for (pore* p: accessiblePores)
        {
            if(p->getPhaseFlag()==phase::water)
            {
                if(p->getWettabilityFlag()==wettability::waterWet)
                    p->setConductivity(1e-200);
                if(p->getWettabilityFlag()==wettability::oilWet)
                {
                    if(p->getOilLayerActivated() && p->getOilFilmVolume()>0 && p->getClusterOilFilm()->getSpanning())
                        p->setConductivity(max(1e-200,min(p->getOilFilmConductivity()/filmConductanceResistivity,p->getConductivity()/10.)));
                    else
                        p->setConductivity(1e-200);
                }
            }
            if(p->getPhaseFlag()==phase::oil && !p->getClusterOilFilm()->getSpanning())
               p->setConductivity(1e-200);
            if(p->getPhaseFlag()==phase::oil && p->getClusterOilFilm()->getSpanning())
                p->setConductivity(max(1e-200,p->getConductivity()*pow(p->getEffectiveVolume()/p->getVolume(),2)));
        }

        solvePressures();
        updateFlows();

        for (pore* p: accessiblePores)
            if(p->getOutlet())
                oilFlow+=p->getFlow();

    }

    clusterWaterFlowingElements();

    if (isWaterSpanning)
    {
        assignViscosities();
        assignConductivities();

        for (pore* p: accessiblePores)
        {
            if(p->getPhaseFlag()==phase::oil)
            {
                {
                    if(p->getWaterCornerActivated() && p->getWaterFilmVolume()>0 && p->getClusterWaterFilm()->getSpanning())
                       p->setConductivity(max(1e-200,min(p->getWaterFilmConductivity()/filmConductanceResistivity,p->getConductivity()/10.)));
                    else
                        p->setConductivity(1e-200);
                }
            }
            if(p->getPhaseFlag()==phase::water && !p->getClusterWaterFilm()->getSpanning())
               p->setConductivity(1e-200);
            if(p->getPhaseFlag()==phase::water && p->getClusterWaterFilm()->getSpanning())
               p->setConductivity(max(1e-200,p->getConductivity()*pow(p->getEffectiveVolume()/p->getVolume(),2)));

        }

        solvePressures();
        updateFlows();

        for (pore* p: accessiblePores)
            if(p->getOutlet())
                waterFlow+=p->getFlow();
    }

    oilRelativePermeability=oilFlow*oilViscosity/flow;
    waterRelativePermeability=waterFlow*waterViscosity/flow;
}

void network::calculateRelativePermeabilitiesUSS()
{
    waterRelativePermeability=0;
    oilRelativePermeability=0;

    pressureIn=1;
    pressureOut=0;

    double waterFlow(0),oilFlow(0);

    clusterOilElements();
    if (isOilSpanning)
    {
        assignViscositiesWithMixedFluids();
        assignConductivities();
        for (pore* p :accessiblePores)
        {
            if(p->getPhaseFlag()==phase::water)
               p->setConductivity(1e-200);
            if(p->getPhaseFlag()==phase::oil && !p->getClusterOil()->getSpanning())
               p->setConductivity(1e-200);
        }
        solvePressures();
        updateFlows();
        for (pore* p :accessiblePores)
            if(p->getOutlet())
                oilFlow+=p->getFlow();
    }

    clusterWaterElements();
    if (isWaterSpanning)
    {
        assignViscositiesWithMixedFluids();
        assignConductivities();
        for (pore* p :accessiblePores)
        {
            if(p->getPhaseFlag()==phase::oil)
               p->setConductivity(1e-200);
            if(p->getPhaseFlag()==phase::water && !p->getClusterWater()->getSpanning())
               p->setConductivity(1e-200);
        }
        solvePressures();
        updateFlows();
        for (pore* p :accessiblePores)
            if(p->getOutlet())
                waterFlow+=p->getFlow();

    }

    oilRelativePermeability=oilFlow*oilViscosity/flow;
    waterRelativePermeability=waterFlow*waterViscosity/flow;
}

}
