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
#include "tools.h"

class element
{
public:
    element();
    int getId() const;
    void setId(int value);

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

    char getWettabilityFlag() const;
    void setWettabilityFlag(char value);

    char getPhaseFlag() const;
    void setPhaseFlag(char value);

    std::vector<element *> getNeighs() const;
    void setNeighs(const std::vector<element *> &value);

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

    cluster *getClusterGas() const;
    void setClusterGas(cluster *value);

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

    double getOldOilFraction() const;
    void setOldOilFraction(double value);

    double getOldWaterFraction() const;
    void setOldWaterFraction(double value);

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

    bool getFilm1() const;
    void setFilm1(bool value);

    bool getFilm2() const;
    void setFilm2(bool value);

    bool getFilm3() const;
    void setFilm3(bool value);

    cluster *getClusterExist() const;
    void setClusterExist(cluster *value);

    char getExist() const;
    void setExist(char value);

    cluster *getClusterWaterFilm() const;
    void setClusterWaterFilm(cluster *value);

    cluster *getClusterOilFilm() const;
    void setClusterOilFilm(cluster *value);

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

    double getOilFilmConductance() const;
    void setOilFilmConductance(double value);

    double getWaterFilmConductance() const;
    void setWaterFilmConductance(double value);

    char getOilFlowing() const;
    char getWaterFlowing() const;
    void assignViscosity(double oilViscosity, double oilFraction, double waterViscosity, double waterFraction);

    double getMassFlow() const;
    void setMassFlow(double value);

protected:
    int type;

    int id;
    double radius;
    double length;
    double volume;
    double shapeFactor;
    double shapeFactorConstant;
    double conductivity;
    double theta;
    char wettabilityFlag;
    char phaseFlag;
    double viscosity;
    double oilFraction;
    double waterFraction;
    double oldOilFraction;
    double oldWaterFraction;
    double concentration;
    bool inlet;
    bool outlet;

    bool closed;

    std::vector<element*> neighs;

    //Clustering
    int clusterTemp;
    cluster* clusterWaterWet;
    cluster* clusterOilWet;
    cluster* clusterWater;
    cluster* clusterOil;
    cluster* clusterGas;
    cluster* clusterWaterFilm;
    cluster* clusterOilFilm;
    cluster* clusterExist;

    bool waterTrapped;
    bool oilTrapped;

    double flow;
    double massFlow;

    double beta1,beta2,beta3;
    bool film1,film2,film3;
    double effectiveVolume;

    bool oilCanFlowViaFilm, waterCanFlowViaFilm;
    bool waterCornerActivated, oilLayerActivated;
    double oilFilmVolume,waterFilmVolume;
    double oilFilmConductance, waterFilmConductance;
    double filmAreaCoefficient;

    char exist;
};

#endif // ELEMENT_H
