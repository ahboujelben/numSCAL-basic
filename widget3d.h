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
#include "network.h"
#include <cmath>
#include <iostream>
#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QOpenGLFramebufferObject>
#include <QApplication>

using namespace PNM;

class widget3d : public QGLWidget
{
    Q_OBJECT
public :
    widget3d (QWidget *parent=0)
        : QGLWidget (QGLFormat(QGL::SampleBuffers),parent)
    {
        net=0;
        light=true;
        animation=false;
        render=false;
        load=false;
        poreBodies=true;
        poreLines=true;
        nodeBodies=true;
        oilVisible=true;
        waterVisible=true;
        gasVisible=true;
        waterWetVisible=true;
        oilWetVisible=true;
        cutX=false;
        cutY=false;
        cutZ=false;
        cutXValue=0.5;
        cutYValue=0.5;
        cutZValue=0.5;

        phase1Color= glm::vec3(0.9f, 0.35f, 0.2f);
        phase2Color= glm::vec3( 0.3f, 0.65f, 0.9f);
        phase3Color= glm::vec3(0.65f, 0.95f, 0.15f);

        connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    }

protected :
    double xRot,yRot,zRot,xTran,yTran,scale,xInitRot,yInitRot,xInitTran,yInitTran,zInitTran,aspect,cutXValue,cutYValue,cutZValue;
    bool light,animation,update,render,load,poreBodies,nodeBodies,poreLines,oilVisible,waterVisible,gasVisible,waterWetVisible,oilWetVisible,cutX,cutY,cutZ;
    int totalImages,imageCount;
    std::string phasePoresPath, phaseNodesPath;
    glm::vec3 phase1Color, phase2Color, phase3Color;
    QPoint lastPos; 
    network* net;
    QTimer timer;
    QOpenGLFramebufferObject *fbo;
    unsigned int sphereVBO, sphereVAO, cylinderVBO, cylinderVAO, lineVBO, lineVAO;
    Shader sphereShader, cylinderShader, lineShader;

    void mousePressEvent(QMouseEvent *event) 
    {
        lastPos = event->pos();
    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();
        if (event->buttons() & Qt::LeftButton)
        {
            if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))//
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
        } else if (event->buttons() & Qt::RightButton)
        {
            xRot += 0.005 * dy;
            zRot += 0.005 * dx;
            updateGL();
        }
        lastPos = event->pos();
    }

    void wheelEvent(QWheelEvent *event) 
    {
        double numDegrees = -event->delta() / 8.0;
        double numSteps = numDegrees / 50.0;
        scale += event->delta() / 800.;
        updateGL();
    }

    void upload(GLuint buffer, float* h_data, const unsigned count, GLenum target, GLenum access)
    {
        glBindBuffer(target, buffer);
        glBufferData(target, count * sizeof(GLfloat), NULL, access);
        void* d_data = (void*) glMapBuffer(target, GL_READ_WRITE);
        if (d_data == NULL)
        {
        fprintf(stderr, "Could not map gpu buffer.\n");
        exit(1);
        }
        memcpy(d_data, (const void*) h_data, count * sizeof(GLfloat));
        if (!glUnmapBuffer(target))
        {
        fprintf(stderr, "Unmap buffer failed.\n");
        exit(1);
        }
        d_data = NULL;
        glBindBuffer(target, 0);
    }

    unsigned bufferCylinderData()
    {
        unsigned index(0);
        unsigned numberOfObjectsToDraw(0);
        if(net!=0)
        if(net->getReady())
        {
            int NUMBER_CYLINDERS=net->getTotalPores();
            GLfloat *h_data = new GLfloat[11 * NUMBER_CYLINDERS];
            for(int i=0;i<NUMBER_CYLINDERS;++i)
            {
                pore* p=net->getPore(i);
                if(!p->getClosed())
                {
                    if(p->getInlet() || p->getOutlet()) continue;
                    if(p->getPhaseFlag()==phase::invalid) continue;
                    if(p->getPhaseFlag()==phase::oil && !oilVisible) continue;
                    if(p->getPhaseFlag()==phase::temp && !oilVisible) continue;
                    if(p->getPhaseFlag()==phase::water && !waterVisible) continue;
                    if(p->getWettabilityFlag()==wettability::waterWet && !waterWetVisible) continue;
                    if(p->getWettabilityFlag()==wettability::oilWet && !oilWetVisible) continue;
                    if(cutX && p->getMaxXCoordinate()>cutXValue*net->getXEdgeLength()) continue;
                    if(cutY && p->getMaxYCoordinate()>cutYValue*net->getYEdgeLength()) continue;
                    if(cutZ && p->getMaxZCoordinate()>cutZValue*net->getZEdgeLength()) continue;

                    // center
                    h_data[index] = (p->getNodeIn()->getXCoordinate()+p->getNodeOut()->getXCoordinate())/2/aspect; // vertex.x
                    h_data[index + 1] = (p->getNodeIn()->getYCoordinate()+p->getNodeOut()->getYCoordinate())/2/aspect;; // vertex.y
                    h_data[index + 2] = (p->getNodeIn()->getZCoordinate()+p->getNodeOut()->getZCoordinate())/2/aspect;; // vertex.z

                    // height
                    h_data[index + 3] = p->getFullLength()/2/aspect;

                    // direction
                    glm::vec3 dir=glm::vec3(float((p->getNodeIn()->getXCoordinate()-p->getNodeOut()->getXCoordinate())/aspect), float((p->getNodeIn()->getYCoordinate()-p->getNodeOut()->getYCoordinate())/aspect), float((p->getNodeIn()->getZCoordinate()-p->getNodeOut()->getZCoordinate())/aspect));
                    h_data[index + 4] = dir[0]; // vertex.x
                    h_data[index + 5] = dir[1]; // vertex.y
                    h_data[index + 6] = dir[2]; // vertex.z

                    // color data
                    glm::vec3 color= p->getPhaseFlag()==phase::oil ||p->getPhaseFlag()==phase::temp?phase1Color:phase2Color;
                    color= color+float(p->getConcentration())*(phase3Color-color);
                    h_data[index + 7] = color.x;
                    h_data[index + 8] = color.y;
                    h_data[index + 9] = color.z;
                    // radius
                    h_data[index + 10] = p->getRadius()/aspect;

                    index+=11;
                    numberOfObjectsToDraw++;
                }
            }
            if(numberOfObjectsToDraw!=0)
                upload(cylinderVBO, h_data, 11 * NUMBER_CYLINDERS, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

            delete[] h_data;
        }

        return numberOfObjectsToDraw;
    }

    unsigned bufferLinesData()
    {
        unsigned index(0);
        unsigned numberOfObjectsToDraw(0);
        if(net!=0)
        if(net->getReady())
        {
            int NUMBER_CYLINDERS=net->getTotalPores();
            GLfloat *h_data = new GLfloat[2 * 6 * NUMBER_CYLINDERS];
            for(int i=0;i<NUMBER_CYLINDERS;++i)
            {
                pore* p=net->getPore(i);
                if(!p->getClosed())
                {
                    if(p->getInlet() || p->getOutlet()) continue;
                    if(p->getPhaseFlag()==phase::invalid) continue;
                    if(p->getPhaseFlag()==phase::oil && !oilVisible) continue;
                    if(p->getPhaseFlag()==phase::temp && !oilVisible) continue;
                    if(p->getPhaseFlag()==phase::water && !waterVisible) continue;
                    if(p->getWettabilityFlag()==wettability::waterWet && !waterWetVisible) continue;
                    if(p->getWettabilityFlag()==wettability::oilWet && !oilWetVisible) continue;
                    if(cutX && p->getMaxXCoordinate()>cutXValue*net->getXEdgeLength()) continue;
                    if(cutY && p->getMaxYCoordinate()>cutYValue*net->getYEdgeLength()) continue;
                    if(cutZ && p->getMaxZCoordinate()>cutZValue*net->getZEdgeLength()) continue;

                    // node1
                    h_data[index] = (p->getNodeIn()->getXCoordinate())/aspect; // vertex.x
                    h_data[index + 1] = (p->getNodeIn()->getYCoordinate())/aspect;; // vertex.y
                    h_data[index + 2] = (p->getNodeIn()->getZCoordinate())/aspect;; // vertex.z

                    // color data
                    glm::vec3 color= p->getPhaseFlag()==phase::oil||p->getPhaseFlag()==phase::temp?phase1Color:phase2Color;
                    color= color+float(p->getConcentration())*(phase3Color-color);
                    h_data[index + 3] = color.x;
                    h_data[index + 4] = color.y;
                    h_data[index + 5] = color.z;

                    index+=6;
                    numberOfObjectsToDraw++;

                    // node2
                    h_data[index] = (p->getNodeOut()->getXCoordinate())/aspect; // vertex.x
                    h_data[index + 1] = (p->getNodeOut()->getYCoordinate())/aspect;; // vertex.y
                    h_data[index + 2] = (p->getNodeOut()->getZCoordinate())/aspect;; // vertex.z

                    // color data
                    color= p->getPhaseFlag()==phase::oil||p->getPhaseFlag()==phase::temp?phase1Color:phase2Color;
                    color= color+float(p->getConcentration())*(phase3Color-color);
                    h_data[index + 3] = color.x;
                    h_data[index + 4] = color.y;
                    h_data[index + 5] = color.z;

                    index+=6;
                    numberOfObjectsToDraw++;
                }
            }
            if(numberOfObjectsToDraw!=0)
                upload(lineVBO, h_data, 2 * 6 * NUMBER_CYLINDERS, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

            delete[] h_data;
        }

        return numberOfObjectsToDraw;
    }

    unsigned bufferSphereData()
    {
        unsigned index(0);
        unsigned numberOfObjectsToDraw(0);
        if(net!=0)
        if(net->getReady())
        {
            int NUMBER_SPHERES=net->getTotalNodes();
            GLfloat *h_data = new GLfloat[7 * NUMBER_SPHERES];
            for(int i=0;i<NUMBER_SPHERES;++i)
            {
                node* p=net->getNode(i);
                if(!p->getClosed())
                {
                    if(p->getPhaseFlag()==phase::invalid) continue;
                    if(p->getPhaseFlag()==phase::oil && !oilVisible) continue;
                    if(p->getPhaseFlag()==phase::temp && !oilVisible) continue;
                    if(p->getPhaseFlag()==phase::water && !waterVisible) continue;
                    if(p->getWettabilityFlag()==wettability::waterWet && !waterWetVisible) continue;
                    if(p->getWettabilityFlag()==wettability::oilWet && !oilWetVisible) continue;
                    if(cutX && p->getXCoordinate()>cutXValue*net->getXEdgeLength()) continue;
                    if(cutY && p->getYCoordinate()>cutYValue*net->getYEdgeLength()) continue;
                    if(cutZ && p->getZCoordinate()>cutZValue*net->getZEdgeLength()) continue;

                    // center
                    h_data[index] = p->getXCoordinate()/aspect; // vertex.x
                    h_data[index + 1] = p->getYCoordinate()/aspect; // vertex.y
                    h_data[index + 2] = p->getZCoordinate()/aspect; // vertex.z

                    // radius
                    h_data[index + 3] = p->getRadius()/aspect;

                    // color data
                    glm::vec3 color= p->getPhaseFlag()==phase::oil||p->getPhaseFlag()==phase::temp?phase1Color:phase2Color;
                    color= color+float(p->getConcentration())*(phase3Color-color);
                    h_data[index + 4] = color.x;
                    h_data[index + 5] = color.y;
                    h_data[index + 6] = color.z;

                    index+=7;
                    numberOfObjectsToDraw++;
                }
            }

            if(numberOfObjectsToDraw!=0)
                upload(sphereVBO, h_data, 7 * NUMBER_SPHERES, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

            delete[] h_data;
        }

        return numberOfObjectsToDraw;
    }

    unsigned bufferAxesData()
    {
        unsigned index(0);
        unsigned numberOfObjectsToDraw(3);

        int NUMBER_CYLINDERS=3;
        GLfloat *h_data = new GLfloat[11 * NUMBER_CYLINDERS];

        //X arrow
        // center
        h_data[index] = 0.1; // vertex.x
        h_data[index + 1] = 0; // vertex.y
        h_data[index + 2] = 0; // vertex.z
        // height
        h_data[index + 3] = 0.1;
        // direction
        h_data[index + 4] = 1; // vertex.x
        h_data[index + 5] = 0; // vertex.y
        h_data[index + 6] = 0; // vertex.z
        // color data
        h_data[index + 7] = 1;// R
        h_data[index + 8] = 0.0; // G
        h_data[index + 9] = 0.0; // B
        // radius
        h_data[index + 10] = 0.01;
        index+=11;

        //Y arrow
        // center
        h_data[index] = 0; // vertex.x
        h_data[index + 1] = 0.1; // vertex.y
        h_data[index + 2] = 0; // vertex.z
        // height
        h_data[index + 3] = 0.1;
        // direction
        h_data[index + 4] = 0; // vertex.x
        h_data[index + 5] = 1; // vertex.y
        h_data[index + 6] = 0; // vertex.z
        // color data
        h_data[index + 7] = 0;// R
        h_data[index + 8] = 1; // G
        h_data[index + 9] = 0.0; // B
        // radius
        h_data[index + 10] = 0.01;
        index+=11;

        //Z arrow
        // center
        h_data[index] = 0; // vertex.x
        h_data[index + 1] = 0; // vertex.y
        h_data[index + 2] = 0.1; // vertex.z
        // height
        h_data[index + 3] = 0.1;
        // direction
        h_data[index + 4] = 0; // vertex.x
        h_data[index + 5] = 0; // vertex.y
        h_data[index + 6] = 1; // vertex.z
        // color data
        h_data[index + 7] = 0.0;// R
        h_data[index + 8] = 0.0; // G
        h_data[index + 9] = 1.0; // B
        // radius
        h_data[index + 10] = 0.01;
        index+=11;

        upload(cylinderVBO, h_data, 11 * NUMBER_CYLINDERS, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

        delete[] h_data;

        return numberOfObjectsToDraw;
    }

    void loadShaderUniforms(Shader &shader)
    {
        // create transformations
        glm::mat4 view;
        glm::mat4 projection;

        projection = glm::perspective(glm::radians(45.0f), (float)width() / (float)height(), 0.1f, 100.0f);

        view       = glm::translate(view, glm::vec3(0.0f, 0.0f,float(scale)));
        view       = glm::translate(view, glm::vec3(0.0f+xTran, 0.0f+yTran, -2.5f));
        view       = glm::rotate(view, float(xInitRot+xRot) , glm::vec3(1.0f, 0.0f, 0.0f));
        view       = glm::rotate(view, float(yInitRot+yRot) , glm::vec3(0.0f, 1.0f, 0.0f));
        view       = glm::rotate(view, 3.14f/2.0f+float(zRot) , glm::vec3(0.0f, 0.0f, 1.0f));
        view       = glm::translate(view, glm::vec3(float(xInitTran), float(yInitTran), float(zInitTran)));

        // pass transformation matrices to the shader
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        //lighting
        shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        shader.setVec3("lightPos", 1.0f, 0.0f, 2.0f);

        //Normal
        shader.setVec4("eyePoint", inverse(view)*glm::vec4(0.0,0.0,0.0,1.0));
        shader.setMat3("normalMatrix", transpose(inverse(glm::mat3(view))));

        //Phase Colors
        shader.setVec3("phase1Color", 0.9f, 0.35f, 0.2f);
        shader.setVec3("phase2Color", 0.3f, 0.65f, 0.9f);
        shader.setVec3("phase3Color", 0.65f, 0.95f, 0.15f);
        shader.setVec3("phase4Color", 0.65f, 0.95f, 0.15f);
        shader.setVec3("phase5Color", 0.1f, 0.15f, 0.9f);
    }

    void loadShaderUniformsAxes(Shader &shader)
    {
        // create transformations
        glm::mat4 view;
        glm::mat4 projection;

        projection = glm::perspective(glm::radians(45.0f), (float)width() / (float)height(), 0.1f, 100.0f);

        view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.5f));
        view       = glm::translate(view, glm::vec3(1.0f, -1.0f, -0.5f));
        view       = glm::rotate(view, float(xInitRot+xRot) , glm::vec3(1.0f, 0.0f, 0.0f));
        view       = glm::rotate(view, float(yInitRot+yRot) , glm::vec3(0.0f, 1.0f, 0.0f));
        view       = glm::rotate(view, 3.14f/2.0f+float(zRot) , glm::vec3(0.0f, 0.0f, 1.0f));

        // pass transformation matrices to the shader
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        //lighting
        shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        shader.setVec3("lightPos", 1.0f, 0.0f, 2.0f);

        //Normal
        shader.setVec4("eyePoint", inverse(view)*glm::vec4(0.0,0.0,0.0,1.0));
        shader.setMat3("normalMatrix", transpose(inverse(glm::mat3(view))));

        //Phase Colors
        shader.setVec3("phase1Color", 0.9f, 0.2f, 0.2f);
        shader.setVec3("phase2Color", 0.2f, 0.9f, 0.4f);
        shader.setVec3("phase3Color", 0.2f, 0.2f, 0.9f);
        shader.setVec3("phase4Color", 0.7f, 1.0f, 0.15f);
    }

    void drawSpheres()
    {
        sphereShader.use();
        loadShaderUniforms(sphereShader);
        unsigned numberOfObjectsToDraw=bufferSphereData();
        glBindVertexArray(sphereVAO);
        if(numberOfObjectsToDraw!=0)
            glDrawArrays(GL_POINTS, 0, numberOfObjectsToDraw);
        glBindVertexArray(0);
    }

    void drawCylinders()
    {
        cylinderShader.use();
        loadShaderUniforms(cylinderShader);
        unsigned numberOfObjectsToDraw=bufferCylinderData();
        glBindVertexArray(cylinderVAO);
        glDrawArrays(GL_POINTS, 0, numberOfObjectsToDraw);
    }

    void drawLines()
    {
        lineShader.use();
        loadShaderUniforms(lineShader);
        unsigned numberOfObjectsToDraw=bufferLinesData();
        glBindVertexArray(lineVAO);
        glDrawArrays(GL_LINES, 0, numberOfObjectsToDraw);
    }

    void drawAxes()
    {
        cylinderShader.use();
        loadShaderUniformsAxes(cylinderShader);
        unsigned numberOfObjectsToDraw=bufferAxesData();
        glBindVertexArray(cylinderVAO);
        if(numberOfObjectsToDraw!=0)
            glDrawArrays(GL_POINTS, 0, numberOfObjectsToDraw);
        glBindVertexArray(0);
    }

    void initializeGL()
    {
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        timer.start(10);
        glewInit();
        cylinderShader.create(":/shaders/cylinder.vert", ":/shaders/cylinder.frag", ":/shaders/cylinder.geom");
        sphereShader.create(":/shaders/sphere.vert", ":/shaders/sphere.frag", ":/shaders/sphere.geom");
        lineShader.create(":/shaders/line.vert", ":/shaders/line.frag");

        glGenVertexArrays(1, &sphereVAO);
        glGenBuffers(1, &sphereVBO);
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glEnableVertexAttribArray(0); //pos
        glEnableVertexAttribArray(1); //radius
        glEnableVertexAttribArray(2); //color
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*4, 0);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7*4, (GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7*4, (GLvoid*)16);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11*4, 0);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 11*4, (GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11*4, (GLvoid*)16);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11*4, (GLvoid*)28);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 11*4, (GLvoid*)40);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glEnableVertexAttribArray(0); // pos
        glEnableVertexAttribArray(1); // color
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*4, 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*4, (GLvoid*)12);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void resizeGL( int w, int h)
    {
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    }

    void paintGL()
    {
        glClearColor(16.f/255.f, 26.f/255.f ,32.f/255.f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(poreBodies)
            drawCylinders();
        else if(poreLines)
            drawLines();

        if(nodeBodies)
            drawSpheres();

        if(light)
            drawAxes();

        if(animation)
            yRot+=0.005;
    }

public:

    bool getLight() const;
    void setLight(bool value);

    bool getPoreBodies() const;
    void setPoreBodies(bool value);

    bool getNodeBodies() const;
    void setNodeBodies(bool value);

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

    int getI() const;
    void setI(int value);

    std::string getPhasePoresPath() const;
    void setPhasePoresPath(const std::string &value);

    std::string getPhaseNodesPath() const;
    void setPhaseNodesPath(const std::string &value);

    bool getRender() const;
    void setRender(bool value);

    bool getLoad() const;
    void setLoad(bool value);

    int getTotalImages() const;
    void setTotalImages(int value);

    int getImageCount() const;
    void setImageCount(int value);

    network *getNet() const;
    void setNet(network *value);

    glm::vec3 &getPhase1Color();
    void setPhase1Color(const glm::vec3 &value);

    glm::vec3 &getPhase2Color();
    void setPhase2Color(const glm::vec3 &value);

    glm::vec3 &getPhase3Color();
    void setPhase3Color(const glm::vec3 &value);

    bool getPoreLines() const;
    void setPoreLines(bool value);

public slots:
    void timerUpdate()
    {
        if(animation || update)
        {
            updateGL();
            if(net!=0)
                if(net->getReady())
                    if(net->getRecord() && net->getVideoRecording())
                        emit plotted();
            update=false;
        }

        if(render)
        {
            string path="Results/Network_Status/phases_pores"+boost::lexical_cast<std::string>(1000000000+imageCount)+".txt";
            ifstream file(path.c_str());

            if(file.is_open())
            {
                int value;
                char phase;
                double concentration;
                while(file>>value)
                {
                    pore* p=net->getPore(value-1);
                    {
                        file>>phase>>concentration;
                        PNM::phase phaseFlag=phase=='o'?PNM::phase::oil:PNM::phase::water;
                        p->setPhaseFlag(phaseFlag);
                        p->setConcentration(concentration);
                    }
                }

                {
                    string path="Results/Network_Status/phases_nodes"+boost::lexical_cast<std::string>(1000000000+imageCount)+".txt";
                    ifstream file(path.c_str());
                    if(file.is_open())
                    {
                        int value;
                        char phase;
                        double concentration;
                        while(file>>value)
                        {
                            node* p=net->getNode(value-1);
                            {
                                file>>phase>>concentration;
                                PNM::phase phaseFlag=phase=='o'?PNM::phase::oil:PNM::phase::water;
                                p->setPhaseFlag(phaseFlag);
                                p->setConcentration(concentration);
                            }
                        }
                    }
                    else
                    {
                        totalImages=0;
                    }
                }

                QImage image = grabFrameBuffer();
                image.save( "Videos/IMG"+QString::number(1000000000+imageCount)+".png" );

                imageCount++;
            }
            else
            {
                totalImages=0;
            }

            if(imageCount>=totalImages)
            {
                render=false;
                tools::renderVideo();
                tools::cleanVideosFolder();
                emit rendered();
            }

            updateGL();
        }

        if(load)
        {
            ifstream file(phasePoresPath.c_str());

            if(file.is_open())
            {
                int value;
                char phase;
                double concentration;
                while(file>>value)
                {
                    pore* p=net->getPore(value-1);
                    {
                        file>>phase>>concentration;
                        PNM::phase phaseFlag=phase=='o'?PNM::phase::oil:PNM::phase::water;
                        p->setPhaseFlag(phaseFlag);
                        p->setConcentration(concentration);
                    }
                }

                {
                    ifstream file(phaseNodesPath.c_str());
                    if(file.is_open())
                    {
                        int value;
                        char phase;
                        double concentration;
                        while(file>>value)
                        {
                            node* p=net->getNode(value-1);
                            {
                                file>>phase>>concentration;
                                PNM::phase phaseFlag=phase=='o'?PNM::phase::oil:PNM::phase::water;
                                p->setPhaseFlag(phaseFlag);
                                p->setConcentration(concentration);
                            }
                        }
                    }
                }
            }

            load=false;
            emit rendered();
            updateGL();
        }
    }

    void updateNetwork()
    {
        update=true;
    }

signals:
    void plotted();
    void rendered();
};

#endif // WIDGET3D_H
