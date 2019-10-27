/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef WIDGET3D_H
#define WIDGET3D_H

#include <QOpenGLWidget>
#include <libs/glm/glm.hpp>
#include <memory>

namespace PNM {
class networkModel;
}

class Shader;
class QMouseEvent;
class QTimer;

class widget3d : public QOpenGLWidget {
  Q_OBJECT
 public:
  widget3d(QWidget *parent = nullptr);
  void setNetwork(const std::shared_ptr<PNM::networkModel> &value);
  void setAspectRatio();

  // getters and setters

  bool getNetworkBuilt() const;
  void setNetworkBuilt(bool value);

  bool getAxes() const;
  void setAxes(bool value);

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

  glm::vec3 &getOilColor();
  void setOilColor(const glm::vec3 &value);

  glm::vec3 &getWaterColor();
  void setWaterColor(const glm::vec3 &value);

  glm::vec3 &getTracerColor();
  void setTracerColor(const glm::vec3 &value);

  void setSimulationRunnning(bool value);

 public slots:
  void timerUpdate();

 protected:
  template <typename T>
  void uploadDataToGPU(GLuint buffer, std::vector<T> h_data,
                       const unsigned count, GLenum target, GLenum access);
  void initialiseDataBuffers();
  void bufferCylinderDynamicData();
  void bufferCylinderStaticData();
  void bufferCylinderIndicesData();
  void bufferLineDynamicData();
  void bufferLineStaticData();
  void bufferLinesIndicesData();
  void bufferSphereStaticData();
  void bufferSphereDynamicData();
  void bufferSphereIndicesData();

  void bufferAxesData();
  void loadShaderUniforms(Shader *shader);
  void loadShaderUniformsAxes(Shader *shader);
  void drawSpheres();
  void drawCylinders();
  void drawLines();
  void drawAxes();
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  double xRot, yRot, zRot, xTran, yTran, scale, xInitRot, yInitRot, xInitTran,
      yInitTran, zInitTran, aspect, cutXValue, cutYValue, cutZValue;
  int sphereCount, cylinderCount, lineCount;
  bool networkBuilt, simulationRunnning, buffersAllocated, refreshRequested,
      axes, animation, poreBodies, nodeBodies, poreLines, oilVisible,
      waterVisible, waterWetVisible, oilWetVisible, cutX, cutY, cutZ;
  glm::vec4 backgroundColor;
  glm::vec3 oilColor, waterColor, tracerColor;
  QPoint lastPos;
  // data buffers
  std::vector<GLfloat> dynamicSphereBuffer, staticSphereBuffer,
      dynamicCylinderBuffer, staticCylinderBuffer, dynamicLineBuffer,
      staticLineBuffer;
  std::vector<GLint> sphereIndicesBuffer, cylinderIndicesBuffer,
      lineIndicesBuffer;
  // shader attributes
  unsigned int dynamicSphereVBO, staticSphereVBO, sphereIndicesVBO, sphereVAO,
      dynamicCylinderVBO, staticCylinderVBO, cylinderIndicesVBO, cylinderVAO,
      dynamicLineVBO, staticLineVBO, lineIndicesVBO, lineVAO, axesVBO, axesVAO;
  // matrices
  glm::mat4 view, viewInv, projection;
  // shaders
  std::shared_ptr<Shader> sphereShader, cylinderShader, lineShader;
  // network model
  std::shared_ptr<PNM::networkModel> network;
  // timer
  std::shared_ptr<QTimer> timer;
};

#endif  // WIDGET3D_H
