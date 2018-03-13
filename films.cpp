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

namespace PNM {

void network::assignHalfAngles()
{
    ofstream file("Results/halfAngles.txt");
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            double beta1(0), beta2(0),beta3(0);
            if(p->getShapeFactor()<=sqrt(3)/36.)
            {
                double beta2Min=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3+4*tools::pi()/3));
                double beta2Max=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3));
                beta2=uniform_real(beta2Min,beta2Max);
                beta1=-0.5*beta2+0.5*asin((tan(beta2)+4*p->getShapeFactor())/(tan(beta2)-4*p->getShapeFactor())*sin(beta2));
                beta3=tools::pi()/2-beta1-beta2;
            }
            p->setBeta1(beta1);
            p->setBeta2(beta2);
            p->setBeta3(beta3);
            file<<p->getId()<<" "<<p->getBeta1()<<" "<<p->getBeta2()<<" "<<p->getBeta3()<<endl;
        }
    }

    if(networkSource==2 || networkSource==3)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* p=getNode(i);
        if(!p->getClosed())
        {
            double beta1(0), beta2(0),beta3(0);
            if(p->getShapeFactor()<=sqrt(3)/36.)
            {
                double beta2Min=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3+4*tools::pi()/3));
                double beta2Max=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3));
                beta2=uniform_real(beta2Min,beta2Max);
                beta1=-0.5*beta2+0.5*asin((tan(beta2)+4*p->getShapeFactor())/(tan(beta2)-4*p->getShapeFactor())*sin(beta2));
                beta3=tools::pi()/2-beta1-beta2;
            }
            p->setBeta1(beta1);
            p->setBeta2(beta2);
            p->setBeta3(beta3);
            file<<p->getId()<<" "<<p->getBeta1()<<" "<<p->getBeta2()<<" "<<p->getBeta3()<<endl;
        }
    }
}

void network::assignFilmStability()
{
    for (pore* p: accessiblePores)
    {
        bool oilCanFlowViaFilm(false), waterCanFlowViaFilm(false);
        if(p->getShapeFactor()<=sqrt(3)/36.)
        {
            double theta=p->getTheta();
            if(p->getWettabilityFlag()==wettability::waterWet)
            {
                double sum(0);
                if(theta<tools::pi()/2-p->getBeta1())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(theta+p->getBeta1())/(tools::pi()/2));
                }
                if(theta<tools::pi()/2-p->getBeta2())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(theta+p->getBeta2())/(tools::pi()/2));
                }
                if(theta<tools::pi()/2-p->getBeta3())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(theta+p->getBeta3())/(tools::pi()/2));
                }
                p->setFilmAreaCoefficient(sum);
            }

            if(p->getWettabilityFlag()==wettability::oilWet)
            {
                double sum(0);
                if(tools::pi()-theta<tools::pi()/2-p->getBeta1())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(-1+(theta-p->getBeta1())/(tools::pi()/2));
                }
                if(tools::pi()-theta<tools::pi()/2-p->getBeta2())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(-1+(theta-p->getBeta2())/(tools::pi()/2));
                }
                if(tools::pi()-theta<tools::pi()/2-p->getBeta3())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(-1+(theta-p->getBeta3())/(tools::pi()/2));
                }
                p->setFilmAreaCoefficient(sum);
            }
        }
        p->setOilCanFlowViaFilm(oilCanFlowViaFilm);
        p->setWaterCanFlowViaFilm(waterCanFlowViaFilm);
    }

    for (node* p: accessibleNodes)
    {
        bool oilCanFlowViaFilm(false), waterCanFlowViaFilm(false);
        if(p->getShapeFactor()<=sqrt(3)/36.)
        {
            double theta=p->getTheta();
            if(p->getWettabilityFlag()==wettability::waterWet)
            {
                double sum(0);
                if(theta<tools::pi()/2-p->getBeta1())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(theta+p->getBeta1())/(tools::pi()/2));
                }
                if(theta<tools::pi()/2-p->getBeta2())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(theta+p->getBeta2())/(tools::pi()/2));
                }
                if(theta<tools::pi()/2-p->getBeta3())
                {
                    waterCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(theta+p->getBeta3())/(tools::pi()/2));
                }
                p->setFilmAreaCoefficient(sum);
            }

            if(p->getWettabilityFlag()==wettability::oilWet)
            {
                double sum(0);
                if(tools::pi()-theta<tools::pi()/2-p->getBeta1())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(-1+(theta-p->getBeta1())/(tools::pi()/2));
                }
                if(tools::pi()-theta<tools::pi()/2-p->getBeta2())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(-1+(theta-p->getBeta2())/(tools::pi()/2));
                }
                if(tools::pi()-theta<tools::pi()/2-p->getBeta3())
                {
                    oilCanFlowViaFilm=true;
                    sum+=cos(theta)*cos(theta-p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(-1+(theta-p->getBeta3())/(tools::pi()/2));
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
    if(wettingTypeFlag==1) //WW
    {
        for (int i = 0; i < totalElements; ++i)
        {
            element* e=getElement(i);
            if(!e->getClosed())
            {
                e->setTheta(uniform_real(minWaterWetTheta,maxWaterWetTheta));
                e->setWettabilityFlag(wettability::waterWet);
            }
        }
    }

    if(wettingTypeFlag==2) //OW
    {
        for (int i = 0; i < totalElements; ++i)
        {
            element* e=getElement(i);
            if(!e->getClosed())
            {
                e->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                e->setWettabilityFlag(wettability::oilWet);
            }
        }
    }

    if(wettingTypeFlag==3) //FW
    {
        for (int i = 0; i < totalElements; ++i)
        {
            element* p=getElement(i);
            if(!p->getClosed())
            {
                p->setTheta(uniform_real(minWaterWetTheta,maxWaterWetTheta));
                p->setWettabilityFlag(wettability::waterWet);
            }
        }

        int  oilWetSoFar=0;
        while((double(oilWetSoFar)/double(totalOpenedNodes))<oilWetFraction)
        {
            int index=uniform_int(0,totalNodes-1);
            node* p=getNode(index);
            if(!p->getClosed() && p->getWettabilityFlag()!=wettability::oilWet)
                {
                    p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                    p->setWettabilityFlag(wettability::oilWet);
                    oilWetSoFar++;
                }
        }

        for (int i = 0; i < totalPores; ++i)
        {
            pore* p=getPore(i);
            if(!p->getClosed())
            {
                if(p->getNeighboors().size()==1)
                {
                    element* connectedNode=p->getNeighboors()[0];
                    p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                    p->setTheta(connectedNode->getTheta());
                }
                else
                {
                    element* connectedNode1=p->getNeighboors()[0];
                    element* connectedNode2=p->getNeighboors()[1];
                    if(connectedNode1->getWettabilityFlag()==connectedNode2->getWettabilityFlag())
                    {
                        p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                        p->setTheta(connectedNode1->getTheta());
                    }
                    else
                    {
                        int dice=uniform_int();
                        if(dice==0)
                        {
                            p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                            p->setTheta(connectedNode1->getTheta());
                        }
                        else
                        {
                            p->setWettabilityFlag(connectedNode2->getWettabilityFlag());
                            p->setTheta(connectedNode2->getTheta());
                        }
                    }

                }
            }
        }
    }

    if(wettingTypeFlag==4) //MWS
        {
            for (int i = 0; i < totalElements; ++i)
            {
                element* p=getElement(i);
                if(!p->getClosed())
                {
                    p->setTheta(uniform_real(minWaterWetTheta,maxWaterWetTheta));
                    p->setWettabilityFlag(wettability::waterWet);
                }
            }

            int  oilWetSoFar=0;
            double currentRadius=minNodeRadius;
            double radiusStep=(maxNodeRadius-minNodeRadius)/100;

            while((double(oilWetSoFar)/double(totalOpenedNodes))<oilWetFraction)
            {
                currentRadius+=radiusStep;
                for (int i = 0; i < totalNodes; ++i)
                {
                    element* p=getNode(i);
                    if(!p->getClosed() && p->getRadius()<=currentRadius && p->getWettabilityFlag()!=wettability::oilWet)
                    {
                        p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                        p->setWettabilityFlag(wettability::oilWet);
                        oilWetSoFar++;
                    }
                }
            }

            for (int i = 0; i < totalPores; ++i)
            {
                pore* p=getPore(i);
                if(!p->getClosed())
                {
                    if(p->getNeighboors().size()==1)
                    {
                        element* connectedNode=p->getNeighboors()[0];
                        p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                        p->setTheta(connectedNode->getTheta());
                    }
                    else
                    {
                        element* connectedNode1=p->getNeighboors()[0];
                        element* connectedNode2=p->getNeighboors()[1];
                        if(connectedNode1->getWettabilityFlag()==connectedNode2->getWettabilityFlag())
                        {
                            p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                            p->setTheta(connectedNode1->getTheta());
                        }
                        else
                        {
                            int dice=uniform_int();
                            if(dice==0)
                            {
                                p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                                p->setTheta(connectedNode1->getTheta());
                            }
                            else
                            {
                                p->setWettabilityFlag(connectedNode2->getWettabilityFlag());
                                p->setTheta(connectedNode2->getTheta());
                            }
                        }

                    }
                }
            }
        }

        if(wettingTypeFlag==5) //MWL
        {
            for (int i = 0; i < totalElements; ++i)
            {
                element* p=getElement(i);
                if(!p->getClosed())
                {
                    p->setTheta(uniform_real(minWaterWetTheta,maxWaterWetTheta));
                    p->setWettabilityFlag(wettability::waterWet);
                }
            }

            int  oilWetSoFar=0;
            double currentRadius=maxNodeRadius;
            double radiusStep=(maxNodeRadius-minNodeRadius)/100;

            while((double(oilWetSoFar)/double(totalOpenedNodes))<oilWetFraction)
            {
                currentRadius-=radiusStep;
                for (int i = 0; i < totalNodes; ++i)
                {
                    element* p=getNode(i);
                    if(!p->getClosed() && p->getRadius()>=currentRadius && p->getWettabilityFlag()!=wettability::oilWet)
                    {
                        p->setTheta(uniform_real(minOilWetTheta,maxOilWetTheta));
                        p->setWettabilityFlag(wettability::oilWet);
                        oilWetSoFar++;
                    }
                }
            }

            for (int i = 0; i < totalPores; ++i)
            {
                pore* p=getPore(i);
                if(!p->getClosed())
                {
                    if(p->getNeighboors().size()==1)
                    {
                        element* connectedNode=p->getNeighboors()[0];
                        p->setWettabilityFlag(connectedNode->getWettabilityFlag());
                        p->setTheta(connectedNode->getTheta());
                    }
                    else
                    {
                        element* connectedNode1=p->getNeighboors()[0];
                        element* connectedNode2=p->getNeighboors()[1];
                        if(connectedNode1->getWettabilityFlag()==connectedNode2->getWettabilityFlag())
                        {
                            p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                            p->setTheta(connectedNode1->getTheta());
                        }
                        else
                        {
                            int dice=uniform_int();
                            if(dice==0)
                            {
                                p->setWettabilityFlag(connectedNode1->getWettabilityFlag());
                                p->setTheta(connectedNode1->getTheta());
                            }
                            else
                            {
                                p->setWettabilityFlag(connectedNode2->getWettabilityFlag());
                                p->setTheta(connectedNode2->getTheta());
                            }
                        }

                    }
                }
            }
        }
        backupWettabilityPT();
}

void network::backupWettabilityPT()
{
    for_each(accessibleElements.begin(),accessibleElements.end(),[this](element* e){
        e->setOriginalTheta(e->getTheta());
    });
}

void network::assignWWWettabilityPT(double theta)
{
    for_each(accessibleElements.begin(),accessibleElements.end(),[=,this](element* e){
        e->setTheta(theta);
        e->setWettabilityFlag(wettability::waterWet);
    });

    clusterWaterWetElements();
    clusterOilWetElements();
}

void network::restoreWettabilityPT()
{
    for_each(accessibleElements.begin(),accessibleElements.end(),[this](element* e){
        if(e->getPhaseFlag()==phase::oil){
            e->setTheta(e->getOriginalTheta());
            if(e->getTheta()<tools::pi()/2){
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
