/////////////////////////////////////////////////////////////////////////////
/// Name:        widget3d.h
/// Purpose:     OpenGL functions are declared and defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef WIDGET3D_H
#define WIDGET3D_H

#include "shader.h"
#include <cmath>
#include <iostream>
#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QOpenGLFramebufferObject>
#include <QApplication>

namespace PNM {
    class network;
}

class widget3d : public QGLWidget
{
    Q_OBJECT
public :
    widget3d (QWidget *parent=0);

    bool getLight() const;
    void setLight(bool value);

    bool getPoreBodies() const;
    void setPoreBodies(bool value);

    bool getNodeBodies() const;
    void setNodeBodies(bool value);

    bool getPoreLines() const;
    void setPoreLines(bool value);

    bool getOilVisible() const;
    void setOilVisible(bool value);

    bool getWaterVisible() const;
    void setWaterVisible(bool value);

    bool getGasVisible() const;
    void setGasVisible(bool value);

    bool getWaterWetVisible() const;
    void setWaterWetVisible(bool value);

    bool getOilWetVisible() const;
    void setOilWetVisible(bool value);

    double getXRot() const;
    void setXRot(double value);

    double getYRot() const;
    void setYRot(double value);

    double getZRot() const;
    void setZRot(double value);

    double getXTran() const;
    void setXTran(double value);

    double getYTran() const;
    void setYTran(double value);

    double getScale() const;
    void setScale(double value);

    double getAspect() const;
    void setAspect(double value);

    bool getAnimation() const;
    void setAnimation(bool value);

    double getXInitTran() const;
    void setXInitTran(double value);

    double getYInitTran() const;
    void setYInitTran(double value);

    double getZInitTran() const;
    void setZInitTran(double value);

    double getYInitRot() const;
    void setYInitRot(double value);

    double getXInitRot() const;
    void setXInitRot(double value);

    bool getCutX() const;
    void setCutX(bool value);

    bool getCutY() const;
    void setCutY(bool value);

    bool getCutZ() const;
    void setCutZ(bool value);

    double getCutXValue() const;
    void setCutXValue(double value);

    double getCutYValue() const;
    void setCutYValue(double value);

    double getCutZValue() const;
    void setCutZValue(double value);

    std::string getPhasePoresPath() const;
    void setPhasePoresPath(const std::string &value);

    std::string getPhaseNodesPath() const;
    void setPhaseNodesPath(const std::string &value);

    bool getRender() const;
    void setRender(bool value);

    bool getLoad() const;
    void setLoad(bool value);

    bool getUpdate() const;
    void setUpdate(bool value);

    int getTotalImages() const;
    void setTotalImages(int value);

    int getImageCount() const;
    void setImageCount(int value);

    PNM::network *getNet() const;
    void setNet(PNM::network *value);

    glm::vec3 &getPhase1Color();
    void setPhase1Color(const glm::vec3 &value);

    glm::vec3 &getPhase2Color();
    void setPhase2Color(const glm::vec3 &value);

    glm::vec3 &getPhase3Color();
    void setPhase3Color(const glm::vec3 &value);

public slots:
    void timerUpdate();

signals:
    void plotted();
    void rendered();


protected :
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    void uploadDataToGPU(GLuint buffer, float* h_data, const unsigned count, GLenum target, GLenum access);

    unsigned bufferCylinderData();
    unsigned bufferLinesData();
    unsigned bufferSphereData();
    unsigned bufferAxesData();

    void loadShaderUniforms(Shader &shader);
    void loadShaderUniformsAxes(Shader &shader);

    void drawSpheres();
    void drawCylinders();
    void drawLines();
    void drawAxes();

    void initializeGL();
    void resizeGL( int w, int h);
    void paintGL();

    double xRot,yRot,zRot,xTran,yTran,scale,xInitRot,yInitRot,xInitTran,yInitTran,zInitTran,aspect,cutXValue,cutYValue,cutZValue;
    int totalImages,imageCount;
    bool light,animation,update,render,load,poreBodies,nodeBodies,poreLines,oilVisible,waterVisible,gasVisible,waterWetVisible,oilWetVisible,cutX,cutY,cutZ;
    std::string phasePoresPath, phaseNodesPath;
    glm::vec3 phase1Color, phase2Color, phase3Color;
    QPoint lastPos;
    PNM::network* net;
    QTimer timer;
    QOpenGLFramebufferObject *fbo;

    //shader attributes
    unsigned int sphereVBO, sphereVAO, cylinderVBO, cylinderVAO, lineVBO, lineVAO;
    Shader sphereShader, cylinderShader, lineShader;
};

#endif // WIDGET3D_H
