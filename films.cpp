/////////////////////////////////////////////////////////////////////////////
/// Name:        films.cpp
/// Purpose:     methods related to film calculations (half angles, volumes,
///              conductivities) are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "network.h"

using namespace std;

void network::assign3PhaseTheta()
{
    ofstream file("Results/thetas.txt");
    double Cs=min(0.0,WGSurfaceTension-OGSurfaceTension-OWSurfaceTension);
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            p->setThetaOG(acos((Cs*cos(p->getTheta())+Cs+2*OGSurfaceTension)/(2*OGSurfaceTension)));
            p->setThetaWG(acos(((Cs+2*OWSurfaceTension)*cos(p->getTheta())+Cs+2*OGSurfaceTension )/(2*WGSurfaceTension)));
            file<<p->getId()<<" "<<p->getTheta()<<" "<<p->getThetaOG()<<" "<<p->getThetaWG()<<endl;
        }
    }

    if(networkSource==2 || networkSource==3)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* p=getNode(i);
        if(!p->getClosed())
        {
            p->setThetaOG(acos((Cs*cos(p->getTheta())+Cs+2*OGSurfaceTension)/(2*OGSurfaceTension)));
            p->setThetaWG(acos(((Cs+2*OWSurfaceTension)*cos(p->getTheta())+Cs+2*OGSurfaceTension )/(2*WGSurfaceTension)));
            file<<p->getId()<<" "<<p->getTheta()<<" "<<p->getThetaOG()<<" "<<p->getThetaWG()<<endl;
        }
    }
}

void network::assignHalfAngles()
{
    ofstream file("Results/halfAngles.txt");
    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            if(p->getShapeFactor()<=sqrt(3)/36.)
            {
                double beta2Min=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3+4*tools::pi()/3));
                double beta2Max=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3));
                double beta2=uniform_real(beta2Min,beta2Max);
                double beta1=-0.5*beta2+0.5*asin((tan(beta2)+4*p->getShapeFactor())/(tan(beta2)-4*p->getShapeFactor())*sin(beta2));
                double beta3=tools::pi()/2-beta1-beta2;
                p->setBeta1(beta1);
                p->setBeta2(beta2);
                p->setBeta3(beta3);
            }

            file<<p->getId()<<" "<<p->getBeta1()<<" "<<p->getBeta2()<<" "<<p->getBeta3()<<endl;
        }
    }

    if(networkSource==2 || networkSource==3)
    for (int i = 0; i < totalNodes; ++i)
    {
        node* p=getNode(i);
        if(!p->getClosed())
        {
            if(p->getShapeFactor()<=sqrt(3)/36.)
            {
                double beta2Min=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3+4*tools::pi()/3));
                double beta2Max=atan(2/sqrt(3)*cos(acos(-12*sqrt(3)*p->getShapeFactor())/3));
                double beta2=uniform_real(beta2Min,beta2Max);
                double beta1=-0.5*beta2+0.5*asin((tan(beta2)+4*p->getShapeFactor())/(tan(beta2)-4*p->getShapeFactor())*sin(beta2));
                double beta3=tools::pi()/2-beta1-beta2;
                p->setBeta1(beta1);
                p->setBeta2(beta2);
                p->setBeta3(beta3);
            }

            file<<p->getId()<<" "<<p->getBeta1()<<" "<<p->getBeta2()<<" "<<p->getBeta3()<<endl;
        }
    }
}

void network::assignFilmStability()
{
    ofstream file("Results/FilmsStabiliy.txt");

    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            p->setFilm1(false);
            p->setFilm2(false);
            p->setFilm3(false);
            p->setFilm1OG(false);
            p->setFilm2OG(false);
            p->setFilm3OG(false);
            p->setFilm1WG(false);
            p->setFilm2WG(false);
            p->setFilm3WG(false);
        }
    }

    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            if(p->getShapeFactor()<=sqrt(3)/36.)
            {
                if(p->getWettabilityFlag()=='w')
                {
                    if(p->getTheta()<tools::pi()/2-p->getBeta1())
                        p->setFilm1(true);
                    if(p->getTheta()<tools::pi()/2-p->getBeta2())
                        p->setFilm2(true);
                    if(p->getTheta()<tools::pi()/2-p->getBeta3())
                        p->setFilm3(true);
                }

                if(p->getThetaOG()<tools::pi()/2)
                {
                    if(p->getThetaOG()<tools::pi()/2-p->getBeta1())
                        p->setFilm1OG(true);
                    if(p->getThetaOG()<tools::pi()/2-p->getBeta2())
                        p->setFilm2OG(true);
                    if(p->getThetaOG()<tools::pi()/2-p->getBeta3())
                        p->setFilm3OG(true);
                }

                if(p->getThetaWG()<tools::pi()/2)
                {
                    if(p->getThetaWG()<tools::pi()/2-p->getBeta1())
                        p->setFilm1WG(true);
                    if(p->getThetaWG()<tools::pi()/2-p->getBeta2())
                        p->setFilm2WG(true);
                    if(p->getThetaWG()<tools::pi()/2-p->getBeta3())
                        p->setFilm3WG(true);
                }

                if(p->getWettabilityFlag()=='o')
                {
                    if(tools::pi()-p->getTheta()<tools::pi()/2-p->getBeta1())
                        p->setFilm1(true);
                    if(tools::pi()-p->getTheta()<tools::pi()/2-p->getBeta2())
                        p->setFilm2(true);
                    if(tools::pi()-p->getTheta()<tools::pi()/2-p->getBeta3())
                        p->setFilm3(true);
                }
            }

            file<<p->getId()<<" "<<p->getFilm1()<<" "<<p->getFilm2()<<" "<<p->getFilm3()<<" "<<p->getFilm1OG()<<" "<<p->getFilm2OG()<<" "<<p->getFilm3OG()<<" "<<p->getFilm1WG()<<" "<<p->getFilm2WG()<<" "<<p->getFilm3WG()<<endl;
        }
    }

    if(networkSource==2 || networkSource==3)
    {
        for (int i = 0; i < totalNodes; ++i)
        {
            node* p=getNode(i);
            if(!p->getClosed())
            {
                p->setFilm1(false);
                p->setFilm2(false);
                p->setFilm3(false);
                p->setFilm1OG(false);
                p->setFilm2OG(false);
                p->setFilm3OG(false);
                p->setFilm1WG(false);
                p->setFilm2WG(false);
                p->setFilm3WG(false);
            }
        }

        for (int i = 0; i < totalNodes; ++i)
        {
            node* p=getNode(i);
            if(!p->getClosed())
            {
                if(p->getShapeFactor()<=sqrt(3)/36.)
                {
                    if(p->getWettabilityFlag()=='w')
                    {
                        if(p->getTheta()<tools::pi()/2-p->getBeta1())
                            p->setFilm1(true);
                        if(p->getTheta()<tools::pi()/2-p->getBeta2())
                            p->setFilm2(true);
                        if(p->getTheta()<tools::pi()/2-p->getBeta3())
                            p->setFilm3(true);
                    }

                    if(p->getThetaOG()<tools::pi()/2)
                    {
                        if(p->getThetaOG()<tools::pi()/2-p->getBeta1())
                            p->setFilm1OG(true);
                        if(p->getThetaOG()<tools::pi()/2-p->getBeta2())
                            p->setFilm2OG(true);
                        if(p->getThetaOG()<tools::pi()/2-p->getBeta3())
                            p->setFilm3OG(true);
                    }

                    if(p->getThetaWG()<tools::pi()/2)
                    {
                        if(p->getThetaWG()<tools::pi()/2-p->getBeta1())
                            p->setFilm1WG(true);
                        if(p->getThetaWG()<tools::pi()/2-p->getBeta2())
                            p->setFilm2WG(true);
                        if(p->getThetaWG()<tools::pi()/2-p->getBeta3())
                            p->setFilm3WG(true);
                    }

                    if(p->getWettabilityFlag()=='o')
                    {
                        if(tools::pi()-p->getTheta()<tools::pi()/2-p->getBeta1())
                            p->setFilm1(true);
                        if(tools::pi()-p->getTheta()<tools::pi()/2-p->getBeta2())
                            p->setFilm2(true);
                        if(tools::pi()-p->getTheta()<tools::pi()/2-p->getBeta3())
                            p->setFilm3(true);
                    }
                }
            }
        }
    }
}

void network::assignFd()
{
    ofstream file("Results/Fds.txt");

    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            p->setFd(1);
            p->setFdOG(1);
            p->setFdWG(1);
        }
    }

    for (int i = 0; i < totalPores; ++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            if(p->getShapeFactor()<=sqrt(3)/36.)
            {
                double theta=p->getWettabilityFlag()=='w'?p->getTheta():tools::pi()-p->getTheta();
                double thetaOG=p->getThetaOG();
                double thetaWG=p->getThetaWG();
                double G=p->getShapeFactor();
                double S1(0),S2(0),S3(0),D(0),Fd(0);
                if(p->getFilm1())
                {
                    S1+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(theta+p->getBeta1())/(tools::pi()/2));
                    S2+=cos(theta+p->getBeta1())/sin(p->getBeta1());
                    S3+=tools::pi()-theta-p->getBeta1();
                }
                if(p->getFilm2())
                {
                    S1+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(theta+p->getBeta2())/(tools::pi()/2));
                    S2+=cos(theta+p->getBeta2())/sin(p->getBeta2());
                    S3+=tools::pi()-theta-p->getBeta2();
                }
                if(p->getFilm3())
                {
                    S1+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(theta+p->getBeta3())/(tools::pi()/2));
                    S2+=cos(theta+p->getBeta3())/sin(p->getBeta3());
                    S3+=tools::pi()-theta-p->getBeta3();
                }
                D=S1-2*cos(theta)*S2+S3;
                Fd=(1+sqrt(1+4*G*D))/(1+2*sqrt(tools::pi()*G));
                p->setFd(D==0?1:Fd);

                S1=0;S2=0;S3=0;D=0;Fd=0;
                if(p->getFilm1OG())
                {
                    S1+=cos(thetaOG)*cos(thetaOG+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(thetaOG+p->getBeta1())/(tools::pi()/2));
                    S2+=cos(thetaOG+p->getBeta1())/sin(p->getBeta1());
                    S3+=tools::pi()-thetaOG-p->getBeta1();
                }
                if(p->getFilm2OG())
                {
                    S1+=cos(thetaOG)*cos(thetaOG+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(thetaOG+p->getBeta2())/(tools::pi()/2));
                    S2+=cos(thetaOG+p->getBeta2())/sin(p->getBeta2());
                    S3+=tools::pi()-thetaOG-p->getBeta2();
                }
                if(p->getFilm3OG())
                {
                    S1+=cos(thetaOG)*cos(thetaOG+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(thetaOG+p->getBeta3())/(tools::pi()/2));
                    S2+=cos(thetaOG+p->getBeta3())/sin(p->getBeta3());
                    S3+=tools::pi()-thetaOG-p->getBeta3();
                }
                D=S1-2*cos(thetaOG)*S2+S3;
                Fd=(1+sqrt(1+4*G*D))/(1+2*sqrt(tools::pi()*G));
                p->setFdOG(D==0?1:Fd);

                S1=0;S2=0;S3=0;D=0;Fd=0;
                if(p->getFilm1WG())
                {
                    S1+=cos(thetaWG)*cos(thetaWG+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(thetaWG+p->getBeta1())/(tools::pi()/2));
                    S2+=cos(thetaWG+p->getBeta1())/sin(p->getBeta1());
                    S3+=tools::pi()-thetaWG-p->getBeta1();
                }
                if(p->getFilm2WG())
                {
                    S1+=cos(thetaWG)*cos(thetaWG+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(thetaWG+p->getBeta2())/(tools::pi()/2));
                    S2+=cos(thetaWG+p->getBeta2())/sin(p->getBeta2());
                    S3+=tools::pi()-thetaWG-p->getBeta2();
                }
                if(p->getFilm3WG())
                {
                    S1+=cos(thetaWG)*cos(thetaWG+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(thetaWG+p->getBeta3())/(tools::pi()/2));
                    S2+=cos(thetaWG+p->getBeta3())/sin(p->getBeta3());
                    S3+=tools::pi()-thetaWG-p->getBeta3();
                }
                D=S1-2*cos(thetaWG)*S2+S3;
                Fd=(1+sqrt(1+4*G*D))/(1+2*sqrt(tools::pi()*G));
                p->setFdWG(D==0?1:Fd);
            }
            file<<p->getFd()<<" "<<p->getFdOG()<<" "<<p->getFdWG()<<endl;
        }
    }

    if(networkSource==2 || networkSource==3)
    {
        for (int i = 0; i < totalNodes; ++i)
        {
            node* p=getNode(i);
            if(!p->getClosed())
            {
                p->setFd(1);
                p->setFdOG(1);
                p->setFdWG(1);
            }
        }

        for (int i = 0; i < totalNodes; ++i)
        {
            node* p=getNode(i);
            if(!p->getClosed())
            {
                if(p->getShapeFactor()<=sqrt(3)/36.)
                {
                    double theta=p->getWettabilityFlag()=='w'?p->getTheta():tools::pi()-p->getTheta();
                    double thetaOG=p->getThetaOG();
                    double thetaWG=p->getThetaWG();
                    double G=p->getShapeFactor();
                    double S1(0),S2(0),S3(0),D(0),Fd(0);//nope
                    if(p->getFilm1())
                    {
                        S1+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(theta+p->getBeta1())/(tools::pi()/2));
                        S2+=cos(theta+p->getBeta1())/sin(p->getBeta1());
                        S3+=tools::pi()-theta-p->getBeta1();
                    }
                    if(p->getFilm2())
                    {
                        S1+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(theta+p->getBeta2())/(tools::pi()/2));
                        S2+=cos(theta+p->getBeta2())/sin(p->getBeta2());
                        S3+=tools::pi()-theta-p->getBeta2();
                    }
                    if(p->getFilm3())
                    {
                        S1+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(theta+p->getBeta3())/(tools::pi()/2));
                        S2+=cos(theta+p->getBeta3())/sin(p->getBeta3());
                        S3+=tools::pi()-theta-p->getBeta3();
                    }
                    D=S1-2*cos(theta)*S2+S3;
                    Fd=(1+sqrt(1+4*G*D))/(1+2*sqrt(tools::pi()*G));
                    p->setFd(D==0?1:Fd);

                    S1=0;S2=0;S3=0;D=0;Fd=0;
                    if(p->getFilm1OG())
                    {
                        S1+=cos(thetaOG)*cos(thetaOG+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(thetaOG+p->getBeta1())/(tools::pi()/2));
                        S2+=cos(thetaOG+p->getBeta1())/sin(p->getBeta1());
                        S3+=tools::pi()-thetaOG-p->getBeta1();
                    }
                    if(p->getFilm2OG())
                    {
                        S1+=cos(thetaOG)*cos(thetaOG+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(thetaOG+p->getBeta2())/(tools::pi()/2));
                        S2+=cos(thetaOG+p->getBeta2())/sin(p->getBeta2());
                        S3+=tools::pi()-thetaOG-p->getBeta2();
                    }
                    if(p->getFilm3OG())
                    {
                        S1+=cos(thetaOG)*cos(thetaOG+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(thetaOG+p->getBeta3())/(tools::pi()/2));
                        S2+=cos(thetaOG+p->getBeta3())/sin(p->getBeta3());
                        S3+=tools::pi()-thetaOG-p->getBeta3();
                    }
                    D=S1-2*cos(thetaOG)*S2+S3;
                    Fd=(1+sqrt(1+4*G*D))/(1+2*sqrt(tools::pi()*G));
                    p->setFdOG(D==0?1:Fd);

                    S1=0;S2=0;S3=0;D=0;Fd=0;
                    if(p->getFilm1WG())
                    {
                        S1+=cos(thetaWG)*cos(thetaWG+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(thetaWG+p->getBeta1())/(tools::pi()/2));
                        S2+=cos(thetaWG+p->getBeta1())/sin(p->getBeta1());
                        S3+=tools::pi()-thetaWG-p->getBeta1();
                    }
                    if(p->getFilm2WG())
                    {
                        S1+=cos(thetaWG)*cos(thetaWG+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(thetaWG+p->getBeta2())/(tools::pi()/2));
                        S2+=cos(thetaWG+p->getBeta2())/sin(p->getBeta2());
                        S3+=tools::pi()-thetaWG-p->getBeta2();
                    }
                    if(p->getFilm3WG())
                    {
                        S1+=cos(thetaWG)*cos(thetaWG+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(thetaWG+p->getBeta3())/(tools::pi()/2));
                        S2+=cos(thetaWG+p->getBeta3())/sin(p->getBeta3());
                        S3+=tools::pi()-thetaWG-p->getBeta3();
                    }
                    D=S1-2*cos(thetaWG)*S2+S3;
                    Fd=(1+sqrt(1+4*G*D))/(1+2*sqrt(tools::pi()*G));
                    p->setFdWG(D==0?1:Fd);
                }
                file<<p->getFd()<<" "<<p->getFdOG()<<" "<<p->getFdWG()<<endl;
            }
        }
    }
}

void network::updateFilmProperties(double capPressure, char invadingPhase)
{
    for(int i=0;i<totalPores;++i)
    {
        pore* p=getPore(i);
        if(!p->getClosed())
        {
            if(p->getShapeFactor()<=0.048)
            {
                double filmVolume=0;
                double filmConductivity=0;
                if(p->getWettabilityFlag()=='w' )
                {
                    if(p->getPhaseFlag()=='o' &&
                            (invadingPhase=='o' && p->getClusterOil()->getInlet() && p->getWaterFilmFlowing() && p->getClusterWaterFilm()->getOutlet()
                             || invadingPhase=='w' && p->getWaterFilmFlowing() && p->getClusterWaterFilm()->getInlet() && p->getClusterOil()->getOutlet()
                             )
                            )
                    {
                        double theta=p->getTheta();
                        double sigma=OWSurfaceTension;

                        double sum=0;
                        if(p->getFilm1())
                            sum+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(theta+p->getBeta1())/(tools::pi()/2));
                        if(p->getFilm2())
                            sum+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(theta+p->getBeta2())/(tools::pi()/2));
                        if(p->getFilm3())
                            sum+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(theta+p->getBeta3())/(tools::pi()/2));

                        if(capPressure!=0)
                        {
                            double curvature=min(p->getRadius(),sigma/capPressure);
                            double Aw=pow(curvature,2)*sum;
                            filmVolume=min(1-4*tools::pi()*p->getShapeFactor()*p->getVolume(),Aw*p->getLength());
                            filmConductivity=pow(curvature,4)*sum/(waterViscosity*p->getLength());
                            if(invadingPhase=='o' && filmVolume<p->getFilmVolume()  || invadingPhase=='w' && filmVolume>p->getFilmVolume())
                            {
                                p->setFilmVolume(filmVolume);
                                p->setFilmConductivity(filmConductivity);
                            }
                        }
                    }

                    if(p->getPhaseFlag()=='g' &&
                            (invadingPhase=='g' && p->getClusterGas()->getInlet() && p->getWaterFilmFlowing() && p->getClusterWaterFilm()->getOutlet()
                             || invadingPhase=='w' && p->getWaterFilmFlowing()&& p->getClusterWaterFilm()->getInlet() && p->getClusterGas()->getOutlet()
                             )
                            )
                    {
                        double thetaWG=p->getThetaWG();
                        double sigma=p->getOilLayerFlowing()?OWSurfaceTension+OGSurfaceTension:WGSurfaceTension;


                        double sum=0;


                        if(p->getFilm1WG())
                            sum+=cos(thetaWG)*cos(thetaWG+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(thetaWG+p->getBeta1())/(tools::pi()/2));
                        if(p->getFilm2WG())
                            sum+=cos(thetaWG)*cos(thetaWG+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(thetaWG+p->getBeta2())/(tools::pi()/2));
                        if(p->getFilm3WG())
                            sum+=cos(thetaWG)*cos(thetaWG+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(thetaWG+p->getBeta3())/(tools::pi()/2));

                        if(capPressure!=0)
                        {
                            double curvature=min(p->getRadius(),sigma/capPressure);
                            double Aw=pow(curvature,2)*sum;
                            filmVolume=min(1-4*tools::pi()*p->getShapeFactor()*p->getVolume(),Aw*p->getLength());
                            filmConductivity=pow(curvature,4)*sum/(waterViscosity*p->getLength());
                            if(invadingPhase=='g' && filmVolume<p->getFilmVolume()  || invadingPhase=='w' && filmVolume>p->getFilmVolume())
                            {
                                p->setFilmVolume(filmVolume);
                                p->setFilmConductivity(filmConductivity);
                            }
                        }

                    }

                    if(p->getPhaseFlag()=='w')
                    {
                        filmVolume=0;
                        filmConductivity=0;
                        p->setFilmVolume(filmVolume);
                        p->setFilmConductivity(filmConductivity);
                    }

                }

                if(p->getWettabilityFlag()=='o')
                {
                    if(p->getPhaseFlag()=='w' && (invadingPhase=='w' && p->getClusterWater()->getInlet() && p->getOilFilmFlowing() && p->getClusterOilFilm()->getOutlet()
                                                  || invadingPhase=='o' && p->getOilFilmFlowing() && p->getClusterOilFilm()->getInlet() && p->getClusterWater()->getOutlet()
                                                  )
                                                 )

                    {
                        double theta=tools::pi()-p->getTheta();
                        double sigma=OWSurfaceTension;

                        double sum=0;
                        if(p->getFilm1())
                            sum+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(theta+p->getBeta1())/(tools::pi()/2));
                        if(p->getFilm2())
                            sum+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(theta+p->getBeta2())/(tools::pi()/2));
                        if(p->getFilm3())
                            sum+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(theta+p->getBeta3())/(tools::pi()/2));

                        if(capPressure!=0)
                        {
                            double curvature=min(p->getRadius(),sigma/capPressure);
                            double Aw=pow(curvature,2)*sum;
                            filmVolume=min(1-4*tools::pi()*p->getShapeFactor()*p->getVolume(),Aw*p->getLength());
                            filmConductivity=pow(curvature,4)*sum/(oilViscosity*p->getLength());
                            if(invadingPhase=='w' && filmVolume<p->getFilmVolume()  || invadingPhase=='o' && filmVolume>p->getFilmVolume())
                            {
                                p->setFilmVolume(filmVolume);
                                p->setFilmConductivity(filmConductivity);
                            }
                        }
                    }

                    if(p->getPhaseFlag()=='g' &&
                            (invadingPhase=='g' && p->getClusterGas()->getInlet() && p->getOilFilmFlowing() && p->getClusterOilFilm()->getOutlet()
                             || invadingPhase=='o' && p->getOilFilmFlowing() && p->getClusterOilFilm()->getInlet() && p->getClusterGas()->getOutlet()
                             )
                            )
                    {
                        double thetaOG=p->getThetaOG();
                        double sigma=p->getWaterLayerFlowing()?OWSurfaceTension+WGSurfaceTension:OGSurfaceTension;


                        double sum=0;

                        if(p->getFilm1OG())
                            sum+=cos(thetaOG)*cos(thetaOG+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(thetaOG+p->getBeta1())/(tools::pi()/2));
                        if(p->getFilm2OG())
                            sum+=cos(thetaOG)*cos(thetaOG+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(thetaOG+p->getBeta2())/(tools::pi()/2));
                        if(p->getFilm3OG())
                            sum+=cos(thetaOG)*cos(thetaOG+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(thetaOG+p->getBeta3())/(tools::pi()/2));

                        if(capPressure!=0)
                        {
                            double curvature=min(p->getRadius(),sigma/capPressure);
                            double Aw=pow(curvature,2)*sum;
                            filmVolume=min(1-4*tools::pi()*p->getShapeFactor()*p->getVolume(),Aw*p->getLength());
                            filmConductivity=pow(curvature,4)*sum/(oilViscosity*p->getLength());
                            if(invadingPhase=='g' && filmVolume<p->getFilmVolume()  || invadingPhase=='o' && filmVolume>p->getFilmVolume())
                            {
                                p->setFilmVolume(filmVolume);
                                p->setFilmConductivity(filmConductivity);
                            }
                        }
                    }

                    if(p->getPhaseFlag()=='o')
                    {
                        filmVolume=0;
                        filmConductivity=0;
                        p->setFilmVolume(filmVolume);
                        p->setFilmConductivity(filmConductivity);
                    }
                }
            }

        }
    }
}

void network::updateFilmPropertiesPT(double capPressure, char invadingPhase)
{
    for(int i=0;i<totalElements;++i)
    {
        element* p=getElement(i);
        if(!p->getClosed())
        {
            if(p->getShapeFactor()<=0.048)
            {
                double filmVolume=0;
                double filmConductivity=0;

                if(p->getWettabilityFlag()=='w' )
                {
                    if(p->getPhaseFlag()=='o' &&
                            (invadingPhase=='o' && p->getClusterOil()->getInlet() && p->getWaterFilmFlowing() && p->getClusterWaterFilm()->getOutlet()
                             || invadingPhase=='w' && p->getWaterFilmFlowing() && p->getClusterWaterFilm()->getInlet() && p->getClusterOil()->getOutlet()
                             )
                            )
                    {
                        double theta=p->getTheta();
                        double sigma=OWSurfaceTension;

                        double sum=0;
                        if(p->getFilm1())
                            sum+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(theta+p->getBeta1())/(tools::pi()/2));
                        if(p->getFilm2())
                            sum+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(theta+p->getBeta2())/(tools::pi()/2));
                        if(p->getFilm3())
                            sum+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(theta+p->getBeta3())/(tools::pi()/2));

                        if(capPressure!=0)
                        {
                            double curvature=min(p->getRadius(),sigma/capPressure);
                            double Aw=pow(curvature,2)*sum;
                            filmVolume=min(1-4*tools::pi()*p->getShapeFactor()*p->getVolume(),Aw*p->getLength());
                            filmConductivity=pow(curvature,4)*sum/(waterViscosity*p->getLength());
                            if(invadingPhase=='o' && filmVolume<p->getFilmVolume()  || invadingPhase=='w' && filmVolume>p->getFilmVolume())
                            {
                                p->setFilmVolume(filmVolume);
                                p->setFilmConductivity(filmConductivity);
                            }
                        }
                    }

                    if(p->getPhaseFlag()=='g' &&
                            (invadingPhase=='g' && p->getClusterGas()->getInlet() && p->getWaterFilmFlowing() && p->getClusterWaterFilm()->getOutlet()
                             || invadingPhase=='w' && p->getWaterFilmFlowing()&& p->getClusterWaterFilm()->getInlet() && p->getClusterGas()->getOutlet()
                             )
                            )
                    {
                        double thetaWG=p->getThetaWG();
                        double sigma=p->getOilLayerFlowing()?OWSurfaceTension+OGSurfaceTension:WGSurfaceTension;


                            double sum=0;

                            {
                                if(p->getFilm1WG())
                                    sum+=cos(thetaWG)*cos(thetaWG+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(thetaWG+p->getBeta1())/(tools::pi()/2));
                                if(p->getFilm2WG())
                                    sum+=cos(thetaWG)*cos(thetaWG+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(thetaWG+p->getBeta2())/(tools::pi()/2));
                                if(p->getFilm3WG())
                                    sum+=cos(thetaWG)*cos(thetaWG+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(thetaWG+p->getBeta3())/(tools::pi()/2));
                            }

                            if(capPressure!=0)
                            {
                                double curvature=min(p->getRadius(),sigma/capPressure);
                                double Aw=pow(curvature,2)*sum;
                                filmVolume=min(1-4*tools::pi()*p->getShapeFactor()*p->getVolume(),Aw*p->getLength());
                                filmConductivity=pow(curvature,4)*sum/(waterViscosity*p->getLength());
                                if(invadingPhase=='g' && filmVolume<p->getFilmVolume()  || invadingPhase=='w' && filmVolume>p->getFilmVolume())
                                {
                                    p->setFilmVolume(filmVolume);
                                    p->setFilmConductivity(filmConductivity);
                                }
                            }
                    }

                    if(p->getPhaseFlag()=='w')
                    {
                        filmVolume=0;
                        filmConductivity=0;
                        p->setFilmVolume(filmVolume);
                        p->setFilmConductivity(filmConductivity);
                    }
                }

                if(p->getWettabilityFlag()=='o')
                {
                    if(p->getPhaseFlag()=='w' && (invadingPhase=='w' && p->getClusterWater()->getInlet() && p->getOilFilmFlowing() && p->getClusterOilFilm()->getOutlet()
                                                  || invadingPhase=='o' && p->getOilFilmFlowing() && p->getClusterOilFilm()->getInlet() && p->getClusterWater()->getOutlet()
                                                  )
                                                 )
                    {
                        double theta=tools::pi()-p->getTheta();
                        double sigma=OWSurfaceTension;

                        double sum=0;
                        if(p->getFilm1())
                            sum+=cos(theta)*cos(theta+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(theta+p->getBeta1())/(tools::pi()/2));
                        if(p->getFilm2())
                            sum+=cos(theta)*cos(theta+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(theta+p->getBeta2())/(tools::pi()/2));
                        if(p->getFilm3())
                            sum+=cos(theta)*cos(theta+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(theta+p->getBeta3())/(tools::pi()/2));

                        if(capPressure!=0)
                        {
                            double curvature=min(p->getRadius(),sigma/capPressure);
                            double Aw=pow(curvature,2)*sum;
                            filmVolume=min(1-4*tools::pi()*p->getShapeFactor()*p->getVolume(),Aw*p->getLength());
                            filmConductivity=pow(curvature,4)*sum/(oilViscosity*p->getLength());
                            if(invadingPhase=='w' && filmVolume<p->getFilmVolume()  || invadingPhase=='o' && filmVolume>p->getFilmVolume())
                            {
                                p->setFilmVolume(filmVolume);
                                p->setFilmConductivity(filmConductivity);
                            }
                        }
                    }

                    if(p->getPhaseFlag()=='g' &&
                            (invadingPhase=='g' && p->getClusterGas()->getInlet() && p->getOilFilmFlowing() && p->getClusterOilFilm()->getOutlet()
                             || invadingPhase=='o' && p->getOilFilmFlowing() && p->getClusterOilFilm()->getInlet() && p->getClusterGas()->getOutlet()
                             )
                            )
                    {
                        double thetaOG=p->getThetaOG();
                        double sigma=p->getWaterLayerFlowing()?OWSurfaceTension+WGSurfaceTension:OGSurfaceTension;


                            double sum=0;
                            {
                                if(p->getFilm1OG())
                                    sum+=cos(thetaOG)*cos(thetaOG+p->getBeta1())/sin(p->getBeta1())-tools::pi()/2*(1-(thetaOG+p->getBeta1())/(tools::pi()/2));
                                if(p->getFilm2OG())
                                    sum+=cos(thetaOG)*cos(thetaOG+p->getBeta2())/sin(p->getBeta2())-tools::pi()/2*(1-(thetaOG+p->getBeta2())/(tools::pi()/2));
                                if(p->getFilm3OG())
                                    sum+=cos(thetaOG)*cos(thetaOG+p->getBeta3())/sin(p->getBeta3())-tools::pi()/2*(1-(thetaOG+p->getBeta3())/(tools::pi()/2));
                            }


                            if(capPressure!=0)
                            {
                                double curvature=min(p->getRadius(),sigma/capPressure);
                                double Aw=pow(curvature,2)*sum;
                                filmVolume=min(1-4*tools::pi()*p->getShapeFactor()*p->getVolume(),Aw*p->getLength());
                                filmConductivity=pow(curvature,4)*sum/(oilViscosity*p->getLength());
                                if(invadingPhase=='g' && filmVolume<p->getFilmVolume()  || invadingPhase=='o' && filmVolume>p->getFilmVolume())
                                {
                                    p->setFilmVolume(filmVolume);
                                    p->setFilmConductivity(filmConductivity);
                                }
                            }
                    }

                    if(p->getPhaseFlag()=='o')
                    {
                        filmVolume=0;
                        filmConductivity=0;
                        p->setFilmVolume(filmVolume);
                        p->setFilmConductivity(filmConductivity);
                    }
                } 
            }

        }
    }
}
