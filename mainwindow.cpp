/////////////////////////////////////////////////////////////////////////////
/// Name:        mainwindow.cpp
/// Purpose:     attributes and methods of the main application window
///              are defined here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution 4.0 International (CC BY 4.0)
/////////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <QStyle>

Qt::GlobalColor QtColours[]= { Qt::blue, Qt::red, Qt::green, Qt::gray, Qt::black, Qt::magenta, Qt::yellow};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    net = new network;
    ui->widget->setNetwork(net);
    connect(net,SIGNAL(plot()),ui->widget,SLOT(updateNetwork()));
    connect(ui->widget,SIGNAL(plotted()),this,SLOT(saveImages()));
    connect(ui->widget,SIGNAL(rendered()),this,SLOT(renderFinished()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(plotCurvesRealTime()));
    //plot
    plotTitle=new QCPPlotTitle(ui->plotWidget, "title");
    ui->plotWidget->plotLayout()->insertRow(0);
    ui->plotWidget->plotLayout()->addElement(0, 0, plotTitle);
    ui->plotWidget->xAxis->setLabelFont(font());
    ui->plotWidget->xAxis->setTickLabelFont(font());
    ui->plotWidget->yAxis->setLabelFont(font());
    ui->plotWidget->yAxis->setTickLabelFont(font());
    ui->plotWidget->legend->setFont(font());
    ui->plotWidget->legend->setVisible(true);

    this->setWindowTitle("numSCAL (Basic)");
    imageIndex=0;
    totalCurves=0;

    timer.start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete net;
    delete plotTitle;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(net->getSimulationRunning())
    {
        net->setCancel(true);
        while(net->getSimulationRunning()){usleep(200);}
    }
    event->accept();
}

void MainWindow::setup3dWidget()
{
    double aspect=qMax(qMax(net->getXEdgeLength(),net->getYEdgeLength()),net->getZEdgeLength());
    double xInitTran=-0.5*net->getXEdgeLength()/aspect;
    double yInitTran=-0.5*net->getYEdgeLength()/aspect;
    double zInitTran=-0.5*net->getZEdgeLength()/aspect;

    ui->widget->setAspect(aspect);
    ui->widget->setXInitTran(xInitTran);
    ui->widget->setYInitTran(yInitTran);
    ui->widget->setZInitTran(zInitTran);

    ui->widget->setXRot(0);
    ui->widget->setYRot(0);
    ui->widget->setZRot(0);
    ui->widget->setXTran(0);
    ui->widget->setYTran(0);
    ui->widget->setScale(1.25);

    if(net->getNz()==1 && net->getNetworkSource()!=3)//
    {
        ui->widget->setXInitRot(0);
        ui->widget->setYInitRot(0);
        ui->widget->setScale(1.75);
    }
    else
    {
        ui->widget->setXInitRot(20);
        ui->widget->setYInitRot(30);
    }
}

void MainWindow::exportNetworkDataFromGUI()
{
    QSettings settings("Input Data/network_data.txt", QSettings::IniFormat);

    settings.beginGroup("Network_Source");
    int source;
    if(ui->regularRadioButton->isChecked())source=1;
    if(ui->calibratedRadioButton->isChecked())source=2;
    if(ui->extractedRadioButton->isChecked())source=3;
    settings.setValue("source",source );
    settings.setValue("extractedNetworkPath", ui->pathToExtractedLineEdit->text());
    settings.setValue("rockPrefix", ui->rockList->currentText());
    settings.setValue("extension", ui->extendCheckBox->isChecked());
    settings.setValue("extensionNumber", ui->extensionNumber->text());
    settings.setValue("extensionOneDirection", ui->oneDirection->isChecked());
    settings.endGroup();

    settings.beginGroup("Geometry");
    settings.setValue("Nx",ui->NxSpinBox->text());
    settings.setValue("Ny",ui->NySpinBox->text());
    settings.setValue("Nz",ui->NzSpinBox->text());
    settings.setValue("minRadius",ui->minRadiusLineEdit->text());
    settings.setValue("maxRadius",ui->maxRadiusLineEdit->text());
    int distribution;
    if(ui->psdUniformRadioButton->isChecked())distribution=1;
    if(ui->psdRayleighRadioButton->isChecked())distribution=2;
    if(ui->psdTriangularRadioButton->isChecked())distribution=3;
    if(ui->psdNormalRadioButton->isChecked())distribution=4;
    settings.setValue("radiusDistribution",distribution);
    settings.setValue("rayleighParameter",ui->rayleighParamLineEdit->text());
    settings.setValue("triangularParameter",ui->triangularParameter->text());
    settings.setValue("normalMuParameter",ui->muNormalParameter->text());
    settings.setValue("normalSigmaParameter",ui->sigmaNormalParameter->text());
    settings.setValue("poreVolumeConstant",ui->volConsRadiusLineEdit->text());
    settings.setValue("poreVolumeExponent",ui->volExpLineEdit->text());
    settings.setValue("poreConductivityConstant",ui->condConsLineEdit->text());
    settings.setValue("poreConductivityExponent",ui->condExpLineEdit->text());
    settings.setValue("coordinationNumber",ui->zSpinBox->text());
    settings.setValue("degreeOfDistortion",ui->distortionSpinBox->text());
    settings.setValue("aspectRatio",ui->aspectRatioSpinBox->text());
    settings.setValue("length",ui->lengthLineEdit->text());
    settings.setValue("seed",ui->seed->text());
    int solverChoice;
    if(ui->choleskyRadioButton->isChecked())solverChoice=1;
    if(ui->bicstabRadioButton->isChecked())solverChoice=2;
    settings.setValue("solverChoice",solverChoice);
    settings.setValue("absolutePermeabilityCalculation",ui->calcPermCheckBox->isChecked());
    settings.setValue("extractData",ui->extractDataCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup("Wettability");
    int wettabilityFlag;
    if(ui->wwSpinBox->isChecked())wettabilityFlag=1;
    if(ui->owSpinBox->isChecked())wettabilityFlag=2;
    if(ui->fwSpinBox->isChecked())wettabilityFlag=3;
    if(ui->mwsSpinBox->isChecked())wettabilityFlag=4;
    if(ui->mwlSpinBox->isChecked())wettabilityFlag=5;
    settings.setValue("wettabilityFlag",wettabilityFlag);
    settings.setValue("minWaterWetTheta",ui->minWWThetaSpinBox->text());
    settings.setValue("maxWaterWetTheta",ui->maxWWThetaSpinBox->text());
    settings.setValue("minOilWetTheta",ui->minOWThetaSpinBox->text());
    settings.setValue("maxOilWetTheta",ui->maxOWThetaSpinBox->text());
    settings.setValue("oilWetFraction",ui->owFractionSpinBox->text());
    settings.setValue("shapeFactor",ui->shapeFactor->text());
    settings.endGroup();
}

void MainWindow::exportTwoPhaseDataFromGUI()
{
    QSettings settings("Input Data/twoPhaseFlow_data.txt", QSettings::IniFormat);

    settings.beginGroup("Cycles");
    settings.setValue("twoPhaseSS",ui->twoPhaseFlowSteady->isChecked());
    settings.setValue("threePhaseSS",ui->threePhaseFlowSteady->isChecked());
    settings.setValue("ganglionUSS",ui->twoPhaseFlowUnSteady->isChecked());
    settings.setValue("drainageUSS",ui->twoPhaseFlowUnSteadyFast->isChecked());
    settings.setValue("primaryDrainageSimulation", ui->PDCheckBox->isChecked());
    settings.setValue("primaryImbibitionSimulation", ui->PICheckBox->isChecked());
    settings.setValue("secondaryDrainageSimulation", ui->SDCheckBox->isChecked());
    settings.setValue("secondaryImbibitionSimulation", ui->SICheckBox->isChecked());
    settings.setValue("tertiaryDrainageSimulation", ui->TDCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup("Parameters");
    settings.setValue("twoPhaseSimulationSteps",ui->twoPhaseStepsLineEdit->text());
    settings.setValue("twoPhaseSeed",ui->twoPhaseSeed->text());
    settings.setValue("waterWetThetaIncrementAfterAging",ui->twoPhaseWWThetaIncSpinBox->text());
    settings.setValue("oilWetThetaIncrementAfterAging",ui->twoPhaseOWThetaIncSpinBox->text());
    settings.setValue("waterFilms",ui->waterFilmsCheckBox->isChecked());
    settings.setValue("oilFilms",ui->oilFilmsCheckBox->isChecked());
    settings.setValue("gravity",ui->twoPhaseGravityCheckBox->isChecked());
    settings.setValue("g",ui->twoPhaseGLineEdit->text());
    settings.setValue("viscousForces",ui->twoPhaseViscousForcesCheckBox->isChecked());
    settings.setValue("PinViscous",ui->twoPhasePinViscousLineEdit->text());
    settings.setValue("PoutViscous",ui->twoPhasePoutViscousLineEdit->text());
    settings.setValue("relativePermeabilitiesCalculation",ui->twoPhaseRPermCheckBox->isChecked());
    settings.setValue("videoRecording",ui->twoPhasePostProcessCheckBox->isChecked());
    //settings.setValue("",);
    settings.endGroup();

    settings.beginGroup("Fluids");
    settings.setValue("oilViscosity",ui->twoPhaseOilVisLineEdit->text());
    settings.setValue("waterViscosity",ui->twoPhaseWaterVisLineEdit->text());
    settings.setValue("gasViscosity",ui->twoPhaseGasVisLineEdit->text());
    settings.setValue("OGSurfaceTension",ui->twoPhaseOGIFTLineEdit->text());
    settings.setValue("OWSurfaceTension",ui->twoPhaseOWIFTLineEdit->text());
    settings.setValue("WGSurfaceTension",ui->twoPhaseWGIFTLineEdit->text());
    settings.setValue("initialWaterSaturation",ui->twoPhaseInitialSwSpinBox->text());
    int distribution;
    if(ui->twoPhaseSwRandomRadioButton->isChecked())distribution=1;
    if(ui->twoPhaseSwSmallRadioButton->isChecked())distribution=2;
    if(ui->twoPhaseSwBigRadioButton->isChecked())distribution=3;
    if(ui->twoPhaseSwAPDRadioButton->isChecked())distribution=4;
    settings.setValue("waterDistribution",distribution);
    settings.setValue("waterWetSwi",ui->twoPhaseAging->isChecked());
    settings.endGroup();

    settings.beginGroup("UnsteadyState");
    settings.setValue("flowRate",ui->twoPhaseFlowRateLineEdit->text());
    settings.setValue("precisionFactor",ui->twoPhasePrecisionFactorLineEdit->text());
    settings.setValue("simulationTime",ui->twoPhaseSimulationTimeLineEdit->text());
    settings.setValue("overrideByInjectedPVs",ui->overrideByInjectedPVs->isChecked());
    settings.setValue("injectedPVs",ui->injectedPVs->text());
    settings.setValue("rateBump",ui->rateBump->isChecked());
    settings.setValue("rateBumpMultiplier",ui->rateBumpMultiplier->value());
    settings.setValue("rateBumpTiming",ui->rateBumpTiming->value());
    settings.endGroup();

    settings.beginGroup("threePhase");
    settings.setValue("WAGCycle",ui->threePhaseWAGCycle->isChecked());
    settings.setValue("customCycle",ui->threePhaseCustomCycle->isChecked());
    settings.setValue("initialWaterFlood",ui->threePhaseInitialWaterFlood->isChecked());
    settings.setValue("waterFlood",ui->threePhaseWaterFlood->isChecked());
    settings.setValue("gasFlood",ui->threePhaseGasFlood->isChecked());
    settings.setValue("repeatWAG",ui->threePhaseRepeatWAG->isChecked());
    settings.setValue("WAGCyclesNumber",ui->threePhaseCyclesNumber->text());
    settings.setValue("threePhaseInitialPhase",(ui->threePhaseInitialPhase->currentText()).toLower());
    settings.setValue("threePhaseSecondPhase",ui->threePhaseSecondPhase->currentText().toLower());
    settings.setValue("threePhaseInvadingPhase",ui->threePhaseInvadingPhase->currentText().toLower());
    settings.setValue("stopAfterBreakthrough",ui->threePhaseStopAfterBreakthrough->isChecked());
    settings.setValue("noSnapOff",ui->threePhaseNoSnapOff->isChecked());
    settings.endGroup();

    settings.beginGroup("EOR");
    settings.setValue("surfactantInjection",ui->twoPhaseSurfactantInjection->isChecked());
    settings.setValue("surfactantInjectionAfterBreakthrough",ui->twoPhaseSurfactantInjectionAfterBr->isChecked());
    settings.setValue("surfactantInjectionAfterHST",ui->twoPhaseSurfactantInjectionAfterHST->isChecked());
    settings.setValue("surfactantRemoveAfterBreakthrough",ui->twoPhaseSurfactantRemoveAfterBr->isChecked());
    settings.setValue("surfactantRemoveAfterHST",ui->twoPhaseSurfactantRemoveAfterHST->isChecked());
    settings.setValue("angleChange",ui->twoPhaseAngleChange->text());
    settings.setValue("surfaceTensionChange",ui->twoPhaseIFTChange->text());

    settings.setValue("polymerInjection",ui->twoPhasePolymerInjection->isChecked());
    settings.setValue("polymerInjectionAfterBreakthrough",ui->twoPhasePolymerInjectionAfterBr->isChecked());
    settings.setValue("polymerInjectionAfterHST",ui->twoPhasePolymerInjectionAfterHST->isChecked());
    settings.setValue("polymerRemoveAfterBreakthrough",ui->twoPhasePolymerRemoveAfterBr->isChecked());
    settings.setValue("polymerRemoveAfterHST",ui->twoPhasePolymerRemoveAfterHST->isChecked());
    settings.setValue("polymerMicroscopicDiversion",ui->twoPhasePolymerMicroscopicDiversion->isChecked());
    settings.setValue("polymerCriticalRadius",ui->twoPhasePolymerCriticalRadius->text());
    settings.setValue("polymerViscosityChange",ui->twoPhasePolymerViscosityChange->text());

    settings.endGroup();

}

void MainWindow::exportDepletionDataFromGUI()
{

    QSettings settings("Input Data/depletion_data.txt", QSettings::IniFormat);

    settings.beginGroup("Cycles");
    settings.setValue("injectionSimulation",ui->injectionRadioButton->isChecked());
    settings.setValue("diffusionSimulation",ui->diffusionRadioButton->isChecked());
    settings.setValue("growthSimulation",ui->growthCheckBox->isChecked());
    settings.setValue("migrationSimulation",ui->migrationCheckBox->isChecked());
    settings.setValue("threePhaseDepletion",ui->threePhaseDepletion->isChecked());
    settings.endGroup();

    settings.beginGroup("Parameters");
    settings.setValue("BPPressure",ui->depBPPressLineEdit->text());
    settings.setValue("depletionRate",ui->depDepletionRateLineEdit->text());
    settings.setValue("depletionSteps",ui->depDepletionStepsLineEdit->text());
    settings.setValue("depletionTime",ui->depDepletionTimeLineEdit->text());
    settings.setValue("optimalDepletionStep",ui->depOptimalStepCheckBox->isChecked());
    settings.setValue("injectionRate",ui->depInjectionRateLineEdit->text());
    settings.setValue("depletionSeed",ui->depSeed->text());
    int nucleationType;
    if(ui->depINRadioBox->isChecked())nucleationType=1;
    if(ui->depPNRadioBox->isChecked())nucleationType=2;
    settings.setValue("nucleationType",nucleationType);
    settings.setValue("bubblesToNucleateIN",ui->depBubbleNumSpinBox->text());
    settings.setValue("maxCavityRadius",ui->depMaxCavRadiusLineEdit->text());
    settings.setValue("minCavityRadius",ui->depMinCavRadiusLineEdit->text());
    settings.setValue("cavityDensity",ui->depCavityDensityLineEdit->text());
    int growthType;
    if(ui->depMultiFillRadioBox->isChecked())growthType=1;
    if(ui->depInvasionPercRadioBox->isChecked())growthType=2;
    settings.setValue("growthType",growthType);

    settings.endGroup();

    settings.beginGroup("Fluids");
    settings.setValue("pressureDependentDataPath",ui->depPressDependDataPathLineEdit->text());
    settings.setValue("oilDiffusionCoef",ui->depOilDiffCoefLineEdit->text());
    settings.setValue("waterDiffusionCoef",ui->depWaterDiffCoefLineEdit->text());
    settings.setValue("overrideWithFixedData",ui->depLoadPressDependCheckBox->isChecked());
    settings.setValue("oilViscosity",ui->depOilVisLineEdit->text());
    settings.setValue("waterViscosity",ui->depWaterVisLineEdit->text());
    settings.setValue("gasViscosity",ui->depGasVisLineEdit->text());
    settings.setValue("OGSurfaceTension",ui->depOGIFTLineEdit->text());
    settings.setValue("OWSurfaceTension",ui->depOWIFTLineEdit->text());
    settings.setValue("WGSurfaceTension",ui->depWGIFTLineEdit->text());
    settings.setValue("initialWaterSaturation",ui->depletionInitialSwSpinBox->text());
    int distribution;
    if(ui->depletionSwRandomRadioButton->isChecked())distribution=1;
    if(ui->depletionSwSmallRadioButton->isChecked())distribution=2;
    if(ui->depletionSwBigRadioButton->isChecked())distribution=3;
    if(ui->depSwAPDRadioButton->isChecked())distribution=4;
    settings.setValue("waterDistribution",distribution);
    settings.endGroup();

    settings.beginGroup("Misc");
    settings.setValue("gravity",ui->depGravityCheckBox->isChecked());
    settings.setValue("g",ui->depGLineEdit->text());
    settings.setValue("viscousForces",ui->depViscousForcesCheckBox->isChecked());
    settings.setValue("PinViscous",ui->depPinViscousLineEdit->text());
    settings.setValue("PoutViscous",ui->depPoutViscousLineEdit->text());
    settings.setValue("relativePermeabilitiesCalculation",ui->depRPermCheckBox->isChecked());
    settings.setValue("videoRecording",ui->depPostProcessCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup("threePhase");
    settings.setValue("threePhaseInitialPhase",(ui->depletionInitialPhase->currentText()).toLower());
    settings.setValue("threePhaseSecondPhase",ui->depletionSecondPhase->currentText().toLower());
    settings.setValue("stopAfterBreakthrough",ui->depletionStopAfterBreakthrough->isChecked());
    settings.setValue("noSnapOff",ui->depletionNoSnapOff->isChecked());
    settings.endGroup();

}

void MainWindow::updatePlotRelativePermeabilties(string filename)
{
    //prepare data
    ifstream file(filename.c_str());
    QVector<double> sw;
    QVector<double> krw;
    QVector<double> kro;
    string dummy;

    if(file)
    {
        getline(file,dummy);
        double value;
        while(file>>value)
        {
            sw.push_back(value);
            file>>value;
            krw.push_back(value);
            file>>value;
            kro.push_back(value);
        }
    }

    //remove previous graph
    ui->plotWidget->clearGraphs();

    //update title
    plotTitle->setText("Relative Permeabilities Curves");

    // create graph and assign data to it
    ui->plotWidget->addGraph();
    ui->plotWidget->graph(0)->setPen(QPen(Qt::blue));
    ui->plotWidget->graph(0)->setName("krw");
    ui->plotWidget->graph(0)->setData(sw, krw);
    ui->plotWidget->addGraph();
    ui->plotWidget->graph(1)->setPen(QPen(Qt::red));
    ui->plotWidget->graph(1)->setName("kro");
    ui->plotWidget->graph(1)->setData(sw, kro);

    // give the axes labels and ranges
    ui->plotWidget->xAxis->setLabel("Sw");
    ui->plotWidget->yAxis->setLabel("kr/k");
    ui->plotWidget->xAxis->setRange(0, 1);
    ui->plotWidget->yAxis->setRange(0, 1);

    ui->plotWidget->replot();
    ui->plotWidget->show();
}

void MainWindow::updatePlotCapillaryPressure(string filename)
{
    //prepare data
    ifstream file(filename.c_str());
    QVector<double> sw;
    QVector<double> pc;
    string dummy;

    if(file)
    {
        getline(file,dummy);
        double value;
        while(file>>value)
        {
            sw.push_back(value);
            file>>value;
            pc.push_back(value);
        }
    }

    //remove previous graph
    ui->plotWidget->clearGraphs();

    //update title
    plotTitle->setText("Capillary Pressure Curve");

    // create graph and assign data to it
    ui->plotWidget->addGraph();
    ui->plotWidget->graph(0)->setData(sw, pc);
    ui->plotWidget->graph(0)->setName("pc");

    // give the axes some labels and ranges
    ui->plotWidget->xAxis->setLabel("Sw");
    ui->plotWidget->yAxis->setLabel("Pc(psi)");

    ui->plotWidget->graph(0)->rescaleAxes();
    ui->plotWidget->xAxis->setRange(0, 1);

    ui->plotWidget->replot();
    ui->plotWidget->show();
}


void MainWindow::getNetworkResults()
{
    ui->permeabilityLabel->setText(QString::number(net->getAbsolutePermeability()/0.987e-15,'f',2));
    ui->porosityLabel->setText(QString::number(net->getPorosity()*100,'f',2));
    ui->totalPoresLabel->setText(QString::number(net->getTotalOpenedPores()));
    ui->totalNodesLabel->setText(QString::number(net->getTotalOpenedNodes()));
    ui->networkRunningLabel->setText("");
    ui->loadNetworkButton->setEnabled(true);
    ui->loadNetworkButton->setFocus();
    setup3dWidget();
}

void MainWindow::getTwoPhaseSimulationResults()
{
    ui->twoPhaseRunningLabel->setText("");
    ui->twoPhaseSimButton->setEnabled(true);
    ui->twoPhaseSimStopButton->setEnabled(false);
}

void MainWindow::getDepletionSimulationResults()
{
    ui->depRunningLabel->setText("");
    ui->depSimButton->setEnabled(true);
    ui->depSimStopButton->setEnabled(false);
}

void MainWindow::saveImages()
{
    if(net->getRecord() && net->getVideoRecording())
    {
        ++imageIndex;
        QImage image = ui->widget->grabFrameBuffer();
        image.save( "Videos/IMG"+QString::number(1000000000+imageIndex)+".png" );
    }
}

void MainWindow::renderFinished()
{
    ui->renderNetworkButton->setEnabled(true);
    ui->renderStopButton->setEnabled(false);
    ui->networkRenderingLabel->setText("");
    net->setSimulationRunning(false);
}

void MainWindow::on_loadNetworkButton_clicked()
{
    if(net->getSimulationRunning())
        return;

    ui->networkRunningLabel->setText("loading...");
    ui->loadNetworkButton->setEnabled(false);

    if(!ui->loadFromFileRadioButton->isChecked())
        exportNetworkDataFromGUI();

    QThread *t = new QThread;
    worker *w = new worker(net,1);
    w->moveToThread(t);
    connect(t, SIGNAL(started()), w, SLOT(process()));
    connect(w, SIGNAL(finished()), t, SLOT(quit()));
    connect(w, SIGNAL(finished()), this, SLOT(getNetworkResults()));
    connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
    connect(t, SIGNAL(finished()), w, SLOT(deleteLater()));
    t->start();
}

void MainWindow::on_regularRadioButton_clicked()
{
    ui->pathToExtractedLabel->setEnabled(false);
    ui->pathToExtractedLineEdit->setEnabled(false);
    ui->rockPrefixLabel->setEnabled(false);
    ui->rockList->setEnabled(false);
    ui->extendCheckBox->setEnabled(false);
    ui->extensionNumber->setEnabled(false);
    ui->oneDirection->setEnabled(false);
}

void MainWindow::on_calibratedRadioButton_clicked()
{
    ui->pathToExtractedLabel->setEnabled(false);
    ui->pathToExtractedLineEdit->setEnabled(false);
    ui->rockPrefixLabel->setEnabled(false);
    ui->rockList->setEnabled(false);
    ui->extendCheckBox->setEnabled(false);
    ui->extensionNumber->setEnabled(false);
    ui->oneDirection->setEnabled(false);
}

void MainWindow::on_extractedRadioButton_clicked()
{
    ui->pathToExtractedLabel->setEnabled(true);
    ui->pathToExtractedLineEdit->setEnabled(true);
    ui->rockPrefixLabel->setEnabled(true);
    ui->rockList->setEnabled(true);
    ui->extendCheckBox->setEnabled(true);
    ui->extensionNumber->setEnabled(true);
    ui->oneDirection->setEnabled(true);
}

void MainWindow::on_twoPhaseSimButton_clicked()
{
    if(!ui->twoPhaseFlowSteady->isChecked())
    {
        QMessageBox Msgbox(this);
        Msgbox.setText("Only steady-state two-phase simulations are supported in the basic version of numSCAL.\n To have full access to the other features, contact the author: Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>.");
        Msgbox.exec();
    }
    else
    {
        if(!net->getReady() || net->getSimulationRunning())
            return;

        ui->twoPhaseRunningLabel->setText("running...");
        ui->twoPhaseSimButton->setEnabled(false);
        ui->twoPhaseSimStopButton->setEnabled(true);
        ui->widget->setI(0);
        imageIndex=0;

        if(!ui->twoPhaseLoadFromFileRadioButton->isChecked())
            exportTwoPhaseDataFromGUI();

        QThread *t = new QThread;
        worker *w = new worker(net,2);
        w->moveToThread(t);
        connect(t, SIGNAL(started()), w, SLOT(process()));
        connect(w, SIGNAL(finished()), t, SLOT(quit()));
        connect(w, SIGNAL(finished()), this, SLOT(getTwoPhaseSimulationResults()));
        connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
        connect(t, SIGNAL(finished()), w, SLOT(deleteLater()));
        t->start();
    }
}

void MainWindow::on_twoPhaseSimStopButton_clicked()
{
    net->setCancel(true);
    ui->twoPhaseRunningLabel->setText("stopping...");
}

void MainWindow::on_visibleOilCheckBox_clicked()
{
    ui->widget->setOilVisible(ui->visibleOilCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_visibleWaterCheckBox_clicked()
{
    ui->widget->setWaterVisible(ui->visibleWaterCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_visibleGasCheckBox_clicked()
{
    ui->widget->setGasVisible(ui->visibleGasCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_visibleOilWetCheckBox_clicked()
{
    ui->widget->setOilWetVisible(ui->visibleOilWetCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_visibleWaterWetCheckBox_clicked()
{
    ui->widget->setWaterWetVisible(ui->visibleWaterWetCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_pore3DCheckBox_clicked()
{
    ui->widget->setPoreBodies(ui->pore3DCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_node3DCheckBox_clicked()
{
    ui->widget->setNodeBodies(ui->node3DCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_lightCheckBox_clicked()
{
    ui->widget->setLight(ui->lightCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_resetRadioButton_clicked()
{
    setup3dWidget();
    ui->widget->updateGL();
    ui->resetRadioButton->setChecked(false);
}

void MainWindow::on_rotateCheckBox_clicked()
{
    ui->widget->setAnimation(ui->rotateCheckBox->isChecked());
}

void MainWindow::on_PDCheckBox_clicked()
{
    ui->PDCheckBox->setChecked(true);
    ui->PICheckBox->setChecked(false);
    ui->SDCheckBox->setChecked(false);
    ui->SICheckBox->setChecked(false);
    ui->TDCheckBox->setChecked(false);
}

void MainWindow::on_PICheckBox_clicked()
{
    ui->PDCheckBox->setChecked(true);
    ui->PICheckBox->setChecked(true);
    ui->SDCheckBox->setChecked(false);
    ui->SICheckBox->setChecked(false);
    ui->TDCheckBox->setChecked(false);
}

void MainWindow::on_SDCheckBox_clicked()
{
    ui->PDCheckBox->setChecked(true);
    ui->PICheckBox->setChecked(true);
    ui->SDCheckBox->setChecked(true);
    ui->SICheckBox->setChecked(false);
    ui->TDCheckBox->setChecked(false);
}

void MainWindow::on_SICheckBox_clicked()
{
    ui->PDCheckBox->setChecked(true);
    ui->PICheckBox->setChecked(true);
    ui->SDCheckBox->setChecked(true);
    ui->SICheckBox->setChecked(true);
    ui->TDCheckBox->setChecked(false);
}

void MainWindow::on_TDCheckBox_clicked()
{
    ui->PDCheckBox->setChecked(true);
    ui->PICheckBox->setChecked(true);
    ui->SDCheckBox->setChecked(true);
    ui->SICheckBox->setChecked(true);
    ui->TDCheckBox->setChecked(true);
}

void MainWindow::on_depSimButton_clicked()
{
//    if(!net->getReady() || net->getSimulationRunning())
//        return;

//    ui->depRunningLabel->setText("running...");
//    ui->depSimButton->setEnabled(false);
//    ui->depSimStopButton->setEnabled(true);
//    ui->widget->setI(0);
//    imageIndex=0;

//    if(!ui->depLoadFromFileRadioButton->isChecked())
//        exportDepletionDataFromGUI();

//    QThread *t = new QThread;
//    worker *w = new worker(net,3);
//    w->moveToThread(t);
//    connect(t, SIGNAL(started()), w, SLOT(process()));
//    connect(w, SIGNAL(finished()), t, SLOT(quit()));
//    connect(w, SIGNAL(finished()), this, SLOT(getDepletionSimulationResults()));
//    connect(w, SIGNAL(finished()), w, SLOT(deleteLater()));
//    connect(t, SIGNAL(finished()), w, SLOT(deleteLater()));
//    t->start();

    QMessageBox Msgbox(this);
    Msgbox.setText("Only steady-state two-phase simulations are supported in the basic version of numSCAL.\n To have full access to the other features, contact the author: Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>.");
    Msgbox.exec();
}

void MainWindow::on_depSimStopButton_clicked()
{
    net->setCancel(true);
    ui->depRunningLabel->setText("stopping...");
}

void MainWindow::on_growthCheckBox_clicked()
{
    ui->growthCheckBox->setChecked(true);
    ui->migrationCheckBox->setChecked(false);
}

void MainWindow::on_migrationCheckBox_clicked()
{
    ui->growthCheckBox->setChecked(true);
    ui->migrationCheckBox->setChecked(true);
}

void MainWindow::on_depOptimalStepCheckBox_clicked(bool checked)
{
    ui->depDepletionStepsLabel->setEnabled(!checked);
    ui->depDepletionStepsLineEdit->setEnabled(!checked);
}

void MainWindow::on_depINRadioBox_clicked()
{
    ui->depBubbleNumSpinBox->setEnabled(true);
    ui->depMaxCavRadiusLineEdit->setEnabled(false);
    ui->depMinCavRadiusLineEdit->setEnabled(false);
    ui->depCavityDensityLineEdit->setEnabled(false);
}

void MainWindow::on_depPNRadioBox_clicked()
{
    ui->depBubbleNumSpinBox->setEnabled(false);
    ui->depMaxCavRadiusLineEdit->setEnabled(true);
    ui->depMinCavRadiusLineEdit->setEnabled(true);
    ui->depCavityDensityLineEdit->setEnabled(true);
}

void MainWindow::on_depLoadPressDependCheckBox_clicked()
{

}

void MainWindow::on_xCutCheckBox_clicked(bool checked)
{
    ui->xCutSpinBox->setEnabled(checked);
    ui->widget->setCutX(checked);
    ui->widget->setCutXValue(ui->xCutSpinBox->text().toDouble());
    ui->widget->updateGL();
}

void MainWindow::on_yCutCheckBox_clicked(bool checked)
{
    ui->yCutSpinBox->setEnabled(checked);
    ui->widget->setCutY(checked);
    ui->widget->setCutYValue(ui->yCutSpinBox->text().toDouble());
    ui->widget->updateGL();
}

void MainWindow::on_zCutCheckBox_clicked(bool checked)
{
    ui->zCutSpinBox->setEnabled(checked);
    ui->widget->setCutZ(checked);
    ui->widget->setCutZValue(ui->zCutSpinBox->text().toDouble());
    ui->widget->updateGL();
}

void MainWindow::on_twoPhaseSaveCurveButton_clicked()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Images (*.png *.PNG)"));
    if (fn.isEmpty())
            return;
    if (! (fn.endsWith(".png", Qt::CaseInsensitive) ) )
        fn += ".png"; // default

    ui->plotWidget->savePng(fn,540,540);
}

void MainWindow::on_saveNetworkImageButton_clicked()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Images (*.png *.PNG)"));
    if (fn.isEmpty())
            return;
    if (! (fn.endsWith(".png", Qt::CaseInsensitive) ) )
        fn += ".png"; // default

    QImage image = ui->widget->grabFrameBuffer();
    image.save( fn, "png" );
}

void MainWindow::on_renderNetworkButton_clicked()
{
    if(ui->loadNetworkStatusRadioButton->isChecked())
    {
        ui->widget->setLoad(true);
        ui->widget->setOnlyPhases(ui->onlyPhasesCheckBox->isChecked());
        ui->widget->setPhasePoresPath(ui->pathToPhasePoresLineEdit->text().toStdString());
        ui->widget->setPhaseNodesPath(ui->pathToPhaseNodesLineEdit->text().toStdString());
    }

    if(ui->renderImagesRadioButton->isChecked())
    {
        ui->widget->setRender(true);
        ui->widget->setOnlyPhases(ui->onlyPhasesCheckBox->isChecked());
        ui->widget->setTotalImages(ui->imagesCountLineEdit->text().toInt());
        ui->widget->setImageCount(0);
    }

    ui->renderNetworkButton->setEnabled(false);
    ui->renderStopButton->setEnabled(true);

    ui->networkRenderingLabel->setText("rendering...");
    net->setSimulationRunning(true);
}

void MainWindow::on_renderStopButton_clicked()
{
    ui->widget->setTotalImages(0);
    ui->networkRenderingLabel->setText("");
    net->setSimulationRunning(false);
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->title->text()!="") plotTitle->setText(ui->title->text());
    if(ui->xAxisTitle->text()!="") ui->plotWidget->xAxis->setLabel(ui->xAxisTitle->text());
    if(ui->yAxisTitle->text()!="") ui->plotWidget->yAxis->setLabel(ui->yAxisTitle->text());
    if(ui->minXAxis->text()!="") ui->plotWidget->xAxis->setRangeLower(ui->minXAxis->text().toDouble());
    if(ui->maxXAxis->text()!="") ui->plotWidget->xAxis->setRangeUpper(ui->maxXAxis->text().toDouble());
    if(ui->minYAxis->text()!="") ui->plotWidget->yAxis->setRangeLower(ui->minYAxis->text().toDouble());
    if(ui->maxYAxis->text()!="") ui->plotWidget->yAxis->setRangeUpper(ui->maxYAxis->text().toDouble());

    if(ui->tickStep->text()!=""){ui->plotWidget->xAxis->setAutoTickStep(false);ui->plotWidget->xAxis->setTickStep(ui->tickStep->text().toDouble());}
    ui->plotWidget->replot();

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->fileToPlot->setText(QFileDialog::getOpenFileName(this,tr("Select file to plot"),"Results",tr("text files (*.txt)")));
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->plotWidget->clearGraphs();
    ui->plotWidget->xAxis->setLabel("");
    ui->plotWidget->yAxis->setLabel("");
    plotTitle->setText("title");
    ui->plotWidget->xAxis->setAutoTickStep(true);
    ui->plotWidget->yAxis->setAutoTickStep(true);
    ui->plotWidget->rescaleAxes();
    ui->plotWidget->replot();

    totalCurves=0;
}

void MainWindow::on_plot_clicked()
{
    ifstream file(ui->fileToPlot->text().toStdString().c_str());

    if(file && totalCurves<7)
    {
        string header;
        getline(file,header);

        std::stringstream ss(header);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);

        if(vstrings.size()<=1) return;

        vector<QVector<double> > data;
        vector<QString> dataHeaders;
        data.resize(vstrings.size());

        for(int i=0;i<vstrings.size();++i)
            dataHeaders.push_back(QString::fromStdString(( vstrings[i])));

        double value;
        while(file>>value)
        {
            data[0].push_back(value);
            for(int i=1;i<vstrings.size();++i)
            {
                file>>value;
                data[i].push_back(value);
            }
        }

        for(int i=1;i<vstrings.size();++i)
        {
            QPen pen(QtColours[totalCurves]);
            pen.setWidth(2);
            ui->plotWidget->addGraph();
            ui->plotWidget->graph(totalCurves)->setPen(pen);
            ui->plotWidget->graph(totalCurves)->setName(dataHeaders[i]);
            ui->plotWidget->graph(totalCurves)->setData(data[0], data[i]);
            ui->plotWidget->rescaleAxes();
            totalCurves++;
        }
        ui->plotWidget->replot();
    }
}

void MainWindow::plotCurvesRealTime()
{
    if(ui->realTimeCheckBox->isChecked())
    {
        ui->plotWidget->clearGraphs();

        totalCurves=0;

        ifstream file(ui->fileToPlot->text().toStdString().c_str());

        if(file && totalCurves<7)
        {
            string header;
            getline(file,header);

            std::stringstream ss(header);
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> vstrings(begin, end);

            if(vstrings.size()<=1) return;

            vector<QVector<double> > data;
            vector<QString> dataHeaders;
            data.resize(vstrings.size());

            for(int i=0;i<vstrings.size();++i)
                dataHeaders.push_back(QString::fromStdString(( vstrings[i])));

            double value;
            while(file>>value)
            {
                data[0].push_back(value);
                for(int i=1;i<vstrings.size();++i)
                {
                    file>>value;
                    data[i].push_back(value);
                }
            }

            for(int i=1;i<vstrings.size();++i)
            {
                QPen pen(QtColours[totalCurves]);
                pen.setWidth(2);
                ui->plotWidget->addGraph();
                ui->plotWidget->graph(totalCurves)->setPen(pen);
                ui->plotWidget->graph(totalCurves)->setName(dataHeaders[i]);
                ui->plotWidget->graph(totalCurves)->setData(data[0], data[i]);
                totalCurves++;
            }
            if(ui->title->text()!="") plotTitle->setText(ui->title->text());
            if(ui->xAxisTitle->text()!="") ui->plotWidget->xAxis->setLabel(ui->xAxisTitle->text());
            if(ui->yAxisTitle->text()!="") ui->plotWidget->yAxis->setLabel(ui->yAxisTitle->text());
            if(ui->minXAxis->text()!="") ui->plotWidget->xAxis->setRangeLower(ui->minXAxis->text().toDouble());
            if(ui->maxXAxis->text()!="") ui->plotWidget->xAxis->setRangeUpper(ui->maxXAxis->text().toDouble());
            if(ui->minYAxis->text()!="") ui->plotWidget->yAxis->setRangeLower(ui->minYAxis->text().toDouble());
            if(ui->maxYAxis->text()!="") ui->plotWidget->yAxis->setRangeUpper(ui->maxYAxis->text().toDouble());

            if(ui->tickStep->text()!=""){ui->plotWidget->xAxis->setAutoTickStep(false);ui->plotWidget->xAxis->setTickStep(ui->tickStep->text().toDouble());}
            ui->plotWidget->replot();
        }
    }
}

void MainWindow::on_twoPhaseSurfactantInjection_clicked(bool checked)
{
    if(checked)
    {
        ui->twoPhaseSurfactantInjectionAfterBr->setChecked(false);
        ui->twoPhaseSurfactantInjectionAfterHST->setChecked(false);
    }
}

void MainWindow::on_twoPhaseSurfactantInjectionAfterBr_clicked(bool checked)
{
    if(checked)
    {
        ui->twoPhaseSurfactantInjection->setChecked(false);
        ui->twoPhaseSurfactantInjectionAfterHST->setChecked(false);
    }
}

void MainWindow::on_twoPhaseSurfactantInjectionAfterHST_clicked(bool checked)
{
    if(checked)
    {
        ui->twoPhaseSurfactantInjectionAfterBr->setChecked(false);
        ui->twoPhaseSurfactantInjection->setChecked(false);
    }
}

void MainWindow::on_xCutSpinBox_returnPressed()
{
    ui->widget->setCutXValue(ui->xCutSpinBox->text().toDouble());
    ui->widget->updateGL();
}

void MainWindow::on_yCutSpinBox_returnPressed()
{
    ui->widget->setCutYValue(ui->yCutSpinBox->text().toDouble());
    ui->widget->updateGL();
}

void MainWindow::on_zCutSpinBox_returnPressed()
{
    ui->widget->setCutZValue(ui->zCutSpinBox->text().toDouble());
    ui->widget->updateGL();
}
