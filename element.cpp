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

namespace PNM {

element::element()
{
    radius=0;
    length=0;
    theta=0;
    wettabilityFlag=wettability::oilWet;
    phaseFlag=phase::oil;
    volume=0;
    viscosity=1;
    conductivity=0;
    concentration=0;
    flow=0;
    closed=false;
    active=true;
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
wettability element::getWettabilityFlag() const
{
    return wettabilityFlag;
}

void element::setWettabilityFlag(wettability value)
{
    wettabilityFlag = value;
}
phase element::getPhaseFlag() const
{
    return phaseFlag;
}

void element::setPhaseFlag(phase value)
{
    phaseFlag = value;
}
std::vector<element *> element::getNeighboors() const
{
    return neighboors;
}

void element::setNeighboors(const std::vector<element *> &value)
{
    neighboors = value;
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
    if(value)
    {
        active = false;
        wettabilityFlag=wettability::invalid;
        phaseFlag=phase::invalid;
        conductivity=1e-200;
    }
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

cluster *element::getClusterActive() const
{
    return clusterActive;
}

void element::setClusterActive(cluster *value)
{
    clusterActive = value;
}
bool element::getActive() const
{
    return active;
}

void element::setActive(bool value)
{
    active = value;
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

double element::getMassFlow() const
{
    return massFlow;
}

void element::setMassFlow(double value)
{
    massFlow = value;
}

int element::getAbsId() const
{
    return absId;
}

void element::setAbsId(int value)
{
    absId = value;
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
double element::getOilFilmConductivity() const
{
    return oilFilmConductivity;
}

void element::setOilFilmConductivity(double value)
{
    oilFilmConductivity = value;
}
double element::getWaterFilmConductivity() const
{
    return waterFilmConductivity;
}

void element::setWaterFilmConductivity(double value)
{
    waterFilmConductivity = value;
}

// Defined methods

bool element::getOilFlowing() const
{
    if(!closed && (phaseFlag==phase::oil || oilLayerActivated))
        return true;
    else
        return false;
}

bool element::getWaterFlowing() const
{
    if(!closed && (phaseFlag==phase::water || waterCornerActivated))
        return true;
    else
        return false;
}

void element::assignViscosity(double oilViscosity, double waterViscosity)
{
    viscosity=oilViscosity*oilFraction+waterViscosity*waterFraction;
}


}
