/////////////////////////////////////////////////////////////////////////////
/// Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
/// Created:     2018
/// Copyright:   (c) 2018-2021 Ahmed Hamdi Boujelben
/// Licence:     MIT
/////////////////////////////////////////////////////////////////////////////

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

namespace PNM {
class networkModel;
class networkBuilder;
class simulation;
}  // namespace PNM

class QCPPlotTitle;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void exportNetworkDataFromGUI();
  void exportSimulationDataFromGUI();
  void importNetworkDataFromFile();
  void importSimulationDataFromFile();

 private:
  void initialiseGUI();
  void initialiseCurvesWidget();
  void initialiseVariables();

  Ui::MainWindow *ui;
  QCPPlotTitle *curvesWidget;
  std::shared_ptr<PNM::networkModel> network;
  std::shared_ptr<PNM::networkBuilder> builder;
  std::shared_ptr<PNM::simulation> sim;
  int imageIndex;
  int totalCurves;
  bool currentlyBusy;
  bool networkBuilt;

  static Qt::GlobalColor COLOR_LIST[];

 signals:
  void closing();
  void updateGui();

 private slots:
  void updateGUIBeforeLoadingNetwork();
  void updateGUIAfterLoadingNetwork();
  void updateGUIAfterNetworkFailure();
  void updateNetworkProgressBar();
  void updateGUIBeforeSimulation();
  void updateGUIAfterSimulation();
  void updateGUIDuringSimulation();
  void updateGUIBeforeRendering();
  void updateGUIAfterRendering();
  void updateGUIDuringRendering();
  void updateGUIAfterSimulationFailure();
  void exportNetworkToImage();
  void on_loadNetworkButton_clicked();
  void on_twoPhaseSimButton_clicked();
  void on_twoPhaseSimStopButton_clicked();
  void on_visibleOilCheckBox_clicked();
  void on_visibleWaterCheckBox_clicked();
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
  void on_regularNetworkButton_clicked();
  void on_numscalNetworkButton_clicked();
  void on_statoilNetworkButton_clicked();
};

#endif  // MAINWINDOW_H
