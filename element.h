/////////////////////////////////////////////////////////////////////////////
/// Name:        element.h
/// Purpose:     attributes and methods of capillary elements (pores/ nodes)
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef ELEMENT_H
#define ELEMENT_H

#include"cluster.h"
#include <vector>

namespace PNM {

enum class phase{oil, water, temp, invalid};
enum class wettability{oilWet, waterWet, invalid};

class element
{
public:
    element();
    virtual ~element() {}

    int getId() const;
    void setId(int value);

    int getAbsId() const;
    void setAbsId(int value);

    double getRadius() const;
    void setRadius(double value);

    double getLength() const;
    void setLength(double value);

    double getVolume() const;
    void setVolume(double value);

    double getShapeFactor() const;
    void setShapeFactor(double value);

    double getConductivity() const;
    void setConductivity(double value);

    double getTheta() const;
    void setTheta(double value);

    wettability getWettabilityFlag() const;
    void setWettabilityFlag(wettability value);

    phase getPhaseFlag() const;
    void setPhaseFlag(phase value);

    std::vector<element *> getNeighboors() const;
    void setNeighboors(const std::vector<element *> &value);

    int getClusterTemp() const;
    void setClusterTemp(int value);

    cluster *getClusterWaterWet() const;
    void setClusterWaterWet(cluster *value);

    cluster *getClusterOilWet() const;
    void setClusterOilWet(cluster *value);

    cluster *getClusterWater() const;
    void setClusterWater(cluster *value);

    cluster *getClusterOil() const;
    void setClusterOil(cluster *value);

    cluster *getClusterActive() const;
    void setClusterActive(cluster *value);

    cluster *getClusterWaterFilm() const;
    void setClusterWaterFilm(cluster *value);

    cluster *getClusterOilFilm() const;
    void setClusterOilFilm(cluster *value);

    bool getWaterTrapped() const;
    void setWaterTrapped(bool value);

    bool getOilTrapped() const;
    void setOilTrapped(bool value);

    bool getClosed() const;
    void setClosed(bool value);

    bool getInlet() const;
    void setInlet(bool value);

    bool getOutlet() const;
    void setOutlet(bool value);

    int getType() const;
    void setType(int value);

    bool getActive() const;
    void setActive(bool value);

    double getConcentration() const;
    void setConcentration(double value);

    double getViscosity() const;
    void setViscosity(double value);

    double getShapeFactorConstant() const;
    void setShapeFactorConstant(double value);

    double getOilFraction() const;
    void setOilFraction(double value);

    double getWaterFraction() const;
    void setWaterFraction(double value);

    double getFlow() const;
    void setFlow(double value);

    double getBeta1() const;
    void setBeta1(double value);

    double getBeta2() const;
    void setBeta2(double value);

    double getBeta3() const;
    void setBeta3(double value);

    double getEffectiveVolume() const;
    void setEffectiveVolume(double value);

    double getOilFilmVolume() const;
    void setOilFilmVolume(double value);

    double getWaterFilmVolume() const;
    void setWaterFilmVolume(double value);

    double getFilmAreaCoefficient() const;
    void setFilmAreaCoefficient(double value);

    bool getOilCanFlowViaFilm() const;
    void setOilCanFlowViaFilm(bool value);

    bool getWaterCanFlowViaFilm() const;
    void setWaterCanFlowViaFilm(bool value);

    bool getWaterCornerActivated() const;
    void setWaterCornerActivated(bool value);

    bool getOilLayerActivated() const;
    void setOilLayerActivated(bool value);

    double getOilFilmConductivity() const;
    void setOilFilmConductivity(double value);

    double getWaterFilmConductivity() const;
    void setWaterFilmConductivity(double value);

    bool getOilFlowing() const;
    bool getWaterFlowing() const;

    double getMassFlow() const;
    void setMassFlow(double value);

    // defined methods
    void assignViscosity(double oilViscosity, double waterViscosity);

protected:
    int type;

    //Basic attributes
    int id; // capillary relative ID: from 1 to totalPores (if pore); from 1 to totalNodes (if node)
    int absId; // capillary absolute ID : from 0 to (totalPores+totalNodes)-1
    double radius; // capillary radius (SI)
    double length; // capillary length (SI)
    double volume; // capillary volume (SI)
    double shapeFactor; // capillary shape factor (dimensionless)
    double shapeFactorConstant; // capillary shape factor constant (dimensionless)
    double conductivity; // capillary conductivity (SI)
    double theta; // capillary oil-water contact angle
    wettability wettabilityFlag; // capillary wettability
    phase phaseFlag; // capillary occupying phase
    double viscosity; // capillary average viscosity (SI)
    double concentration; // capillary concentration in tracer (between 0 and 1)
    bool inlet; // a flag whether the capillary is connected to the inlet boundary
    bool outlet; // a flag whether the capillary is connected to the outlet boundary
    bool closed; // a flag whether the capillary is undefinetely closed (i.e. when assigning the coordination number)
    bool active; // a flag whether the capillary is momentarily closed (i.e. when closing the capillaries with counter imbibition flow)

    std::vector<element*> neighboors; // a table to the adjacent capillaries

    //Simulation attributes

    double oilFraction; // oil fraction in the capillary
    double waterFraction; // water fraction in the capillary

    bool waterTrapped; // a flag to whether water is topologically trapped in the capillary
    bool oilTrapped; // a flag to whether oil is topologically trapped in the capillary

    double flow; // fluid flow (SI) in the capillary
    double massFlow; // mass flow (SI) in the capillary

    double beta1,beta2,beta3; // half angles in the capillary
    bool oilCanFlowViaFilm, waterCanFlowViaFilm; // flags whether a fluid can flow via layer/film
    bool oilLayerActivated, waterCornerActivated; // flags whether a fluid can flow via layer/film
    double oilFilmVolume, waterFilmVolume; // layer/film volumes
    double oilFilmConductivity, waterFilmConductivity; // layer/film conductivity
    double effectiveVolume; // bulk volume (volume - (film+layer) volume)
    double filmAreaCoefficient; // a mathematical coefficient used in the calculation of film area (Oren, 98)

    //Clustering attributes
    int clusterTemp;
    cluster* clusterWaterWet;
    cluster* clusterOilWet;
    cluster* clusterWater;
    cluster* clusterOil;
    cluster* clusterWaterFilm;
    cluster* clusterOilFilm;
    cluster* clusterActive;
};

}

#endif // ELEMENT_H
