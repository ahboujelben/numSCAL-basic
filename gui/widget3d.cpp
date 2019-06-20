/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

// this needs to be included first, otherwise
// conflicts with definitions in widget3d.h might arise
#include <libs/glew/include/GL/glew.h>

#include "shader.h"
#include "widget3d.h"

#include "misc/tools.h"
#include "misc/userInput.h"
#include "network/iterator.h"

#include <QApplication>
#include <QMouseEvent>
#include <QTimer>

#include <libs/glm/glm.hpp>

using namespace PNM;
using namespace std;

widget3d::widget3d(QWidget *parent) : QOpenGLWidget(parent) {
  axes = true;
  animation = false;
  networkBuilt = false;
  simulationRunnning = false;
  refreshRequested = false;
  buffersAllocated = false;
  poreBodies = true;
  poreLines = true;
  nodeBodies = true;
  oilVisible = true;
  waterVisible = true;
  waterWetVisible = true;
  oilWetVisible = true;
  cutX = false;
  cutY = false;
  cutZ = false;
  cutXValue = 0.5;
  cutYValue = 0.5;
  cutZValue = 0.5;
  backgroundColor = glm::vec4(16.f / 255.f, 26.f / 255.f, 32.f / 255.f, 0.0f);
  oilColor = glm::vec3(0.9f, 0.35f, 0.2f);
  waterColor = glm::vec3(0.3f, 0.65f, 0.9f);
  tracerColor = glm::vec3(0.65f, 0.95f, 0.15f);

  sphereCount = cylinderCount = lineCount = 0;

  timer = std::make_shared<QTimer>();
  connect(timer.get(), SIGNAL(timeout()), this, SLOT(timerUpdate()));
  timer->start(10);

  sphereShader = std::make_shared<Shader>();
  cylinderShader = std::make_shared<Shader>();
  lineShader = std::make_shared<Shader>();
}

void widget3d::setNetwork(const std::shared_ptr<PNM::networkModel> &value) {
  network = value;
}

void widget3d::setAspectRatio() {
  if (!networkBuilt) return;

  aspect = qMax(qMax(network->xEdgeLength, network->yEdgeLength),
                network->zEdgeLength);
  xInitTran = -0.5 * network->xEdgeLength / aspect + 0.1;
  yInitTran = -0.5 * network->yEdgeLength / aspect;
  zInitTran = -0.5 * network->zEdgeLength / aspect;

  xInitRot = 20. / 180. * 3.14;
  yInitRot = 30. / 180. * 3.14;

  xRot = 0;
  yRot = 0;
  zRot = 0;

  xTran = 0;
  yTran = 0;

  scale = 0.4;

  if (network->is2D) {
    xInitRot = 0;
    yInitRot = 0;
    xInitTran -= 0.1;
    scale = 0.9;
  }
}

template <typename T>
void widget3d::allocateBufferOnGPU(GLuint buffer, const unsigned count,
                                   GLenum target, GLenum access) {
  glBindBuffer(target, buffer);
  glBufferData(target, count * sizeof(T), nullptr, access);
  glBindBuffer(target, 0);
}

template <typename T>
void widget3d::uploadDataToGPU(GLuint buffer, std::vector<T> h_data,
                               const unsigned count, GLenum target) {
  glBindBuffer(target, buffer);
  glBufferSubData(target, 0, count * sizeof(T), (const void *)&h_data[0]);
  glBindBuffer(target, 0);
}

void widget3d::initialiseDataBuffers() {
  // spheres

  int sphereCount = network->totalNodes;
  staticSphereBuffer.clear();
  staticSphereBuffer.resize(4 * sphereCount);
  dynamicSphereBuffer.clear();
  dynamicSphereBuffer.resize(2 * sphereCount);
  sphereIndicesBuffer.clear();
  sphereIndicesBuffer.resize(sphereCount);

  allocateBufferOnGPU<GLfloat>(staticSphereVBO, 4 * sphereCount,
                               GL_ARRAY_BUFFER, GL_STATIC_DRAW);
  allocateBufferOnGPU<GLfloat>(dynamicSphereVBO, 2 * sphereCount,
                               GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
  allocateBufferOnGPU<GLint>(sphereIndicesVBO, sphereCount,
                             GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

  // cylinders

  int cylinderCount = network->totalPores;
  staticCylinderBuffer.clear();
  staticCylinderBuffer.resize(8 * cylinderCount);
  dynamicCylinderBuffer.clear();
  dynamicCylinderBuffer.resize(3 * cylinderCount);
  cylinderIndicesBuffer.clear();
  cylinderIndicesBuffer.resize(cylinderCount);

  allocateBufferOnGPU<GLfloat>(staticCylinderVBO, 8 * cylinderCount,
                               GL_ARRAY_BUFFER, GL_STATIC_DRAW);
  allocateBufferOnGPU<GLfloat>(dynamicCylinderVBO, 2 * cylinderCount,
                               GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
  allocateBufferOnGPU<GLint>(cylinderIndicesVBO, cylinderCount,
                             GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

  // lines

  int lineCount = network->totalPores;
  staticLineBuffer.clear();
  staticLineBuffer.resize(2 * 3 * lineCount);
  dynamicLineBuffer.clear();
  dynamicLineBuffer.resize(2 * 2 * lineCount);
  lineIndicesBuffer.clear();
  lineIndicesBuffer.resize(2 * lineCount);

  allocateBufferOnGPU<GLfloat>(staticLineVBO, 2 * 3 * lineCount,
                               GL_ARRAY_BUFFER, GL_STATIC_DRAW);
  allocateBufferOnGPU<GLfloat>(dynamicLineVBO, 2 * 2 * lineCount,
                               GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
  allocateBufferOnGPU<GLint>(lineIndicesVBO, 2 * lineCount,
                             GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

  // initialise buffers

  bufferSphereStaticData();
  bufferSphereDynamicData();

  bufferCylinderStaticData();
  bufferCylinderDynamicData();

  bufferLineStaticData();
  bufferLineDynamicData();

  buffersAllocated = true;
}

void widget3d::bufferSphereStaticData() {
  unsigned indexStatic(0);

  for (node *p : pnmRange<node>(network)) {
    // center and radius
    staticSphereBuffer[indexStatic] = p->getXCoordinate() / aspect;  // vertex.x
    staticSphereBuffer[indexStatic + 1] =
        p->getYCoordinate() / aspect;  // vertex.y
    staticSphereBuffer[indexStatic + 2] =
        p->getZCoordinate() / aspect;  // vertex.z
    staticSphereBuffer[indexStatic + 3] = p->getRadius() / aspect;

    // update indices
    indexStatic += 4;
  }

  uploadDataToGPU(staticSphereVBO, staticSphereBuffer, 4 * network->totalNodes,
                  GL_ARRAY_BUFFER);
}

void widget3d::bufferSphereDynamicData() {
  unsigned indexDynamic(0);

  for (node *p : pnmRange<node>(network)) {
    // color data
    float colorKey =
        p->getPhaseFlag() == phase::oil || p->getPhaseFlag() == phase::temp ? 0
                                                                            : 1;
    dynamicSphereBuffer[indexDynamic] = colorKey;
    dynamicSphereBuffer[indexDynamic + 1] = float(p->getConcentration());

    // update indices
    indexDynamic += 2;
  }

  uploadDataToGPU(dynamicSphereVBO, dynamicSphereBuffer,
                  2 * network->totalNodes, GL_ARRAY_BUFFER);
}

void widget3d::bufferSphereIndicesData() {
  unsigned index(0);

  for (node *p : pnmRange<node>(network)) {
    if ((p->getPhaseFlag() == phase::invalid) ||
        (p->getPhaseFlag() == phase::oil && !oilVisible) ||
        (p->getPhaseFlag() == phase::temp && !oilVisible) ||
        (p->getPhaseFlag() == phase::water && !waterVisible) ||
        (p->getWettabilityFlag() == wettability::waterWet &&
         !waterWetVisible) ||
        (p->getWettabilityFlag() == wettability::oilWet && !oilWetVisible) ||
        (cutX && p->getXCoordinate() > cutXValue * network->xEdgeLength) ||
        (cutY && p->getYCoordinate() > cutYValue * network->yEdgeLength) ||
        (cutZ && p->getZCoordinate() > cutZValue * network->zEdgeLength))
      continue;

    sphereIndicesBuffer[index++] = p->getId() - 1;
  }

  sphereCount = index;
  if (index != 0) {
    uploadDataToGPU(sphereIndicesVBO, sphereIndicesBuffer, index,
                    GL_ELEMENT_ARRAY_BUFFER);
  }
}

void widget3d::bufferCylinderStaticData() {
  unsigned indexStatic(0);

  for (pore *p : pnmRange<pore>(network)) {
    if (p->getInlet() || p->getOutlet()) {
      indexStatic += 8;
      continue;
    }

    // center
    staticCylinderBuffer[indexStatic] =
        (p->getNodeIn()->getXCoordinate() + p->getNodeOut()->getXCoordinate()) /
        2 / aspect;  // vertex.x
    staticCylinderBuffer[indexStatic + 1] =
        (p->getNodeIn()->getYCoordinate() + p->getNodeOut()->getYCoordinate()) /
        2 / aspect;  // vertex.y
    staticCylinderBuffer[indexStatic + 2] =
        (p->getNodeIn()->getZCoordinate() + p->getNodeOut()->getZCoordinate()) /
        2 / aspect;  // vertex.z

    // height
    staticCylinderBuffer[indexStatic + 3] = p->getFullLength() / 2 / aspect;

    // direction
    glm::vec3 dir = glm::vec3(float((p->getNodeIn()->getXCoordinate() -
                                     p->getNodeOut()->getXCoordinate()) /
                                    aspect),
                              float((p->getNodeIn()->getYCoordinate() -
                                     p->getNodeOut()->getYCoordinate()) /
                                    aspect),
                              float((p->getNodeIn()->getZCoordinate() -
                                     p->getNodeOut()->getZCoordinate()) /
                                    aspect));
    staticCylinderBuffer[indexStatic + 4] = dir[0];  // vertex.x
    staticCylinderBuffer[indexStatic + 5] = dir[1];  // vertex.y
    staticCylinderBuffer[indexStatic + 6] = dir[2];  // vertex.z

    // radius
    staticCylinderBuffer[indexStatic + 7] = p->getRadius() / aspect;

    indexStatic += 8;
  }

  uploadDataToGPU(staticCylinderVBO, staticCylinderBuffer,
                  8 * network->totalPores, GL_ARRAY_BUFFER);
}

void widget3d::bufferCylinderDynamicData() {
  unsigned indexDynamic(0);

  for (pore *p : pnmRange<pore>(network)) {
    if (p->getInlet() || p->getOutlet()) {
      indexDynamic += 2;
      continue;
    }

    // color data
    float colorKey =
        p->getPhaseFlag() == phase::oil || p->getPhaseFlag() == phase::temp ? 0
                                                                            : 1;
    dynamicCylinderBuffer[indexDynamic] = colorKey;
    dynamicCylinderBuffer[indexDynamic + 1] = p->getConcentration();

    indexDynamic += 2;
  }

  uploadDataToGPU(dynamicCylinderVBO, dynamicCylinderBuffer,
                  2 * network->totalPores, GL_ARRAY_BUFFER);
}

void widget3d::bufferCylinderIndicesData() {
  unsigned index(0);

  for (pore *p : pnmRange<pore>(network)) {
    if ((p->getInlet() || p->getOutlet()) ||
        (p->getPhaseFlag() == phase::invalid) ||
        (p->getPhaseFlag() == phase::oil && !oilVisible) ||
        (p->getPhaseFlag() == phase::temp && !oilVisible) ||
        (p->getPhaseFlag() == phase::water && !waterVisible) ||
        (p->getWettabilityFlag() == wettability::waterWet &&
         !waterWetVisible) ||
        (p->getWettabilityFlag() == wettability::oilWet && !oilWetVisible) ||
        (cutX &&
         p->getNodeIn()->getXCoordinate() > cutXValue * network->xEdgeLength) ||
        (cutY &&
         p->getNodeIn()->getYCoordinate() > cutYValue * network->yEdgeLength) ||
        (cutZ &&
         p->getNodeIn()->getZCoordinate() > cutZValue * network->zEdgeLength))
      continue;

    cylinderIndicesBuffer[index] = p->getId() - 1;
    index++;
  }

  cylinderCount = index;
  if (index != 0) {
    uploadDataToGPU(cylinderIndicesVBO, cylinderIndicesBuffer, index,
                    GL_ELEMENT_ARRAY_BUFFER);
  }
}

void widget3d::bufferLineStaticData() {
  unsigned indexStatic(0);

  for (pore *p : pnmRange<pore>(network)) {
    if (p->getInlet() || p->getOutlet()) {
      indexStatic += 6;
      continue;
    }

    // node1
    staticLineBuffer[indexStatic] =
        (p->getNodeIn()->getXCoordinate()) / aspect;  // vertex.x
    staticLineBuffer[indexStatic + 1] =
        (p->getNodeIn()->getYCoordinate()) / aspect;  // vertex.y
    staticLineBuffer[indexStatic + 2] =
        (p->getNodeIn()->getZCoordinate()) / aspect;  // vertex.z

    // node2
    staticLineBuffer[indexStatic + 3] =
        (p->getNodeOut()->getXCoordinate()) / aspect;  // vertex.x
    staticLineBuffer[indexStatic + 4] =
        (p->getNodeOut()->getYCoordinate()) / aspect;  // vertex.y
    staticLineBuffer[indexStatic + 5] =
        (p->getNodeOut()->getZCoordinate()) / aspect;  // vertex.z

    indexStatic += 6;
  }

  uploadDataToGPU(staticLineVBO, staticLineBuffer, 2 * 3 * network->totalPores,
                  GL_ARRAY_BUFFER);
}

void widget3d::bufferLineDynamicData() {
  unsigned indexDynamic(0);

  for (pore *p : pnmRange<pore>(network)) {
    if (p->getInlet() || p->getOutlet()) {
      indexDynamic += 4;
      continue;
    }

    // color data
    float colorKey =
        p->getPhaseFlag() == phase::oil || p->getPhaseFlag() == phase::temp ? 0
                                                                            : 1;

    // node1
    dynamicLineBuffer[indexDynamic] = colorKey;
    dynamicLineBuffer[indexDynamic + 1] = p->getConcentration();

    // node2
    dynamicLineBuffer[indexDynamic + 2] = colorKey;
    dynamicLineBuffer[indexDynamic + 3] = p->getConcentration();

    indexDynamic += 4;
  }

  uploadDataToGPU(dynamicLineVBO, dynamicLineBuffer,
                  2 * 2 * network->totalPores, GL_ARRAY_BUFFER);
}

void widget3d::bufferLinesIndicesData() {
  unsigned index(0);

  for (pore *p : pnmRange<pore>(network)) {
    if ((p->getInlet() || p->getOutlet()) ||
        (p->getPhaseFlag() == phase::invalid) ||
        (p->getPhaseFlag() == phase::oil && !oilVisible) ||
        (p->getPhaseFlag() == phase::temp && !oilVisible) ||
        (p->getPhaseFlag() == phase::water && !waterVisible) ||
        (p->getWettabilityFlag() == wettability::waterWet &&
         !waterWetVisible) ||
        (p->getWettabilityFlag() == wettability::oilWet && !oilWetVisible) ||
        (cutX && p->getXCoordinate() > cutXValue * network->xEdgeLength) ||
        (cutY && p->getYCoordinate() > cutYValue * network->yEdgeLength) ||
        (cutZ && p->getZCoordinate() > cutZValue * network->zEdgeLength))
      continue;

    lineIndicesBuffer[index] = 2 * (p->getId() - 1);
    lineIndicesBuffer[index + 1] = 2 * (p->getId() - 1) + 1;
    index += 2;
  }

  lineCount = index;
  if (index != 0) {
    uploadDataToGPU(lineIndicesVBO, lineIndicesBuffer, index,
                    GL_ELEMENT_ARRAY_BUFFER);
  }
}

void widget3d::bufferAxesData() {
  std::vector<GLfloat> axesBuffer;
  axesBuffer.resize(11 * 3);
  unsigned index(0);

  // X arrow
  // center
  axesBuffer[index] = 0.1f;   // vertex.x
  axesBuffer[index + 1] = 0;  // vertex.y
  axesBuffer[index + 2] = 0;  // vertex.z
  // height
  axesBuffer[index + 3] = 0.1f;
  // direction
  axesBuffer[index + 4] = 1;  // vertex.x
  axesBuffer[index + 5] = 0;  // vertex.y
  axesBuffer[index + 6] = 0;  // vertex.z
  // radius
  axesBuffer[index + 7] = 0.01f;
  // color data
  axesBuffer[index + 8] = 2;
  axesBuffer[index + 9] = 0.0;

  index += 10;

  // Y arrow
  // center
  axesBuffer[index] = 0;         // vertex.x
  axesBuffer[index + 1] = 0.1f;  // vertex.y
  axesBuffer[index + 2] = 0;     // vertex.z
  // height
  axesBuffer[index + 3] = 0.1f;
  // direction
  axesBuffer[index + 4] = 0;  // vertex.x
  axesBuffer[index + 5] = 1;  // vertex.y
  axesBuffer[index + 6] = 0;  // vertex.z
  // radius
  axesBuffer[index + 7] = 0.01f;
  // color data
  axesBuffer[index + 8] = 3;  // R
  axesBuffer[index + 9] = 0;  // G

  index += 10;

  // Z arrow
  // center
  axesBuffer[index] = 0;         // vertex.x
  axesBuffer[index + 1] = 0;     // vertex.y
  axesBuffer[index + 2] = 0.1f;  // vertex.z
  // height
  axesBuffer[index + 3] = 0.1f;
  // direction
  axesBuffer[index + 4] = 0;  // vertex.x
  axesBuffer[index + 5] = 0;  // vertex.y
  axesBuffer[index + 6] = 1;  // vertex.z
  // radius
  axesBuffer[index + 7] = 0.01f;
  // color data
  axesBuffer[index + 8] = 4;
  axesBuffer[index + 9] = 0;

  allocateBufferOnGPU<GLfloat>(axesVBO, 10 * 3, GL_ARRAY_BUFFER,
                               GL_STATIC_DRAW);
  uploadDataToGPU(axesVBO, axesBuffer, 10 * 3, GL_ARRAY_BUFFER);
}

void widget3d::loadShaderUniforms(Shader *shader) {
  // set view transformation
  view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, float(scale)));
  view = glm::translate(view, glm::vec3(0.0f + xTran, 0.0f + yTran, -2.5f));
  view = glm::rotate(view, float(xInitRot + xRot), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::rotate(view, float(yInitRot + yRot), glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::rotate(view, 3.14f / 2.0f + float(zRot),
                     glm::vec3(0.0f, 0.0f, 1.0f));
  view = glm::translate(
      view, glm::vec3(float(xInitTran), float(yInitTran), float(zInitTran)));

  // pass transformation matrices to the shader
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);

  // lighting
  shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
  shader->setVec3("lightPos", 1.0f, 0.0f, 2.0f);

  // Normal
  viewInv = glm::inverse(view);
  shader->setVec4("eyePoint", viewInv * glm::vec4(0.0, 0.0, 0.0, 1.0));
  shader->setMat3("normalMatrix", glm::mat3(glm::transpose(viewInv)));

  // Phase Colors
  shader->setVec3("oilColor", oilColor.x, oilColor.y, oilColor.z);
  shader->setVec3("waterColor", waterColor.x, waterColor.y, waterColor.z);
  shader->setVec3("tracerColor", tracerColor.x, tracerColor.y, tracerColor.z);
}

void widget3d::loadShaderUniformsAxes(Shader *shader) {
  // set view transformation
  view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.5f));
  view = glm::translate(view, glm::vec3(1.0f, -1.0f, -0.5f));
  view = glm::rotate(view, float(xInitRot + xRot), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::rotate(view, float(yInitRot + yRot), glm::vec3(0.0f, 1.0f, 0.0f));
  view = glm::rotate(view, 3.14f / 2.0f + float(zRot),
                     glm::vec3(0.0f, 0.0f, 1.0f));

  // pass transformation matrices to the shader
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);

  // lighting
  shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
  shader->setVec3("lightPos", 1.0f, 0.0f, 2.0f);

  // Normal
  viewInv = glm::inverse(view);
  shader->setVec4("eyePoint", viewInv * glm::vec4(0.0, 0.0, 0.0, 1.0));
  shader->setMat3("normalMatrix", glm::mat3(glm::transpose(viewInv)));
}

void widget3d::drawSpheres() {
  sphereShader->use();
  loadShaderUniforms(sphereShader.get());
  if (simulationRunnning || refreshRequested) bufferSphereDynamicData();
  bufferSphereIndicesData();
  glBindVertexArray(sphereVAO);
  glDrawElements(GL_POINTS, sphereCount, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void widget3d::drawCylinders() {
  cylinderShader->use();
  loadShaderUniforms(cylinderShader.get());
  if (simulationRunnning || refreshRequested) bufferCylinderDynamicData();
  bufferCylinderIndicesData();
  glBindVertexArray(cylinderVAO);
  glDrawElements(GL_POINTS, cylinderCount, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void widget3d::drawLines() {
  lineShader->use();
  loadShaderUniforms(lineShader.get());
  if (simulationRunnning || refreshRequested) bufferLineDynamicData();
  bufferLinesIndicesData();
  glBindVertexArray(lineVAO);
  glDrawElements(GL_LINES, lineCount, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

void widget3d::drawAxes() {
  cylinderShader->use();
  loadShaderUniformsAxes(cylinderShader.get());
  glBindVertexArray(axesVAO);
  glDrawArrays(GL_POINTS, 0, 3);
  glBindVertexArray(0);
}

void widget3d::mousePressEvent(QMouseEvent *event) { lastPos = event->pos(); }

void widget3d::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - lastPos.x();
  int dy = event->y() - lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))  //
    {
      xTran += 0.002 * dx;
      yTran += -0.002 * dy;
    } else {
      xRot += 0.005 * dy;
      yRot += 0.005 * dx;
    }
    update();
  } else if (event->buttons() & Qt::RightButton) {
    xRot += 0.005 * dy;
    zRot += 0.005 * dx;
    update();
  }

  lastPos = event->pos();
}

void widget3d::wheelEvent(QWheelEvent *event) {
  scale += event->delta() / 800.;
  update();
}

void widget3d::initializeGL() {
  glewInit();

  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);

  cylinderShader->create(":/shaders/cylinder.vert", ":/shaders/cylinder.frag",
                         ":/shaders/cylinder.geom");
  sphereShader->create(":/shaders/sphere.vert", ":/shaders/sphere.frag",
                       ":/shaders/sphere.geom");
  lineShader->create(":/shaders/line.vert", ":/shaders/line.frag");

  // Generate and bind Sphere VAO and VBOs
  glGenVertexArrays(1, &sphereVAO);
  glGenBuffers(1, &staticSphereVBO);
  glGenBuffers(1, &dynamicSphereVBO);
  glGenBuffers(1, &sphereIndicesVBO);

  glBindVertexArray(sphereVAO);

  glBindBuffer(GL_ARRAY_BUFFER, staticSphereVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * 4, nullptr);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * 4, (GLvoid *)12);
  glEnableVertexAttribArray(0);  // pos
  glEnableVertexAttribArray(1);  // radius

  glBindBuffer(GL_ARRAY_BUFFER, dynamicSphereVBO);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * 4, nullptr);
  glEnableVertexAttribArray(2);  // color

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndicesVBO);

  glBindVertexArray(0);

  // Generate and bind Cylinder VAO and VBOs
  glGenVertexArrays(1, &cylinderVAO);
  glGenBuffers(1, &staticCylinderVBO);
  glGenBuffers(1, &dynamicCylinderVBO);
  glGenBuffers(1, &cylinderIndicesVBO);

  glBindVertexArray(cylinderVAO);

  glBindBuffer(GL_ARRAY_BUFFER, staticCylinderVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * 4, nullptr);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * 4, (GLvoid *)12);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * 4, (GLvoid *)16);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8 * 4, (GLvoid *)28);
  glEnableVertexAttribArray(0);  // pos
  glEnableVertexAttribArray(1);  // height
  glEnableVertexAttribArray(2);  // direction
  glEnableVertexAttribArray(3);  // radius

  glBindBuffer(GL_ARRAY_BUFFER, dynamicCylinderVBO);
  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * 4, (GLvoid *)nullptr);
  glEnableVertexAttribArray(4);  // color

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderIndicesVBO);

  glBindVertexArray(0);

  // Generate and bind Line VAO and VBOs
  glGenVertexArrays(1, &lineVAO);
  glGenBuffers(1, &staticLineVBO);
  glGenBuffers(1, &dynamicLineVBO);
  glGenBuffers(1, &lineIndicesVBO);

  glBindVertexArray(lineVAO);

  glBindBuffer(GL_ARRAY_BUFFER, staticLineVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, nullptr);
  glEnableVertexAttribArray(0);  // pos

  glBindBuffer(GL_ARRAY_BUFFER, dynamicLineVBO);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * 4, nullptr);
  glEnableVertexAttribArray(1);  // color

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineIndicesVBO);

  glBindVertexArray(0);

  // Generate and bind Axes VAO and VBOs
  glGenVertexArrays(1, &axesVAO);
  glGenBuffers(1, &axesVBO);

  glBindVertexArray(axesVAO);

  glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * 4, nullptr);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 10 * 4, (GLvoid *)12);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * 4, (GLvoid *)16);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * 4, (GLvoid *)28);
  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 10 * 4, (GLvoid *)32);
  glEnableVertexAttribArray(0);  // pos
  glEnableVertexAttribArray(1);  // height
  glEnableVertexAttribArray(2);  // direction
  glEnableVertexAttribArray(3);  // radius
  glEnableVertexAttribArray(4);  // color

  glBindVertexArray(0);

  // Buffer axes data
  bufferAxesData();
}

void widget3d::resizeGL(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  projection =
      glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
}

void widget3d::paintGL() {
  glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z,
               backgroundColor.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (networkBuilt) {
    if (!buffersAllocated) initialiseDataBuffers();

    // draw nodes
    if (nodeBodies) drawSpheres();

    // draw throats
    if (poreBodies)
      drawCylinders();
    else if (poreLines)
      drawLines();

    refreshRequested = false;
  }

  if (axes) drawAxes();

  if (animation) yRot += 0.005;
}

void widget3d::timerUpdate() { update(); }

bool widget3d::getNetworkBuilt() const { return networkBuilt; }

void widget3d::setNetworkBuilt(bool value) {
  networkBuilt = value;

  if (value) {
    buffersAllocated = false;
    sphereCount = cylinderCount = lineCount = 0;
  }
}

void widget3d::setSimulationRunnning(bool value) { simulationRunnning = value; }

glm::vec3 &widget3d::getOilColor() { return oilColor; }

void widget3d::setOilColor(const glm::vec3 &value) { oilColor = value; }

glm::vec3 &widget3d::getWaterColor() { return waterColor; }

void widget3d::setWaterColor(const glm::vec3 &value) { waterColor = value; }

glm::vec3 &widget3d::getTracerColor() { return tracerColor; }

void widget3d::setTracerColor(const glm::vec3 &value) { tracerColor = value; }

bool widget3d::getPoreLines() const { return poreLines; }

void widget3d::setPoreLines(bool value) {
  refreshRequested = true;
  poreLines = value;
}

bool widget3d::getAxes() const { return axes; }

void widget3d::setAxes(bool value) { axes = value; }
bool widget3d::getPoreBodies() const { return poreBodies; }

void widget3d::setPoreBodies(bool value) {
  refreshRequested = true;
  poreBodies = value;
}
bool widget3d::getNodeBodies() const { return nodeBodies; }

void widget3d::setNodeBodies(bool value) {
  refreshRequested = true;
  nodeBodies = value;
}
bool widget3d::getOilVisible() const { return oilVisible; }

void widget3d::setOilVisible(bool value) {
  refreshRequested = true;
  oilVisible = value;
}
bool widget3d::getWaterVisible() const { return waterVisible; }

void widget3d::setWaterVisible(bool value) {
  refreshRequested = true;
  waterVisible = value;
}

bool widget3d::getWaterWetVisible() const { return waterWetVisible; }

void widget3d::setWaterWetVisible(bool value) {
  refreshRequested = true;
  waterWetVisible = value;
}
bool widget3d::getOilWetVisible() const { return oilWetVisible; }

void widget3d::setOilWetVisible(bool value) {
  refreshRequested = true;
  oilWetVisible = value;
}
double widget3d::getXRot() const { return xRot; }

void widget3d::setXRot(double value) { xRot = value; }
double widget3d::getYRot() const { return yRot; }

void widget3d::setYRot(double value) { yRot = value; }
double widget3d::getZRot() const { return zRot; }

void widget3d::setZRot(double value) { zRot = value; }
double widget3d::getXTran() const { return xTran; }

void widget3d::setXTran(double value) { xTran = value; }
double widget3d::getYTran() const { return yTran; }

void widget3d::setYTran(double value) { yTran = value; }
double widget3d::getScale() const { return scale; }

void widget3d::setScale(double value) { scale = value; }
double widget3d::getAspect() const { return aspect; }

void widget3d::setAspect(double value) { aspect = value; }

bool widget3d::getAnimation() const { return animation; }

void widget3d::setAnimation(bool value) { animation = value; }
double widget3d::getXInitTran() const { return xInitTran; }

void widget3d::setXInitTran(double value) { xInitTran = value; }
double widget3d::getYInitTran() const { return yInitTran; }

void widget3d::setYInitTran(double value) { yInitTran = value; }
double widget3d::getZInitTran() const { return zInitTran; }

void widget3d::setZInitTran(double value) { zInitTran = value; }
double widget3d::getYInitRot() const { return yInitRot; }

void widget3d::setYInitRot(double value) { yInitRot = value; }
double widget3d::getXInitRot() const { return xInitRot; }

void widget3d::setXInitRot(double value) { xInitRot = value; }
bool widget3d::getCutX() const { return cutX; }

void widget3d::setCutX(bool value) {
  refreshRequested = true;
  cutX = value;
}
bool widget3d::getCutY() const { return cutY; }

void widget3d::setCutY(bool value) {
  refreshRequested = true;
  cutY = value;
}
bool widget3d::getCutZ() const { return cutZ; }

void widget3d::setCutZ(bool value) {
  refreshRequested = true;
  cutZ = value;
}
double widget3d::getCutXValue() const { return cutXValue; }

void widget3d::setCutXValue(double value) {
  refreshRequested = true;
  cutXValue = value;
}
double widget3d::getCutYValue() const { return cutYValue; }

void widget3d::setCutYValue(double value) {
  refreshRequested = true;
  cutYValue = value;
}
double widget3d::getCutZValue() const { return cutZValue; }

void widget3d::setCutZValue(double value) {
  refreshRequested = true;
  cutZValue = value;
}
