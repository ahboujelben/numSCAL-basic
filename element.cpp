/////////////////////////////////////////////////////////////////////////////
/// Name:        element.cpp
/// Purpose:     attributes and methods of capillary elements (pores/ nodes)
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "element.h"

element::element()
{
    radius=0;
    length=0;
    theta=0;
    wettabilityFlag='c';
    phaseFlag='o';
    volume=0;
    conductivity=0;
    viscosity=1;
    closed=false;
    inlet=false;
    outlet=false;

    clusterTemp=0;
    clusterOil=0;
    clusterWater=0;
    clusterOilWet=0;
    clusterWaterWet=0;

    oilFraction=1;
    waterFraction=0;

    beta1=0;
    beta2=0;
    beta3=0;
    film1=false;
    film2=false;
    film3=false;

    film1OG=false;
    film2OG=false;
    film3OG=false;

    film1WG=false;
    film2WG=false;
    film3WG=false;

    fd=1;
    fdOG=1;
    fdWG=1;

    snapOffCoefficient=1;
    snapOffCoefficientOG=1;
    snapOffCoefficientWG=1;

    filmVolume=0;
    filmConductivity=0;
    layerConductivity=0;

    exist='t';
}
int element::getId() const
{
    return id;
}

void element::setId(int value)
{
    id = value;
}
double element::getRadius() const
{
    return radius;
}

void element::setRadius(double value)
{
    radius = value;
}
double element::getLength() const
{
    return length;
}

void element::setLength(double value)
{
    length = value;
}
double element::getVolume() const
{
    return volume;
}

void element::setVolume(double value)
{
    volume = value;
}
double element::getShapeFactor() const
{
    return shapeFactor;
}

void element::setShapeFactor(double value)
{
    shapeFactor = value;
}
double element::getConductivity() const
{
    return conductivity;
}

void element::setConductivity(double value)
{
    conductivity = value;
}
double element::getTheta() const
{
    return theta;
}

void element::setTheta(double value)
{
    theta = value;
}

double element::getViscosity() const
{
    return viscosity;
}

void element::setViscosity(double value)
{
    viscosity = value;
}

char element::getWettabilityFlag() const
{
    return wettabilityFlag;
}

void element::setWettabilityFlag(char value)
{
    wettabilityFlag = value;
}
char element::getPhaseFlag() const
{
    return phaseFlag;
}

void element::setPhaseFlag(char value)
{
    phaseFlag = value;
}
std::vector<element *> element::getNeighs() const
{
    return neighs;
}

void element::setNeighs(const std::vector<element *> &value)
{
    neighs = value;
}
int element::getClusterTemp() const
{
    return clusterTemp;
}

void element::setClusterTemp(int value)
{
    clusterTemp = value;
}
cluster *element::getClusterWaterWet() const
{
    return clusterWaterWet;
}

void element::setClusterWaterWet(cluster *value)
{
    clusterWaterWet = value;
}
cluster *element::getClusterOilWet() const
{
    return clusterOilWet;
}

void element::setClusterOilWet(cluster *value)
{
    clusterOilWet = value;
}
cluster *element::getClusterWater() const
{
    return clusterWater;
}

void element::setClusterWater(cluster *value)
{
    clusterWater = value;
}
cluster *element::getClusterOil() const
{
    return clusterOil;
}

void element::setClusterOil(cluster *value)
{
    clusterOil = value;
}
cluster *element::getClusterGas() const
{
    return clusterGas;
}

void element::setClusterGas(cluster *value)
{
    clusterGas = value;
}
bool element::getWaterTrapped() const
{
    return waterTrapped;
}

void element::setWaterTrapped(bool value)
{
    waterTrapped = value;
}
bool element::getOilTrapped() const
{
    return oilTrapped;
}

void element::setOilTrapped(bool value)
{
    oilTrapped = value;
}
bool element::getClosed() const
{
    return closed;
}

void element::setClosed(bool value)
{
    closed = value;
    exist = value==true?'f':'t';
}
bool element::getInlet() const
{
    return inlet;
}

void element::setInlet(bool value)
{
    inlet = value;
}
bool element::getOutlet() const
{
    return outlet;
}

void element::setOutlet(bool value)
{
    outlet = value;
}
int element::getType() const
{
    return type;
}

void element::setType(int value)
{
    type = value;
}
bool element::getGasTrapped() const
{
    return gasTrapped;
}

void element::setGasTrapped(bool value)
{
    gasTrapped = value;
}

double element::getShapeFactorConstant() const
{
    return shapeFactorConstant;
}

void element::setShapeFactorConstant(double value)
{
    shapeFactorConstant = value;
}
char element::getOilOrGas() const
{
    return (getPhaseFlag()=='o' || getPhaseFlag()=='g')?'t':'f';
}

char element::getWaterOrGas() const
{
    return (getPhaseFlag()=='w' || getPhaseFlag()=='g')?'t':'f';
}

double element::getOilFraction() const
{
    return oilFraction;
}

void element::setOilFraction(double value)
{
    oilFraction = value;
}
double element::getWaterFraction() const
{
    return waterFraction;
}

void element::setWaterFraction(double value)
{
    waterFraction = value;
}

cluster *element::getClusterOilOrGas() const
{
    return clusterOilOrGas;
}

void element::setClusterOilOrGas(cluster *value)
{
    clusterOilOrGas = value;
}
cluster *element::getClusterWaterOrGas() const
{
    return clusterWaterOrGas;
}

void element::setClusterWaterOrGas(cluster *value)
{
    clusterWaterOrGas = value;
}
double element::getThetaOG() const
{
    return thetaOG;
}

void element::setThetaOG(double value)
{
    thetaOG = value;
}
double element::getThetaWG() const
{
    return thetaWG;
}

void element::setThetaWG(double value)
{
    thetaWG = value;
}

double element::getBeta1() const
{
    return beta1;
}

void element::setBeta1(double value)
{
    beta1 = value;
}
double element::getBeta2() const
{
    return beta2;
}

void element::setBeta2(double value)
{
    beta2 = value;
}
double element::getBeta3() const
{
    return beta3;
}

void element::setBeta3(double value)
{
    beta3 = value;
}
double element::getEffectiveVolume() const
{
    return effectiveVolume;
}

void element::setEffectiveVolume(double value)
{
    effectiveVolume = value;
}
bool element::getFilm1() const
{
    return film1;
}

void element::setFilm1(bool value)
{
    film1 = value;
}
bool element::getFilm2() const
{
    return film2;
}

void element::setFilm2(bool value)
{
    film2 = value;
}
bool element::getFilm3() const
{
    return film3;
}

void element::setFilm3(bool value)
{
    film3 = value;
}


double element::getFd() const
{
    return fd;
}

void element::setFd(double value)
{
    fd = value;
}

double element::getSnapOffCoefficient() const
{
    return snapOffCoefficient;
}

void element::setSnapOffCoefficient(double value)
{
    snapOffCoefficient = value;
}
bool element::getFilm1OG() const
{
    return film1OG;
}

void element::setFilm1OG(bool value)
{
    film1OG = value;
}
bool element::getFilm2OG() const
{
    return film2OG;
}

void element::setFilm2OG(bool value)
{
    film2OG = value;
}
bool element::getFilm3OG() const
{
    return film3OG;
}

void element::setFilm3OG(bool value)
{
    film3OG = value;
}
bool element::getFilm1WG() const
{
    return film1WG;
}

void element::setFilm1WG(bool value)
{
    film1WG = value;
}
bool element::getFilm2WG() const
{
    return film2WG;
}

void element::setFilm2WG(bool value)
{
    film2WG = value;
}
bool element::getFilm3WG() const
{
    return film3WG;
}

void element::setFilm3WG(bool value)
{
    film3WG = value;
}
double element::getFdOG() const
{
    return fdOG;
}

void element::setFdOG(double value)
{
    fdOG = value;
}
double element::getFdWG() const
{
    return fdWG;
}

void element::setFdWG(double value)
{
    fdWG = value;
}
double element::getSnapOffCoefficientOG() const
{
    return snapOffCoefficientOG;
}

void element::setSnapOffCoefficientOG(double value)
{
    snapOffCoefficientOG = value;
}
double element::getSnapOffCoefficientWG() const
{
    return snapOffCoefficientWG;
}

void element::setSnapOffCoefficientWG(double value)
{
    snapOffCoefficientWG = value;
}

cluster *element::getClusterExist() const
{
    return clusterExist;
}

void element::setClusterExist(cluster *value)
{
    clusterExist = value;
}
char element::getExist() const
{
    return exist;
}

void element::setExist(char value)
{
    exist = value;
}

char element::getOilFlowingViaFilm() const
{
    if((getPhaseFlag()=='o' && (getWettabilityFlag()=='o'))
            || (getPhaseFlag()=='w' && getWettabilityFlag()=='o' && (getFilm1() || getFilm2() || getFilm3()))
            || (getPhaseFlag()=='g' && getWettabilityFlag()=='o' && (getFilm1OG() || getFilm2OG() || getFilm3OG()))
            )
        return 't';
    else
        return 'f';
}

char element::getWaterFlowingViaFilm() const
{
    if((getPhaseFlag()=='w' && (getWettabilityFlag()=='w'))
            || (getPhaseFlag()=='o' && getWettabilityFlag()=='w' && (getFilm1() || getFilm2() || getFilm3()))
            || (getPhaseFlag()=='g' && getWettabilityFlag()=='w' && (getFilm1WG() || getFilm2WG() || getFilm3WG()))
            )
        return 't';
    else
        return 'f';
}

char element::getOilFlowingViaLayer() const
{
    if(getPhaseFlag()=='o'
            || (getPhaseFlag()=='w' && getWettabilityFlag()=='o' && (getFilm1() || getFilm2() || getFilm3()))
            || (getPhaseFlag()=='g' && getWettabilityFlag()=='o' && (getFilm1OG() || getFilm2OG() || getFilm3OG()))
            || (getPhaseFlag()=='g' && getWettabilityFlag()=='w' && getOilLayer() && (getFilm1OG() && getFilm1() || getFilm2OG() && getFilm2() || getFilm3OG() && getFilm3()))
            )
        return 't';
    else
        return 'f';
}

char element::getWaterFlowingViaLayer() const
{
    if(getPhaseFlag()=='w'
            || (getPhaseFlag()=='o' && getWettabilityFlag()=='w' && (getFilm1() || getFilm2() || getFilm3()))
            || (getPhaseFlag()=='g' && getWettabilityFlag()=='w' && (getFilm1WG() || getFilm2WG() || getFilm3WG()))
            || (getPhaseFlag()=='g' && getWettabilityFlag()=='o' && getWaterLayer() && (getFilm1WG() && getFilm1() || getFilm2WG() && getFilm2() || getFilm3WG() && getFilm3()))
            )
        return 't';
    else
        return 'f';
}

bool element::getOilLayer() const
{
    return oilLayer;
}

void element::setOilLayer(bool value)
{
    oilLayer = value;
}

bool element::getWaterLayer() const
{
    return waterLayer;
}

void element::setWaterLayer(bool value)
{
    waterLayer = value;
}

cluster *element::getClusterWaterFilm() const
{
    return clusterWaterFilm;
}

void element::setClusterWaterFilm(cluster *value)
{
    clusterWaterFilm = value;
}

cluster *element::getClusterOilFilm() const
{
    return clusterOilFilm;
}

void element::setClusterOilFilm(cluster *value)
{
    clusterOilFilm = value;
}

cluster *element::getClusterWaterLayer() const
{
    return clusterWaterLayer;
}

void element::setClusterWaterLayer(cluster *value)
{
    clusterWaterLayer = value;
}

cluster *element::getClusterOilLayer() const
{
    return clusterOilLayer;
}

void element::setClusterOilLayer(cluster *value)
{
    clusterOilLayer = value;
}

bool element::getOilLayerFlowing() const
{
    return getPhaseFlag()=='g' && getOilLayer() && getWettabilityFlag()=='w' && (getFilm1OG() && getFilm1() || getFilm2OG() && getFilm2()|| getFilm3OG() && getFilm3());
}

void element::setOilLayerFlowing(bool value)
{
    oilLayerFlowing = value;
}

bool element::getWaterLayerFlowing() const
{
    return getPhaseFlag()=='g' &&  getWaterLayer() && getWettabilityFlag()=='o' && (getFilm1WG() && getFilm1() || getFilm2WG() && getFilm2()|| getFilm3WG() && getFilm3());
}

void element::setWaterLayerFlowing(bool value)
{
    waterLayerFlowing = value;
}

bool element::getOilFilmFlowing() const
{
    return (getPhaseFlag()=='w' && getWettabilityFlag()=='o' && (getFilm1() || getFilm2() || getFilm3()))
            || (getPhaseFlag()=='g' && getWettabilityFlag()=='o' && (getFilm1OG() || getFilm2OG() || getFilm3OG()));
}

void element::setOilFilmFlowing(bool value)
{
    oilFilmFlowing = value;
}

bool element::getWaterFilmFlowing() const
{
    return (getPhaseFlag()=='o' && getWettabilityFlag()=='w' && (getFilm1() || getFilm2() || getFilm3()))
            || (getPhaseFlag()=='g' && getWettabilityFlag()=='w' && (getFilm1WG() || getFilm2WG() || getFilm3WG()));
}

void element::setWaterFilmFlowing(bool value)
{
    waterFilmFlowing = value;
}

double element::getFilmVolume() const
{
    return filmVolume;
}

void element::setFilmVolume(double value)
{
    filmVolume = value;
}

double element::getFilmConductivity() const
{
    return filmConductivity;
}

void element::setFilmConductivity(double value)
{
    filmConductivity = value;
}
double element::getLayerConductivity() const
{
    return getLayerConductivity();
}

void element::setLayerConductivity(double value)
{
    layerConductivity = value;
}



























