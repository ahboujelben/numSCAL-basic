/////////////////////////////////////////////////////////////////////////////
/// Name:        films.cpp
/// Purpose:     methods related to film calculations (half angles, volumes,
///              conductivities) are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

#include "randomGenerator.h"

namespace PNM {

void network::assignHalfAngles()
{
    for (element* p : accessibleElements)
    {
        double beta1(0), beta2(0),beta3(0);
        randomGenerator gen(seed);
        if(p->getShapeFactor()<=sqrt(3)/36.)
        {
            double beta2Min=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3+4*pi()/3));
            double beta2Max=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3));
            beta2=gen.uniform_real(beta2Min,beta2Max);
            beta1=-0.5*beta2+0.5*asin((tan(beta2)+4*p->getShapeFactor())/(tan(beta2)-4*p->getShapeFactor())*sin(beta2));
            beta3=pi()/2-beta1-beta2;
        }
        p->setBeta1(beta1);
        p->setBeta2(beta2);
        p->setBeta3(beta3);
    }
}

void network::assignFilmStability()
{
    for (element* p : accessibleElements)
    {
        bool oilCanFlowViaFilm(false), waterCanFlowViaFilm(false);
        if(p->getShapeFactor()<=sqrt(3)/36.)
        {
            double theta=p->getTheta();
            if(p->getWettabilityFlag()==wettability::waterWet)
            {
                double sum(0);
                if(theta<pi()/2-p->getBeta1())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-pi()/2*(1-(theta+p->getBeta1())/(pi()/2));
                }
                if(theta<pi()/2-p->getBeta2())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-pi()/2*(1-(theta+p->getBeta2())/(pi()/2));
                }
                if(theta<pi()/2-p->getBeta3())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-pi()/2*(1-(theta+p->getBeta3())/(pi()/2));
                }
                p->setFilmAreaCoefficient(sum);
            }

            if(p->getWettabilityFlag()==wettability::oilWet)
            {
                double sum(0);
                if(pi()-theta<pi()/2-p->getBeta1())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta1())/sin(p->getBeta1())-pi()/2*(-1+(theta-p->getBeta1())/(pi()/2));
                }
                if(pi()-theta<pi()/2-p->getBeta2())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta2())/sin(p->getBeta2())-pi()/2*(-1+(theta-p->getBeta2())/(pi()/2));
                }
                if(pi()-theta<pi()/2-p->getBeta3())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta3())/sin(p->getBeta3())-pi()/2*(-1+(theta-p->getBeta3())/(pi()/2));
                }
                p->setFilmAreaCoefficient(sum);
            }
        }
        p->setOilCanFlowViaFilm(oilCanFlowViaFilm);
        p->setWaterCanFlowViaFilm(waterCanFlowViaFilm);
    }
}

void network::assignWettability()
{
    randomGenerator gen(seed);
    if(wettingTypeFlag==2){ //OW
        for_each(accessibleElements.begin(),accessibleElements.end(),[this, &gen](element* e){
            e->setTheta(gen.uniform_real(minOilWetTheta,maxOilWetTheta));
            e->setWettabilityFlag(wettability::oilWet);
        });
        backupWettability();
        return;
    }

    for_each(accessibleElements.begin(),accessibleElements.end(),[this, &gen](element* e){
        e->setTheta(gen.uniform_real(minWaterWetTheta,maxWaterWetTheta));
        e->setWettabilityFlag(wettability::waterWet);
    });

    if(wettingTypeFlag==1){ //WW
        backupWettability();
        return;
    }

    if(wettingTypeFlag==3){ //FW

        vector<node*> shuffledNodes;
        shuffledNodes.reserve(totalOpenedNodes);
        for(node* n : accessibleNodes)
            shuffledNodes.push_back(n);
        shuffle(shuffledNodes.begin(), shuffledNodes.end(), gen.getGen());

        auto oilWetSoFar(0);
        while((double(oilWetSoFar)/totalOpenedNodes)<oilWetFraction)
        {
            node* p=shuffledNodes.back();
            shuffledNodes.pop_back();
            if(p->getWettabilityFlag()!=wettability::oilWet){
                p->setTheta(gen.uniform_real(minOilWetTheta,maxOilWetTheta));
                p->setWettabilityFlag(wettability::oilWet);
                oilWetSoFar++;
            }
        }
    }

    if(wettingTypeFlag==4){ //MWL
        auto workingElements=accessibleNodes;

        sort(workingElements.begin(),workingElements.end(), [this](node* e1, node* e2){
            return e1->getRadius()>e2->getRadius();
        });

        auto oilWetSoFar(0);

        while((double(oilWetSoFar)/totalOpenedNodes)<oilWetFraction){
            auto biggestElement=workingElements.back();
            biggestElement->setTheta(gen.uniform_real(minOilWetTheta,maxOilWetTheta));
            biggestElement->setWettabilityFlag(wettability::oilWet);
            oilWetSoFar++;
            workingElements.pop_back();
        }
    }

    if(wettingTypeFlag==5) //MWS
    {
        auto workingElements=accessibleNodes;

        sort(workingElements.begin(),workingElements.end(), [this](node* e1, node* e2){
            return e1->getRadius()<e2->getRadius();
        });

        auto oilWetSoFar(0);

        while((double(oilWetSoFar)/totalOpenedNodes)<oilWetFraction){
            auto smallestElement=workingElements.back();
            smallestElement->setTheta(gen.uniform_real(minOilWetTheta,maxOilWetTheta));
            smallestElement->setWettabilityFlag(wettability::oilWet);
            oilWetSoFar++;
            workingElements.pop_back();
        }
    }

    for_each(accessiblePores.begin(),accessiblePores.end(),[this, &gen](pore* p){
        if(p->getNodeIn()==0){
            auto connectedNode=p->getNodeOut();
            p->setTheta(connectedNode->getTheta());
            p->setWettabilityFlag(connectedNode->getWettabilityFlag());
        }
        else if (p->getNodeOut()==0){
            auto connectedNode=p->getNodeIn();
            p->setTheta(connectedNode->getTheta());
            p->setWettabilityFlag(connectedNode->getWettabilityFlag());
        }
        else{
            auto connectedNode1=p->getNodeIn();
            auto connectedNode2=p->getNodeOut();
            if(connectedNode1->getWettabilityFlag()==connectedNode2->getWettabilityFlag()){
                p->setTheta((connectedNode1->getTheta()+connectedNode2->getTheta())/2);
                p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
            }
            else{
                p->setTheta(gen.uniform_int()?connectedNode1->getTheta():connectedNode2->getTheta());
                if(p->getTheta()>pi()/2)
                    p->setWettabilityFlag(wettability::oilWet);
            }
        }
    });

    backupWettability();
}

void network::backupWettability()
{
    for_each(accessibleElements.begin(),accessibleElements.end(),[this](element* e){
        e->setOriginalTheta(e->getTheta());
    });
}

void network::assignWWWettability(double theta)
{
    for_each(accessibleElements.begin(),accessibleElements.end(),[=,this](element* e){
        e->setTheta(theta);
        e->setWettabilityFlag(wettability::waterWet);
    });

    clusterWaterWetElements();
    clusterOilWetElements();
}

void network::restoreWettability()
{
    for_each(accessibleElements.begin(),accessibleElements.end(),[this](element* e){
        if(e->getPhaseFlag()==phase::oil){
            e->setTheta(e->getOriginalTheta());
            if(e->getTheta()<=pi()/2){
                e->setWettabilityFlag(wettability::waterWet);
            }
            else{
                e->setWettabilityFlag(wettability::oilWet);
                e->setWaterCornerActivated(false);
            }
        }
    });

    clusterWaterWetElements();
    clusterOilWetElements();
}

}
