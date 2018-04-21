/////////////////////////////////////////////////////////////////////////////
/// Name:        element.h
/// Purpose:     attributes and methods of capillary elements (pores/ nodes)
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef ELEMENT_H
#define ELEMENT_H

#include"cluster.h"
#include <vector>

namespace PNM {

enum class phase {oil, water, temp, invalid};
enum class wettability {oilWet, waterWet, invalid};
enum class capillaryType {throat, poreBody};

class element
{
public:
    element();
    virtual ~element() {}

    int getId() const {return id;}
    void setId(int value) {id = value;}

    int getAbsId() const {return absId;}
    void setAbsId(int value) {absId=value;}

    capillaryType getType() const {return type;}

    bool getClosed() const {return closed;}
    void setClosed(bool value);

    bool getActive() const {return active;}
    void setActive(bool value) {active=value;}

    bool getInlet() const {return inlet;}
    void setInlet(bool value) {inlet=value;}

    bool getOutlet() const {return outlet;}
    void setOutlet(bool value) {outlet=value;}

    double getRadius() const {return radius;}
    void setRadius(double value) {radius=value;}

    double getLength() const {return length;}
    void setLength(double value) {length=value;}

    double getVolume() const {return volume;}
    void setVolume(double value) {volume=value;}

    double getShapeFactor() const {return shapeFactor;}
    void setShapeFactor(double value) {shapeFactor=value;}

    double getShapeFactorConstant() const {return shapeFactorConstant;}
    void setShapeFactorConstant(double value) {shapeFactorConstant=value;}

    double getConductivity() const {return conductivity;}
    void setConductivity(double value) {conductivity=value;}

    double getTheta() const {return theta;}
    void setTheta(double value) {theta=value;}

    double getOriginalTheta() const {return originalTheta;}
    void setOriginalTheta(double value) {originalTheta=value;}

    wettability getWettabilityFlag() const {return wettabilityFlag;}
    void setWettabilityFlag(wettability value) {wettabilityFlag=value;}

    phase getPhaseFlag() const {return phaseFlag;}
    void setPhaseFlag(phase value) {phaseFlag=value;}

    std::vector<element *>& getNeighboors() {return neighboors;}
    void setNeighboors(const std::vector<element *> &value) {neighboors=value;}

    double getConcentration() const {return concentration;}
    void setConcentration(double value) {concentration=value;}

    double getViscosity() const {return viscosity;}
    void setViscosity(double value) {viscosity=value;}

    double getOilFraction() const {return oilFraction;}
    void setOilFraction(double value) {oilFraction=value;}

    double getWaterFraction() const {return waterFraction;}
    void setWaterFraction(double value) {waterFraction=value;}

    bool getWaterTrapped() const {return waterTrapped;}
    void setWaterTrapped(bool value) {waterTrapped=value;}

    bool getOilTrapped() const {return oilTrapped;}
    void setOilTrapped(bool value) {oilTrapped=value;}

    double getFlow() const {return flow;}
    void setFlow(double value) {flow=value;}

    double getMassFlow() const {return massFlow;}
    void setMassFlow(double value) {massFlow=value;}

    double getBeta1() const {return beta1;}
    void setBeta1(double value) {beta1=value;}

    double getBeta2() const {return beta2;}
    void setBeta2(double value) {beta2=value;}

    double getBeta3() const {return beta3;}
    void setBeta3(double value) {beta3=value;}

    double getEffectiveVolume() const {return effectiveVolume;}
    void setEffectiveVolume(double value) {effectiveVolume=value;}

    double getOilFilmVolume() const {return oilFilmVolume;}
    void setOilFilmVolume(double value) {oilFilmVolume=value;}

    double getWaterFilmVolume() const {return waterFilmVolume;}
    void setWaterFilmVolume(double value) {waterFilmVolume=value;}

    double getFilmAreaCoefficient() const {return filmAreaCoefficient;}
    void setFilmAreaCoefficient(double value) {filmAreaCoefficient=value;}

    bool getOilCanFlowViaFilm() const {return oilCanFlowViaFilm;}
    void setOilCanFlowViaFilm(bool value) {oilCanFlowViaFilm=value;}

    bool getWaterCanFlowViaFilm() const {return waterCanFlowViaFilm;}
    void setWaterCanFlowViaFilm(bool value) {waterCanFlowViaFilm=value;}

    bool getWaterCornerActivated() const {return waterCornerActivated;}
    void setWaterCornerActivated(bool value) {waterCornerActivated=value;}

    bool getOilLayerActivated() const {return oilLayerActivated;}
    void setOilLayerActivated(bool value) {oilLayerActivated=value;}

    bool getWaterConductor() const {return waterConductor;}
    void setWaterConductor(bool value) {waterConductor=value;}

    bool getOilConductor() const {return oilConductor;}
    void setOilConductor(bool value) {oilConductor=value;}

    double getOilFilmConductivity() const {return oilFilmConductivity;}
    void setOilFilmConductivity(double value) {oilFilmConductivity=value;}

    double getWaterFilmConductivity() const {return waterFilmConductivity;}
    void setWaterFilmConductivity(double value) {waterFilmConductivity=value;}

    // clustering methods
    int getClusterTemp() const {return clusterTemp;}
    void setClusterTemp(int value) {clusterTemp=value;}

    cluster *getClusterActive() const {return clusterActive;}
    void setClusterActive(cluster *value) {clusterActive=value;}

    cluster *getClusterWaterWet() const {return clusterWaterWet;}
    void setClusterWaterWet(cluster *value) {clusterWaterWet=value;}

    cluster *getClusterOilWet() const {return clusterOilWet;}
    void setClusterOilWet(cluster *value) {clusterOilWet=value;}

    cluster *getClusterWater() const {return clusterWater;}
    void setClusterWater(cluster *value) {clusterWater=value;}

    cluster *getClusterOil() const {return clusterOil;}
    void setClusterOil(cluster *value) {clusterOil=value;}

    cluster *getClusterWaterFilm() const {return clusterWaterFilm;}
    void setClusterWaterFilm(cluster *value) {clusterWaterFilm=value;}

    cluster *getClusterOilFilm() const {return clusterOilFilm;}
    void setClusterOilFilm(cluster *value) {clusterOilFilm=value;}

    // defined methods
    void assignViscosity(double oilViscosity, double waterViscosity);

protected:
    capillaryType type; // type of the capillary element: pore (throat) or pore body (node)

    //Basic attributes
    int id; // capillary relative ID: from 1 to totalPores (if pore); from 1 to totalNodes (if node)
    int absId; // capillary absolute ID : from 0 to (totalPores+totalNodes)-1
    double radius; // capillary radius (SI)
    double length; // capillary length (SI)
    double volume; // capillary volume (SI)
    double shapeFactor; // capillary shape factor (dimensionless)
    double shapeFactorConstant; // capillary shape factor constant (dimensionless)
    double conductivity; // capillary conductivity (SI)
    double theta, originalTheta; // capillary oil-water contact angle
    double viscosity; // capillary average viscosity (SI)
    double concentration; // capillary concentration in tracer (between 0 and 1)
    wettability wettabilityFlag; // capillary wettability
    phase phaseFlag; // capillary occupying phase
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
    bool oilConductor, waterConductor; // flags whether a fluid can flow through the capillary - through bulk OR film
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
