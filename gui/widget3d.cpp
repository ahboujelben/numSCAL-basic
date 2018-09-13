/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "shader.h"
#include "widget3d.h"
#include "network/iterator.h"
#include "misc/userInput.h"
#include "misc/tools.h"

#include <QApplication>
#include <QMouseEvent>
#include <QTimer>

#include <libs/glm/glm.hpp>

using namespace PNM;
using namespace std;

widget3d::widget3d(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    axes = true;
    animation = false;
    networkBuilt = false;
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
    phase1Color = glm::vec3(0.9f, 0.35f, 0.2f);
    phase2Color = glm::vec3(0.3f, 0.65f, 0.9f);
    phase3Color = glm::vec3(0.65f, 0.95f, 0.15f);

    timer = std::make_shared<QTimer>();
    connect(timer.get(), SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(10);

    sphereShader = std::make_shared<Shader>();
    cylinderShader = std::make_shared<Shader>();
    lineShader = std::make_shared<Shader>();
}

void widget3d::setNetwork(const std::shared_ptr<PNM::networkModel> &value)
{
    network = value;
}

void widget3d::setAspectRatio()
{
    if (!networkBuilt)
        return;

    aspect = qMax(qMax(network->xEdgeLength, network->yEdgeLength), network->zEdgeLength);
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

    if (network->is2D)
    {
        xInitRot = 0;
        yInitRot = 0;
        xInitTran -= 0.1;
        scale = 0.9;
    }
}

void widget3d::uploadDataToGPU(GLuint buffer, float *h_data, const unsigned count, GLenum target, GLenum access)
{
    glBindBuffer(target, buffer);
    glBufferData(target, count * sizeof(GLfloat), NULL, access);
    void *d_data = (void *)glMapBuffer(target, GL_READ_WRITE);
    if (d_data == NULL)
    {
        fprintf(stderr, "Could not map gpu buffer.\n");
        exit(1);
    }
    memcpy(d_data, (const void *)h_data, count * sizeof(GLfloat));
    if (!glUnmapBuffer(target))
    {
        fprintf(stderr, "Unmap buffer failed.\n");
        exit(1);
    }
    d_data = NULL;
    glBindBuffer(target, 0);
}

unsigned widget3d::bufferCylinderData()
{
    unsigned index(0);
    unsigned numberOfObjectsToDraw(0);
    if (networkBuilt)
    {
        int NUMBER_CYLINDERS = network->totalPores;
        GLfloat *h_data = new GLfloat[11 * NUMBER_CYLINDERS];
        for (pore *p : pnmRange<pore>(network))
        {
            if (p->getInlet() || p->getOutlet())
                continue;
            if (p->getPhaseFlag() == phase::invalid)
                continue;
            if (p->getPhaseFlag() == phase::oil && !oilVisible)
                continue;
            if (p->getPhaseFlag() == phase::temp && !oilVisible)
                continue;
            if (p->getPhaseFlag() == phase::water && !waterVisible)
                continue;
            if (p->getWettabilityFlag() == wettability::waterWet && !waterWetVisible)
                continue;
            if (p->getWettabilityFlag() == wettability::oilWet && !oilWetVisible)
                continue;
            if (cutX && p->getXCoordinate() > cutXValue * network->xEdgeLength)
                continue;
            if (cutY && p->getYCoordinate() > cutYValue * network->yEdgeLength)
                continue;
            if (cutZ && p->getZCoordinate() > cutZValue * network->zEdgeLength)
                continue;

            // center
            h_data[index] = (p->getNodeIn()->getXCoordinate() + p->getNodeOut()->getXCoordinate()) / 2 / aspect; // vertex.x
            h_data[index + 1] = (p->getNodeIn()->getYCoordinate() + p->getNodeOut()->getYCoordinate()) / 2 / aspect;
            ; // vertex.y
            h_data[index + 2] = (p->getNodeIn()->getZCoordinate() + p->getNodeOut()->getZCoordinate()) / 2 / aspect;
            ; // vertex.z

            // height
            h_data[index + 3] = p->getFullLength() / 2 / aspect;

            // direction
            glm::vec3 dir = glm::vec3(float((p->getNodeIn()->getXCoordinate() - p->getNodeOut()->getXCoordinate()) / aspect), float((p->getNodeIn()->getYCoordinate() - p->getNodeOut()->getYCoordinate()) / aspect), float((p->getNodeIn()->getZCoordinate() - p->getNodeOut()->getZCoordinate()) / aspect));
            h_data[index + 4] = dir[0]; // vertex.x
            h_data[index + 5] = dir[1]; // vertex.y
            h_data[index + 6] = dir[2]; // vertex.z

            // color data
            glm::vec3 color = p->getPhaseFlag() == phase::oil || p->getPhaseFlag() == phase::temp ? phase1Color : phase2Color;
            color = color + float(p->getConcentration()) * (phase3Color - color);
            h_data[index + 7] = color.x;
            h_data[index + 8] = color.y;
            h_data[index + 9] = color.z;
            // radius
            h_data[index + 10] = p->getRadius() / aspect;

            index += 11;
            numberOfObjectsToDraw++;
        }
        if (numberOfObjectsToDraw != 0)
            uploadDataToGPU(cylinderVBO, h_data, 11 * NUMBER_CYLINDERS, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

        delete[] h_data;
    }

    return numberOfObjectsToDraw;
}

unsigned widget3d::bufferLinesData()
{
    unsigned index(0);
    unsigned numberOfObjectsToDraw(0);
    if (networkBuilt)
    {
        int NUMBER_CYLINDERS = network->totalPores;
        GLfloat *h_data = new GLfloat[2 * 6 * NUMBER_CYLINDERS];
        for (pore *p : pnmRange<pore>(network))
        {
            if (p->getInlet() || p->getOutlet())
                continue;
            if (p->getPhaseFlag() == phase::invalid)
                continue;
            if (p->getPhaseFlag() == phase::oil && !oilVisible)
                continue;
            if (p->getPhaseFlag() == phase::temp && !oilVisible)
                continue;
            if (p->getPhaseFlag() == phase::water && !waterVisible)
                continue;
            if (p->getWettabilityFlag() == wettability::waterWet && !waterWetVisible)
                continue;
            if (p->getWettabilityFlag() == wettability::oilWet && !oilWetVisible)
                continue;
            if (cutX && p->getXCoordinate() > cutXValue * network->xEdgeLength)
                continue;
            if (cutY && p->getYCoordinate() > cutYValue * network->yEdgeLength)
                continue;
            if (cutZ && p->getZCoordinate() > cutZValue * network->zEdgeLength)
                continue;

            // node1
            h_data[index] = (p->getNodeIn()->getXCoordinate()) / aspect; // vertex.x
            h_data[index + 1] = (p->getNodeIn()->getYCoordinate()) / aspect;
            ; // vertex.y
            h_data[index + 2] = (p->getNodeIn()->getZCoordinate()) / aspect;
            ; // vertex.z

            // color data
            glm::vec3 color = p->getPhaseFlag() == phase::oil || p->getPhaseFlag() == phase::temp ? phase1Color : phase2Color;
            color = color + float(p->getConcentration()) * (phase3Color - color);
            h_data[index + 3] = color.x;
            h_data[index + 4] = color.y;
            h_data[index + 5] = color.z;

            index += 6;
            numberOfObjectsToDraw++;

            // node2
            h_data[index] = (p->getNodeOut()->getXCoordinate()) / aspect; // vertex.x
            h_data[index + 1] = (p->getNodeOut()->getYCoordinate()) / aspect;
            ; // vertex.y
            h_data[index + 2] = (p->getNodeOut()->getZCoordinate()) / aspect;
            ; // vertex.z

            // color data
            color = p->getPhaseFlag() == phase::oil || p->getPhaseFlag() == phase::temp ? phase1Color : phase2Color;
            color = color + float(p->getConcentration()) * (phase3Color - color);
            h_data[index + 3] = color.x;
            h_data[index + 4] = color.y;
            h_data[index + 5] = color.z;

            index += 6;
            numberOfObjectsToDraw++;
        }
        if (numberOfObjectsToDraw != 0)
            uploadDataToGPU(lineVBO, h_data, 2 * 6 * NUMBER_CYLINDERS, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

        delete[] h_data;
    }

    return numberOfObjectsToDraw;
}

unsigned widget3d::bufferSphereData()
{
    unsigned index(0);
    unsigned numberOfObjectsToDraw(0);
    if (networkBuilt)
    {
        int NUMBER_SPHERES = network->totalNodes;
        GLfloat *h_data = new GLfloat[7 * NUMBER_SPHERES];
        for (node *p : pnmRange<node>(network))
        {
            if (p->getPhaseFlag() == phase::invalid)
                continue;
            if (p->getPhaseFlag() == phase::oil && !oilVisible)
                continue;
            if (p->getPhaseFlag() == phase::temp && !oilVisible)
                continue;
            if (p->getPhaseFlag() == phase::water && !waterVisible)
                continue;
            if (p->getWettabilityFlag() == wettability::waterWet && !waterWetVisible)
                continue;
            if (p->getWettabilityFlag() == wettability::oilWet && !oilWetVisible)
                continue;
            if (cutX && p->getXCoordinate() > cutXValue * network->xEdgeLength)
                continue;
            if (cutY && p->getYCoordinate() > cutYValue * network->yEdgeLength)
                continue;
            if (cutZ && p->getZCoordinate() > cutZValue * network->zEdgeLength)
                continue;

            // center
            h_data[index] = p->getXCoordinate() / aspect;     // vertex.x
            h_data[index + 1] = p->getYCoordinate() / aspect; // vertex.y
            h_data[index + 2] = p->getZCoordinate() / aspect; // vertex.z

            // radius
            h_data[index + 3] = p->getRadius() / aspect;

            // color data
            glm::vec3 color = p->getPhaseFlag() == phase::oil || p->getPhaseFlag() == phase::temp ? phase1Color : phase2Color;
            color = color + float(p->getConcentration()) * (phase3Color - color);
            h_data[index + 4] = color.x;
            h_data[index + 5] = color.y;
            h_data[index + 6] = color.z;

            index += 7;
            numberOfObjectsToDraw++;
        }

        if (numberOfObjectsToDraw != 0)
            uploadDataToGPU(sphereVBO, h_data, 7 * NUMBER_SPHERES, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

        delete[] h_data;
    }

    return numberOfObjectsToDraw;
}

unsigned widget3d::bufferAxesData()
{
    unsigned index(0);
    unsigned numberOfObjectsToDraw(3);

    int NUMBER_CYLINDERS = 3;
    GLfloat *h_data = new GLfloat[11 * NUMBER_CYLINDERS];

    //X arrow
    // center
    h_data[index] = 0.1;   // vertex.x
    h_data[index + 1] = 0; // vertex.y
    h_data[index + 2] = 0; // vertex.z
    // height
    h_data[index + 3] = 0.1;
    // direction
    h_data[index + 4] = 1; // vertex.x
    h_data[index + 5] = 0; // vertex.y
    h_data[index + 6] = 0; // vertex.z
    // color data
    h_data[index + 7] = 1;   // R
    h_data[index + 8] = 0.0; // G
    h_data[index + 9] = 0.0; // B
    // radius
    h_data[index + 10] = 0.01;
    index += 11;

    //Y arrow
    // center
    h_data[index] = 0;       // vertex.x
    h_data[index + 1] = 0.1; // vertex.y
    h_data[index + 2] = 0;   // vertex.z
    // height
    h_data[index + 3] = 0.1;
    // direction
    h_data[index + 4] = 0; // vertex.x
    h_data[index + 5] = 1; // vertex.y
    h_data[index + 6] = 0; // vertex.z
    // color data
    h_data[index + 7] = 0;   // R
    h_data[index + 8] = 1;   // G
    h_data[index + 9] = 0.0; // B
    // radius
    h_data[index + 10] = 0.01;
    index += 11;

    //Z arrow
    // center
    h_data[index] = 0;       // vertex.x
    h_data[index + 1] = 0;   // vertex.y
    h_data[index + 2] = 0.1; // vertex.z
    // height
    h_data[index + 3] = 0.1;
    // direction
    h_data[index + 4] = 0; // vertex.x
    h_data[index + 5] = 0; // vertex.y
    h_data[index + 6] = 1; // vertex.z
    // color data
    h_data[index + 7] = 0.0; // R
    h_data[index + 8] = 0.0; // G
    h_data[index + 9] = 1.0; // B
    // radius
    h_data[index + 10] = 0.01;
    index += 11;

    uploadDataToGPU(cylinderVBO, h_data, 11 * NUMBER_CYLINDERS, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    delete[] h_data;

    return numberOfObjectsToDraw;
}

void widget3d::loadShaderUniforms(Shader *shader)
{
    // create transformations
    glm::mat4 view;
    glm::mat4 projection;

    projection = glm::perspective(glm::radians(45.0f), (float)width() / (float)height(), 0.1f, 100.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, float(scale)));
    view = glm::translate(view, glm::vec3(0.0f + xTran, 0.0f + yTran, -2.5f));
    view = glm::rotate(view, float(xInitRot + xRot), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, float(yInitRot + yRot), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, 3.14f / 2.0f + float(zRot), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, glm::vec3(float(xInitTran), float(yInitTran), float(zInitTran)));

    // pass transformation matrices to the shader
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    //lighting
    shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->setVec3("lightPos", 1.0f, 0.0f, 2.0f);

    //Normal
    shader->setVec4("eyePoint", inverse(view) * glm::vec4(0.0, 0.0, 0.0, 1.0));
    shader->setMat3("normalMatrix", transpose(inverse(glm::mat3(view))));

    //Phase Colors
    shader->setVec3("phase1Color", 0.9f, 0.35f, 0.2f);
    shader->setVec3("phase2Color", 0.3f, 0.65f, 0.9f);
    shader->setVec3("phase3Color", 0.65f, 0.95f, 0.15f);
    shader->setVec3("phase4Color", 0.65f, 0.95f, 0.15f);
    shader->setVec3("phase5Color", 0.1f, 0.15f, 0.9f);
}

void widget3d::loadShaderUniformsAxes(Shader *shader)
{
    // create transformations
    glm::mat4 view;
    glm::mat4 projection;

    projection = glm::perspective(glm::radians(45.0f), (float)width() / (float)height(), 0.1f, 100.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.5f));
    view = glm::translate(view, glm::vec3(1.0f, -1.0f, -0.5f));
    view = glm::rotate(view, float(xInitRot + xRot), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, float(yInitRot + yRot), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, 3.14f / 2.0f + float(zRot), glm::vec3(0.0f, 0.0f, 1.0f));

    // pass transformation matrices to the shader
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    //lighting
    shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader->setVec3("lightPos", 1.0f, 0.0f, 2.0f);

    //Normal
    shader->setVec4("eyePoint", inverse(view) * glm::vec4(0.0, 0.0, 0.0, 1.0));
    shader->setMat3("normalMatrix", transpose(inverse(glm::mat3(view))));

    //Phase Colors
    shader->setVec3("phase1Color", 0.9f, 0.2f, 0.2f);
    shader->setVec3("phase2Color", 0.2f, 0.9f, 0.4f);
    shader->setVec3("phase3Color", 0.2f, 0.2f, 0.9f);
    shader->setVec3("phase4Color", 0.7f, 1.0f, 0.15f);
}

void widget3d::drawSpheres()
{
    sphereShader->use();
    loadShaderUniforms(sphereShader.get());
    unsigned numberOfObjectsToDraw = bufferSphereData();
    glBindVertexArray(sphereVAO);
    if (numberOfObjectsToDraw != 0)
        glDrawArrays(GL_POINTS, 0, numberOfObjectsToDraw);
    glBindVertexArray(0);
}

void widget3d::drawCylinders()
{
    cylinderShader->use();
    loadShaderUniforms(cylinderShader.get());
    unsigned numberOfObjectsToDraw = bufferCylinderData();
    glBindVertexArray(cylinderVAO);
    glDrawArrays(GL_POINTS, 0, numberOfObjectsToDraw);
}

void widget3d::drawLines()
{
    lineShader->use();
    loadShaderUniforms(lineShader.get());
    unsigned numberOfObjectsToDraw = bufferLinesData();
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, numberOfObjectsToDraw);
}

void widget3d::drawAxes()
{
    cylinderShader->use();
    loadShaderUniformsAxes(cylinderShader.get());
    unsigned numberOfObjectsToDraw = bufferAxesData();
    glBindVertexArray(cylinderVAO);
    if (numberOfObjectsToDraw != 0)
        glDrawArrays(GL_POINTS, 0, numberOfObjectsToDraw);
    glBindVertexArray(0);
}

void widget3d::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void widget3d::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    if (event->buttons() & Qt::LeftButton)
    {
        if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) //
        {
            xTran += 0.002 * dx;
            yTran += -0.002 * dy;
        }
        else
        {
            xRot += 0.005 * dy;
            yRot += 0.005 * dx;
        }
        updateGL();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        xRot += 0.005 * dy;
        zRot += 0.005 * dx;
        updateGL();
    }
    lastPos = event->pos();
}

void widget3d::wheelEvent(QWheelEvent *event)
{
    scale += event->delta() / 800.;
    updateGL();
}

void widget3d::initializeGL()
{
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glewInit();
    cylinderShader->create(":/shaders/cylinder.vert", ":/shaders/cylinder.frag", ":/shaders/cylinder.geom");
    sphereShader->create(":/shaders/sphere.vert", ":/shaders/sphere.frag", ":/shaders/sphere.geom");
    lineShader->create(":/shaders/line.vert", ":/shaders/line.frag");

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glEnableVertexAttribArray(0); //pos
    glEnableVertexAttribArray(1); //radius
    glEnableVertexAttribArray(2); //color
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * 4, 0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * 4, (GLvoid *)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * 4, (GLvoid *)16);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    glBindVertexArray(cylinderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glEnableVertexAttribArray(0); // pos
    glEnableVertexAttribArray(1); // height
    glEnableVertexAttribArray(2); // direction
    glEnableVertexAttribArray(3); // color
    glEnableVertexAttribArray(4); // radius
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * 4, 0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 11 * 4, (GLvoid *)12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * 4, (GLvoid *)16);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * 4, (GLvoid *)28);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 11 * 4, (GLvoid *)40);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glEnableVertexAttribArray(0); // pos
    glEnableVertexAttribArray(1); // color
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * 4, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * 4, (GLvoid *)12);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void widget3d::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void widget3d::paintGL()
{
    glClearColor(16.f / 255.f, 26.f / 255.f, 32.f / 255.f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (poreBodies)
        drawCylinders();
    else if (poreLines)
        drawLines();

    if (nodeBodies)
        drawSpheres();

    if (axes)
        drawAxes();

    if (animation)
        yRot += 0.005;
}

bool widget3d::getNetworkBuilt() const
{
    return networkBuilt;
}

void widget3d::setNetworkBuilt(bool value)
{
    networkBuilt = value;
}

void widget3d::timerUpdate()
{
    updateGL();
}

glm::vec3 &widget3d::getPhase1Color()
{
    return phase1Color;
}

void widget3d::setPhase1Color(const glm::vec3 &value)
{
    phase1Color = value;
}

glm::vec3 &widget3d::getPhase2Color()
{
    return phase2Color;
}

void widget3d::setPhase2Color(const glm::vec3 &value)
{
    phase2Color = value;
}

glm::vec3 &widget3d::getPhase3Color()
{
    return phase3Color;
}

void widget3d::setPhase3Color(const glm::vec3 &value)
{
    phase3Color = value;
}

bool widget3d::getPoreLines() const
{
    return poreLines;
}

void widget3d::setPoreLines(bool value)
{
    poreLines = value;
}

bool widget3d::getAxes() const
{
    return axes;
}

void widget3d::setAxes(bool value)
{
    axes = value;
}
bool widget3d::getPoreBodies() const
{
    return poreBodies;
}

void widget3d::setPoreBodies(bool value)
{
    poreBodies = value;
}
bool widget3d::getNodeBodies() const
{
    return nodeBodies;
}

void widget3d::setNodeBodies(bool value)
{
    nodeBodies = value;
}
bool widget3d::getOilVisible() const
{
    return oilVisible;
}

void widget3d::setOilVisible(bool value)
{
    oilVisible = value;
}
bool widget3d::getWaterVisible() const
{
    return waterVisible;
}

void widget3d::setWaterVisible(bool value)
{
    waterVisible = value;
}

bool widget3d::getWaterWetVisible() const
{
    return waterWetVisible;
}

void widget3d::setWaterWetVisible(bool value)
{
    waterWetVisible = value;
}
bool widget3d::getOilWetVisible() const
{
    return oilWetVisible;
}

void widget3d::setOilWetVisible(bool value)
{
    oilWetVisible = value;
}
double widget3d::getXRot() const
{
    return xRot;
}

void widget3d::setXRot(double value)
{
    xRot = value;
}
double widget3d::getYRot() const
{
    return yRot;
}

void widget3d::setYRot(double value)
{
    yRot = value;
}
double widget3d::getZRot() const
{
    return zRot;
}

void widget3d::setZRot(double value)
{
    zRot = value;
}
double widget3d::getXTran() const
{
    return xTran;
}

void widget3d::setXTran(double value)
{
    xTran = value;
}
double widget3d::getYTran() const
{
    return yTran;
}

void widget3d::setYTran(double value)
{
    yTran = value;
}
double widget3d::getScale() const
{
    return scale;
}

void widget3d::setScale(double value)
{
    scale = value;
}
double widget3d::getAspect() const
{
    return aspect;
}

void widget3d::setAspect(double value)
{
    aspect = value;
}

bool widget3d::getAnimation() const
{
    return animation;
}

void widget3d::setAnimation(bool value)
{
    animation = value;
}
double widget3d::getXInitTran() const
{
    return xInitTran;
}

void widget3d::setXInitTran(double value)
{
    xInitTran = value;
}
double widget3d::getYInitTran() const
{
    return yInitTran;
}

void widget3d::setYInitTran(double value)
{
    yInitTran = value;
}
double widget3d::getZInitTran() const
{
    return zInitTran;
}

void widget3d::setZInitTran(double value)
{
    zInitTran = value;
}
double widget3d::getYInitRot() const
{
    return yInitRot;
}

void widget3d::setYInitRot(double value)
{
    yInitRot = value;
}
double widget3d::getXInitRot() const
{
    return xInitRot;
}

void widget3d::setXInitRot(double value)
{
    xInitRot = value;
}
bool widget3d::getCutX() const
{
    return cutX;
}

void widget3d::setCutX(bool value)
{
    cutX = value;
}
bool widget3d::getCutY() const
{
    return cutY;
}

void widget3d::setCutY(bool value)
{
    cutY = value;
}
bool widget3d::getCutZ() const
{
    return cutZ;
}

void widget3d::setCutZ(bool value)
{
    cutZ = value;
}
double widget3d::getCutXValue() const
{
    return cutXValue;
}

void widget3d::setCutXValue(double value)
{
    cutXValue = value;
}
double widget3d::getCutYValue() const
{
    return cutYValue;
}

void widget3d::setCutYValue(double value)
{
    cutYValue = value;
}
double widget3d::getCutZValue() const
{
    return cutZValue;
}

void widget3d::setCutZValue(double value)
{
    cutZValue = value;
}
