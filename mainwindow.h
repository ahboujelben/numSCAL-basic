/////////////////////////////////////////////////////////////////////////////
/// Name:        mainwindow.h
/// Purpose:     attributes and methods of the main application window
///              are declared here.
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2017
/// Copyright:   (c) 2017 Ahmed Hamdi Boujelben
/// Licence:     Attribution-NonCommercial 4.0 International
/////////////////////////////////////////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "network.h"
#include "worker.h"
#include "qcustomplot.h"
#include <QMainWindow>
#include "QThread"
#include <unistd.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    void setup3dWidget();
    void exportNetworkDataFromGUI();
    void exportTwoPhaseDataFromGUI();

private:
    Ui::MainWindow *ui;
    PNM::network* net;
    QCPPlotTitle* plotTitle;
    QTimer timer;
    int imageIndex;
    int totalCurves;

signals :
    void closing();

private slots:
    void getNetworkResults();
    void getTwoPhaseSimulationResults();
    void saveImages();
    void renderFinished();
    void updateNotificationArea();
    void on_loadNetworkButton_clicked();
    void on_calibratedRadioButton_clicked();
    void on_extractedRadioButton_clicked();
    void on_twoPhaseSimButton_clicked();
    void on_twoPhaseSimStopButton_clicked();
    void on_visibleOilCheckBox_clicked();
    void on_visibleWaterCheckBox_clicked();
    void on_visibleGasCheckBox_clicked();
    void on_visibleOilWetCheckBox_clicked();
    void on_visibleWaterWetCheckBox_clicked();
    void on_pore3DCheckBox_clicked();
    void on_node3DCheckBox_clicked();
    void on_lightCheckBox_clicked();
    void on_resetRadioButton_clicked();
    void on_rotateCheckBox_clicked();
    void on_PDCheckBox_clicked();
    void on_PICheckBox_clicked();
    void on_SDCheckBox_clicked();
    void on_SICheckBox_clicked();
    void on_TDCheckBox_clicked();
    void on_xCutCheckBox_clicked(bool checked);
    void on_yCutCheckBox_clicked(bool checked);
    void on_zCutCheckBox_clicked(bool checked);

    void on_twoPhaseSaveCurveButton_clicked();
    void on_saveNetworkImageButton_clicked();
    void on_renderNetworkButton_clicked();
    void on_renderStopButton_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_plot_clicked();
    void plotCurvesRealTime();
    void on_oilColor_clicked();
    void on_waterColor_clicked();
    void on_tracerColor_clicked();
    void on_Rcolor_valueChanged(int value);
    void on_Gcolor_valueChanged(int value);
    void on_Bcolor_valueChanged(int value);
    void on_pushButton_4_clicked();
    void on_xSlider_valueChanged(int value);
    void on_ySlider_valueChanged(int value);
    void on_zSlider_valueChanged(int value);
    void on_linesCheckbox_clicked(bool checked);
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H
