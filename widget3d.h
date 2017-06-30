/////////////////////////////////////////////////////////////////////////////
/// Name:        widget3d.h
/// Purpose:     OpenGL 3D functions are declared and defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#ifndef WIDGET3D_H
#define WIDGET3D_H

#include "network.h"

#include <cmath>
#include <GL/glu.h>

#include <QGLWidget>
#include <QMouseEvent>
#include <QVector3D>
#include <QTimer>
#include <QOpenGLFramebufferObject>

class widget3d : public QGLWidget
{
    Q_OBJECT
public :
    widget3d (QWidget *parent=0)
        : QGLWidget (QGLFormat(QGL::SampleBuffers),parent)
    {
        net=0;
        i=0;
        light=true;
        animation=false;
        render=false;
        load=false;
        poreBodies=true;
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

        connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    }

    void setNetwork(network* pN)
    {
            net=pN;
    }

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

    bool getOnlyPhases() const;
    void setOnlyPhases(bool value);

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
                while(file>>value)
                {
                    pore* p=net->getPore(value-1);
                    file>>phase;
                    p->setPhaseFlag(phase);
                }

                if(net->getNetworkSource()!=1)
                {
                    string path="Results/Network_Status/phases_nodes"+boost::lexical_cast<std::string>(1000000000+imageCount)+".txt";
                    ifstream file(path.c_str());
                    if(file.is_open())
                    {
                        int value;
                        char phase;
                        node* p=net->getNode(value-1);
                        while(file>>value)
                        {
                            file>>phase;
                            p->setPhaseFlag(phase);;
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
                while(file>>value)
                {
                    pore* p=net->getPore(value-1);
                    file>>phase;
                    p->setPhaseFlag(phase);

                }

                if(net->getNetworkSource()!=1)
                {
                    ifstream file(phaseNodesPath.c_str());
                    if(file.is_open())
                    {
                        int value;
                        char phase;
                        while(file>>value)
                        {
                            node* p=net->getNode(value-1);
                            file>>phase;
                            p->setPhaseFlag(phase);;

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

protected :
    double xRot,yRot,zRot,xTran,yTran,scale,xInitRot,yInitRot,xInitTran,yInitTran,zInitTran,aspect,cutXValue,cutYValue,cutZValue;
    bool light,animation,update,render,load,poreBodies,nodeBodies,oilVisible,waterVisible,gasVisible,waterWetVisible,oilWetVisible,cutX,cutY,cutZ,onlyPhases;
    int totalImages,imageCount;
    std::string phasePoresPath, phaseNodesPath;
    QPoint lastPos; 
    network* net;
    QTimer timer;
    int i;
    QOpenGLFramebufferObject *fbo;

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
            if(net!=0)
                if(net->getReady())
                {
                    if(net->getNz()==1 && net->getNetworkSource()!=3)//
                    {
                        xTran += 0.005 * dx;
                        yTran += 0.005 * dy;
                    }
                    else
                    {
                        xRot += 2 * dy;
                        yRot += 2 * dx;
                    }
                }

            updateGL();
        } else if (event->buttons() & Qt::RightButton) 
        {
            if(!(net->getNz()==1 && !net->getNetworkSource()==3)) //
            {
                xRot += 2 * dy;
                zRot += 2 * dx;
            }
            updateGL();
        }
        lastPos = event->pos();
    }
    void wheelEvent(QWheelEvent *event) 
    {
        double numDegrees = -event->delta() / 8.0;
        double numSteps = numDegrees / 50.0;
        scale *= pow(0.925,numSteps);
        updateGL();
    }

    void myGluPerspective(GLdouble fovY, GLdouble aspect,
                              GLdouble zNear, GLdouble zFar)
    {
        const GLdouble pi = 3.1415926535897932384626433832795;
        GLdouble fW, fH;
        fH = tan(fovY / 360 * pi) * zNear;
        fW = fH * aspect;
        glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    }

    void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
    {
        float vx = x2-x1;
        float vy = y2-y1;
        float vz = z2-z1;
        if(vz == 0)
            vz = -.001;
        float v = sqrt( vx*vx + vy*vy + vz*vz );
        float ax = 57.2957795*acos( vz/v );
        if ( vz < 0.0 )
            ax = -ax;
        float rx = -vy*vz;
        float ry = vx*vz;
        glPushMatrix();
        GLUquadricObj *quadric=gluNewQuadric();
        glTranslatef( x1,y1,z1 );
        glRotatef(ax, rx, ry, 0.0);
        gluQuadricOrientation(quadric,GLU_OUTSIDE);
        gluCylinder(quadric, radius, radius, v, subdivisions, 1);

        gluDeleteQuadric(quadric);
        glPopMatrix();
    } 

    void renderSphere(float x, float y, float z, float radius,int rings, int sectors)
    {
        glPushMatrix();
        glTranslatef(x,y,z);
        GLUquadricObj *quadric=gluNewQuadric();
        gluSphere(quadric,radius,rings,sectors);
        gluDeleteQuadric(quadric);
        glPopMatrix();
    }

    void setupLight()
    {
        GLfloat ambient1[] = {1.0f, 1.0f, 1.0f};
        GLfloat diffuse1[] = {0.7f, 0.7f, 0.7f};
        GLfloat specular1[] = {1.0f, 1.0f, 1.0f};
        GLfloat position1[] = {0.0f, 0.0f, 2.0f, 0.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);
        glLightfv(GL_LIGHT0, GL_POSITION, position1);
        glEnable(GL_LIGHT0);

        GLfloat ambient2[] = {1.0f, 1.0f, 0.0f};
        GLfloat diffuse2[] = {1.0f, 0.0f, 0.0f};
        GLfloat position2[] = {0.0f, 0.5f, 0.0f, 1.0f};
        GLfloat direction2[] = {0.0f, 2.0f, 1.0f};
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
        glLightfv(GL_LIGHT1, GL_SPECULAR, position2);
        glLightfv(GL_LIGHT1, GL_POSITION, direction2);
        glEnable(GL_LIGHT1);

    }

    void setupOilMaterial(double conc=0)
    {
        GLfloat ambient[] = {0.2f+(0.2-0.2)*conc,0.01f+(0.7-0.01)*conc,0.01f+(0.05-0.01)*conc};
        GLfloat diffuse[] = {0.6f+(0.8-0.6)*conc,0.04f+(0.5-0.04)*conc,0.04f+(0.5-0.04)*conc};
        GLfloat specular[] = {0.7+(0.2-0.7)*conc,0.6f+(0.5-0.6)*conc,0.6f+(0.5-0.6)*conc};
        GLfloat shine = 0.25f+0.25*conc;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shine*128);
    }

    void setupWaterMaterial(double conc=0)
    {
        GLfloat ambient[] ={ 0.0215f+(0.2-0.0215f)*conc, 0.1745f+(0.7-0.1745f)*conc, 0.0215f+(0.05-0.0215f)*conc};
        GLfloat diffuse[] ={0.07568f+(0.8-0.07568f)*conc, 0.61424f+(0.5-0.61424f)*conc, 0.07568f+(0.5-0.07568f)*conc };
        GLfloat specular[] ={0.633f+(0.2-0.633f)*conc, 0.727811f+(0.5-0.727811f)*conc, 0.633f+(0.5-0.633f)*conc };;
        GLfloat shine =76.8f ;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shine * 128);
    }

    void setupGasMaterial(double conc=0)
    {
        GLfloat ambient[] = {0.25f+(0.45-0.25)*conc,0.25f+(0.45-0.25)*conc,0.25f+(0.45-0.25)*conc};
        GLfloat diffuse[] = {0.8f+(0.45-0.25)*conc,0.8f+(0.45-0.25)*conc,0.8f+(0.45-0.25)*conc};
        GLfloat specular[] = {0.8f+(0.45-0.25)*conc,0.8f+(0.45-0.25)*conc,0.8f+(0.45-0.25)*conc};
        GLfloat shine = 0.2f;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shine * 128);
    }

    void drawPores()
    {
        if(net!=0)
        if(net->getReady())
        {
            glLineWidth(0.8);
            glBegin(GL_LINES);

            for(int i=0;i<net->getTotalPores();++i)
            {
                pore* p=net->getPore(i);
                if(!p->getClosed() && !p->getInlet() && !p->getOutlet())
                {
                    if(p->getPhaseFlag()=='o' && !oilVisible) continue;
                    if(p->getPhaseFlag()=='m' && !oilVisible) continue;
                    if(p->getPhaseFlag()=='w' && !waterVisible) continue;
                    if(p->getPhaseFlag()=='g' && !gasVisible) continue;
                    if(p->getWettabilityFlag()=='w' && !waterWetVisible) continue;
                    if(p->getWettabilityFlag()=='o' && !oilWetVisible) continue;
                    if(cutX && p->getMaxXCoordinate()>cutXValue*net->getXEdgeLength()) continue;
                    if(cutY && p->getMaxYCoordinate()>cutYValue*net->getYEdgeLength()) continue;
                    if(cutZ && p->getMaxZCoordinate()>cutZValue*net->getZEdgeLength()) continue;

                    if(p->getPhaseFlag()=='o')
                    {
                        glColor3f (0.0f,0.0f,0.0f);
                        setupOilMaterial();
                    }
                    if(p->getPhaseFlag()=='w')
                    {
                        glColor3f (0,1.0f,1.0f);
                        setupWaterMaterial();
                    }
                    if(p->getPhaseFlag()=='g')
                    {
                        glColor3f (1.0f,1.0f,1.0f);
                        setupGasMaterial();
                    }

                    glVertex3f(p->getNodeIn()->getXCoordinate()/aspect,p->getNodeIn()->getYCoordinate()/aspect,p->getNodeIn()->getZCoordinate()/aspect);
                    glVertex3f(p->getNodeOut()->getXCoordinate()/aspect,p->getNodeOut()->getYCoordinate()/aspect,p->getNodeOut()->getZCoordinate()/aspect);
                }
            }
           glEnd();
        }
    }

    void drawPoresBodies()
    {
        if(net!=0)
        if(net->getReady())
        {
            for(int i=0;i<net->getTotalPores();++i)
            {
                pore* p=net->getPore(i);
                if(!p->getClosed())
                {
                    if(p->getInlet() || p->getOutlet()) continue;
                    if(p->getPhaseFlag()=='c') continue;
                    if(p->getPhaseFlag()=='o' && !oilVisible) continue;
                    if(p->getPhaseFlag()=='m' && !oilVisible) continue;
                    if(p->getPhaseFlag()=='w' && !waterVisible) continue;
                    if(p->getPhaseFlag()=='g' && !gasVisible) continue;
                    if(p->getWettabilityFlag()=='w' && !waterWetVisible) continue;
                    if(p->getWettabilityFlag()=='o' && !oilWetVisible) continue;
                    if(cutX && p->getMaxXCoordinate()>cutXValue*net->getXEdgeLength()) continue;
                    if(cutY && p->getMaxYCoordinate()>cutYValue*net->getYEdgeLength()) continue;
                    if(cutZ && p->getMaxZCoordinate()>cutZValue*net->getZEdgeLength()) continue;

                    if(p->getPhaseFlag()=='o')
                    {
                        glColor3f (0.0f,0.0f,0.0f);
                        setupOilMaterial();
                    }
                    if(p->getPhaseFlag()=='w')
                    {
                        glColor3f (0,1.0f,1.0f);
                        setupWaterMaterial();
                    }
                    if(p->getPhaseFlag()=='g')
                    {
                        glColor3f (1.0f,1.0f,1.0f);
                        setupGasMaterial();
                    }

                    //double PoreRadius=p->getRadius();
                    double PoreRadius=((p->getRadius()-net->getMinRadius()+0.15*(net->getMaxRadius()-net->getMinRadius()))/(net->getMaxRadius()-net->getMinRadius()))*net->getLength()/3;
                    renderCylinder(p->getNodeIn()->getXCoordinate()/aspect,p->getNodeIn()->getYCoordinate()/aspect,p->getNodeIn()->getZCoordinate()/aspect, p->getNodeOut()->getXCoordinate()/aspect,p->getNodeOut()->getYCoordinate()/aspect,p->getNodeOut()->getZCoordinate()/aspect,PoreRadius/aspect, 7);
                }
            }
        }
    }

    void drawNodes()
    {
        if(net!=0)
        if(net->getReady())
        {
            for(int i=0;i<net->getTotalNodes();++i)
            {
                node *n=net->getNode(i);
                if(!n->getClosed())
                {
                    if(n->getPhaseFlag()=='o' && !oilVisible) continue;
                    if(n->getPhaseFlag()=='w' && !waterVisible) continue;
                    if(n->getPhaseFlag()=='g' && !gasVisible) continue;
                    if(n->getWettabilityFlag()=='w' && !waterWetVisible) continue;
                    if(n->getWettabilityFlag()=='o' && !oilWetVisible) continue;
                    if(cutX && n->getXCoordinate()>cutXValue*net->getXEdgeLength()) continue;
                    if(cutY && n->getYCoordinate()>cutYValue*net->getYEdgeLength()) continue;
                    if(cutZ && n->getZCoordinate()>cutZValue*net->getZEdgeLength()) continue;

                    if(n->getPhaseFlag()=='o')
                    {
                        glColor3f (0.0f,0.0f,0.0f);
                        setupOilMaterial();
                    }
                    if(n->getPhaseFlag()=='w')
                    {
                        glColor3f (0,1.0f,1.0f);
                        setupWaterMaterial();
                    }
                    if(n->getPhaseFlag()=='g')
                    {
                        glColor3f (1.0f,1.0f,1.0f);
                        setupGasMaterial();
                    }

                    //double nodeRadius=n->getRadius();
                    double nodeRadius=((n->getRadius()-net->getMinNodeRadius()+0.15*(net->getMaxNodeRadius()-net->getMinNodeRadius()))/(net->getMaxNodeRadius()-net->getMinNodeRadius()))*net->getLength()/1.5;
                    renderSphere(net->getNode(i)->getXCoordinate()/aspect,net->getNode(i)->getYCoordinate()/aspect,net->getNode(i)->getZCoordinate()/aspect,nodeRadius/aspect,8,8);
                }
            }
        }
    }

    void initializeGL()
    {
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_NORMALIZE);
        setupLight();
        timer.start(40);
    }
    void resizeGL( int w, int h) 
    {
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspectRatio=float(w)/float(h);
        if (w >= h)
           glOrtho(-aspectRatio, aspectRatio, -1, 1, 0.01, 100);
        else
           glOrtho(-1, 1, -1.0/aspectRatio, 1.0/aspectRatio, 0.01, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    void paintGL() 
    {
        glClearColor(0.7f, 0.7f ,0.7f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glTranslatef(xTran, -yTran, -2.5f);
        glRotatef(xInitRot+xRot / 16.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(yInitRot+yRot / 16.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(90+zRot / 16.0f, 0.0f, 0.0f, 1.0f);
        glScalef(scale, scale, scale);
        glTranslatef(xInitTran, yInitTran, zInitTran);

        if(light)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);

        if(!poreBodies)
            drawPores();
        else
            drawPoresBodies();

        if(nodeBodies)
            drawNodes();

        if(animation)
            yRot+=10;

        if(light)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);

    }
};

#endif // WIDGET3D_H
