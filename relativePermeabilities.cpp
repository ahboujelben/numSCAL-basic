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
#include "iterator.h"

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

        for (pore* p: networkRange<pore*>(this))
        {
            p->setActive(true);
            if(p->getPhaseFlag()==phase::water)
            {
                if(p->getWettabilityFlag()==wettability::waterWet)
                    p->setActive(false);
                if(p->getWettabilityFlag()==wettability::oilWet)
                {
                    if(p->getOilLayerActivated() && p->getOilFilmVolume()>0 && p->getClusterOilFilm()->getSpanning())
                        p->setConductivity(max(1e-200,min(p->getOilFilmConductivity()/filmConductanceResistivity,p->getConductivity()/10.)));
                    else
                        p->setActive(false);
                }
            }
            if(p->getPhaseFlag()==phase::oil && !p->getClusterOilFilm()->getSpanning())
                p->setActive(false);
            if(p->getPhaseFlag()==phase::oil && p->getClusterOilFilm()->getSpanning())
                p->setConductivity(max(1e-200,p->getConductivity()*pow(p->getEffectiveVolume()/p->getVolume(),2)));
        }

        solvePressures();
        oilFlow=updateFlows();
    }

    clusterWaterFlowingElements();

    if (isWaterSpanning)
    {
        assignViscosities();
        assignConductivities();

        for (pore* p: networkRange<pore*>(this))
        {
            p->setActive(true);
            if(p->getPhaseFlag()==phase::oil)
            {
                {
                    if(p->getWaterCornerActivated() && p->getWaterFilmVolume()>0 && p->getClusterWaterFilm()->getSpanning())
                        p->setConductivity(max(1e-200,min(p->getWaterFilmConductivity()/filmConductanceResistivity,p->getConductivity()/10.)));
                    else
                        p->setActive(false);
                }
            }
            if(p->getPhaseFlag()==phase::water && !p->getClusterWaterFilm()->getSpanning())
               p->setActive(false);
            if(p->getPhaseFlag()==phase::water && p->getClusterWaterFilm()->getSpanning())
               p->setConductivity(max(1e-200,p->getConductivity()*pow(p->getEffectiveVolume()/p->getVolume(),2)));

        }

        solvePressures();
        waterFlow=updateFlows();
    }

    oilRelativePermeability=oilFlow*oilViscosity/flow;
    waterRelativePermeability=waterFlow*waterViscosity/flow;
}

}
