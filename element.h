/////////////////////////////////////////////////////////////////////////////
/// Name:        element.h
/// Purpose:     attributes and methods of capillary elements (pores/ nodes)
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
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

    bool getGasTrapped() const;
    void setGasTrapped(bool value);

    double getConcentration() const;
    void setConcentration(double value);

    double getCavityRadius() const;
    void setCavityRadius(double value);

    double getViscosity() const;
    void setViscosity(double value);

    double getShapeFactorConstant() const;
    void setShapeFactorConstant(double value);

    char getOilOrGas() const;
    char getWaterOrGas() const;

    double getGasFraction() const;
    void setGasFraction(double value);

    double getBuoyancyPressure() const;
    void setBuoyancyPressure(double value);

    double getHeadPressure() const;
    void setHeadPressure(double value);

    double getMassOfGasInBubble() const;
    void setMassOfGasInBubble(double value);

    double getBubblePressure() const;
    void setBubblePressure(double value);

    bool getTooSmall() const;
    void setTooSmall(bool value);

    double getOilFraction() const;
    void setOilFraction(double value);

    double getWaterFraction() const;
    void setWaterFraction(double value);

    double getOldOilFraction() const;
    void setOldOilFraction(double value);

    double getOldWaterFraction() const;
    void setOldWaterFraction(double value);

    double getMaxFilmFraction() const;
    void setMaxFilmFraction(double value);

    double getOilFlowIn() const;
    void setOilFlowIn(double value);

    double getWaterFlowIn() const;
    void setWaterFlowIn(double value);

    double getOilFlowOut() const;
    void setOilFlowOut(double value);

    double getWaterFlowOut() const;
    void setWaterFlowOut(double value);

    double getFlow() const;
    void setFlow(double value);

    double getMassFlow() const;
    void setMassFlow(double value);

    double getSurfaceTension() const;
    void setSurfaceTension(double value);

    cluster *getClusterOilOrGas() const;
    void setClusterOilOrGas(cluster *value);

    cluster *getClusterWaterOrGas() const;
    void setClusterWaterOrGas(cluster *value);

    double getThetaOG() const;
    void setThetaOG(double value);

    double getThetaWG() const;
    void setThetaWG(double value);

    bool getVisited() const;
    void setVisited(bool value);

    bool getIsNeighboor() const;
    void setIsNeighboor(bool value);

    bool getClay() const;
    void setClay(bool value);

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

    double getFd() const;
    void setFd(double value);

    double getFilmVolume() const;
    void setFilmVolume(double value);

    double getSnapOffCoefficient() const;
    void setSnapOffCoefficient(double value);

    bool getFilm1OG() const;
    void setFilm1OG(bool value);

    bool getFilm2OG() const;
    void setFilm2OG(bool value);

    bool getFilm3OG() const;
    void setFilm3OG(bool value);

    bool getFilm1WG() const;
    void setFilm1WG(bool value);

    bool getFilm2WG() const;
    void setFilm2WG(bool value);

    bool getFilm3WG() const;
    void setFilm3WG(bool value);

    double getFdOG() const;
    void setFdOG(double value);

    double getFdWG() const;
    void setFdWG(double value);

    double getSnapOffCoefficientOG() const;
    void setSnapOffCoefficientOG(double value);

    double getSnapOffCoefficientWG() const;
    void setSnapOffCoefficientWG(double value);

    double getPolymerConcentration() const;
    void setPolymerConcentration(double value);


    double getPolymerMassFlow() const;
    void setPolymerMassFlow(double value);

    bool getPolymerShut() const;
    void setPolymerShut(bool value);

    double getVesselType() const;
    void setVesselType(double value);

    cluster *getClusterExist() const;
    void setClusterExist(cluster *value);


    char getExist() const;
    void setExist(char value);

    std::map<element *, double> &getNeighsArea();
    void setNeighsArea(const std::map<element *, double> &value);

    double getVolume2() const;
    void setVolume2(double value);

    bool getInitialWater() const;
    void setInitialWater(bool value);

    char getOilFlowingViaFilm() const;
    char getWaterFlowingViaFilm() const;

    char getOilFlowingViaLayer() const;
    char getWaterFlowingViaLayer() const;


    bool getOilLayer() const;
    void setOilLayer(bool value);

    bool getWaterLayer() const;
    void setWaterLayer(bool value);

    cluster *getClusterWaterFilm() const;
    void setClusterWaterFilm(cluster *value);

    cluster *getClusterOilFilm() const;
    void setClusterOilFilm(cluster *value);

    cluster *getClusterWaterLayer() const;
    void setClusterWaterLayer(cluster *value);

    cluster *getClusterOilLayer() const;
    void setClusterOilLayer(cluster *value);

    bool getOilLayerFlowing() const;
    void setOilLayerFlowing(bool value);

    bool getWaterLayerFlowing() const;
    void setWaterLayerFlowing(bool value);

    bool getOilFilmFlowing() const;
    void setOilFilmFlowing(bool value);

    bool getWaterFilmFlowing() const;
    void setWaterFilmFlowing(bool value);

    double getFilmConductivity() const;
    void setFilmConductivity(double value);

    double getLayerConductivity() const;
    void setLayerConductivity(double value);

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
    double thetaOG;
    double thetaWG;
    char wettabilityFlag;
    char phaseFlag;
    double oilFraction;
    double waterFraction;
    double viscosity;

    bool inlet;
    bool outlet;

    bool closed;

    std::vector<element*> neighs;

    int clusterTemp;
    cluster* clusterWaterWet;
    cluster* clusterOilWet;
    cluster* clusterWater;
    cluster* clusterOil;
    cluster* clusterGas;
    cluster* clusterOilOrGas;
    cluster* clusterWaterOrGas;
    cluster* clusterWaterFilm;
    cluster* clusterOilFilm;
    cluster* clusterWaterLayer;
    cluster* clusterOilLayer;
    cluster* clusterExist;

    bool waterTrapped;
    bool oilTrapped;
    bool gasTrapped;

    double beta1,beta2,beta3;
    bool film1,film2,film3,film1OG,film2OG,film3OG,film1WG,film2WG,film3WG;
    double fd,fdOG,fdWG;
    double snapOffCoefficient,snapOffCoefficientOG,snapOffCoefficientWG;
    double effectiveVolume;

    bool oilLayer;
    bool waterLayer;
    bool oilLayerFlowing;
    bool waterLayerFlowing;
    bool oilFilmFlowing;
    bool waterFilmFlowing;

    double filmVolume;
    double filmConductivity;
    double layerConductivity;

    char exist;
};

#endif // ELEMENT_H
