/////////////////////////////////////////////////////////////////////////////
/// Name:        element.cpp
/// Purpose:     attributes and methods of capillary elements (pores/ nodes)
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "element.h"

element::element()
{
    radius=0;
    length=0;
    theta=0;
    wettabilityFlag='c';
    viscosity=1;
    phaseFlag='o';
    volume=0;
    conductivity=0;
    concentration=0;
    flow=0;
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

double element::getConcentration() const
{
    return concentration;
}

void element::setConcentration(double value)
{
    concentration = value;
}

double element::getViscosity() const
{
    return viscosity;
}

void element::setViscosity(double value)
{
    viscosity = value;
}
double element::getShapeFactorConstant() const
{
    return shapeFactorConstant;
}

void element::setShapeFactorConstant(double value)
{
    shapeFactorConstant = value;
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
double element::getOldOilFraction() const
{
    return oldOilFraction;
}

void element::setOldOilFraction(double value)
{
    oldOilFraction = value;
}
double element::getOldWaterFraction() const
{
    return oldWaterFraction;
}

void element::setOldWaterFraction(double value)
{
    oldWaterFraction = value;
}

double element::getFlow() const
{
    return flow;
}

void element::setFlow(double value)
{
    flow = value;
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

char element::getOilFlowing() const
{
    if(!closed && (phaseFlag=='o' || oilLayerActivated))
        return 't';
    else
        return 'f';
}

char element::getWaterFlowing() const
{
    if(!closed && (phaseFlag=='w' || waterCornerActivated))
        return 't';
    else
        return 'f';
}

void element::assignViscosity(double oilViscosity, double oilFraction, double waterViscosity, double waterFraction)
{
    viscosity=oilViscosity*oilFraction+waterViscosity*waterFraction;
}

double element::getMassFlow() const
{
    return massFlow;
}

void element::setMassFlow(double value)
{
    massFlow = value;
}
double element::getOilFilmVolume() const
{
    return oilFilmVolume;
}

void element::setOilFilmVolume(double value)
{
    oilFilmVolume = value;
}
double element::getWaterFilmVolume() const
{
    return waterFilmVolume;
}

void element::setWaterFilmVolume(double value)
{
    waterFilmVolume = value;
}

double element::getFilmAreaCoefficient() const
{
    return filmAreaCoefficient;
}

void element::setFilmAreaCoefficient(double value)
{
    filmAreaCoefficient = value;
}
bool element::getOilCanFlowViaFilm() const
{
    return oilCanFlowViaFilm;
}

void element::setOilCanFlowViaFilm(bool value)
{
    oilCanFlowViaFilm = value;
}
bool element::getWaterCanFlowViaFilm() const
{
    return waterCanFlowViaFilm;
}

void element::setWaterCanFlowViaFilm(bool value)
{
    waterCanFlowViaFilm = value;
}
bool element::getWaterCornerActivated() const
{
    return waterCornerActivated;
}

void element::setWaterCornerActivated(bool value)
{
    waterCornerActivated = value;
}
bool element::getOilLayerActivated() const
{
    return oilLayerActivated;
}

void element::setOilLayerActivated(bool value)
{
    oilLayerActivated = value;
}
double element::getOilFilmConductance() const
{
    return oilFilmConductance;
}

void element::setOilFilmConductance(double value)
{
    oilFilmConductance = value;
}
double element::getWaterFilmConductance() const
{
    return waterFilmConductance;
}

void element::setWaterFilmConductance(double value)
{
    waterFilmConductance = value;
}

