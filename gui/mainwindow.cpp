/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "builders/networkbuilder.h"
#include "simulations/simulation.h"
#include "misc/userInput.h"

#include "libs/boost/format.hpp"
#include "qcustomplot.h"

#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>

Qt::GlobalColor QtColours[] = {Qt::red, Qt::blue, Qt::green, Qt::gray, Qt::black, Qt::magenta, Qt::yellow, Qt::gray, Qt::darkBlue, Qt::darkRed, Qt::darkGreen, Qt::darkGray, Qt::darkMagenta, Qt::cyan};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)

{
    initialiseGUI();
    initialiseCurvesWidget();
    initialiseVariables();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete curvesWidget;
}

void MainWindow::on_loadNetworkButton_clicked()
{
    if (currentlyBusy)
        return;

    updateGUIBeforeLoadingNetwork();
    exportNetworkDataFromGUI();
    importNetworkDataFromGUI();

    std::thread([this]() {
        builder = PNM::networkBuilder::createBuilder();

        connect(builder.get(), SIGNAL(notifyGUI()), this, SLOT(updateNetworkProgressBar()));
        connect(builder.get(), SIGNAL(finished()), this, SLOT(updateGUIAfterLoadingNetwork()));

        try
        {
            network = builder->build();
        }
        catch (std::bad_alloc)
        {
            std::cout << "Not enough RAM to load the network.\nAborting.\n\n";
            updateGUIAfterNetworkFailure();
        }
    })
        .detach();
}

void MainWindow::on_twoPhaseSimButton_clicked()
{
    if (!networkBuilt || currentlyBusy)
        return;

    exportSimulationDataFromGUI();
    importSimulationDataFromGUI();
    updateGUIBeforeSimulation();

    std::thread([this]() {
        sim = PNM::simulation::createSimulation();

        sim->setNetwork(network);
        connect(sim.get(), SIGNAL(notifyGUI()), this, SLOT(updateGUIDuringSimulation()), Qt::BlockingQueuedConnection);
        connect(sim.get(), SIGNAL(finished()), this, SLOT(updateGUIAfterSimulation()));

        sim->execute();
    })
        .detach();
}

void MainWindow::on_renderNetworkButton_clicked()
{
    if (!networkBuilt || currentlyBusy)
        return;

    exportSimulationDataFromGUI();
    importSimulationDataFromGUI();
    updateGUIBeforeRendering();

    std::thread([this]() {
        sim = PNM::simulation::createRenderer();

        sim->setNetwork(network);
        connect(sim.get(), SIGNAL(notifyGUI()), this, SLOT(updateGUIDuringRendering()), Qt::BlockingQueuedConnection);
        connect(sim.get(), SIGNAL(finished()), this, SLOT(updateGUIAfterRendering()));

        sim->execute();
    })
        .detach();
}

void MainWindow::exportNetworkDataFromGUI()
{
    if (ui->loadFromFileRadioButton->isChecked())
        return;

    QSettings settings("Input_Data/Parameters.txt", QSettings::IniFormat);

    settings.beginGroup("NetworkGeneration_Source");
    settings.setValue("networkRegular", ui->regularNetworkButton->isChecked());
    settings.setValue("networkStatoil", ui->statoilNetworkButton->isChecked());
    settings.setValue("networkNumscal", ui->numscalNetworkButton->isChecked());
    settings.setValue("extractedNetworkPath", ui->pathToExtractedLineEdit->text());
    settings.setValue("rockPrefix", ui->rockList->currentText());
    settings.endGroup();

    settings.beginGroup("NetworkGeneration_Geometry");
    settings.setValue("Nx", ui->NxSpinBox->text());
    settings.setValue("Ny", ui->NySpinBox->text());
    settings.setValue("Nz", ui->NzSpinBox->text());
    settings.setValue("minRadius", ui->minRadiusLineEdit->text());
    settings.setValue("maxRadius", ui->maxRadiusLineEdit->text());
    int distribution;
    if (ui->psdUniformRadioButton->isChecked())
        distribution = 1;
    if (ui->psdRayleighRadioButton->isChecked())
        distribution = 2;
    if (ui->psdTriangularRadioButton->isChecked())
        distribution = 3;
    if (ui->psdNormalRadioButton->isChecked())
        distribution = 4;
    settings.setValue("radiusDistribution", distribution);
    settings.setValue("rayleighParameter", ui->rayleighParamLineEdit->text());
    settings.setValue("triangularParameter", ui->triangularParameter->text());
    settings.setValue("normalMuParameter", ui->muNormalParameter->text());
    settings.setValue("normalSigmaParameter", ui->sigmaNormalParameter->text());
    settings.setValue("poreVolumeConstant", ui->volConsRadiusLineEdit->text());
    settings.setValue("poreVolumeExponent", ui->volExpLineEdit->text());
    settings.setValue("poreConductivityConstant", ui->condConsLineEdit->text());
    settings.setValue("poreConductivityExponent", ui->condExpLineEdit->text());
    settings.setValue("coordinationNumber", ui->zSpinBox->text());
    settings.setValue("degreeOfDistortion", ui->distortionSpinBox->text());
    settings.setValue("aspectRatio", ui->aspectRatioSpinBox->text());
    settings.setValue("length", ui->lengthLineEdit->text());
    settings.setValue("seed", ui->seed->text());
    settings.endGroup();

    settings.beginGroup("NetworkGeneration_Wettability");
    int wettabilityFlag;
    if (ui->wwSpinBox->isChecked())
        wettabilityFlag = 1;
    if (ui->owSpinBox->isChecked())
        wettabilityFlag = 2;
    if (ui->fwSpinBox->isChecked())
        wettabilityFlag = 3;
    if (ui->mwsSpinBox->isChecked())
        wettabilityFlag = 4;
    if (ui->mwlSpinBox->isChecked())
        wettabilityFlag = 5;
    settings.setValue("wettabilityFlag", wettabilityFlag);
    settings.setValue("minWaterWetTheta", ui->minWWThetaSpinBox->text());
    settings.setValue("maxWaterWetTheta", ui->maxWWThetaSpinBox->text());
    settings.setValue("minOilWetTheta", ui->minOWThetaSpinBox->text());
    settings.setValue("maxOilWetTheta", ui->maxOWThetaSpinBox->text());
    settings.setValue("oilWetFraction", ui->owFractionSpinBox->text());
    settings.setValue("shapeFactor", ui->shapeFactor->text());
    settings.endGroup();
}

void MainWindow::exportSimulationDataFromGUI()
{
    if (ui->twoPhaseLoadFromFileRadioButton->isChecked())
        return;

    QSettings settings("Input_Data/Parameters.txt", QSettings::IniFormat);

    settings.beginGroup("FluidInjection_Cycles");
    settings.setValue("twoPhaseSS", ui->twoPhaseFlowSteady->isChecked());
    settings.setValue("tracerFlow", ui->twoPhaseFlowUnSteady->isChecked());
    settings.setValue("drainageUSS", ui->twoPhaseFlowUnSteadyFast->isChecked());
    settings.setValue("templateFlow", ui->templateFlow->isChecked());
    settings.setValue("primaryDrainageSimulation", ui->PDCheckBox->isChecked());
    settings.setValue("primaryImbibitionSimulation", ui->PICheckBox->isChecked());
    settings.setValue("secondaryDrainageSimulation", ui->SDCheckBox->isChecked());
    settings.setValue("secondaryImbibitionSimulation", ui->SICheckBox->isChecked());
    settings.setValue("tertiaryDrainageSimulation", ui->TDCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup("FluidInjection_SS");
    settings.setValue("twoPhaseSimulationSteps", ui->twoPhaseStepsLineEdit->text());
    settings.setValue("filmConductanceResistivity", ui->twoPhasefilmConductance->text());
    settings.setValue("relativePermeabilitiesCalculation", ui->twoPhaseRPermCheckBox->isChecked());
    settings.setValue("extractDataSS", ui->extractDataSSCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup("FluidInjection_USS");
    settings.setValue("flowRate", ui->twoPhaseFlowRateLineEdit->text());
    settings.setValue("simulationTime", ui->twoPhaseSimulationTimeLineEdit->text());
    settings.setValue("overrideByInjectedPVs", ui->overrideByInjectedPVs->isChecked());
    settings.setValue("injectedPVs", ui->injectedPVs->text());
    settings.setValue("tracerDiffusionCoef", ui->twoPhaseDiffCoef->text());
    settings.setValue("extractDataUSS", ui->extractDataUSSCheckBox->isChecked());
    settings.endGroup();

    settings.beginGroup("FluidInjection_Fluids");
    settings.setValue("oilViscosity", ui->twoPhaseOilVisLineEdit->text());
    settings.setValue("waterViscosity", ui->twoPhaseWaterVisLineEdit->text());
    settings.setValue("gasViscosity", ui->twoPhaseGasVisLineEdit->text());
    settings.setValue("OGSurfaceTension", ui->twoPhaseOGIFTLineEdit->text());
    settings.setValue("OWSurfaceTension", ui->twoPhaseOWIFTLineEdit->text());
    settings.setValue("WGSurfaceTension", ui->twoPhaseWGIFTLineEdit->text());
    settings.setValue("initialWaterSaturation", ui->twoPhaseInitialSwSpinBox->text());
    int distribution;
    if (ui->twoPhaseSwRandomRadioButton->isChecked())
        distribution = 1;
    if (ui->twoPhaseSwSmallRadioButton->isChecked())
        distribution = 2;
    if (ui->twoPhaseSwBigRadioButton->isChecked())
        distribution = 3;
    if (ui->twoPhaseSwAPDRadioButton->isChecked())
        distribution = 4;
    settings.setValue("waterDistribution", distribution);
    settings.endGroup();

    settings.beginGroup("FluidInjection_Misc");
    int solverChoice;
    if (ui->choleskyRadioButton->isChecked())
        solverChoice = 1;
    if (ui->bicstabRadioButton->isChecked())
        solverChoice = 2;
    settings.setValue("solverChoice", solverChoice);
    settings.endGroup();

    settings.beginGroup("FluidInjection_Postprocessing");
    settings.setValue("pathToNetworkStateFiles", ui->pathToStateFiles->text());
    settings.setValue("rendererFPS", ui->fps->text());
    settings.setValue("keepFrames", ui->keepFramesCheckBox->isChecked());
    settings.endGroup();
}

void MainWindow::importNetworkDataFromGUI()
{
    PNM::userInput::get().loadNetworkData();
}

void MainWindow::importSimulationDataFromGUI()
{
    PNM::userInput::get().loadSimulationData();
}

void MainWindow::initialiseGUI()
{
    ui->setupUi(this);
    ui->networkBuildingProgressBar->setVisible(false);
    ui->simulationProgressBar->setVisible(false);
    ui->renderingProgressBar->setVisible(false);
    ui->SimNotif->setText("Ready.\n");
    ui->oilColor->click();
}

void MainWindow::initialiseCurvesWidget()
{
    curvesWidget = new QCPPlotTitle(ui->plotWidget, "title");
    ui->plotWidget->plotLayout()->insertRow(0);
    ui->plotWidget->plotLayout()->addElement(0, 0, curvesWidget);
    ui->plotWidget->xAxis->setLabelFont(font());
    ui->plotWidget->xAxis->setTickLabelFont(font());
    ui->plotWidget->yAxis->setLabelFont(font());
    ui->plotWidget->yAxis->setTickLabelFont(font());
    ui->plotWidget->legend->setFont(font());
    ui->plotWidget->legend->setVisible(true);
}

void MainWindow::initialiseVariables()
{
    imageIndex = 0;
    totalCurves = 0;
    currentlyBusy = false;
    networkBuilt = false;
}

void MainWindow::updateGUIBeforeLoadingNetwork()
{
    currentlyBusy = true;
    networkBuilt = false;
    ui->widget->setNetworkBuilt(false);

    ui->networkBuildingProgressBar->setVisible(true);
    ui->networkBuildingProgressBar->setValue(0);
    ui->loadNetworkButton->setEnabled(false);
}

void MainWindow::updateGUIAfterLoadingNetwork()
{
    currentlyBusy = false;
    networkBuilt = true;

    ui->widget->setNetwork(builder->getNetwork());
    ui->widget->setNetworkBuilt(true);
    ui->widget->setAspectRatio();

    ui->networkBuildingProgressBar->setVisible(false);
    ui->loadNetworkButton->setEnabled(true);

    ui->SimNotif->setText(QString::fromStdString(builder->getNotification()));
}

void MainWindow::updateGUIAfterNetworkFailure()
{
    builder.reset();
    currentlyBusy = false;
    ui->networkBuildingProgressBar->setVisible(false);
    ui->loadNetworkButton->setEnabled(true);
}

void MainWindow::updateNetworkProgressBar()
{
    ui->networkBuildingProgressBar->setValue(builder->getProgress());
}

void MainWindow::updateGUIBeforeSimulation()
{
    currentlyBusy = true;

    ui->widget->setSimulationRunnning(true);

    ui->simulationProgressBar->setVisible(true);
    ui->simulationProgressBar->setValue(0);

    ui->twoPhaseSimButton->setEnabled(false);
    ui->twoPhaseSimStopButton->setEnabled(true);
}

void MainWindow::updateGUIAfterSimulation()
{
    ui->widget->setSimulationRunnning(false);

    ui->simulationProgressBar->setVisible(false);

    ui->twoPhaseSimButton->setEnabled(true);
    ui->twoPhaseSimStopButton->setEnabled(false);

    currentlyBusy = false;
}

void MainWindow::updateGUIDuringSimulation()
{
    ui->simulationProgressBar->setValue(sim->getProgress());
    ui->SimNotif->setText(QString::fromStdString(sim->getNotification()));
}

void MainWindow::updateGUIBeforeRendering()
{
    currentlyBusy = true;

    ui->widget->setSimulationRunnning(true);

    imageIndex = 0;

    ui->renderingProgressBar->setVisible(true);
    ui->renderingProgressBar->setValue(0);

    ui->renderNetworkButton->setEnabled(false);
    ui->renderStopButton->setEnabled(true);
}

void MainWindow::updateGUIAfterRendering()
{
    ui->widget->setSimulationRunnning(false);

    ui->renderingProgressBar->setVisible(false);
    ui->renderNetworkButton->setEnabled(true);
    ui->renderStopButton->setEnabled(false);

    currentlyBusy = false;
}

void MainWindow::updateGUIDuringRendering()
{
    ui->renderingProgressBar->setValue(sim->getProgress());
    ui->SimNotif->setText(QString::fromStdString(sim->getNotification()));

    exportNetworkToImage();
}

void MainWindow::exportNetworkToImage()
{
    ui->widget->updateGL();

    QImage image = ui->widget->grabFrameBuffer();
    std::string imagePath = "Videos/IMG" + boost::str(boost::format("%07d") % imageIndex) + ".png";
    image.save(imagePath.c_str());

    imageIndex++;
}

void MainWindow::on_twoPhaseSimStopButton_clicked()
{
    sim->interrupt();
}

void MainWindow::on_renderStopButton_clicked()
{
    sim->interrupt();
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
    ui->widget->setAxes(ui->lightCheckBox->isChecked());
    ui->widget->updateGL();
}

void MainWindow::on_resetRadioButton_clicked()
{
    ui->widget->setAspectRatio();
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

void MainWindow::on_oilColor_clicked()
{
    ui->Rcolor->setValue(ui->widget->getOilColor().x * 255.);
    ui->Gcolor->setValue(ui->widget->getOilColor().y * 255.);
    ui->Bcolor->setValue(ui->widget->getOilColor().z * 255.);
}

void MainWindow::on_waterColor_clicked()
{
    ui->Rcolor->setValue(ui->widget->getWaterColor().x * 255.);
    ui->Gcolor->setValue(ui->widget->getWaterColor().y * 255.);
    ui->Bcolor->setValue(ui->widget->getWaterColor().z * 255.);
}

void MainWindow::on_tracerColor_clicked()
{
    ui->Rcolor->setValue(ui->widget->getTracerColor().x * 255.);
    ui->Gcolor->setValue(ui->widget->getTracerColor().y * 255.);
    ui->Bcolor->setValue(ui->widget->getTracerColor().z * 255.);
}

void MainWindow::on_Rcolor_valueChanged(int value)
{
    if (ui->oilColor->isChecked())
        ui->widget->getOilColor().x = value / 255.;

    if (ui->waterColor->isChecked())
        ui->widget->getWaterColor().x = value / 255.;

    if (ui->tracerColor->isChecked())
        ui->widget->getTracerColor().x = value / 255.;

    ui->widget->updateGL();
}

void MainWindow::on_Gcolor_valueChanged(int value)
{
    if (ui->oilColor->isChecked())
        ui->widget->getOilColor().y = value / 255.;

    if (ui->waterColor->isChecked())
        ui->widget->getWaterColor().y = value / 255.;

    if (ui->tracerColor->isChecked())
        ui->widget->getTracerColor().y = value / 255.;

    ui->widget->updateGL();
}

void MainWindow::on_Bcolor_valueChanged(int value)
{
    if (ui->oilColor->isChecked())
        ui->widget->getOilColor().z = value / 255.;

    if (ui->waterColor->isChecked())
        ui->widget->getWaterColor().z = value / 255.;

    if (ui->tracerColor->isChecked())
        ui->widget->getTracerColor().z = value / 255.;

    ui->widget->updateGL();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->widget->setOilColor(glm::vec3(0.9f, 0.35f, 0.2f));
    ui->widget->setWaterColor(glm::vec3(0.3f, 0.65f, 0.9f));
    ui->widget->setTracerColor(glm::vec3(0.65f, 0.95f, 0.15f));
    ui->oilColor->click();

    ui->widget->updateGL();
}

void MainWindow::on_xCutCheckBox_clicked(bool checked)
{
    ui->widget->setCutXValue(ui->xSlider->value() / 100.);
    ui->widget->setCutX(checked);
    ui->widget->updateGL();
}

void MainWindow::on_yCutCheckBox_clicked(bool checked)
{
    ui->widget->setCutYValue(ui->ySlider->value() / 100.);
    ui->widget->setCutY(checked);
    ui->widget->updateGL();
}

void MainWindow::on_zCutCheckBox_clicked(bool checked)
{
    ui->widget->setCutZValue(ui->zSlider->value() / 100.);
    ui->widget->setCutZ(checked);
    ui->widget->updateGL();
}

void MainWindow::on_xSlider_valueChanged(int value)
{
    if (ui->xCutCheckBox->isChecked())
    {
        ui->widget->setCutXValue(value / 100.);
        ui->widget->updateGL();
    }
}

void MainWindow::on_ySlider_valueChanged(int value)
{
    if (ui->yCutCheckBox->isChecked())
    {
        ui->widget->setCutYValue(value / 100.);
        ui->widget->updateGL();
    }
}

void MainWindow::on_zSlider_valueChanged(int value)
{
    if (ui->zCutCheckBox->isChecked())
    {
        ui->widget->setCutZValue(value / 100.);
        ui->widget->updateGL();
    }
}

void MainWindow::on_linesCheckbox_clicked(bool checked)
{
    ui->widget->setPoreLines(checked);
    ui->widget->updateGL();
}

void MainWindow::on_regularNetworkButton_clicked()
{
    ui->pathToExtractedLabel->setEnabled(false);
    ui->pathToExtractedLineEdit->setEnabled(false);
    ui->rockPrefixLabel->setEnabled(false);
    ui->rockList->setEnabled(false);
}

void MainWindow::on_numscalNetworkButton_clicked()
{
    ui->pathToExtractedLabel->setEnabled(true);
    ui->pathToExtractedLineEdit->setEnabled(true);
    ui->rockPrefixLabel->setEnabled(true);
    ui->rockList->setEnabled(true);
    ui->pathToExtractedLineEdit->setText("numSCAL_Networks/");
}

void MainWindow::on_statoilNetworkButton_clicked()
{
    ui->pathToExtractedLabel->setEnabled(true);
    ui->pathToExtractedLineEdit->setEnabled(true);
    ui->rockPrefixLabel->setEnabled(true);
    ui->rockList->setEnabled(true);
    ui->pathToExtractedLineEdit->setText("IC_Networks/");
}

void MainWindow::on_saveNetworkImageButton_clicked()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Images (*.png *.PNG)"));
    if (fn.isEmpty())
        return;
    if (!(fn.endsWith(".png", Qt::CaseInsensitive)))
        fn += ".png"; // default

    QImage image = ui->widget->grabFrameBuffer();
    image.save(fn, "png");
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About numSCAL"), tr("numSCAL is a pore network simulator developed by \nAhmed Boujelben "
                                                     "at Heriot-Watt University.\n"
                                                     "For enquiries, please send an e-mail to: "
                                                     "ahmed.hamdi.boujelben@gmail.com"));
}

// Methods related to plotWidget

void MainWindow::on_twoPhaseSaveCurveButton_clicked()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Images (*.png *.PNG)"));
    if (fn.isEmpty())
        return;
    if (!(fn.endsWith(".png", Qt::CaseInsensitive)))
        fn += ".png"; // default

    ui->plotWidget->savePng(fn, 540, 540);
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->title->text() != "")
        curvesWidget->setText(ui->title->text());
    if (ui->xAxisTitle->text() != "")
        ui->plotWidget->xAxis->setLabel(ui->xAxisTitle->text());
    if (ui->yAxisTitle->text() != "")
        ui->plotWidget->yAxis->setLabel(ui->yAxisTitle->text());
    if (ui->minXAxis->text() != "")
        ui->plotWidget->xAxis->setRangeLower(ui->minXAxis->text().toDouble());
    if (ui->maxXAxis->text() != "")
        ui->plotWidget->xAxis->setRangeUpper(ui->maxXAxis->text().toDouble());
    if (ui->minYAxis->text() != "")
        ui->plotWidget->yAxis->setRangeLower(ui->minYAxis->text().toDouble());
    if (ui->maxYAxis->text() != "")
        ui->plotWidget->yAxis->setRangeUpper(ui->maxYAxis->text().toDouble());

    if (ui->tickStep->text() != "")
    {
        ui->plotWidget->xAxis->setAutoTickStep(false);
        ui->plotWidget->xAxis->setTickStep(ui->tickStep->text().toDouble());
    }
    ui->plotWidget->replot();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->fileToPlot->setText(QFileDialog::getOpenFileName(this, tr("Select file to plot"), "Results", tr("text files (*.txt)")));
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->plotWidget->clearGraphs();
    ui->plotWidget->xAxis->setLabel("");
    ui->plotWidget->yAxis->setLabel("");
    curvesWidget->setText("title");
    ui->plotWidget->xAxis->setAutoTickStep(true);
    ui->plotWidget->yAxis->setAutoTickStep(true);
    ui->plotWidget->rescaleAxes();
    ui->plotWidget->replot();

    totalCurves = 0;
}

void MainWindow::on_plot_clicked()
{
    std::ifstream file(ui->fileToPlot->text().toStdString().c_str());

    if (file && totalCurves < 14)
    {
        std::string header;
        std::getline(file, header);

        std::stringstream ss(header);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);

        if (vstrings.size() <= 1)
            return;

        std::vector<QVector<double>> data;
        std::vector<QString> dataHeaders;
        data.resize(vstrings.size());

        for (auto &header : vstrings)
            dataHeaders.push_back(QString::fromStdString((header)));

        double value;
        while (file >> value)
        {
            data[0].push_back(value);
            for (unsigned i = 1; i < vstrings.size(); ++i)
            {
                file >> value;
                data[i].push_back(value);
            }
        }

        for (unsigned i = 1; i < vstrings.size(); ++i)
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
