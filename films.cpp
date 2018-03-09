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

}
