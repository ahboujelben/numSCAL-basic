/////////////////////////////////////////////////////////////////////////////
/// Name:        misc.cpp
/// Purpose:     network utility functions are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

#include "randomGenerator.h"

namespace PNM {

void network::assignViscosities()
{
    for_each(accessibleElements.begin(),accessibleElements.end(),[this](element* e){
        if(e->getPhaseFlag()==phase::oil){
            e->setViscosity(oilViscosity);
        }
        if(e->getPhaseFlag()==phase::water){
            e->setViscosity(waterViscosity);
        }
    });
}

void network::fillWithPhase(PNM::phase phase, double saturation, int distribution, PNM::phase otherPhase)
{
    if(saturation==1){
        for_each(accessibleElements.begin(),accessibleElements.end(),[this,phase](element* e){
            e->setPhaseFlag(phase);
        });
        return;
    }

    for_each(accessibleElements.begin(),accessibleElements.end(),[this,otherPhase](element* e){
        e->setPhaseFlag(otherPhase);
    });

    if(saturation==0){
        return;
    }

    if(distribution==1) //random
    {
        vector<node*> shuffledNodes;
        shuffledNodes.reserve(totalOpenedNodes);
        for(node* n : accessibleNodes)
            shuffledNodes.push_back(n);
        shuffle(shuffledNodes.begin(), shuffledNodes.end(), randomGenerator(seed).getGen());

        auto  actualWaterVolume(0.0);
        while((actualWaterVolume/totalNodesVolume)<saturation)
        {
            node* p=shuffledNodes.back();
            shuffledNodes.pop_back();
            if(p->getPhaseFlag()!=phase){
                p->setPhaseFlag(phase);
                actualWaterVolume+=p->getVolume();
            }
        }
    }

    if(distribution==2) //phase in biggest elements
    {
        auto workingElements=accessibleNodes;

        sort(workingElements.begin(),workingElements.end(), [this](node* e1, node* e2){
            return e1->getRadius()>e2->getRadius();
        });

        auto  actualWaterVolume(0.0);

        while(actualWaterVolume/totalElementsVolume<saturation){
            auto biggestElement=workingElements.back();
            biggestElement->setPhaseFlag(phase);
            actualWaterVolume+=biggestElement->getVolume();
            workingElements.pop_back();
        }
    }

    if(distribution==3) //phase in smallest elements
    {
        auto workingElements=accessibleNodes;

        sort(workingElements.begin(),workingElements.end(), [this](node* e1, node* e2){
            return e1->getRadius()<e2->getRadius();
        });

        auto  actualWaterVolume(0.0);

        while(actualWaterVolume/totalElementsVolume<saturation){
            auto smallestElement=workingElements.back();
            smallestElement->setPhaseFlag(phase);
            actualWaterVolume+=smallestElement->getVolume();
            workingElements.pop_back();
        }
    }

    for_each(accessiblePores.begin(),accessiblePores.end(),[this](pore* p){
        if(p->getNodeIn()==0){
            auto connectedNode=p->getNodeOut();
            p->setPhaseFlag(connectedNode->getPhaseFlag());
        }
        else if (p->getNodeOut()==0){
            auto connectedNode=p->getNodeIn();
            p->setPhaseFlag(connectedNode->getPhaseFlag());
        }
        else{
            auto connectedNode1=p->getNodeIn();
            auto connectedNode2=p->getNodeOut();
            if(connectedNode1->getPhaseFlag()==connectedNode2->getPhaseFlag()){
                p->setPhaseFlag(connectedNode1->getPhaseFlag());
            }
            else{
                randomGenerator g(seed);
                p->setPhaseFlag(g.uniform_int()?connectedNode1->getPhaseFlag():connectedNode2->getPhaseFlag());
            }
        }
    });
}

void network::initialiseCapillaries()
{
    for_each(accessibleElements.begin(),accessibleElements.end(),[this](element* e){
        e->setOilFraction(e->getPhaseFlag()==phase::oil?1:0);
        e->setWaterFraction(e->getPhaseFlag()==phase::water?1:0);
        e->setConcentration(0);
        e->setEffectiveVolume(e->getVolume());
        e->setOilFilmVolume(0);
        e->setWaterFilmVolume(0);
        e->setOilFilmConductivity(0);
        e->setWaterFilmConductivity(0);
        e->setOilLayerActivated(false);
        e->setWaterCornerActivated(false);
        e->setActive(true);
        e->setConductivity(0);
        e->setFlow(0);
        e->setMassFlow(0);
        if(e->getType()==capillaryType::throat){
            pore* p=static_cast<pore*>(e);
            p->setNodeInOil(p->getPhaseFlag()==phase::oil?true:false);
            p->setNodeOutOil(p->getPhaseFlag()==phase::oil?true:false);
            p->setNodeInWater(p->getPhaseFlag()==phase::water?true:false);
            p->setNodeOutWater(p->getPhaseFlag()==phase::water?true:false);
            p->setCapillaryPressure(0);
        }
    });
}

double network::getOutletFlow()
{
    auto Q(0.0);
    for_each(outletPores.begin(),outletPores.end(),[this,&Q](pore* e){
        Q+=e->getFlow();
    });
    return Q;
}

double network::getWaterSaturation()
{
    auto volume(0.0);
    for_each(accessibleElements.begin(),accessibleElements.end(),[this, &volume](element* e){
        volume+=e->getWaterFraction()*e->getVolume();
    });
    return volume/totalElementsVolume;
}

double network::getWaterSaturationWithFilms()
{
    auto volume(0.0);
    for_each(accessibleElements.begin(),accessibleElements.end(),[this, &volume](element* e){
        if(e->getPhaseFlag()==phase::oil){
            volume+=e->getWaterFilmVolume();
        }
        if(e->getPhaseFlag()==phase::water){
            if(e->getWettabilityFlag()==wettability::oilWet){
                volume+=e->getEffectiveVolume()+e->getWaterFilmVolume();
            }
            if(e->getWettabilityFlag()==wettability::waterWet){
                volume+=e->getVolume();
            }
        }
    });
    return volume/totalElementsVolume;
}

// Postprocessing
void network::extractVideo()
{
    if(videoRecording)
    {
        record=false;
        tools::renderVideo();
        tools::cleanVideosFolder();
    }
}

//signals
void network::emitPlotSignal()
{
    emit plot();
}

}
