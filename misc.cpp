/////////////////////////////////////////////////////////////////////////////
/// Name:        misc.cpp
/// Purpose:     network utility functions are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

namespace PNM {

void network::assignViscositiesWithMixedFluids()
{
    for (element* e : accessibleElements)
         e->setViscosity(oilViscosity*e->getOilFraction()+waterViscosity*e->getWaterFraction());
}

void network::assignViscosities()
{
    for (element* e : accessibleElements)
    {
        switch(e->getPhaseFlag())
        {
            case 'o':
            {
                e->setViscosity(oilViscosity);
                break;
            }
            case 'w':
            {
                e->setViscosity(waterViscosity);
                break;
            }
        }
    }
}

void network::fillWithPhasePT(char phase, double saturation, int distribution, char otherPhase)
{
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed())
            p->setPhaseFlag(otherPhase);
    }

    if(distribution==1)
    {
        if(saturation>0 && saturation<1)
        {
            double  actualWaterVolume=0;
            while((actualWaterVolume/totalNodesVolume)<saturation)
            {
                int index=uniform_int(0,totalNodes-1);
                node* p=getNode(index);
                if(!p->getClosed() && p->getPhaseFlag()!=phase)
                {
                    p->setPhaseFlag(phase);
                    actualWaterVolume+=p->getVolume();
                }
            }
        }
    }

    if(distribution==2)
    {
        if(saturation>0 && saturation<1)
        {
            double  actualWaterVolume=0;
            double currentRadius=minNodeRadius;
            double radiusStep=(maxNodeRadius-minNodeRadius)/100;

            while((actualWaterVolume/totalNodesVolume)<saturation)
            {
                currentRadius+=radiusStep;
                for (int i = 0; i < totalNodes; ++i)
                {
                    node* p=getNode(i);
                    if(!p->getClosed() && p->getRadius()<=currentRadius && p->getPhaseFlag()!=phase)
                    {
                        p->setPhaseFlag(phase);
                        actualWaterVolume+=p->getVolume();
                    }
                }
            }
        }
    }

    if(distribution==3)
    {
        if(saturation>0 && saturation<1)
        {
            double actualWaterVolume=0;
            double currentRadius=maxNodeRadius;
            double radiusStep=(maxNodeRadius-minNodeRadius)/100;

            while((actualWaterVolume/totalNodesVolume)<saturation)
            {
                currentRadius-=radiusStep;
                for (int i = 0; i < totalNodes; ++i)
                {
                    node* p=getNode(i);
                    if(!p->getClosed() && p->getRadius()>=currentRadius && p->getPhaseFlag()!=phase)
                    {
                        p->setPhaseFlag(phase);
                        actualWaterVolume+=p->getVolume();
                    }
                }
            }
        }
    }

    if(saturation==1)
    {
        for(int i=0;i<totalElements;++i)
        {
            element* e=getElement(i);
            if(!e->getClosed())
                e->setPhaseFlag(phase);
        }
    }

    if(saturation!=0 && saturation!=1)
        for (int i = 0; i < totalPores; ++i)
        {
            pore* p=getPore(i);
            if(!p->getClosed())
            {
                if(p->getNeighboors().size()==1)
                {
                    element* connectedNode=p->getNeighboors()[0];
                    p->setPhaseFlag(connectedNode->getPhaseFlag());
                }
                else
                {
                    element* connectedNode1=p->getNeighboors()[0];
                    element* connectedNode2=p->getNeighboors()[1];
                    if(connectedNode1->getWettabilityFlag()==connectedNode2->getWettabilityFlag())
                    {
                        p->setPhaseFlag(connectedNode1->getPhaseFlag());
                    }
                    else
                    {
                        int dice=uniform_int();
                        if(dice==0)
                        {
                            p->setPhaseFlag(connectedNode1->getPhaseFlag());
                        }
                        else
                        {
                            p->setPhaseFlag(connectedNode2->getPhaseFlag());
                        }
                    }

                }
            }
        }

    initialiseCapillaries();
}

void network::initialiseCapillaries()
{
    for(pore* p : accessiblePores)
    {
        if(p->getPhaseFlag()=='w')
        {
            p->setNodeInOil(false);
            p->setNodeOutOil(false);
            p->setNodeInWater(true);
            p->setNodeOutWater(true);

            p->setOilFraction(0.0);
            p->setWaterFraction(1);

            p->setOldOilFraction(0);
            p->setOldWaterFraction(1);
        }
        if(p->getPhaseFlag()=='o')
        {
            p->setNodeInOil(true);
            p->setNodeOutOil(true);
            p->setNodeInWater(false);
            p->setNodeOutWater(false);

            p->setOilFraction(1);
            p->setWaterFraction(0);

            p->setOldOilFraction(1);
            p->setOldWaterFraction(0);
        }

        p->setCapillaryPressure(0);
        p->setConcentration(0);
        p->setEffectiveVolume(p->getVolume());
        p->setOilFilmVolume(0);
        p->setWaterFilmVolume(0);
        p->setOilFilmConductivity(0);
        p->setWaterFilmConductivity(0);
        p->setOilLayerActivated(false);
        p->setWaterCornerActivated(false);
        p->setActive('t');
        p->setConductivity(0);
        p->setFlow(0);
        p->setMassFlow(0);
    }

    for (node* n: accessibleNodes)
    {
        if(n->getPhaseFlag()=='w')
        {
            n->setOilFraction(0);
            n->setWaterFraction(1);

            n->setOldOilFraction(0);
            n->setOldWaterFraction(1);
        }

        if(n->getPhaseFlag()=='o')
        {
            n->setOilFraction(1);
            n->setWaterFraction(0);

            n->setOldOilFraction(1);
            n->setOldWaterFraction(0);
        }

        n->setConcentration(0);
        n->setEffectiveVolume(n->getVolume());
        n->setOilFilmVolume(0);
        n->setWaterFilmVolume(0);
        n->setOilFilmConductivity(0);
        n->setWaterFilmConductivity(0);
        n->setOilLayerActivated(false);
        n->setWaterCornerActivated(false);
        n->setActive('t');
        n->setConductivity(0);
        n->setFlow(0);
        n->setMassFlow(0);
    }
}

double network::getOutletFlow()
{
    double Q=0;
    for(pore* p: accessiblePores)
    {
        if(p->getOutlet())
        Q+=p->getFlow();
    }
    return Q;
}

double network::getWaterSaturation()
{
    double volume(0);
    for(element* p : accessibleElements)
        volume+=p->getWaterFraction()*p->getVolume();
    return volume/totalElementsVolume;
}

double network::getWaterSaturationWithFilmsPT()
{
    double waterVolume(0);
    for(element* e: accessibleElements)
    {
        if(e->getPhaseFlag()=='o')
            waterVolume+=e->getWaterFilmVolume();

        if(e->getPhaseFlag()=='w')
        {
            if(e->getWettabilityFlag()=='o')
                waterVolume+=e->getEffectiveVolume()+e->getWaterFilmVolume();
            if(e->getWettabilityFlag()=='w')
                waterVolume+=e->getVolume();
        }
    }
    return waterVolume/totalElementsVolume;
}

void network::extractVideo()
{
    if(videoRecording)
    {
        record=false;
        tools::renderVideo();
        tools::cleanVideosFolder();
    }
}

int network::uniform_int(int a, int b) {
    if(a==b)return a;
    boost::random::uniform_int_distribution<> dist(a, b);
    return dist(gen);
}

double network::uniform_real(double a, double b)
{
    if(a==b || a>b)return a;
    boost::random::uniform_real_distribution<> dist(a, b);
    return dist(gen);
}

double network::rayleigh(double min, double max, double ryParam)
{
    if(min==max)return min;
    double value=min+sqrt(-pow(ryParam,2)*log(1-uniform_real()*(1-exp(-pow((max-min),2)/pow(ryParam,2)))));
    return value;
}

double network::triangular(double a, double b, double c)
{
    if(a==b || c<a || c>b) return a;
    double fc=(c-a)/(b-a);
    double u=uniform_real();
    if(u<fc)
        return a+sqrt(u*(b-a)*(c-a));
    else
        return b-sqrt((1-u)*(b-a)*(b-c));

}

double network::normal(double min, double max, double mu, double sigma)
{
    if(min==max || mu<min || mu>max) return min;

    boost::normal_distribution<> nd(mu, sigma);
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > var_nor(gen, nd);

    double value=min-1;
    while(value<min || value>max)
        value=var_nor();

    return value;
}

double network::weibull(double min, double max, double alpha, double beta)
{
    if(min==max) return min;

    double u=uniform_real();

    double value=(max-min)*pow(-beta*log(u*(1-exp(-1/beta))+exp(-1/beta)),1/alpha)+min;

    return value;
}

//signals
void network::emitPlotSignal()
{
    emit plot();
}

}
