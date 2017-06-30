/////////////////////////////////////////////////////////////////////////////
/// Name:        misc.cpp
/// Purpose:     network utility functions are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

void network::setWaterTrapped(int type)
{
    for(int i=0;i<totalPores;++i)
        if(getPore(i)->getPhaseFlag()=='w')
            getPore(i)->setWaterTrapped(true);
    if(type==0)
        for(int i=0;i<totalPores;++i)
        {
                pore* p=getPore(i);
                if(p->getPhaseFlag()=='w')
                {
                    if(p->getClusterWater()->getInlet() || p->getClusterWater()->getOutlet())
                        p->setWaterTrapped(false);
                    if(waterFilms && p->getWettabilityFlag()=='w' && (p->getClusterWaterWet()->getInlet() || p->getClusterWaterWet()->getOutlet()))
                        p->setWaterTrapped(false);
                }
        }
    if(type==1)
        for(int i=0;i<totalPores;++i)
        {
                pore* p=getPore(i);
                if(p->getPhaseFlag()=='w')
                {
                    if(p->getClusterWater()->getOutlet())
                        p->setWaterTrapped(false);
                    if(waterFilms && p->getWettabilityFlag()=='w' && p->getClusterWaterWet()->getOutlet())
                        p->setWaterTrapped(false);
                }
        }
    if(type==2)
        for(int i=0;i<totalPores;++i)
        {
                pore* p=getPore(i);
                if(p->getPhaseFlag()=='w')
                {
                    if(p->getClusterWater()->getInlet())
                        p->setWaterTrapped(false);
                    if(waterFilms && p->getWettabilityFlag()=='w' && p->getClusterWaterWet()->getInlet())
                        p->setWaterTrapped(false);
                }
        }
}

void network::setOilTrapped(int type)
{
    for(int i=0;i<totalPores;++i)
        if(getPore(i)->getPhaseFlag()=='o')
            getPore(i)->setOilTrapped(true);

    if(type==0)
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(p->getPhaseFlag()=='o' && p->getOilTrapped())
            {
                if(p->getClusterOil()->getInlet() || p->getClusterOil()->getOutlet())
                    p->setOilTrapped(false);
                if(oilFilms && p->getWettabilityFlag()=='o' && (p->getClusterOilWet()->getInlet() || p->getClusterOilWet()->getOutlet()))
                    p->setOilTrapped(false);
            }
        }

    if(type==1)
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(p->getPhaseFlag()=='o' && p->getOilTrapped())
            {
                if(p->getClusterOil()->getOutlet())
                    p->setOilTrapped(false);
                if(oilFilms && p->getWettabilityFlag()=='o' && p->getClusterOilWet()->getOutlet())
                    p->setOilTrapped(false);
            }
        }

    if(type==2)
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(p->getPhaseFlag()=='o' && p->getOilTrapped())
            {
                if(p->getClusterOil()->getInlet())
                    p->setOilTrapped(false);
                if(oilFilms && p->getWettabilityFlag()=='o' && p->getClusterOilWet()->getInlet())
                    p->setOilTrapped(false);
            }
        }
}

void network::setGasTrapped(int type)
{
    for(int i=0;i<totalPores;++i)
        if(getPore(i)->getPhaseFlag()=='g')
            getPore(i)->setGasTrapped(true);

    if(type==0)
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(p->getPhaseFlag()=='g' && p->getGasTrapped())
            {
                if(p->getClusterGas()->getInlet() || p->getClusterGas()->getOutlet())
                    p->setGasTrapped(false);
            }
        }

    if(type==1)
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(p->getPhaseFlag()=='g' && p->getGasTrapped())
            {
                if(p->getClusterGas()->getOutlet())
                    p->setGasTrapped(false);
            }
        }

    if(type==2)
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(p->getPhaseFlag()=='g' && p->getGasTrapped())
            {
                if(p->getClusterGas()->getInlet())
                    p->setGasTrapped(false);
            }
        }
}


void network::setWaterTrappedPT(int type)
{
    for(int i=0;i<totalElements;++i)
        if(getElement(i)->getPhaseFlag()=='w')
            getElement(i)->setWaterTrapped(true);
    if(type==0)
        for(int i=0;i<totalElements;++i)
        {
                element* p=getElement(i);
                if(p->getPhaseFlag()=='w')
                {
                    if(p->getClusterWater()->getInlet() || p->getClusterWater()->getOutlet())
                        p->setWaterTrapped(false);
                    if(waterFilms && p->getWettabilityFlag()=='w' && (p->getClusterWaterWet()->getInlet() || p->getClusterWaterWet()->getOutlet()))
                        p->setWaterTrapped(false);
                }
        }
    if(type==1)
        for(int i=0;i<totalElements;++i)
        {
                element* p=getElement(i);
                if(p->getPhaseFlag()=='w')
                {
                    if(p->getClusterWater()->getOutlet())
                        p->setWaterTrapped(false);
                    if(waterFilms && p->getWettabilityFlag()=='w' && p->getClusterWaterWet()->getOutlet())
                        p->setWaterTrapped(false);
                }
        }
    if(type==2)
        for(int i=0;i<totalElements;++i)
        {
                element* p=getElement(i);
                if(p->getPhaseFlag()=='w')
                {
                    if(p->getClusterWater()->getInlet())
                        p->setWaterTrapped(false);
                    if(waterFilms && p->getWettabilityFlag()=='w' && p->getClusterWaterWet()->getInlet())
                        p->setWaterTrapped(false);
                }
        }
}

void network::setOilTrappedPT(int type)
{
    for(int i=0;i<totalElements;++i)
        if(getElement(i)->getPhaseFlag()=='o')
            getElement(i)->setOilTrapped(true);

    if(type==0)
        for(int i=0;i<totalElements;++i)
        {
            element* p=getElement(i);
            if(p->getPhaseFlag()=='o')
            {
                if(p->getClusterOil()->getInlet() || p->getClusterOil()->getOutlet())
                    p->setOilTrapped(false);
                if(oilFilms && p->getWettabilityFlag()=='o' && (p->getClusterOilWet()->getInlet() || p->getClusterOilWet()->getOutlet()))
                    p->setOilTrapped(false);
            }
        }

    if(type==1)
        for(int i=0;i<totalElements;++i)
        {
            element* p=getElement(i);
            if(p->getPhaseFlag()=='o')
            {
                if(p->getClusterOil()->getOutlet())
                    p->setOilTrapped(false);
                if(oilFilms && p->getWettabilityFlag()=='o' && p->getClusterOilWet()->getOutlet())
                    p->setOilTrapped(false);
            }
        }

    if(type==2)
        for(int i=0;i<totalElements;++i)
        {
            element* p=getElement(i);
            if(p->getPhaseFlag()=='o')
            {
                if(p->getClusterOil()->getInlet())
                    p->setOilTrapped(false);
                if(oilFilms && p->getWettabilityFlag()=='o' && p->getClusterOilWet()->getInlet())
                    p->setOilTrapped(false);
            }
        }
}

void network::setGasTrappedPT(int type)
{
    for(int i=0;i<totalElements;++i)
        if(getElement(i)->getPhaseFlag()=='g')
            getElement(i)->setGasTrapped(true);

    if(type==0)
        for(int i=0;i<totalElements;++i)
        {
            element* p=getElement(i);
            if(p->getPhaseFlag()=='g')
            {
                if(p->getClusterGas()->getInlet() || p->getClusterGas()->getOutlet())
                    p->setGasTrapped(false);
            }
        }

    if(type==1)
        for(int i=0;i<totalElements;++i)
        {
            element* p=getElement(i);
            if(p->getPhaseFlag()=='g')
            {
                if(p->getClusterGas()->getOutlet())
                    p->setGasTrapped(false);

            }
        }

    if(type==2)
        for(int i=0;i<totalElements;++i)
        {
            element* p=getElement(i);
            if(p->getPhaseFlag()=='g')
            {
                if(p->getClusterGas()->getInlet())
                    p->setGasTrapped(false);
            }
        }
}

void network::assignViscositiesWithMixedFluids()
{
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
            p->setViscosity(oilViscosity*p->getOilFraction()+waterViscosity*p->getWaterFraction());
    }

    if(networkSource!=1)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* n=getNode(i);
        if(!n->getClosed())
            n->setViscosity(oilViscosity*n->getOilFraction()+waterViscosity*n->getWaterFraction());
    }
}

void network::fillWithPhase(char phase, double saturation, int distribution, char otherPhase)
{
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
            p->setPhaseFlag(otherPhase);
    }

    if(distribution==1)
    {
        if(saturation>0 && saturation<1)
        {
            double  actualWaterVolume=0;
            while((actualWaterVolume/totalPoresVolume)<saturation)
            {
                int index=uniform_int(0,totalPores-1);
                pore* p=getPore(index);
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
            double currentRadius=minRadius;
            double radiusStep=(maxRadius-minRadius)/100;

            while((actualWaterVolume/totalPoresVolume)<saturation)
            {
                currentRadius+=radiusStep;
                for (int i = 0; i < totalPores; ++i)
                {

                    pore* p=getPore(i);
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
            double currentRadius=maxRadius;
            double radiusStep=(maxRadius-minRadius)/100;

            while((actualWaterVolume/totalPoresVolume)<saturation)
            {
                currentRadius-=radiusStep;
                for (int i = 0; i < totalPores; ++i)
                {
                    pore* p=getPore(i);
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
        for(int i=0;i<totalPores;++i)
        {
            pore* p=getPore(i);
            if(!p->getClosed())
                p->setPhaseFlag(phase);
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
                if(p->getNeighs().size()==1)
                {
                    element* connectedNode=p->getNeighs()[0];
                    p->setPhaseFlag(connectedNode->getPhaseFlag());
                }
                else
                {
                    element* connectedNode1=p->getNeighs()[0];
                    element* connectedNode2=p->getNeighs()[1];
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
}

//Tools

double network::getVolume(char phase)
{
    double volume=0;
    for(int i=0;i<totalPores;++i)
    {
        pore *p=getPore(i);
        if(!p->getClosed())
        {
            if(p->getPhaseFlag()==phase)
                volume+=p->getVolume();
        }
    }
    return volume;
}

double network::getVolumePT(char phase)
{
    double volume=0;
    for(int i=0;i<totalElements;++i)
    {
        element *e=getElement(i);
        if(!e->getClosed() && e->getPhaseFlag()==phase)
        {
            volume+=e->getVolume();
        }
    }
    return volume;
}

int network::getNumberOfPores(char phase)
{
    double number=0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(p->getPhaseFlag()==phase)
            number++;
    }
    return number;
}

double network::getOutletFlow()
{
    double Q=0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed() && p->getOutlet())
        Q+=p->getFlow();
    }
    return Q;
}

double network::getInletFlow()
{
    double Q=0;
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed() && p->getInlet())
        Q+=p->getFlow();
    }
    return Q;
}

double network::getWaterSaturation()
{
    double volume(0);
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
                volume+=p->getWaterFraction()*(p->getVolume()-p->getFilmVolume());
            if(p->getWettabilityFlag()=='w')
                volume+=p->getFilmVolume();
        }
    }
    return volume/totalPoresVolume;
}

double network::getOilSaturation()
{
    double volume(0);
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            volume+=p->getOilFraction()*(p->getVolume()-p->getFilmVolume());
            if(p->getWettabilityFlag()=='o')
                volume+=p->getFilmVolume();
        }
    }
    return volume/totalPoresVolume;
}

double network::getWaterSaturationPT()
{
    double volume(0);
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed())
        {
            volume+=p->getWaterFraction()*(p->getVolume()-p->getFilmVolume());
            if(p->getWettabilityFlag()=='w')
                volume+=p->getFilmVolume();
        }
    }
    return volume/totalElementsVolume;
}

double network::getOilSaturationPT()
{
    double volume(0);
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed())
        {
             volume+=p->getOilFraction()*(p->getVolume()-p->getFilmVolume());
            if(p->getWettabilityFlag()=='o')
                volume+=p->getFilmVolume();
        }
    }
    return volume/totalElementsVolume;
}

void network::assignViscosities()
{
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        switch(p->getPhaseFlag())
        {
            case 'o':
            {
                p->setViscosity(oilViscosity);
                break;
            }
            case 'w':
            {
                p->setViscosity(waterViscosity);
                break;
            }
            case 'g':
            {
                p->setViscosity(gasViscosity);
                break;
            }
        }
    }

    if(networkSource==2 || networkSource==3)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* n=getNode(i);
        switch(n->getPhaseFlag())
        {
            case 'o':
            {
                n->setViscosity(oilViscosity);
                break;
            }
            case 'w':
            {
                n->setViscosity(waterViscosity);
                break;
            }
            case 'g':
            {
                n->setViscosity(gasViscosity);
                break;
            }
        }
    }
}

//Video Extraction
void network::extractVideo()
{
    if(videoRecording)
    {
        record=false;
        tools::renderVideo();
        tools::cleanVideosFolder();
    }
}

//randomness
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
