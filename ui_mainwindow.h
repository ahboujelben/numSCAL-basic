/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>
#include <widget3d.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTabWidget *networkTabBox;
    QWidget *tab;
    QRadioButton *regularRadioButton;
    QRadioButton *extractedRadioButton;
    QRadioButton *calibratedRadioButton;
    QLabel *pathToExtractedLabel;
    QPushButton *loadNetworkButton;
    QLineEdit *pathToExtractedLineEdit;
    QLabel *rockPrefixLabel;
    QLabel *networkRunningLabel;
    QCheckBox *extendCheckBox;
    QRadioButton *loadFromFileRadioButton;
    QCheckBox *oneDirection;
    QComboBox *rockList;
    QLineEdit *extensionNumber;
    QLabel *label_x;
    QWidget *geometryTab;
    QGroupBox *groupBox_3;
    QLabel *label_10;
    QLineEdit *NxSpinBox;
    QLineEdit *NySpinBox;
    QLineEdit *NzSpinBox;
    QLabel *label_25;
    QLabel *label_27;
    QLabel *label_51;
    QLineEdit *seed;
    QGroupBox *groupBox_4;
    QRadioButton *psdRayleighRadioButton;
    QLabel *label_5;
    QLineEdit *minRadiusLineEdit;
    QRadioButton *psdUniformRadioButton;
    QLineEdit *maxRadiusLineEdit;
    QLabel *label_6;
    QLineEdit *rayleighParamLineEdit;
    QLineEdit *volConsRadiusLineEdit;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *volExpLineEdit;
    QLabel *label_17;
    QLineEdit *condConsLineEdit;
    QLabel *label_19;
    QLineEdit *condExpLineEdit;
    QRadioButton *psdTriangularRadioButton;
    QLineEdit *triangularParameter;
    QRadioButton *psdNormalRadioButton;
    QLineEdit *muNormalParameter;
    QLineEdit *sigmaNormalParameter;
    QGroupBox *groupBox_5;
    QLineEdit *lengthLineEdit;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_7;
    QLabel *label_11;
    QLineEdit *zSpinBox;
    QLineEdit *distortionSpinBox;
    QLineEdit *aspectRatioSpinBox;
    QWidget *tab_4;
    QGroupBox *groupBox;
    QRadioButton *wwSpinBox;
    QRadioButton *owSpinBox;
    QRadioButton *mwlSpinBox;
    QRadioButton *fwSpinBox;
    QRadioButton *mwsSpinBox;
    QGroupBox *groupBox_2;
    QLabel *waterWetLabel;
    QLabel *oilWetLabel;
    QLabel *to1Label;
    QLabel *to2Label;
    QLabel *oilWetFractionLabel;
    QLabel *label_12;
    QLineEdit *shapeFactor;
    QLineEdit *minWWThetaSpinBox;
    QLineEdit *maxWWThetaSpinBox;
    QLineEdit *minOWThetaSpinBox;
    QLineEdit *maxOWThetaSpinBox;
    QLineEdit *owFractionSpinBox;
    QWidget *tab_8;
    QGroupBox *groupBox_21;
    QRadioButton *choleskyRadioButton;
    QRadioButton *bicstabRadioButton;
    QCheckBox *calcPermCheckBox;
    QGroupBox *groupBox_24;
    QCheckBox *extractDataCheckBox;
    QWidget *tab_5;
    QGroupBox *groupBox_14;
    QLabel *label_16;
    QLabel *label_15;
    QLabel *porosityLabel;
    QLabel *permeabilityLabel;
    QLabel *label_35;
    QLabel *totalPoresLabel;
    QLabel *totalNodesLabel;
    QLabel *label_38;
    QPushButton *saveNetworkImageButton;
    QWidget *tab_15;
    QRadioButton *loadNetworkStatusRadioButton;
    QLineEdit *pathToPhasePoresLineEdit;
    QLabel *label_18;
    QLabel *label_29;
    QLineEdit *pathToPhaseNodesLineEdit;
    QRadioButton *renderImagesRadioButton;
    QLabel *label_30;
    QLineEdit *imagesCountLineEdit;
    QPushButton *renderNetworkButton;
    QPushButton *renderStopButton;
    QLabel *networkRenderingLabel;
    QCheckBox *onlyPhasesCheckBox;
    QSpacerItem *verticalSpacer;
    QLabel *label_3;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QCheckBox *PDCheckBox;
    QCheckBox *PICheckBox;
    QCheckBox *SDCheckBox;
    QCheckBox *SICheckBox;
    QCheckBox *TDCheckBox;
    QPushButton *twoPhaseSimButton;
    QPushButton *twoPhaseSimStopButton;
    QLabel *twoPhaseRunningLabel;
    QRadioButton *twoPhaseFlowSteady;
    QRadioButton *twoPhaseFlowUnSteady;
    QRadioButton *threePhaseFlowSteady;
    QRadioButton *twoPhaseLoadFromFileRadioButton;
    QRadioButton *twoPhaseFlowUnSteadyFast;
    QWidget *tab_11;
    QGroupBox *groupBox_6;
    QLabel *label_60;
    QLineEdit *twoPhaseStepsLineEdit;
    QLabel *label_40;
    QLineEdit *twoPhaseSeed;
    QGroupBox *groupBox_20;
    QLabel *waterWetLabel_2;
    QLabel *waterWetLabel_3;
    QCheckBox *waterFilmsCheckBox;
    QCheckBox *oilFilmsCheckBox;
    QLineEdit *twoPhaseWWThetaIncSpinBox;
    QLineEdit *twoPhaseOWThetaIncSpinBox;
    QWidget *tab_16;
    QGroupBox *groupBox_25;
    QCheckBox *threePhaseInitialWaterFlood;
    QCheckBox *threePhaseGasFlood;
    QCheckBox *threePhaseWaterFlood;
    QCheckBox *threePhaseRepeatWAG;
    QLineEdit *threePhaseCyclesNumber;
    QGroupBox *groupBox_26;
    QRadioButton *threePhaseWAGCycle;
    QRadioButton *threePhaseCustomCycle;
    QCheckBox *threePhaseStopAfterBreakthrough;
    QCheckBox *threePhaseNoSnapOff;
    QGroupBox *groupBox_27;
    QLabel *label_81;
    QComboBox *threePhaseSecondPhase;
    QLabel *label_82;
    QLabel *label_83;
    QComboBox *threePhaseInvadingPhase;
    QComboBox *threePhaseInitialPhase;
    QWidget *tab_14;
    QGroupBox *groupBox_10;
    QLineEdit *twoPhasePrecisionFactorLineEdit;
    QLineEdit *twoPhaseFlowRateLineEdit;
    QLabel *label_73;
    QLabel *label_75;
    QLineEdit *twoPhaseSimulationTimeLineEdit;
    QLabel *label_80;
    QCheckBox *overrideByInjectedPVs;
    QLineEdit *injectedPVs;
    QCheckBox *rateBump;
    QSpinBox *rateBumpMultiplier;
    QSpinBox *rateBumpTiming;
    QWidget *tab_9;
    QGroupBox *groupBox_11;
    QLabel *oilWetLabel_3;
    QCheckBox *twoPhaseSurfactantInjectionAfterBr;
    QLabel *oilWetLabel_2;
    QCheckBox *twoPhaseSurfactantInjection;
    QCheckBox *twoPhaseSurfactantInjectionAfterHST;
    QLabel *label_47;
    QCheckBox *twoPhaseSurfactantRemoveAfterBr;
    QCheckBox *twoPhaseSurfactantRemoveAfterHST;
    QLineEdit *twoPhaseAngleChange;
    QLineEdit *twoPhaseIFTChange;
    QWidget *tab_13;
    QGroupBox *groupBox_23;
    QCheckBox *twoPhasePolymerInjectionAfterBr;
    QLabel *oilWetLabel_5;
    QCheckBox *twoPhasePolymerInjection;
    QCheckBox *twoPhasePolymerMicroscopicDiversion;
    QLabel *oilWetLabel_7;
    QCheckBox *twoPhasePolymerInjectionAfterHST;
    QCheckBox *twoPhasePolymerRemoveAfterBr;
    QCheckBox *twoPhasePolymerRemoveAfterHST;
    QLabel *label_42;
    QLineEdit *twoPhasePolymerCriticalRadius;
    QLineEdit *twoPhasePolymerViscosityChange;
    QWidget *tab_2;
    QGroupBox *groupBox_7;
    QLabel *label_20;
    QLineEdit *twoPhaseOilVisLineEdit;
    QLineEdit *twoPhaseWaterVisLineEdit;
    QLabel *label_21;
    QLineEdit *twoPhaseGasVisLineEdit;
    QLabel *label_22;
    QLabel *label_23;
    QLineEdit *twoPhaseOWIFTLineEdit;
    QLabel *label_24;
    QLineEdit *twoPhaseOGIFTLineEdit;
    QLineEdit *twoPhaseWGIFTLineEdit;
    QLabel *label_70;
    QGroupBox *groupBox_9;
    QLabel *label_26;
    QRadioButton *twoPhaseSwRandomRadioButton;
    QRadioButton *twoPhaseSwBigRadioButton;
    QRadioButton *twoPhaseSwSmallRadioButton;
    QRadioButton *twoPhaseSwAPDRadioButton;
    QCheckBox *twoPhaseAging;
    QLineEdit *twoPhaseInitialSwSpinBox;
    QWidget *tab_19;
    QGroupBox *groupBox_8;
    QCheckBox *twoPhaseRPermCheckBox;
    QCheckBox *twoPhasePostProcessCheckBox;
    QCheckBox *twoPhaseGravityCheckBox;
    QLineEdit *twoPhaseGLineEdit;
    QLineEdit *twoPhasePinViscousLineEdit;
    QCheckBox *twoPhaseViscousForcesCheckBox;
    QLineEdit *twoPhasePoutViscousLineEdit;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_4;
    QTabWidget *tabWidget_3;
    QWidget *tab_7;
    QPushButton *depSimButton;
    QPushButton *depSimStopButton;
    QCheckBox *migrationCheckBox;
    QRadioButton *injectionRadioButton;
    QRadioButton *diffusionRadioButton;
    QCheckBox *growthCheckBox;
    QLabel *depRunningLabel;
    QCheckBox *threePhaseDepletion;
    QRadioButton *depLoadFromFileRadioButton;
    QWidget *tab_12;
    QGroupBox *groupBox_17;
    QLabel *label_61;
    QLineEdit *depDepletionRateLineEdit;
    QLineEdit *depBPPressLineEdit;
    QLabel *label_62;
    QLineEdit *depDepletionTimeLineEdit;
    QLabel *label_63;
    QLabel *depDepletionStepsLabel;
    QLineEdit *depDepletionStepsLineEdit;
    QCheckBox *depOptimalStepCheckBox;
    QLineEdit *depInjectionRateLineEdit;
    QLabel *label_68;
    QLabel *label_41;
    QLineEdit *depSeed;
    QGroupBox *groupBox_18;
    QRadioButton *depINRadioBox;
    QRadioButton *depPNRadioBox;
    QLabel *label_64;
    QLineEdit *depMinCavRadiusLineEdit;
    QLabel *label_65;
    QLabel *label_66;
    QLineEdit *depMaxCavRadiusLineEdit;
    QLineEdit *depCavityDensityLineEdit;
    QLabel *label_67;
    QLineEdit *depBubbleNumSpinBox;
    QGroupBox *groupBox_19;
    QRadioButton *depMultiFillRadioBox;
    QRadioButton *depInvasionPercRadioBox;
    QWidget *tab_10;
    QGroupBox *groupBox_28;
    QLabel *label_84;
    QComboBox *depletionSecondPhase;
    QLabel *label_85;
    QComboBox *depletionInitialPhase;
    QCheckBox *depletionNoSnapOff;
    QCheckBox *depletionStopAfterBreakthrough;
    QWidget *tab_6;
    QGroupBox *groupBox_16;
    QLabel *label_39;
    QLabel *label_43;
    QLabel *label_44;
    QLabel *label_45;
    QLineEdit *depOWIFTLineEdit;
    QLabel *label_46;
    QLineEdit *depOGIFTLineEdit;
    QLineEdit *depWGIFTLineEdit;
    QLabel *label_48;
    QLineEdit *depOilVisLineEdit;
    QLineEdit *depWaterVisLineEdit;
    QLineEdit *depGasVisLineEdit;
    QCheckBox *depLoadPressDependCheckBox;
    QLabel *label_50;
    QLabel *label_49;
    QLineEdit *depOilDiffCoefLineEdit;
    QLineEdit *depWaterDiffCoefLineEdit;
    QLineEdit *depPressDependDataPathLineEdit;
    QLabel *depPressDependDataPathLabel;
    QGroupBox *groupBox_15;
    QLabel *label_28;
    QRadioButton *depletionSwRandomRadioButton;
    QRadioButton *depletionSwBigRadioButton;
    QRadioButton *depletionSwSmallRadioButton;
    QRadioButton *depSwAPDRadioButton;
    QCheckBox *twoPhaseAging_2;
    QLineEdit *depletionInitialSwSpinBox;
    QWidget *misc;
    QGroupBox *groupBox_22;
    QCheckBox *depPostProcessCheckBox;
    QCheckBox *depRPermCheckBox;
    QCheckBox *depGravityCheckBox;
    QLineEdit *depGLineEdit;
    QCheckBox *depViscousForcesCheckBox;
    QLineEdit *depPinViscousLineEdit;
    QLineEdit *depPoutViscousLineEdit;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QTabWidget *tabWidget;
    QWidget *tab_17;
    QVBoxLayout *verticalLayout_3;
    widget3d *widget;
    QWidget *tab_18;
    QVBoxLayout *verticalLayout_4;
    QCustomPlot *plotWidget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_71;
    QLineEdit *title;
    QLabel *label_37;
    QLineEdit *xAxisTitle;
    QLabel *yAxisTitle22;
    QLineEdit *yAxisTitle;
    QPushButton *pushButton;
    QPushButton *twoPhaseSaveCurveButton;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_33;
    QLineEdit *minXAxis;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_34;
    QLineEdit *maxXAxis;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_31;
    QLineEdit *minYAxis;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_32;
    QLineEdit *maxYAxis;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_36;
    QLineEdit *tickStep;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_2;
    QLineEdit *fileToPlot;
    QCheckBox *realTimeCheckBox;
    QPushButton *plot;
    QPushButton *pushButton_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QGroupBox *visiblePhases;
    QCheckBox *visibleOilCheckBox;
    QCheckBox *visibleWaterCheckBox;
    QCheckBox *visibleGasCheckBox;
    QCheckBox *visibleOilWetCheckBox;
    QCheckBox *visibleWaterWetCheckBox;
    QGroupBox *visiblePhases_2;
    QRadioButton *resetRadioButton;
    QCheckBox *rotateCheckBox;
    QGroupBox *visiblePhases_4;
    QCheckBox *xCutCheckBox;
    QCheckBox *yCutCheckBox;
    QCheckBox *zCutCheckBox;
    QLineEdit *xCutSpinBox;
    QLineEdit *yCutSpinBox;
    QLineEdit *zCutSpinBox;
    QLabel *label_52;
    QLabel *label_53;
    QLabel *label_54;
    QGroupBox *visiblePhases_3;
    QCheckBox *pore3DCheckBox;
    QCheckBox *node3DCheckBox;
    QCheckBox *lightCheckBox;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1082, 694);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icon/icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        networkTabBox = new QTabWidget(centralwidget);
        networkTabBox->setObjectName(QStringLiteral("networkTabBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(networkTabBox->sizePolicy().hasHeightForWidth());
        networkTabBox->setSizePolicy(sizePolicy1);
        networkTabBox->setMinimumSize(QSize(481, 191));
        networkTabBox->setDocumentMode(true);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        regularRadioButton = new QRadioButton(tab);
        regularRadioButton->setObjectName(QStringLiteral("regularRadioButton"));
        regularRadioButton->setGeometry(QRect(10, 10, 311, 21));
        regularRadioButton->setChecked(true);
        extractedRadioButton = new QRadioButton(tab);
        extractedRadioButton->setObjectName(QStringLiteral("extractedRadioButton"));
        extractedRadioButton->setGeometry(QRect(10, 50, 161, 21));
        calibratedRadioButton = new QRadioButton(tab);
        calibratedRadioButton->setObjectName(QStringLiteral("calibratedRadioButton"));
        calibratedRadioButton->setGeometry(QRect(10, 30, 311, 21));
        pathToExtractedLabel = new QLabel(tab);
        pathToExtractedLabel->setObjectName(QStringLiteral("pathToExtractedLabel"));
        pathToExtractedLabel->setEnabled(false);
        pathToExtractedLabel->setGeometry(QRect(10, 70, 161, 21));
        loadNetworkButton = new QPushButton(tab);
        loadNetworkButton->setObjectName(QStringLiteral("loadNetworkButton"));
        loadNetworkButton->setGeometry(QRect(380, 110, 131, 31));
        pathToExtractedLineEdit = new QLineEdit(tab);
        pathToExtractedLineEdit->setObjectName(QStringLiteral("pathToExtractedLineEdit"));
        pathToExtractedLineEdit->setEnabled(false);
        pathToExtractedLineEdit->setGeometry(QRect(170, 70, 181, 21));
        rockPrefixLabel = new QLabel(tab);
        rockPrefixLabel->setObjectName(QStringLiteral("rockPrefixLabel"));
        rockPrefixLabel->setEnabled(false);
        rockPrefixLabel->setGeometry(QRect(10, 90, 161, 21));
        networkRunningLabel = new QLabel(tab);
        networkRunningLabel->setObjectName(QStringLiteral("networkRunningLabel"));
        networkRunningLabel->setGeometry(QRect(390, 140, 81, 21));
        extendCheckBox = new QCheckBox(tab);
        extendCheckBox->setObjectName(QStringLiteral("extendCheckBox"));
        extendCheckBox->setEnabled(false);
        extendCheckBox->setGeometry(QRect(170, 50, 71, 21));
        extendCheckBox->setChecked(false);
        loadFromFileRadioButton = new QRadioButton(tab);
        loadFromFileRadioButton->setObjectName(QStringLiteral("loadFromFileRadioButton"));
        loadFromFileRadioButton->setGeometry(QRect(340, 10, 171, 21));
        loadFromFileRadioButton->setChecked(false);
        oneDirection = new QCheckBox(tab);
        oneDirection->setObjectName(QStringLiteral("oneDirection"));
        oneDirection->setEnabled(false);
        oneDirection->setGeometry(QRect(240, 50, 51, 21));
        oneDirection->setChecked(false);
        rockList = new QComboBox(tab);
        rockList->setObjectName(QStringLiteral("rockList"));
        rockList->setEnabled(false);
        rockList->setGeometry(QRect(170, 90, 181, 21));
        extensionNumber = new QLineEdit(tab);
        extensionNumber->setObjectName(QStringLiteral("extensionNumber"));
        extensionNumber->setEnabled(false);
        extensionNumber->setGeometry(QRect(310, 50, 41, 21));
        label_x = new QLabel(tab);
        label_x->setObjectName(QStringLiteral("label_x"));
        label_x->setEnabled(false);
        label_x->setGeometry(QRect(290, 50, 21, 21));
        networkTabBox->addTab(tab, QString());
        geometryTab = new QWidget();
        geometryTab->setObjectName(QStringLiteral("geometryTab"));
        groupBox_3 = new QGroupBox(geometryTab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 10, 91, 151));
        groupBox_3->setFlat(true);
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 90, 31, 21));
        NxSpinBox = new QLineEdit(groupBox_3);
        NxSpinBox->setObjectName(QStringLiteral("NxSpinBox"));
        NxSpinBox->setGeometry(QRect(50, 20, 41, 21));
        NySpinBox = new QLineEdit(groupBox_3);
        NySpinBox->setObjectName(QStringLiteral("NySpinBox"));
        NySpinBox->setGeometry(QRect(50, 40, 41, 21));
        NzSpinBox = new QLineEdit(groupBox_3);
        NzSpinBox->setObjectName(QStringLiteral("NzSpinBox"));
        NzSpinBox->setGeometry(QRect(50, 60, 41, 21));
        label_25 = new QLabel(groupBox_3);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(10, 20, 41, 21));
        label_27 = new QLabel(groupBox_3);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(10, 40, 41, 21));
        label_51 = new QLabel(groupBox_3);
        label_51->setObjectName(QStringLiteral("label_51"));
        label_51->setGeometry(QRect(10, 60, 41, 21));
        seed = new QLineEdit(groupBox_3);
        seed->setObjectName(QStringLiteral("seed"));
        seed->setGeometry(QRect(50, 90, 41, 21));
        groupBox_4 = new QGroupBox(geometryTab);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(110, 10, 261, 151));
        groupBox_4->setFlat(true);
        psdRayleighRadioButton = new QRadioButton(groupBox_4);
        psdRayleighRadioButton->setObjectName(QStringLiteral("psdRayleighRadioButton"));
        psdRayleighRadioButton->setGeometry(QRect(10, 80, 81, 21));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 40, 81, 21));
        minRadiusLineEdit = new QLineEdit(groupBox_4);
        minRadiusLineEdit->setObjectName(QStringLiteral("minRadiusLineEdit"));
        minRadiusLineEdit->setGeometry(QRect(90, 20, 41, 21));
        psdUniformRadioButton = new QRadioButton(groupBox_4);
        psdUniformRadioButton->setObjectName(QStringLiteral("psdUniformRadioButton"));
        psdUniformRadioButton->setGeometry(QRect(10, 60, 81, 21));
        psdUniformRadioButton->setChecked(true);
        maxRadiusLineEdit = new QLineEdit(groupBox_4);
        maxRadiusLineEdit->setObjectName(QStringLiteral("maxRadiusLineEdit"));
        maxRadiusLineEdit->setGeometry(QRect(90, 40, 41, 21));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 20, 81, 21));
        rayleighParamLineEdit = new QLineEdit(groupBox_4);
        rayleighParamLineEdit->setObjectName(QStringLiteral("rayleighParamLineEdit"));
        rayleighParamLineEdit->setGeometry(QRect(90, 80, 41, 21));
        volConsRadiusLineEdit = new QLineEdit(groupBox_4);
        volConsRadiusLineEdit->setObjectName(QStringLiteral("volConsRadiusLineEdit"));
        volConsRadiusLineEdit->setGeometry(QRect(220, 20, 41, 21));
        label_13 = new QLabel(groupBox_4);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(140, 20, 81, 21));
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(140, 40, 81, 21));
        volExpLineEdit = new QLineEdit(groupBox_4);
        volExpLineEdit->setObjectName(QStringLiteral("volExpLineEdit"));
        volExpLineEdit->setGeometry(QRect(220, 40, 41, 21));
        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(140, 60, 81, 21));
        condConsLineEdit = new QLineEdit(groupBox_4);
        condConsLineEdit->setObjectName(QStringLiteral("condConsLineEdit"));
        condConsLineEdit->setGeometry(QRect(220, 60, 41, 21));
        label_19 = new QLabel(groupBox_4);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(140, 80, 81, 21));
        condExpLineEdit = new QLineEdit(groupBox_4);
        condExpLineEdit->setObjectName(QStringLiteral("condExpLineEdit"));
        condExpLineEdit->setGeometry(QRect(220, 80, 41, 21));
        psdTriangularRadioButton = new QRadioButton(groupBox_4);
        psdTriangularRadioButton->setObjectName(QStringLiteral("psdTriangularRadioButton"));
        psdTriangularRadioButton->setGeometry(QRect(10, 100, 81, 21));
        triangularParameter = new QLineEdit(groupBox_4);
        triangularParameter->setObjectName(QStringLiteral("triangularParameter"));
        triangularParameter->setGeometry(QRect(90, 100, 41, 21));
        psdNormalRadioButton = new QRadioButton(groupBox_4);
        psdNormalRadioButton->setObjectName(QStringLiteral("psdNormalRadioButton"));
        psdNormalRadioButton->setGeometry(QRect(10, 120, 81, 21));
        muNormalParameter = new QLineEdit(groupBox_4);
        muNormalParameter->setObjectName(QStringLiteral("muNormalParameter"));
        muNormalParameter->setGeometry(QRect(90, 120, 41, 21));
        sigmaNormalParameter = new QLineEdit(groupBox_4);
        sigmaNormalParameter->setObjectName(QStringLiteral("sigmaNormalParameter"));
        sigmaNormalParameter->setGeometry(QRect(140, 120, 41, 21));
        groupBox_5 = new QGroupBox(geometryTab);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(380, 10, 111, 151));
        groupBox_5->setFlat(true);
        lengthLineEdit = new QLineEdit(groupBox_5);
        lengthLineEdit->setObjectName(QStringLiteral("lengthLineEdit"));
        lengthLineEdit->setGeometry(QRect(70, 80, 41, 21));
        label_9 = new QLabel(groupBox_5);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 40, 51, 21));
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 80, 51, 21));
        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 20, 51, 21));
        label_11 = new QLabel(groupBox_5);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 60, 51, 21));
        zSpinBox = new QLineEdit(groupBox_5);
        zSpinBox->setObjectName(QStringLiteral("zSpinBox"));
        zSpinBox->setGeometry(QRect(70, 20, 41, 21));
        distortionSpinBox = new QLineEdit(groupBox_5);
        distortionSpinBox->setObjectName(QStringLiteral("distortionSpinBox"));
        distortionSpinBox->setGeometry(QRect(70, 40, 41, 21));
        aspectRatioSpinBox = new QLineEdit(groupBox_5);
        aspectRatioSpinBox->setObjectName(QStringLiteral("aspectRatioSpinBox"));
        aspectRatioSpinBox->setGeometry(QRect(70, 60, 41, 21));
        networkTabBox->addTab(geometryTab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        groupBox = new QGroupBox(tab_4);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 131, 151));
        groupBox->setFlat(true);
        wwSpinBox = new QRadioButton(groupBox);
        wwSpinBox->setObjectName(QStringLiteral("wwSpinBox"));
        wwSpinBox->setGeometry(QRect(10, 20, 121, 21));
        wwSpinBox->setChecked(true);
        owSpinBox = new QRadioButton(groupBox);
        owSpinBox->setObjectName(QStringLiteral("owSpinBox"));
        owSpinBox->setGeometry(QRect(10, 40, 121, 21));
        mwlSpinBox = new QRadioButton(groupBox);
        mwlSpinBox->setObjectName(QStringLiteral("mwlSpinBox"));
        mwlSpinBox->setGeometry(QRect(10, 100, 121, 21));
        fwSpinBox = new QRadioButton(groupBox);
        fwSpinBox->setObjectName(QStringLiteral("fwSpinBox"));
        fwSpinBox->setGeometry(QRect(10, 60, 121, 21));
        mwsSpinBox = new QRadioButton(groupBox);
        mwsSpinBox->setObjectName(QStringLiteral("mwsSpinBox"));
        mwsSpinBox->setGeometry(QRect(10, 80, 121, 21));
        groupBox_2 = new QGroupBox(tab_4);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(150, 10, 211, 151));
        groupBox_2->setFlat(true);
        waterWetLabel = new QLabel(groupBox_2);
        waterWetLabel->setObjectName(QStringLiteral("waterWetLabel"));
        waterWetLabel->setGeometry(QRect(10, 20, 41, 16));
        oilWetLabel = new QLabel(groupBox_2);
        oilWetLabel->setObjectName(QStringLiteral("oilWetLabel"));
        oilWetLabel->setGeometry(QRect(10, 40, 41, 16));
        to1Label = new QLabel(groupBox_2);
        to1Label->setObjectName(QStringLiteral("to1Label"));
        to1Label->setGeometry(QRect(130, 20, 31, 20));
        to2Label = new QLabel(groupBox_2);
        to2Label->setObjectName(QStringLiteral("to2Label"));
        to2Label->setGeometry(QRect(130, 40, 31, 20));
        oilWetFractionLabel = new QLabel(groupBox_2);
        oilWetFractionLabel->setObjectName(QStringLiteral("oilWetFractionLabel"));
        oilWetFractionLabel->setGeometry(QRect(10, 60, 151, 21));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 80, 151, 21));
        shapeFactor = new QLineEdit(groupBox_2);
        shapeFactor->setObjectName(QStringLiteral("shapeFactor"));
        shapeFactor->setGeometry(QRect(160, 80, 51, 21));
        minWWThetaSpinBox = new QLineEdit(groupBox_2);
        minWWThetaSpinBox->setObjectName(QStringLiteral("minWWThetaSpinBox"));
        minWWThetaSpinBox->setGeometry(QRect(60, 20, 51, 21));
        maxWWThetaSpinBox = new QLineEdit(groupBox_2);
        maxWWThetaSpinBox->setObjectName(QStringLiteral("maxWWThetaSpinBox"));
        maxWWThetaSpinBox->setGeometry(QRect(160, 20, 51, 21));
        minOWThetaSpinBox = new QLineEdit(groupBox_2);
        minOWThetaSpinBox->setObjectName(QStringLiteral("minOWThetaSpinBox"));
        minOWThetaSpinBox->setGeometry(QRect(60, 40, 51, 21));
        maxOWThetaSpinBox = new QLineEdit(groupBox_2);
        maxOWThetaSpinBox->setObjectName(QStringLiteral("maxOWThetaSpinBox"));
        maxOWThetaSpinBox->setGeometry(QRect(160, 40, 51, 21));
        owFractionSpinBox = new QLineEdit(groupBox_2);
        owFractionSpinBox->setObjectName(QStringLiteral("owFractionSpinBox"));
        owFractionSpinBox->setGeometry(QRect(160, 60, 51, 20));
        networkTabBox->addTab(tab_4, QString());
        tab_8 = new QWidget();
        tab_8->setObjectName(QStringLiteral("tab_8"));
        groupBox_21 = new QGroupBox(tab_8);
        groupBox_21->setObjectName(QStringLiteral("groupBox_21"));
        groupBox_21->setGeometry(QRect(10, 10, 101, 151));
        groupBox_21->setFlat(true);
        choleskyRadioButton = new QRadioButton(groupBox_21);
        choleskyRadioButton->setObjectName(QStringLiteral("choleskyRadioButton"));
        choleskyRadioButton->setGeometry(QRect(10, 20, 91, 21));
        choleskyRadioButton->setChecked(true);
        bicstabRadioButton = new QRadioButton(groupBox_21);
        bicstabRadioButton->setObjectName(QStringLiteral("bicstabRadioButton"));
        bicstabRadioButton->setGeometry(QRect(10, 40, 91, 21));
        bicstabRadioButton->setChecked(false);
        calcPermCheckBox = new QCheckBox(groupBox_21);
        calcPermCheckBox->setObjectName(QStringLiteral("calcPermCheckBox"));
        calcPermCheckBox->setGeometry(QRect(10, 60, 91, 21));
        calcPermCheckBox->setChecked(true);
        groupBox_24 = new QGroupBox(tab_8);
        groupBox_24->setObjectName(QStringLiteral("groupBox_24"));
        groupBox_24->setGeometry(QRect(120, 10, 151, 151));
        groupBox_24->setFlat(true);
        extractDataCheckBox = new QCheckBox(groupBox_24);
        extractDataCheckBox->setObjectName(QStringLiteral("extractDataCheckBox"));
        extractDataCheckBox->setGeometry(QRect(10, 20, 121, 21));
        networkTabBox->addTab(tab_8, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        groupBox_14 = new QGroupBox(tab_5);
        groupBox_14->setObjectName(QStringLiteral("groupBox_14"));
        groupBox_14->setGeometry(QRect(10, 10, 271, 151));
        groupBox_14->setFlat(true);
        label_16 = new QLabel(groupBox_14);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(10, 40, 150, 16));
        label_15 = new QLabel(groupBox_14);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(10, 20, 150, 16));
        porosityLabel = new QLabel(groupBox_14);
        porosityLabel->setObjectName(QStringLiteral("porosityLabel"));
        porosityLabel->setGeometry(QRect(160, 40, 81, 16));
        permeabilityLabel = new QLabel(groupBox_14);
        permeabilityLabel->setObjectName(QStringLiteral("permeabilityLabel"));
        permeabilityLabel->setGeometry(QRect(160, 20, 81, 16));
        label_35 = new QLabel(groupBox_14);
        label_35->setObjectName(QStringLiteral("label_35"));
        label_35->setGeometry(QRect(10, 60, 150, 16));
        totalPoresLabel = new QLabel(groupBox_14);
        totalPoresLabel->setObjectName(QStringLiteral("totalPoresLabel"));
        totalPoresLabel->setGeometry(QRect(160, 60, 81, 16));
        totalNodesLabel = new QLabel(groupBox_14);
        totalNodesLabel->setObjectName(QStringLiteral("totalNodesLabel"));
        totalNodesLabel->setGeometry(QRect(160, 80, 81, 16));
        label_38 = new QLabel(groupBox_14);
        label_38->setObjectName(QStringLiteral("label_38"));
        label_38->setGeometry(QRect(10, 80, 150, 16));
        saveNetworkImageButton = new QPushButton(tab_5);
        saveNetworkImageButton->setObjectName(QStringLiteral("saveNetworkImageButton"));
        saveNetworkImageButton->setGeometry(QRect(380, 110, 131, 31));
        networkTabBox->addTab(tab_5, QString());
        tab_15 = new QWidget();
        tab_15->setObjectName(QStringLiteral("tab_15"));
        loadNetworkStatusRadioButton = new QRadioButton(tab_15);
        loadNetworkStatusRadioButton->setObjectName(QStringLiteral("loadNetworkStatusRadioButton"));
        loadNetworkStatusRadioButton->setGeometry(QRect(10, 10, 231, 21));
        loadNetworkStatusRadioButton->setChecked(true);
        pathToPhasePoresLineEdit = new QLineEdit(tab_15);
        pathToPhasePoresLineEdit->setObjectName(QStringLiteral("pathToPhasePoresLineEdit"));
        pathToPhasePoresLineEdit->setEnabled(true);
        pathToPhasePoresLineEdit->setGeometry(QRect(60, 30, 181, 21));
        label_18 = new QLabel(tab_15);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(10, 30, 41, 21));
        label_29 = new QLabel(tab_15);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setGeometry(QRect(10, 50, 41, 21));
        pathToPhaseNodesLineEdit = new QLineEdit(tab_15);
        pathToPhaseNodesLineEdit->setObjectName(QStringLiteral("pathToPhaseNodesLineEdit"));
        pathToPhaseNodesLineEdit->setEnabled(true);
        pathToPhaseNodesLineEdit->setGeometry(QRect(60, 50, 181, 21));
        renderImagesRadioButton = new QRadioButton(tab_15);
        renderImagesRadioButton->setObjectName(QStringLiteral("renderImagesRadioButton"));
        renderImagesRadioButton->setGeometry(QRect(290, 10, 211, 21));
        renderImagesRadioButton->setChecked(false);
        label_30 = new QLabel(tab_15);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setGeometry(QRect(290, 30, 161, 21));
        imagesCountLineEdit = new QLineEdit(tab_15);
        imagesCountLineEdit->setObjectName(QStringLiteral("imagesCountLineEdit"));
        imagesCountLineEdit->setGeometry(QRect(450, 30, 51, 21));
        renderNetworkButton = new QPushButton(tab_15);
        renderNetworkButton->setObjectName(QStringLiteral("renderNetworkButton"));
        renderNetworkButton->setGeometry(QRect(380, 100, 131, 31));
        renderStopButton = new QPushButton(tab_15);
        renderStopButton->setObjectName(QStringLiteral("renderStopButton"));
        renderStopButton->setEnabled(false);
        renderStopButton->setGeometry(QRect(240, 100, 131, 31));
        networkRenderingLabel = new QLabel(tab_15);
        networkRenderingLabel->setObjectName(QStringLiteral("networkRenderingLabel"));
        networkRenderingLabel->setGeometry(QRect(390, 130, 81, 21));
        onlyPhasesCheckBox = new QCheckBox(tab_15);
        onlyPhasesCheckBox->setObjectName(QStringLiteral("onlyPhasesCheckBox"));
        onlyPhasesCheckBox->setGeometry(QRect(10, 80, 231, 20));
        networkTabBox->addTab(tab_15, QString());

        verticalLayout->addWidget(networkTabBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);

        tabWidget_2 = new QTabWidget(centralwidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        sizePolicy1.setHeightForWidth(tabWidget_2->sizePolicy().hasHeightForWidth());
        tabWidget_2->setSizePolicy(sizePolicy1);
        tabWidget_2->setMinimumSize(QSize(520, 190));
        tabWidget_2->setDocumentMode(true);
        tabWidget_2->setMovable(false);
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        PDCheckBox = new QCheckBox(tab_3);
        PDCheckBox->setObjectName(QStringLiteral("PDCheckBox"));
        PDCheckBox->setGeometry(QRect(20, 30, 151, 21));
        PDCheckBox->setChecked(true);
        PICheckBox = new QCheckBox(tab_3);
        PICheckBox->setObjectName(QStringLiteral("PICheckBox"));
        PICheckBox->setGeometry(QRect(20, 50, 151, 21));
        PICheckBox->setChecked(false);
        SDCheckBox = new QCheckBox(tab_3);
        SDCheckBox->setObjectName(QStringLiteral("SDCheckBox"));
        SDCheckBox->setGeometry(QRect(20, 70, 151, 21));
        SDCheckBox->setChecked(false);
        SICheckBox = new QCheckBox(tab_3);
        SICheckBox->setObjectName(QStringLiteral("SICheckBox"));
        SICheckBox->setGeometry(QRect(20, 90, 151, 21));
        SICheckBox->setChecked(false);
        TDCheckBox = new QCheckBox(tab_3);
        TDCheckBox->setObjectName(QStringLiteral("TDCheckBox"));
        TDCheckBox->setGeometry(QRect(20, 110, 151, 21));
        TDCheckBox->setChecked(false);
        twoPhaseSimButton = new QPushButton(tab_3);
        twoPhaseSimButton->setObjectName(QStringLiteral("twoPhaseSimButton"));
        twoPhaseSimButton->setGeometry(QRect(380, 110, 131, 31));
        twoPhaseSimStopButton = new QPushButton(tab_3);
        twoPhaseSimStopButton->setObjectName(QStringLiteral("twoPhaseSimStopButton"));
        twoPhaseSimStopButton->setEnabled(false);
        twoPhaseSimStopButton->setGeometry(QRect(240, 110, 131, 31));
        twoPhaseRunningLabel = new QLabel(tab_3);
        twoPhaseRunningLabel->setObjectName(QStringLiteral("twoPhaseRunningLabel"));
        twoPhaseRunningLabel->setGeometry(QRect(390, 140, 81, 21));
        twoPhaseFlowSteady = new QRadioButton(tab_3);
        twoPhaseFlowSteady->setObjectName(QStringLiteral("twoPhaseFlowSteady"));
        twoPhaseFlowSteady->setGeometry(QRect(10, 10, 151, 21));
        twoPhaseFlowSteady->setChecked(true);
        twoPhaseFlowUnSteady = new QRadioButton(tab_3);
        twoPhaseFlowUnSteady->setObjectName(QStringLiteral("twoPhaseFlowUnSteady"));
        twoPhaseFlowUnSteady->setGeometry(QRect(190, 30, 131, 21));
        threePhaseFlowSteady = new QRadioButton(tab_3);
        threePhaseFlowSteady->setObjectName(QStringLiteral("threePhaseFlowSteady"));
        threePhaseFlowSteady->setGeometry(QRect(190, 10, 131, 21));
        twoPhaseLoadFromFileRadioButton = new QRadioButton(tab_3);
        twoPhaseLoadFromFileRadioButton->setObjectName(QStringLiteral("twoPhaseLoadFromFileRadioButton"));
        twoPhaseLoadFromFileRadioButton->setGeometry(QRect(340, 10, 131, 21));
        twoPhaseLoadFromFileRadioButton->setChecked(false);
        twoPhaseFlowUnSteadyFast = new QRadioButton(tab_3);
        twoPhaseFlowUnSteadyFast->setObjectName(QStringLiteral("twoPhaseFlowUnSteadyFast"));
        twoPhaseFlowUnSteadyFast->setGeometry(QRect(190, 50, 131, 21));
        tabWidget_2->addTab(tab_3, QString());
        tab_11 = new QWidget();
        tab_11->setObjectName(QStringLiteral("tab_11"));
        groupBox_6 = new QGroupBox(tab_11);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 10, 121, 151));
        groupBox_6->setFlat(true);
        label_60 = new QLabel(groupBox_6);
        label_60->setObjectName(QStringLiteral("label_60"));
        label_60->setGeometry(QRect(10, 20, 31, 16));
        twoPhaseStepsLineEdit = new QLineEdit(groupBox_6);
        twoPhaseStepsLineEdit->setObjectName(QStringLiteral("twoPhaseStepsLineEdit"));
        twoPhaseStepsLineEdit->setGeometry(QRect(60, 20, 61, 21));
        label_40 = new QLabel(groupBox_6);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setGeometry(QRect(10, 40, 31, 16));
        twoPhaseSeed = new QLineEdit(groupBox_6);
        twoPhaseSeed->setObjectName(QStringLiteral("twoPhaseSeed"));
        twoPhaseSeed->setGeometry(QRect(60, 40, 61, 21));
        groupBox_20 = new QGroupBox(tab_11);
        groupBox_20->setObjectName(QStringLiteral("groupBox_20"));
        groupBox_20->setGeometry(QRect(140, 10, 221, 151));
        groupBox_20->setFlat(true);
        waterWetLabel_2 = new QLabel(groupBox_20);
        waterWetLabel_2->setObjectName(QStringLiteral("waterWetLabel_2"));
        waterWetLabel_2->setGeometry(QRect(10, 20, 151, 21));
        waterWetLabel_3 = new QLabel(groupBox_20);
        waterWetLabel_3->setObjectName(QStringLiteral("waterWetLabel_3"));
        waterWetLabel_3->setGeometry(QRect(10, 40, 151, 21));
        waterFilmsCheckBox = new QCheckBox(groupBox_20);
        waterFilmsCheckBox->setObjectName(QStringLiteral("waterFilmsCheckBox"));
        waterFilmsCheckBox->setGeometry(QRect(10, 60, 101, 21));
        waterFilmsCheckBox->setChecked(true);
        oilFilmsCheckBox = new QCheckBox(groupBox_20);
        oilFilmsCheckBox->setObjectName(QStringLiteral("oilFilmsCheckBox"));
        oilFilmsCheckBox->setGeometry(QRect(10, 80, 101, 21));
        oilFilmsCheckBox->setChecked(true);
        twoPhaseWWThetaIncSpinBox = new QLineEdit(groupBox_20);
        twoPhaseWWThetaIncSpinBox->setObjectName(QStringLiteral("twoPhaseWWThetaIncSpinBox"));
        twoPhaseWWThetaIncSpinBox->setGeometry(QRect(160, 20, 61, 21));
        twoPhaseOWThetaIncSpinBox = new QLineEdit(groupBox_20);
        twoPhaseOWThetaIncSpinBox->setObjectName(QStringLiteral("twoPhaseOWThetaIncSpinBox"));
        twoPhaseOWThetaIncSpinBox->setGeometry(QRect(160, 40, 61, 21));
        tabWidget_2->addTab(tab_11, QString());
        tab_16 = new QWidget();
        tab_16->setObjectName(QStringLiteral("tab_16"));
        groupBox_25 = new QGroupBox(tab_16);
        groupBox_25->setObjectName(QStringLiteral("groupBox_25"));
        groupBox_25->setGeometry(QRect(170, 10, 141, 151));
        groupBox_25->setFlat(true);
        groupBox_25->setCheckable(false);
        threePhaseInitialWaterFlood = new QCheckBox(groupBox_25);
        threePhaseInitialWaterFlood->setObjectName(QStringLiteral("threePhaseInitialWaterFlood"));
        threePhaseInitialWaterFlood->setGeometry(QRect(10, 20, 131, 21));
        threePhaseInitialWaterFlood->setChecked(true);
        threePhaseGasFlood = new QCheckBox(groupBox_25);
        threePhaseGasFlood->setObjectName(QStringLiteral("threePhaseGasFlood"));
        threePhaseGasFlood->setGeometry(QRect(10, 40, 131, 21));
        threePhaseGasFlood->setChecked(true);
        threePhaseWaterFlood = new QCheckBox(groupBox_25);
        threePhaseWaterFlood->setObjectName(QStringLiteral("threePhaseWaterFlood"));
        threePhaseWaterFlood->setGeometry(QRect(10, 60, 131, 21));
        threePhaseWaterFlood->setChecked(true);
        threePhaseRepeatWAG = new QCheckBox(groupBox_25);
        threePhaseRepeatWAG->setObjectName(QStringLiteral("threePhaseRepeatWAG"));
        threePhaseRepeatWAG->setEnabled(true);
        threePhaseRepeatWAG->setGeometry(QRect(10, 90, 91, 21));
        threePhaseRepeatWAG->setChecked(false);
        threePhaseCyclesNumber = new QLineEdit(groupBox_25);
        threePhaseCyclesNumber->setObjectName(QStringLiteral("threePhaseCyclesNumber"));
        threePhaseCyclesNumber->setGeometry(QRect(100, 90, 41, 21));
        groupBox_26 = new QGroupBox(tab_16);
        groupBox_26->setObjectName(QStringLiteral("groupBox_26"));
        groupBox_26->setGeometry(QRect(10, 10, 151, 151));
        groupBox_26->setFlat(true);
        groupBox_26->setCheckable(false);
        threePhaseWAGCycle = new QRadioButton(groupBox_26);
        threePhaseWAGCycle->setObjectName(QStringLiteral("threePhaseWAGCycle"));
        threePhaseWAGCycle->setGeometry(QRect(10, 20, 151, 21));
        threePhaseWAGCycle->setChecked(true);
        threePhaseCustomCycle = new QRadioButton(groupBox_26);
        threePhaseCustomCycle->setObjectName(QStringLiteral("threePhaseCustomCycle"));
        threePhaseCustomCycle->setGeometry(QRect(10, 40, 151, 21));
        threePhaseCustomCycle->setChecked(false);
        threePhaseStopAfterBreakthrough = new QCheckBox(groupBox_26);
        threePhaseStopAfterBreakthrough->setObjectName(QStringLiteral("threePhaseStopAfterBreakthrough"));
        threePhaseStopAfterBreakthrough->setGeometry(QRect(10, 90, 151, 21));
        threePhaseStopAfterBreakthrough->setChecked(false);
        threePhaseNoSnapOff = new QCheckBox(groupBox_26);
        threePhaseNoSnapOff->setObjectName(QStringLiteral("threePhaseNoSnapOff"));
        threePhaseNoSnapOff->setGeometry(QRect(10, 70, 151, 21));
        threePhaseNoSnapOff->setChecked(false);
        groupBox_27 = new QGroupBox(tab_16);
        groupBox_27->setObjectName(QStringLiteral("groupBox_27"));
        groupBox_27->setGeometry(QRect(320, 10, 181, 151));
        groupBox_27->setFlat(true);
        groupBox_27->setCheckable(false);
        label_81 = new QLabel(groupBox_27);
        label_81->setObjectName(QStringLiteral("label_81"));
        label_81->setGeometry(QRect(10, 20, 101, 21));
        threePhaseSecondPhase = new QComboBox(groupBox_27);
        threePhaseSecondPhase->setObjectName(QStringLiteral("threePhaseSecondPhase"));
        threePhaseSecondPhase->setGeometry(QRect(110, 40, 61, 21));
        label_82 = new QLabel(groupBox_27);
        label_82->setObjectName(QStringLiteral("label_82"));
        label_82->setGeometry(QRect(10, 40, 101, 21));
        label_83 = new QLabel(groupBox_27);
        label_83->setObjectName(QStringLiteral("label_83"));
        label_83->setGeometry(QRect(10, 60, 101, 21));
        threePhaseInvadingPhase = new QComboBox(groupBox_27);
        threePhaseInvadingPhase->setObjectName(QStringLiteral("threePhaseInvadingPhase"));
        threePhaseInvadingPhase->setGeometry(QRect(110, 60, 61, 21));
        threePhaseInitialPhase = new QComboBox(groupBox_27);
        threePhaseInitialPhase->setObjectName(QStringLiteral("threePhaseInitialPhase"));
        threePhaseInitialPhase->setGeometry(QRect(110, 20, 61, 21));
        tabWidget_2->addTab(tab_16, QString());
        tab_14 = new QWidget();
        tab_14->setObjectName(QStringLiteral("tab_14"));
        groupBox_10 = new QGroupBox(tab_14);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        groupBox_10->setGeometry(QRect(10, 10, 181, 151));
        groupBox_10->setFlat(true);
        twoPhasePrecisionFactorLineEdit = new QLineEdit(groupBox_10);
        twoPhasePrecisionFactorLineEdit->setObjectName(QStringLiteral("twoPhasePrecisionFactorLineEdit"));
        twoPhasePrecisionFactorLineEdit->setGeometry(QRect(120, 40, 51, 21));
        twoPhaseFlowRateLineEdit = new QLineEdit(groupBox_10);
        twoPhaseFlowRateLineEdit->setObjectName(QStringLiteral("twoPhaseFlowRateLineEdit"));
        twoPhaseFlowRateLineEdit->setGeometry(QRect(120, 20, 51, 21));
        label_73 = new QLabel(groupBox_10);
        label_73->setObjectName(QStringLiteral("label_73"));
        label_73->setGeometry(QRect(10, 20, 111, 21));
        label_75 = new QLabel(groupBox_10);
        label_75->setObjectName(QStringLiteral("label_75"));
        label_75->setGeometry(QRect(10, 40, 111, 21));
        twoPhaseSimulationTimeLineEdit = new QLineEdit(groupBox_10);
        twoPhaseSimulationTimeLineEdit->setObjectName(QStringLiteral("twoPhaseSimulationTimeLineEdit"));
        twoPhaseSimulationTimeLineEdit->setGeometry(QRect(120, 60, 51, 21));
        label_80 = new QLabel(groupBox_10);
        label_80->setObjectName(QStringLiteral("label_80"));
        label_80->setGeometry(QRect(10, 60, 111, 21));
        overrideByInjectedPVs = new QCheckBox(groupBox_10);
        overrideByInjectedPVs->setObjectName(QStringLiteral("overrideByInjectedPVs"));
        overrideByInjectedPVs->setGeometry(QRect(10, 80, 111, 21));
        overrideByInjectedPVs->setChecked(false);
        injectedPVs = new QLineEdit(groupBox_10);
        injectedPVs->setObjectName(QStringLiteral("injectedPVs"));
        injectedPVs->setGeometry(QRect(120, 80, 51, 21));
        rateBump = new QCheckBox(groupBox_10);
        rateBump->setObjectName(QStringLiteral("rateBump"));
        rateBump->setGeometry(QRect(10, 100, 111, 21));
        rateBump->setChecked(false);
        rateBumpMultiplier = new QSpinBox(groupBox_10);
        rateBumpMultiplier->setObjectName(QStringLiteral("rateBumpMultiplier"));
        rateBumpMultiplier->setGeometry(QRect(10, 120, 61, 31));
        rateBumpMultiplier->setMinimum(1);
        rateBumpMultiplier->setMaximum(1000);
        rateBumpMultiplier->setSingleStep(1);
        rateBumpMultiplier->setValue(4);
        rateBumpTiming = new QSpinBox(groupBox_10);
        rateBumpTiming->setObjectName(QStringLiteral("rateBumpTiming"));
        rateBumpTiming->setGeometry(QRect(80, 120, 91, 31));
        rateBumpTiming->setMinimum(10);
        rateBumpTiming->setMaximum(95);
        rateBumpTiming->setSingleStep(1);
        rateBumpTiming->setValue(80);
        tabWidget_2->addTab(tab_14, QString());
        tab_9 = new QWidget();
        tab_9->setObjectName(QStringLiteral("tab_9"));
        groupBox_11 = new QGroupBox(tab_9);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setGeometry(QRect(10, 10, 341, 151));
        groupBox_11->setFlat(true);
        oilWetLabel_3 = new QLabel(groupBox_11);
        oilWetLabel_3->setObjectName(QStringLiteral("oilWetLabel_3"));
        oilWetLabel_3->setGeometry(QRect(170, 40, 91, 21));
        twoPhaseSurfactantInjectionAfterBr = new QCheckBox(groupBox_11);
        twoPhaseSurfactantInjectionAfterBr->setObjectName(QStringLiteral("twoPhaseSurfactantInjectionAfterBr"));
        twoPhaseSurfactantInjectionAfterBr->setGeometry(QRect(10, 40, 161, 21));
        twoPhaseSurfactantInjectionAfterBr->setChecked(false);
        oilWetLabel_2 = new QLabel(groupBox_11);
        oilWetLabel_2->setObjectName(QStringLiteral("oilWetLabel_2"));
        oilWetLabel_2->setGeometry(QRect(170, 20, 91, 21));
        twoPhaseSurfactantInjection = new QCheckBox(groupBox_11);
        twoPhaseSurfactantInjection->setObjectName(QStringLiteral("twoPhaseSurfactantInjection"));
        twoPhaseSurfactantInjection->setGeometry(QRect(10, 20, 161, 21));
        twoPhaseSurfactantInjection->setChecked(false);
        twoPhaseSurfactantInjectionAfterHST = new QCheckBox(groupBox_11);
        twoPhaseSurfactantInjectionAfterHST->setObjectName(QStringLiteral("twoPhaseSurfactantInjectionAfterHST"));
        twoPhaseSurfactantInjectionAfterHST->setGeometry(QRect(10, 60, 161, 21));
        twoPhaseSurfactantInjectionAfterHST->setChecked(false);
        label_47 = new QLabel(groupBox_11);
        label_47->setObjectName(QStringLiteral("label_47"));
        label_47->setGeometry(QRect(10, 80, 111, 21));
        twoPhaseSurfactantRemoveAfterBr = new QCheckBox(groupBox_11);
        twoPhaseSurfactantRemoveAfterBr->setObjectName(QStringLiteral("twoPhaseSurfactantRemoveAfterBr"));
        twoPhaseSurfactantRemoveAfterBr->setGeometry(QRect(10, 100, 161, 21));
        twoPhaseSurfactantRemoveAfterBr->setChecked(false);
        twoPhaseSurfactantRemoveAfterHST = new QCheckBox(groupBox_11);
        twoPhaseSurfactantRemoveAfterHST->setObjectName(QStringLiteral("twoPhaseSurfactantRemoveAfterHST"));
        twoPhaseSurfactantRemoveAfterHST->setGeometry(QRect(10, 120, 161, 21));
        twoPhaseSurfactantRemoveAfterHST->setChecked(false);
        twoPhaseAngleChange = new QLineEdit(groupBox_11);
        twoPhaseAngleChange->setObjectName(QStringLiteral("twoPhaseAngleChange"));
        twoPhaseAngleChange->setGeometry(QRect(280, 20, 61, 21));
        twoPhaseIFTChange = new QLineEdit(groupBox_11);
        twoPhaseIFTChange->setObjectName(QStringLiteral("twoPhaseIFTChange"));
        twoPhaseIFTChange->setGeometry(QRect(280, 40, 61, 21));
        tabWidget_2->addTab(tab_9, QString());
        tab_13 = new QWidget();
        tab_13->setObjectName(QStringLiteral("tab_13"));
        groupBox_23 = new QGroupBox(tab_13);
        groupBox_23->setObjectName(QStringLiteral("groupBox_23"));
        groupBox_23->setGeometry(QRect(10, 10, 341, 151));
        groupBox_23->setFlat(true);
        twoPhasePolymerInjectionAfterBr = new QCheckBox(groupBox_23);
        twoPhasePolymerInjectionAfterBr->setObjectName(QStringLiteral("twoPhasePolymerInjectionAfterBr"));
        twoPhasePolymerInjectionAfterBr->setGeometry(QRect(10, 40, 151, 21));
        twoPhasePolymerInjectionAfterBr->setChecked(false);
        oilWetLabel_5 = new QLabel(groupBox_23);
        oilWetLabel_5->setObjectName(QStringLiteral("oilWetLabel_5"));
        oilWetLabel_5->setGeometry(QRect(170, 20, 111, 21));
        twoPhasePolymerInjection = new QCheckBox(groupBox_23);
        twoPhasePolymerInjection->setObjectName(QStringLiteral("twoPhasePolymerInjection"));
        twoPhasePolymerInjection->setGeometry(QRect(10, 20, 151, 21));
        twoPhasePolymerInjection->setChecked(false);
        twoPhasePolymerMicroscopicDiversion = new QCheckBox(groupBox_23);
        twoPhasePolymerMicroscopicDiversion->setObjectName(QStringLiteral("twoPhasePolymerMicroscopicDiversion"));
        twoPhasePolymerMicroscopicDiversion->setGeometry(QRect(170, 40, 171, 21));
        twoPhasePolymerMicroscopicDiversion->setChecked(false);
        oilWetLabel_7 = new QLabel(groupBox_23);
        oilWetLabel_7->setObjectName(QStringLiteral("oilWetLabel_7"));
        oilWetLabel_7->setGeometry(QRect(170, 60, 111, 21));
        twoPhasePolymerInjectionAfterHST = new QCheckBox(groupBox_23);
        twoPhasePolymerInjectionAfterHST->setObjectName(QStringLiteral("twoPhasePolymerInjectionAfterHST"));
        twoPhasePolymerInjectionAfterHST->setGeometry(QRect(10, 60, 151, 21));
        twoPhasePolymerInjectionAfterHST->setChecked(false);
        twoPhasePolymerRemoveAfterBr = new QCheckBox(groupBox_23);
        twoPhasePolymerRemoveAfterBr->setObjectName(QStringLiteral("twoPhasePolymerRemoveAfterBr"));
        twoPhasePolymerRemoveAfterBr->setGeometry(QRect(10, 100, 151, 21));
        twoPhasePolymerRemoveAfterBr->setChecked(false);
        twoPhasePolymerRemoveAfterHST = new QCheckBox(groupBox_23);
        twoPhasePolymerRemoveAfterHST->setObjectName(QStringLiteral("twoPhasePolymerRemoveAfterHST"));
        twoPhasePolymerRemoveAfterHST->setGeometry(QRect(10, 120, 151, 21));
        twoPhasePolymerRemoveAfterHST->setChecked(false);
        label_42 = new QLabel(groupBox_23);
        label_42->setObjectName(QStringLiteral("label_42"));
        label_42->setGeometry(QRect(10, 80, 151, 21));
        twoPhasePolymerCriticalRadius = new QLineEdit(groupBox_23);
        twoPhasePolymerCriticalRadius->setObjectName(QStringLiteral("twoPhasePolymerCriticalRadius"));
        twoPhasePolymerCriticalRadius->setGeometry(QRect(280, 60, 61, 21));
        twoPhasePolymerViscosityChange = new QLineEdit(groupBox_23);
        twoPhasePolymerViscosityChange->setObjectName(QStringLiteral("twoPhasePolymerViscosityChange"));
        twoPhasePolymerViscosityChange->setGeometry(QRect(280, 20, 61, 21));
        tabWidget_2->addTab(tab_13, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox_7 = new QGroupBox(tab_2);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 10, 311, 151));
        groupBox_7->setFlat(true);
        label_20 = new QLabel(groupBox_7);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(10, 20, 111, 21));
        twoPhaseOilVisLineEdit = new QLineEdit(groupBox_7);
        twoPhaseOilVisLineEdit->setObjectName(QStringLiteral("twoPhaseOilVisLineEdit"));
        twoPhaseOilVisLineEdit->setGeometry(QRect(120, 20, 51, 21));
        twoPhaseWaterVisLineEdit = new QLineEdit(groupBox_7);
        twoPhaseWaterVisLineEdit->setObjectName(QStringLiteral("twoPhaseWaterVisLineEdit"));
        twoPhaseWaterVisLineEdit->setGeometry(QRect(120, 40, 51, 21));
        label_21 = new QLabel(groupBox_7);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(10, 40, 111, 21));
        twoPhaseGasVisLineEdit = new QLineEdit(groupBox_7);
        twoPhaseGasVisLineEdit->setObjectName(QStringLiteral("twoPhaseGasVisLineEdit"));
        twoPhaseGasVisLineEdit->setGeometry(QRect(120, 60, 51, 21));
        label_22 = new QLabel(groupBox_7);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(10, 60, 111, 21));
        label_23 = new QLabel(groupBox_7);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(190, 20, 61, 16));
        twoPhaseOWIFTLineEdit = new QLineEdit(groupBox_7);
        twoPhaseOWIFTLineEdit->setObjectName(QStringLiteral("twoPhaseOWIFTLineEdit"));
        twoPhaseOWIFTLineEdit->setGeometry(QRect(250, 20, 51, 21));
        label_24 = new QLabel(groupBox_7);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(190, 40, 61, 16));
        twoPhaseOGIFTLineEdit = new QLineEdit(groupBox_7);
        twoPhaseOGIFTLineEdit->setObjectName(QStringLiteral("twoPhaseOGIFTLineEdit"));
        twoPhaseOGIFTLineEdit->setGeometry(QRect(250, 40, 51, 21));
        twoPhaseWGIFTLineEdit = new QLineEdit(groupBox_7);
        twoPhaseWGIFTLineEdit->setObjectName(QStringLiteral("twoPhaseWGIFTLineEdit"));
        twoPhaseWGIFTLineEdit->setGeometry(QRect(250, 60, 51, 21));
        label_70 = new QLabel(groupBox_7);
        label_70->setObjectName(QStringLiteral("label_70"));
        label_70->setGeometry(QRect(190, 60, 61, 20));
        groupBox_9 = new QGroupBox(tab_2);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setGeometry(QRect(330, 10, 161, 151));
        groupBox_9->setFlat(true);
        label_26 = new QLabel(groupBox_9);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(10, 20, 61, 16));
        twoPhaseSwRandomRadioButton = new QRadioButton(groupBox_9);
        twoPhaseSwRandomRadioButton->setObjectName(QStringLiteral("twoPhaseSwRandomRadioButton"));
        twoPhaseSwRandomRadioButton->setGeometry(QRect(10, 40, 141, 21));
        twoPhaseSwRandomRadioButton->setChecked(true);
        twoPhaseSwBigRadioButton = new QRadioButton(groupBox_9);
        twoPhaseSwBigRadioButton->setObjectName(QStringLiteral("twoPhaseSwBigRadioButton"));
        twoPhaseSwBigRadioButton->setGeometry(QRect(10, 80, 141, 21));
        twoPhaseSwSmallRadioButton = new QRadioButton(groupBox_9);
        twoPhaseSwSmallRadioButton->setObjectName(QStringLiteral("twoPhaseSwSmallRadioButton"));
        twoPhaseSwSmallRadioButton->setGeometry(QRect(10, 60, 141, 21));
        twoPhaseSwAPDRadioButton = new QRadioButton(groupBox_9);
        twoPhaseSwAPDRadioButton->setObjectName(QStringLiteral("twoPhaseSwAPDRadioButton"));
        twoPhaseSwAPDRadioButton->setGeometry(QRect(10, 100, 81, 21));
        twoPhaseAging = new QCheckBox(groupBox_9);
        twoPhaseAging->setObjectName(QStringLiteral("twoPhaseAging"));
        twoPhaseAging->setGeometry(QRect(90, 100, 71, 21));
        twoPhaseAging->setChecked(false);
        twoPhaseInitialSwSpinBox = new QLineEdit(groupBox_9);
        twoPhaseInitialSwSpinBox->setObjectName(QStringLiteral("twoPhaseInitialSwSpinBox"));
        twoPhaseInitialSwSpinBox->setGeometry(QRect(90, 20, 61, 21));
        tabWidget_2->addTab(tab_2, QString());
        tab_19 = new QWidget();
        tab_19->setObjectName(QStringLiteral("tab_19"));
        groupBox_8 = new QGroupBox(tab_19);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(10, 10, 181, 151));
        groupBox_8->setFlat(true);
        twoPhaseRPermCheckBox = new QCheckBox(groupBox_8);
        twoPhaseRPermCheckBox->setObjectName(QStringLiteral("twoPhaseRPermCheckBox"));
        twoPhaseRPermCheckBox->setGeometry(QRect(10, 80, 141, 21));
        twoPhasePostProcessCheckBox = new QCheckBox(groupBox_8);
        twoPhasePostProcessCheckBox->setObjectName(QStringLiteral("twoPhasePostProcessCheckBox"));
        twoPhasePostProcessCheckBox->setGeometry(QRect(10, 100, 141, 21));
        twoPhasePostProcessCheckBox->setChecked(false);
        twoPhaseGravityCheckBox = new QCheckBox(groupBox_8);
        twoPhaseGravityCheckBox->setObjectName(QStringLiteral("twoPhaseGravityCheckBox"));
        twoPhaseGravityCheckBox->setGeometry(QRect(10, 20, 91, 21));
        twoPhaseGravityCheckBox->setChecked(false);
        twoPhaseGLineEdit = new QLineEdit(groupBox_8);
        twoPhaseGLineEdit->setObjectName(QStringLiteral("twoPhaseGLineEdit"));
        twoPhaseGLineEdit->setGeometry(QRect(110, 20, 61, 21));
        twoPhasePinViscousLineEdit = new QLineEdit(groupBox_8);
        twoPhasePinViscousLineEdit->setObjectName(QStringLiteral("twoPhasePinViscousLineEdit"));
        twoPhasePinViscousLineEdit->setGeometry(QRect(40, 60, 61, 21));
        twoPhaseViscousForcesCheckBox = new QCheckBox(groupBox_8);
        twoPhaseViscousForcesCheckBox->setObjectName(QStringLiteral("twoPhaseViscousForcesCheckBox"));
        twoPhaseViscousForcesCheckBox->setGeometry(QRect(10, 40, 141, 21));
        twoPhaseViscousForcesCheckBox->setChecked(false);
        twoPhasePoutViscousLineEdit = new QLineEdit(groupBox_8);
        twoPhasePoutViscousLineEdit->setObjectName(QStringLiteral("twoPhasePoutViscousLineEdit"));
        twoPhasePoutViscousLineEdit->setGeometry(QRect(110, 60, 61, 21));
        tabWidget_2->addTab(tab_19, QString());

        verticalLayout->addWidget(tabWidget_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(label_4);

        tabWidget_3 = new QTabWidget(centralwidget);
        tabWidget_3->setObjectName(QStringLiteral("tabWidget_3"));
        tabWidget_3->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(50);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget_3->sizePolicy().hasHeightForWidth());
        tabWidget_3->setSizePolicy(sizePolicy2);
        tabWidget_3->setMinimumSize(QSize(481, 200));
        tabWidget_3->setDocumentMode(true);
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        depSimButton = new QPushButton(tab_7);
        depSimButton->setObjectName(QStringLiteral("depSimButton"));
        depSimButton->setGeometry(QRect(380, 100, 131, 31));
        depSimStopButton = new QPushButton(tab_7);
        depSimStopButton->setObjectName(QStringLiteral("depSimStopButton"));
        depSimStopButton->setEnabled(false);
        depSimStopButton->setGeometry(QRect(240, 100, 131, 31));
        migrationCheckBox = new QCheckBox(tab_7);
        migrationCheckBox->setObjectName(QStringLiteral("migrationCheckBox"));
        migrationCheckBox->setGeometry(QRect(10, 70, 101, 21));
        migrationCheckBox->setChecked(true);
        injectionRadioButton = new QRadioButton(tab_7);
        injectionRadioButton->setObjectName(QStringLiteral("injectionRadioButton"));
        injectionRadioButton->setGeometry(QRect(10, 30, 101, 21));
        injectionRadioButton->setChecked(false);
        diffusionRadioButton = new QRadioButton(tab_7);
        diffusionRadioButton->setObjectName(QStringLiteral("diffusionRadioButton"));
        diffusionRadioButton->setGeometry(QRect(10, 10, 101, 21));
        diffusionRadioButton->setChecked(true);
        growthCheckBox = new QCheckBox(tab_7);
        growthCheckBox->setObjectName(QStringLiteral("growthCheckBox"));
        growthCheckBox->setGeometry(QRect(10, 50, 101, 21));
        growthCheckBox->setChecked(true);
        depRunningLabel = new QLabel(tab_7);
        depRunningLabel->setObjectName(QStringLiteral("depRunningLabel"));
        depRunningLabel->setGeometry(QRect(390, 130, 81, 21));
        threePhaseDepletion = new QCheckBox(tab_7);
        threePhaseDepletion->setObjectName(QStringLiteral("threePhaseDepletion"));
        threePhaseDepletion->setGeometry(QRect(190, 10, 121, 21));
        depLoadFromFileRadioButton = new QRadioButton(tab_7);
        depLoadFromFileRadioButton->setObjectName(QStringLiteral("depLoadFromFileRadioButton"));
        depLoadFromFileRadioButton->setGeometry(QRect(340, 10, 131, 21));
        depLoadFromFileRadioButton->setChecked(false);
        tabWidget_3->addTab(tab_7, QString());
        tab_12 = new QWidget();
        tab_12->setObjectName(QStringLiteral("tab_12"));
        groupBox_17 = new QGroupBox(tab_12);
        groupBox_17->setObjectName(QStringLiteral("groupBox_17"));
        groupBox_17->setGeometry(QRect(10, 10, 171, 161));
        groupBox_17->setFlat(true);
        label_61 = new QLabel(groupBox_17);
        label_61->setObjectName(QStringLiteral("label_61"));
        label_61->setGeometry(QRect(10, 20, 111, 21));
        depDepletionRateLineEdit = new QLineEdit(groupBox_17);
        depDepletionRateLineEdit->setObjectName(QStringLiteral("depDepletionRateLineEdit"));
        depDepletionRateLineEdit->setGeometry(QRect(110, 40, 61, 21));
        depBPPressLineEdit = new QLineEdit(groupBox_17);
        depBPPressLineEdit->setObjectName(QStringLiteral("depBPPressLineEdit"));
        depBPPressLineEdit->setGeometry(QRect(110, 20, 61, 21));
        label_62 = new QLabel(groupBox_17);
        label_62->setObjectName(QStringLiteral("label_62"));
        label_62->setGeometry(QRect(10, 40, 111, 21));
        depDepletionTimeLineEdit = new QLineEdit(groupBox_17);
        depDepletionTimeLineEdit->setObjectName(QStringLiteral("depDepletionTimeLineEdit"));
        depDepletionTimeLineEdit->setGeometry(QRect(110, 60, 61, 21));
        label_63 = new QLabel(groupBox_17);
        label_63->setObjectName(QStringLiteral("label_63"));
        label_63->setGeometry(QRect(10, 60, 111, 21));
        depDepletionStepsLabel = new QLabel(groupBox_17);
        depDepletionStepsLabel->setObjectName(QStringLiteral("depDepletionStepsLabel"));
        depDepletionStepsLabel->setGeometry(QRect(10, 80, 111, 21));
        depDepletionStepsLineEdit = new QLineEdit(groupBox_17);
        depDepletionStepsLineEdit->setObjectName(QStringLiteral("depDepletionStepsLineEdit"));
        depDepletionStepsLineEdit->setGeometry(QRect(110, 80, 61, 21));
        depOptimalStepCheckBox = new QCheckBox(groupBox_17);
        depOptimalStepCheckBox->setObjectName(QStringLiteral("depOptimalStepCheckBox"));
        depOptimalStepCheckBox->setGeometry(QRect(10, 100, 111, 21));
        depOptimalStepCheckBox->setChecked(false);
        depInjectionRateLineEdit = new QLineEdit(groupBox_17);
        depInjectionRateLineEdit->setObjectName(QStringLiteral("depInjectionRateLineEdit"));
        depInjectionRateLineEdit->setGeometry(QRect(110, 120, 61, 21));
        label_68 = new QLabel(groupBox_17);
        label_68->setObjectName(QStringLiteral("label_68"));
        label_68->setGeometry(QRect(10, 120, 111, 21));
        label_41 = new QLabel(groupBox_17);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setGeometry(QRect(10, 140, 111, 21));
        depSeed = new QLineEdit(groupBox_17);
        depSeed->setObjectName(QStringLiteral("depSeed"));
        depSeed->setGeometry(QRect(110, 140, 61, 21));
        groupBox_18 = new QGroupBox(tab_12);
        groupBox_18->setObjectName(QStringLiteral("groupBox_18"));
        groupBox_18->setGeometry(QRect(190, 10, 181, 161));
        groupBox_18->setFlat(true);
        depINRadioBox = new QRadioButton(groupBox_18);
        depINRadioBox->setObjectName(QStringLiteral("depINRadioBox"));
        depINRadioBox->setGeometry(QRect(10, 20, 141, 21));
        depINRadioBox->setChecked(true);
        depPNRadioBox = new QRadioButton(groupBox_18);
        depPNRadioBox->setObjectName(QStringLiteral("depPNRadioBox"));
        depPNRadioBox->setGeometry(QRect(10, 40, 141, 21));
        label_64 = new QLabel(groupBox_18);
        label_64->setObjectName(QStringLiteral("label_64"));
        label_64->setGeometry(QRect(10, 60, 111, 16));
        depMinCavRadiusLineEdit = new QLineEdit(groupBox_18);
        depMinCavRadiusLineEdit->setObjectName(QStringLiteral("depMinCavRadiusLineEdit"));
        depMinCavRadiusLineEdit->setEnabled(false);
        depMinCavRadiusLineEdit->setGeometry(QRect(120, 80, 61, 21));
        label_65 = new QLabel(groupBox_18);
        label_65->setObjectName(QStringLiteral("label_65"));
        label_65->setEnabled(false);
        label_65->setGeometry(QRect(10, 120, 111, 16));
        label_66 = new QLabel(groupBox_18);
        label_66->setObjectName(QStringLiteral("label_66"));
        label_66->setEnabled(false);
        label_66->setGeometry(QRect(10, 80, 111, 16));
        depMaxCavRadiusLineEdit = new QLineEdit(groupBox_18);
        depMaxCavRadiusLineEdit->setObjectName(QStringLiteral("depMaxCavRadiusLineEdit"));
        depMaxCavRadiusLineEdit->setEnabled(false);
        depMaxCavRadiusLineEdit->setGeometry(QRect(120, 100, 61, 21));
        depCavityDensityLineEdit = new QLineEdit(groupBox_18);
        depCavityDensityLineEdit->setObjectName(QStringLiteral("depCavityDensityLineEdit"));
        depCavityDensityLineEdit->setEnabled(false);
        depCavityDensityLineEdit->setGeometry(QRect(120, 120, 61, 21));
        label_67 = new QLabel(groupBox_18);
        label_67->setObjectName(QStringLiteral("label_67"));
        label_67->setEnabled(false);
        label_67->setGeometry(QRect(10, 100, 111, 16));
        depBubbleNumSpinBox = new QLineEdit(groupBox_18);
        depBubbleNumSpinBox->setObjectName(QStringLiteral("depBubbleNumSpinBox"));
        depBubbleNumSpinBox->setGeometry(QRect(120, 60, 61, 21));
        groupBox_19 = new QGroupBox(tab_12);
        groupBox_19->setObjectName(QStringLiteral("groupBox_19"));
        groupBox_19->setGeometry(QRect(380, 10, 121, 161));
        groupBox_19->setFlat(true);
        depMultiFillRadioBox = new QRadioButton(groupBox_19);
        depMultiFillRadioBox->setObjectName(QStringLiteral("depMultiFillRadioBox"));
        depMultiFillRadioBox->setGeometry(QRect(10, 20, 111, 21));
        depMultiFillRadioBox->setChecked(true);
        depInvasionPercRadioBox = new QRadioButton(groupBox_19);
        depInvasionPercRadioBox->setObjectName(QStringLiteral("depInvasionPercRadioBox"));
        depInvasionPercRadioBox->setGeometry(QRect(10, 40, 111, 21));
        tabWidget_3->addTab(tab_12, QString());
        tab_10 = new QWidget();
        tab_10->setObjectName(QStringLiteral("tab_10"));
        groupBox_28 = new QGroupBox(tab_10);
        groupBox_28->setObjectName(QStringLiteral("groupBox_28"));
        groupBox_28->setGeometry(QRect(10, 10, 181, 161));
        groupBox_28->setFlat(true);
        groupBox_28->setCheckable(false);
        label_84 = new QLabel(groupBox_28);
        label_84->setObjectName(QStringLiteral("label_84"));
        label_84->setGeometry(QRect(10, 20, 101, 21));
        depletionSecondPhase = new QComboBox(groupBox_28);
        depletionSecondPhase->setObjectName(QStringLiteral("depletionSecondPhase"));
        depletionSecondPhase->setGeometry(QRect(110, 40, 71, 21));
        label_85 = new QLabel(groupBox_28);
        label_85->setObjectName(QStringLiteral("label_85"));
        label_85->setGeometry(QRect(10, 40, 101, 21));
        depletionInitialPhase = new QComboBox(groupBox_28);
        depletionInitialPhase->setObjectName(QStringLiteral("depletionInitialPhase"));
        depletionInitialPhase->setGeometry(QRect(110, 20, 71, 21));
        depletionNoSnapOff = new QCheckBox(groupBox_28);
        depletionNoSnapOff->setObjectName(QStringLiteral("depletionNoSnapOff"));
        depletionNoSnapOff->setGeometry(QRect(10, 70, 171, 21));
        depletionNoSnapOff->setChecked(false);
        depletionStopAfterBreakthrough = new QCheckBox(groupBox_28);
        depletionStopAfterBreakthrough->setObjectName(QStringLiteral("depletionStopAfterBreakthrough"));
        depletionStopAfterBreakthrough->setGeometry(QRect(10, 90, 171, 21));
        depletionStopAfterBreakthrough->setChecked(false);
        tabWidget_3->addTab(tab_10, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        groupBox_16 = new QGroupBox(tab_6);
        groupBox_16->setObjectName(QStringLiteral("groupBox_16"));
        groupBox_16->setEnabled(true);
        groupBox_16->setGeometry(QRect(10, 10, 311, 161));
        groupBox_16->setFlat(true);
        label_39 = new QLabel(groupBox_16);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setGeometry(QRect(10, 90, 111, 21));
        label_43 = new QLabel(groupBox_16);
        label_43->setObjectName(QStringLiteral("label_43"));
        label_43->setGeometry(QRect(10, 110, 111, 21));
        label_44 = new QLabel(groupBox_16);
        label_44->setObjectName(QStringLiteral("label_44"));
        label_44->setGeometry(QRect(10, 130, 111, 21));
        label_45 = new QLabel(groupBox_16);
        label_45->setObjectName(QStringLiteral("label_45"));
        label_45->setGeometry(QRect(190, 90, 61, 21));
        depOWIFTLineEdit = new QLineEdit(groupBox_16);
        depOWIFTLineEdit->setObjectName(QStringLiteral("depOWIFTLineEdit"));
        depOWIFTLineEdit->setGeometry(QRect(249, 90, 51, 21));
        label_46 = new QLabel(groupBox_16);
        label_46->setObjectName(QStringLiteral("label_46"));
        label_46->setGeometry(QRect(190, 110, 61, 21));
        depOGIFTLineEdit = new QLineEdit(groupBox_16);
        depOGIFTLineEdit->setObjectName(QStringLiteral("depOGIFTLineEdit"));
        depOGIFTLineEdit->setGeometry(QRect(249, 110, 51, 21));
        depWGIFTLineEdit = new QLineEdit(groupBox_16);
        depWGIFTLineEdit->setObjectName(QStringLiteral("depWGIFTLineEdit"));
        depWGIFTLineEdit->setGeometry(QRect(249, 130, 51, 21));
        label_48 = new QLabel(groupBox_16);
        label_48->setObjectName(QStringLiteral("label_48"));
        label_48->setGeometry(QRect(190, 130, 61, 21));
        depOilVisLineEdit = new QLineEdit(groupBox_16);
        depOilVisLineEdit->setObjectName(QStringLiteral("depOilVisLineEdit"));
        depOilVisLineEdit->setGeometry(QRect(120, 90, 51, 21));
        depWaterVisLineEdit = new QLineEdit(groupBox_16);
        depWaterVisLineEdit->setObjectName(QStringLiteral("depWaterVisLineEdit"));
        depWaterVisLineEdit->setGeometry(QRect(120, 110, 51, 21));
        depGasVisLineEdit = new QLineEdit(groupBox_16);
        depGasVisLineEdit->setObjectName(QStringLiteral("depGasVisLineEdit"));
        depGasVisLineEdit->setGeometry(QRect(120, 130, 51, 21));
        depLoadPressDependCheckBox = new QCheckBox(groupBox_16);
        depLoadPressDependCheckBox->setObjectName(QStringLiteral("depLoadPressDependCheckBox"));
        depLoadPressDependCheckBox->setGeometry(QRect(10, 70, 221, 21));
        depLoadPressDependCheckBox->setChecked(true);
        label_50 = new QLabel(groupBox_16);
        label_50->setObjectName(QStringLiteral("label_50"));
        label_50->setGeometry(QRect(10, 20, 111, 21));
        label_49 = new QLabel(groupBox_16);
        label_49->setObjectName(QStringLiteral("label_49"));
        label_49->setGeometry(QRect(10, 40, 111, 21));
        depOilDiffCoefLineEdit = new QLineEdit(groupBox_16);
        depOilDiffCoefLineEdit->setObjectName(QStringLiteral("depOilDiffCoefLineEdit"));
        depOilDiffCoefLineEdit->setGeometry(QRect(120, 20, 51, 21));
        depWaterDiffCoefLineEdit = new QLineEdit(groupBox_16);
        depWaterDiffCoefLineEdit->setObjectName(QStringLiteral("depWaterDiffCoefLineEdit"));
        depWaterDiffCoefLineEdit->setGeometry(QRect(120, 40, 51, 21));
        depPressDependDataPathLineEdit = new QLineEdit(groupBox_16);
        depPressDependDataPathLineEdit->setObjectName(QStringLiteral("depPressDependDataPathLineEdit"));
        depPressDependDataPathLineEdit->setGeometry(QRect(180, 40, 121, 21));
        depPressDependDataPathLineEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        depPressDependDataPathLabel = new QLabel(groupBox_16);
        depPressDependDataPathLabel->setObjectName(QStringLiteral("depPressDependDataPathLabel"));
        depPressDependDataPathLabel->setGeometry(QRect(180, 20, 131, 16));
        groupBox_15 = new QGroupBox(tab_6);
        groupBox_15->setObjectName(QStringLiteral("groupBox_15"));
        groupBox_15->setGeometry(QRect(330, 10, 161, 161));
        groupBox_15->setFlat(true);
        label_28 = new QLabel(groupBox_15);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(10, 20, 61, 21));
        depletionSwRandomRadioButton = new QRadioButton(groupBox_15);
        depletionSwRandomRadioButton->setObjectName(QStringLiteral("depletionSwRandomRadioButton"));
        depletionSwRandomRadioButton->setGeometry(QRect(10, 40, 151, 21));
        depletionSwRandomRadioButton->setChecked(true);
        depletionSwBigRadioButton = new QRadioButton(groupBox_15);
        depletionSwBigRadioButton->setObjectName(QStringLiteral("depletionSwBigRadioButton"));
        depletionSwBigRadioButton->setGeometry(QRect(10, 80, 151, 21));
        depletionSwSmallRadioButton = new QRadioButton(groupBox_15);
        depletionSwSmallRadioButton->setObjectName(QStringLiteral("depletionSwSmallRadioButton"));
        depletionSwSmallRadioButton->setGeometry(QRect(10, 60, 151, 21));
        depSwAPDRadioButton = new QRadioButton(groupBox_15);
        depSwAPDRadioButton->setObjectName(QStringLiteral("depSwAPDRadioButton"));
        depSwAPDRadioButton->setGeometry(QRect(10, 100, 81, 21));
        twoPhaseAging_2 = new QCheckBox(groupBox_15);
        twoPhaseAging_2->setObjectName(QStringLiteral("twoPhaseAging_2"));
        twoPhaseAging_2->setGeometry(QRect(90, 100, 71, 21));
        twoPhaseAging_2->setChecked(false);
        depletionInitialSwSpinBox = new QLineEdit(groupBox_15);
        depletionInitialSwSpinBox->setObjectName(QStringLiteral("depletionInitialSwSpinBox"));
        depletionInitialSwSpinBox->setGeometry(QRect(90, 20, 61, 21));
        tabWidget_3->addTab(tab_6, QString());
        misc = new QWidget();
        misc->setObjectName(QStringLiteral("misc"));
        groupBox_22 = new QGroupBox(misc);
        groupBox_22->setObjectName(QStringLiteral("groupBox_22"));
        groupBox_22->setGeometry(QRect(10, 10, 181, 161));
        groupBox_22->setFlat(true);
        depPostProcessCheckBox = new QCheckBox(groupBox_22);
        depPostProcessCheckBox->setObjectName(QStringLiteral("depPostProcessCheckBox"));
        depPostProcessCheckBox->setGeometry(QRect(10, 100, 151, 21));
        depPostProcessCheckBox->setChecked(false);
        depRPermCheckBox = new QCheckBox(groupBox_22);
        depRPermCheckBox->setObjectName(QStringLiteral("depRPermCheckBox"));
        depRPermCheckBox->setGeometry(QRect(10, 80, 151, 21));
        depRPermCheckBox->setChecked(false);
        depGravityCheckBox = new QCheckBox(groupBox_22);
        depGravityCheckBox->setObjectName(QStringLiteral("depGravityCheckBox"));
        depGravityCheckBox->setGeometry(QRect(10, 20, 121, 21));
        depGravityCheckBox->setChecked(false);
        depGLineEdit = new QLineEdit(groupBox_22);
        depGLineEdit->setObjectName(QStringLiteral("depGLineEdit"));
        depGLineEdit->setGeometry(QRect(110, 20, 61, 21));
        depViscousForcesCheckBox = new QCheckBox(groupBox_22);
        depViscousForcesCheckBox->setObjectName(QStringLiteral("depViscousForcesCheckBox"));
        depViscousForcesCheckBox->setGeometry(QRect(10, 40, 151, 21));
        depViscousForcesCheckBox->setChecked(false);
        depPinViscousLineEdit = new QLineEdit(groupBox_22);
        depPinViscousLineEdit->setObjectName(QStringLiteral("depPinViscousLineEdit"));
        depPinViscousLineEdit->setGeometry(QRect(30, 60, 61, 21));
        depPoutViscousLineEdit = new QLineEdit(groupBox_22);
        depPoutViscousLineEdit->setObjectName(QStringLiteral("depPoutViscousLineEdit"));
        depPoutViscousLineEdit->setGeometry(QRect(110, 60, 61, 21));
        tabWidget_3->addTab(misc, QString());

        verticalLayout->addWidget(tabWidget_3);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setDocumentMode(true);
        tab_17 = new QWidget();
        tab_17->setObjectName(QStringLiteral("tab_17"));
        verticalLayout_3 = new QVBoxLayout(tab_17);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget = new widget3d(tab_17);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy3);
        widget->setMinimumSize(QSize(480, 480));

        verticalLayout_3->addWidget(widget);

        tabWidget->addTab(tab_17, QString());
        tab_18 = new QWidget();
        tab_18->setObjectName(QStringLiteral("tab_18"));
        verticalLayout_4 = new QVBoxLayout(tab_18);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        plotWidget = new QCustomPlot(tab_18);
        plotWidget->setObjectName(QStringLiteral("plotWidget"));
        sizePolicy3.setHeightForWidth(plotWidget->sizePolicy().hasHeightForWidth());
        plotWidget->setSizePolicy(sizePolicy3);
        plotWidget->setMinimumSize(QSize(0, 0));

        verticalLayout_4->addWidget(plotWidget);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_71 = new QLabel(tab_18);
        label_71->setObjectName(QStringLiteral("label_71"));

        horizontalLayout_4->addWidget(label_71);

        title = new QLineEdit(tab_18);
        title->setObjectName(QStringLiteral("title"));
        title->setMaximumSize(QSize(16777215, 21));

        horizontalLayout_4->addWidget(title);

        label_37 = new QLabel(tab_18);
        label_37->setObjectName(QStringLiteral("label_37"));

        horizontalLayout_4->addWidget(label_37);

        xAxisTitle = new QLineEdit(tab_18);
        xAxisTitle->setObjectName(QStringLiteral("xAxisTitle"));
        xAxisTitle->setMaximumSize(QSize(16777215, 21));

        horizontalLayout_4->addWidget(xAxisTitle);

        yAxisTitle22 = new QLabel(tab_18);
        yAxisTitle22->setObjectName(QStringLiteral("yAxisTitle22"));

        horizontalLayout_4->addWidget(yAxisTitle22);

        yAxisTitle = new QLineEdit(tab_18);
        yAxisTitle->setObjectName(QStringLiteral("yAxisTitle"));
        yAxisTitle->setMaximumSize(QSize(16777215, 21));

        horizontalLayout_4->addWidget(yAxisTitle);

        pushButton = new QPushButton(tab_18);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_4->addWidget(pushButton);

        twoPhaseSaveCurveButton = new QPushButton(tab_18);
        twoPhaseSaveCurveButton->setObjectName(QStringLiteral("twoPhaseSaveCurveButton"));
        twoPhaseSaveCurveButton->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_4->addWidget(twoPhaseSaveCurveButton);


        verticalLayout_4->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_33 = new QLabel(tab_18);
        label_33->setObjectName(QStringLiteral("label_33"));

        horizontalLayout_3->addWidget(label_33, 0, Qt::AlignHCenter);

        minXAxis = new QLineEdit(tab_18);
        minXAxis->setObjectName(QStringLiteral("minXAxis"));
        minXAxis->setMinimumSize(QSize(0, 0));
        minXAxis->setMaximumSize(QSize(16777215, 21));

        horizontalLayout_3->addWidget(minXAxis, 0, Qt::AlignLeft);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        label_34 = new QLabel(tab_18);
        label_34->setObjectName(QStringLiteral("label_34"));

        horizontalLayout_3->addWidget(label_34);

        maxXAxis = new QLineEdit(tab_18);
        maxXAxis->setObjectName(QStringLiteral("maxXAxis"));
        maxXAxis->setMaximumSize(QSize(16777215, 21));

        horizontalLayout_3->addWidget(maxXAxis, 0, Qt::AlignLeft);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        label_31 = new QLabel(tab_18);
        label_31->setObjectName(QStringLiteral("label_31"));

        horizontalLayout_3->addWidget(label_31);

        minYAxis = new QLineEdit(tab_18);
        minYAxis->setObjectName(QStringLiteral("minYAxis"));
        minYAxis->setMaximumSize(QSize(16777215, 21));

        horizontalLayout_3->addWidget(minYAxis, 0, Qt::AlignLeft);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        label_32 = new QLabel(tab_18);
        label_32->setObjectName(QStringLiteral("label_32"));

        horizontalLayout_3->addWidget(label_32);

        maxYAxis = new QLineEdit(tab_18);
        maxYAxis->setObjectName(QStringLiteral("maxYAxis"));
        maxYAxis->setMaximumSize(QSize(16777215, 21));

        horizontalLayout_3->addWidget(maxYAxis, 0, Qt::AlignHCenter);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        label_36 = new QLabel(tab_18);
        label_36->setObjectName(QStringLiteral("label_36"));

        horizontalLayout_3->addWidget(label_36);

        tickStep = new QLineEdit(tab_18);
        tickStep->setObjectName(QStringLiteral("tickStep"));
        tickStep->setMaximumSize(QSize(16777215, 21));

        horizontalLayout_3->addWidget(tickStep);


        verticalLayout_4->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton_2 = new QPushButton(tab_18);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_5->addWidget(pushButton_2);

        fileToPlot = new QLineEdit(tab_18);
        fileToPlot->setObjectName(QStringLiteral("fileToPlot"));

        horizontalLayout_5->addWidget(fileToPlot);

        realTimeCheckBox = new QCheckBox(tab_18);
        realTimeCheckBox->setObjectName(QStringLiteral("realTimeCheckBox"));

        horizontalLayout_5->addWidget(realTimeCheckBox);

        plot = new QPushButton(tab_18);
        plot->setObjectName(QStringLiteral("plot"));
        plot->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_5->addWidget(plot);

        pushButton_3 = new QPushButton(tab_18);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_5->addWidget(pushButton_3);


        verticalLayout_4->addLayout(horizontalLayout_5);

        tabWidget->addTab(tab_18, QString());

        verticalLayout_2->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        visiblePhases = new QGroupBox(centralwidget);
        visiblePhases->setObjectName(QStringLiteral("visiblePhases"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(visiblePhases->sizePolicy().hasHeightForWidth());
        visiblePhases->setSizePolicy(sizePolicy4);
        visiblePhases->setMinimumSize(QSize(150, 91));
        visiblePhases->setCheckable(false);
        visibleOilCheckBox = new QCheckBox(visiblePhases);
        visibleOilCheckBox->setObjectName(QStringLiteral("visibleOilCheckBox"));
        visibleOilCheckBox->setGeometry(QRect(10, 20, 71, 21));
        visibleOilCheckBox->setChecked(true);
        visibleWaterCheckBox = new QCheckBox(visiblePhases);
        visibleWaterCheckBox->setObjectName(QStringLiteral("visibleWaterCheckBox"));
        visibleWaterCheckBox->setGeometry(QRect(10, 40, 71, 21));
        visibleWaterCheckBox->setChecked(true);
        visibleGasCheckBox = new QCheckBox(visiblePhases);
        visibleGasCheckBox->setObjectName(QStringLiteral("visibleGasCheckBox"));
        visibleGasCheckBox->setGeometry(QRect(10, 60, 71, 21));
        visibleGasCheckBox->setChecked(true);
        visibleOilWetCheckBox = new QCheckBox(visiblePhases);
        visibleOilWetCheckBox->setObjectName(QStringLiteral("visibleOilWetCheckBox"));
        visibleOilWetCheckBox->setGeometry(QRect(80, 20, 71, 21));
        visibleOilWetCheckBox->setChecked(true);
        visibleWaterWetCheckBox = new QCheckBox(visiblePhases);
        visibleWaterWetCheckBox->setObjectName(QStringLiteral("visibleWaterWetCheckBox"));
        visibleWaterWetCheckBox->setGeometry(QRect(80, 40, 71, 21));
        visibleWaterWetCheckBox->setChecked(true);

        horizontalLayout->addWidget(visiblePhases);

        visiblePhases_2 = new QGroupBox(centralwidget);
        visiblePhases_2->setObjectName(QStringLiteral("visiblePhases_2"));
        sizePolicy4.setHeightForWidth(visiblePhases_2->sizePolicy().hasHeightForWidth());
        visiblePhases_2->setSizePolicy(sizePolicy4);
        visiblePhases_2->setMinimumSize(QSize(113, 91));
        visiblePhases_2->setCheckable(false);
        resetRadioButton = new QRadioButton(visiblePhases_2);
        resetRadioButton->setObjectName(QStringLiteral("resetRadioButton"));
        resetRadioButton->setGeometry(QRect(10, 20, 101, 21));
        rotateCheckBox = new QCheckBox(visiblePhases_2);
        rotateCheckBox->setObjectName(QStringLiteral("rotateCheckBox"));
        rotateCheckBox->setGeometry(QRect(10, 40, 101, 21));

        horizontalLayout->addWidget(visiblePhases_2);

        visiblePhases_4 = new QGroupBox(centralwidget);
        visiblePhases_4->setObjectName(QStringLiteral("visiblePhases_4"));
        QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(91);
        sizePolicy5.setHeightForWidth(visiblePhases_4->sizePolicy().hasHeightForWidth());
        visiblePhases_4->setSizePolicy(sizePolicy5);
        visiblePhases_4->setMinimumSize(QSize(113, 91));
        visiblePhases_4->setCheckable(false);
        visiblePhases_4->setChecked(false);
        xCutCheckBox = new QCheckBox(visiblePhases_4);
        xCutCheckBox->setObjectName(QStringLiteral("xCutCheckBox"));
        xCutCheckBox->setGeometry(QRect(10, 20, 21, 21));
        yCutCheckBox = new QCheckBox(visiblePhases_4);
        yCutCheckBox->setObjectName(QStringLiteral("yCutCheckBox"));
        yCutCheckBox->setGeometry(QRect(10, 40, 21, 21));
        zCutCheckBox = new QCheckBox(visiblePhases_4);
        zCutCheckBox->setObjectName(QStringLiteral("zCutCheckBox"));
        zCutCheckBox->setGeometry(QRect(10, 60, 21, 21));
        xCutSpinBox = new QLineEdit(visiblePhases_4);
        xCutSpinBox->setObjectName(QStringLiteral("xCutSpinBox"));
        xCutSpinBox->setEnabled(false);
        xCutSpinBox->setGeometry(QRect(60, 20, 51, 21));
        yCutSpinBox = new QLineEdit(visiblePhases_4);
        yCutSpinBox->setObjectName(QStringLiteral("yCutSpinBox"));
        yCutSpinBox->setEnabled(false);
        yCutSpinBox->setGeometry(QRect(60, 40, 51, 21));
        zCutSpinBox = new QLineEdit(visiblePhases_4);
        zCutSpinBox->setObjectName(QStringLiteral("zCutSpinBox"));
        zCutSpinBox->setEnabled(false);
        zCutSpinBox->setGeometry(QRect(60, 60, 51, 21));
        label_52 = new QLabel(visiblePhases_4);
        label_52->setObjectName(QStringLiteral("label_52"));
        label_52->setGeometry(QRect(40, 20, 21, 21));
        label_53 = new QLabel(visiblePhases_4);
        label_53->setObjectName(QStringLiteral("label_53"));
        label_53->setGeometry(QRect(40, 40, 21, 21));
        label_54 = new QLabel(visiblePhases_4);
        label_54->setObjectName(QStringLiteral("label_54"));
        label_54->setGeometry(QRect(40, 60, 21, 21));

        horizontalLayout->addWidget(visiblePhases_4);

        visiblePhases_3 = new QGroupBox(centralwidget);
        visiblePhases_3->setObjectName(QStringLiteral("visiblePhases_3"));
        sizePolicy5.setHeightForWidth(visiblePhases_3->sizePolicy().hasHeightForWidth());
        visiblePhases_3->setSizePolicy(sizePolicy5);
        visiblePhases_3->setMinimumSize(QSize(113, 91));
        visiblePhases_3->setCheckable(false);
        pore3DCheckBox = new QCheckBox(visiblePhases_3);
        pore3DCheckBox->setObjectName(QStringLiteral("pore3DCheckBox"));
        pore3DCheckBox->setGeometry(QRect(10, 20, 101, 21));
        pore3DCheckBox->setChecked(true);
        node3DCheckBox = new QCheckBox(visiblePhases_3);
        node3DCheckBox->setObjectName(QStringLiteral("node3DCheckBox"));
        node3DCheckBox->setGeometry(QRect(10, 40, 101, 21));
        node3DCheckBox->setChecked(true);
        lightCheckBox = new QCheckBox(visiblePhases_3);
        lightCheckBox->setObjectName(QStringLiteral("lightCheckBox"));
        lightCheckBox->setGeometry(QRect(10, 60, 101, 21));
        lightCheckBox->setChecked(true);

        horizontalLayout->addWidget(visiblePhases_3);


        verticalLayout_2->addLayout(horizontalLayout);


        horizontalLayout_2->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        QWidget::setTabOrder(regularRadioButton, extractedRadioButton);
        QWidget::setTabOrder(extractedRadioButton, calibratedRadioButton);
        QWidget::setTabOrder(calibratedRadioButton, loadNetworkButton);
        QWidget::setTabOrder(loadNetworkButton, pathToExtractedLineEdit);
        QWidget::setTabOrder(pathToExtractedLineEdit, extendCheckBox);
        QWidget::setTabOrder(extendCheckBox, loadFromFileRadioButton);
        QWidget::setTabOrder(loadFromFileRadioButton, psdRayleighRadioButton);
        QWidget::setTabOrder(psdRayleighRadioButton, minRadiusLineEdit);
        QWidget::setTabOrder(minRadiusLineEdit, psdUniformRadioButton);
        QWidget::setTabOrder(psdUniformRadioButton, maxRadiusLineEdit);
        QWidget::setTabOrder(maxRadiusLineEdit, rayleighParamLineEdit);
        QWidget::setTabOrder(rayleighParamLineEdit, volConsRadiusLineEdit);
        QWidget::setTabOrder(volConsRadiusLineEdit, volExpLineEdit);
        QWidget::setTabOrder(volExpLineEdit, condConsLineEdit);
        QWidget::setTabOrder(condConsLineEdit, condExpLineEdit);
        QWidget::setTabOrder(condExpLineEdit, lengthLineEdit);
        QWidget::setTabOrder(lengthLineEdit, wwSpinBox);
        QWidget::setTabOrder(wwSpinBox, owSpinBox);
        QWidget::setTabOrder(owSpinBox, mwlSpinBox);
        QWidget::setTabOrder(mwlSpinBox, fwSpinBox);
        QWidget::setTabOrder(fwSpinBox, mwsSpinBox);
        QWidget::setTabOrder(mwsSpinBox, shapeFactor);
        QWidget::setTabOrder(shapeFactor, waterFilmsCheckBox);
        QWidget::setTabOrder(waterFilmsCheckBox, oilFilmsCheckBox);
        QWidget::setTabOrder(oilFilmsCheckBox, choleskyRadioButton);
        QWidget::setTabOrder(choleskyRadioButton, bicstabRadioButton);
        QWidget::setTabOrder(bicstabRadioButton, calcPermCheckBox);
        QWidget::setTabOrder(calcPermCheckBox, extractDataCheckBox);
        QWidget::setTabOrder(extractDataCheckBox, saveNetworkImageButton);
        QWidget::setTabOrder(saveNetworkImageButton, loadNetworkStatusRadioButton);
        QWidget::setTabOrder(loadNetworkStatusRadioButton, pathToPhasePoresLineEdit);
        QWidget::setTabOrder(pathToPhasePoresLineEdit, pathToPhaseNodesLineEdit);
        QWidget::setTabOrder(pathToPhaseNodesLineEdit, renderImagesRadioButton);
        QWidget::setTabOrder(renderImagesRadioButton, imagesCountLineEdit);
        QWidget::setTabOrder(imagesCountLineEdit, renderNetworkButton);
        QWidget::setTabOrder(renderNetworkButton, renderStopButton);
        QWidget::setTabOrder(renderStopButton, tabWidget_2);
        QWidget::setTabOrder(tabWidget_2, PDCheckBox);
        QWidget::setTabOrder(PDCheckBox, PICheckBox);
        QWidget::setTabOrder(PICheckBox, SDCheckBox);
        QWidget::setTabOrder(SDCheckBox, SICheckBox);
        QWidget::setTabOrder(SICheckBox, TDCheckBox);
        QWidget::setTabOrder(TDCheckBox, twoPhaseSimButton);
        QWidget::setTabOrder(twoPhaseSimButton, twoPhaseSimStopButton);
        QWidget::setTabOrder(twoPhaseSimStopButton, twoPhaseFlowSteady);
        QWidget::setTabOrder(twoPhaseFlowSteady, twoPhaseFlowUnSteady);
        QWidget::setTabOrder(twoPhaseFlowUnSteady, threePhaseFlowSteady);
        QWidget::setTabOrder(threePhaseFlowSteady, twoPhaseStepsLineEdit);
        QWidget::setTabOrder(twoPhaseStepsLineEdit, twoPhaseOilVisLineEdit);
        QWidget::setTabOrder(twoPhaseOilVisLineEdit, twoPhaseWaterVisLineEdit);
        QWidget::setTabOrder(twoPhaseWaterVisLineEdit, twoPhaseGasVisLineEdit);
        QWidget::setTabOrder(twoPhaseGasVisLineEdit, twoPhaseOWIFTLineEdit);
        QWidget::setTabOrder(twoPhaseOWIFTLineEdit, twoPhaseOGIFTLineEdit);
        QWidget::setTabOrder(twoPhaseOGIFTLineEdit, twoPhaseWGIFTLineEdit);
        QWidget::setTabOrder(twoPhaseWGIFTLineEdit, twoPhaseSwRandomRadioButton);
        QWidget::setTabOrder(twoPhaseSwRandomRadioButton, twoPhaseSwBigRadioButton);
        QWidget::setTabOrder(twoPhaseSwBigRadioButton, twoPhaseSwSmallRadioButton);
        QWidget::setTabOrder(twoPhaseSwSmallRadioButton, twoPhasePrecisionFactorLineEdit);
        QWidget::setTabOrder(twoPhasePrecisionFactorLineEdit, twoPhaseFlowRateLineEdit);
        QWidget::setTabOrder(twoPhaseFlowRateLineEdit, twoPhaseSimulationTimeLineEdit);
        QWidget::setTabOrder(twoPhaseSimulationTimeLineEdit, networkTabBox);
        QWidget::setTabOrder(networkTabBox, threePhaseInitialWaterFlood);
        QWidget::setTabOrder(threePhaseInitialWaterFlood, threePhaseGasFlood);
        QWidget::setTabOrder(threePhaseGasFlood, threePhaseWaterFlood);
        QWidget::setTabOrder(threePhaseWaterFlood, threePhaseRepeatWAG);
        QWidget::setTabOrder(threePhaseRepeatWAG, threePhaseWAGCycle);
        QWidget::setTabOrder(threePhaseWAGCycle, threePhaseCustomCycle);
        QWidget::setTabOrder(threePhaseCustomCycle, threePhaseStopAfterBreakthrough);
        QWidget::setTabOrder(threePhaseStopAfterBreakthrough, threePhaseNoSnapOff);
        QWidget::setTabOrder(threePhaseNoSnapOff, threePhaseSecondPhase);
        QWidget::setTabOrder(threePhaseSecondPhase, threePhaseInvadingPhase);
        QWidget::setTabOrder(threePhaseInvadingPhase, threePhaseInitialPhase);
        QWidget::setTabOrder(threePhaseInitialPhase, tabWidget_3);
        QWidget::setTabOrder(tabWidget_3, depSimButton);
        QWidget::setTabOrder(depSimButton, depSimStopButton);
        QWidget::setTabOrder(depSimStopButton, migrationCheckBox);
        QWidget::setTabOrder(migrationCheckBox, injectionRadioButton);
        QWidget::setTabOrder(injectionRadioButton, diffusionRadioButton);
        QWidget::setTabOrder(diffusionRadioButton, growthCheckBox);
        QWidget::setTabOrder(growthCheckBox, threePhaseDepletion);
        QWidget::setTabOrder(threePhaseDepletion, depDepletionRateLineEdit);
        QWidget::setTabOrder(depDepletionRateLineEdit, depBPPressLineEdit);
        QWidget::setTabOrder(depBPPressLineEdit, depDepletionTimeLineEdit);
        QWidget::setTabOrder(depDepletionTimeLineEdit, depDepletionStepsLineEdit);
        QWidget::setTabOrder(depDepletionStepsLineEdit, depOptimalStepCheckBox);
        QWidget::setTabOrder(depOptimalStepCheckBox, depInjectionRateLineEdit);
        QWidget::setTabOrder(depInjectionRateLineEdit, depINRadioBox);
        QWidget::setTabOrder(depINRadioBox, depPNRadioBox);
        QWidget::setTabOrder(depPNRadioBox, depMinCavRadiusLineEdit);
        QWidget::setTabOrder(depMinCavRadiusLineEdit, depMaxCavRadiusLineEdit);
        QWidget::setTabOrder(depMaxCavRadiusLineEdit, depCavityDensityLineEdit);
        QWidget::setTabOrder(depCavityDensityLineEdit, depMultiFillRadioBox);
        QWidget::setTabOrder(depMultiFillRadioBox, depInvasionPercRadioBox);
        QWidget::setTabOrder(depInvasionPercRadioBox, depOWIFTLineEdit);
        QWidget::setTabOrder(depOWIFTLineEdit, depOGIFTLineEdit);
        QWidget::setTabOrder(depOGIFTLineEdit, depWGIFTLineEdit);
        QWidget::setTabOrder(depWGIFTLineEdit, depOilVisLineEdit);
        QWidget::setTabOrder(depOilVisLineEdit, depWaterVisLineEdit);
        QWidget::setTabOrder(depWaterVisLineEdit, depGasVisLineEdit);
        QWidget::setTabOrder(depGasVisLineEdit, depLoadPressDependCheckBox);
        QWidget::setTabOrder(depLoadPressDependCheckBox, depOilDiffCoefLineEdit);
        QWidget::setTabOrder(depOilDiffCoefLineEdit, depWaterDiffCoefLineEdit);
        QWidget::setTabOrder(depWaterDiffCoefLineEdit, depPressDependDataPathLineEdit);
        QWidget::setTabOrder(depPressDependDataPathLineEdit, depletionSwRandomRadioButton);
        QWidget::setTabOrder(depletionSwRandomRadioButton, depletionSwBigRadioButton);
        QWidget::setTabOrder(depletionSwBigRadioButton, depletionSwSmallRadioButton);
        QWidget::setTabOrder(depletionSwSmallRadioButton, tabWidget);
        QWidget::setTabOrder(tabWidget, visibleOilCheckBox);
        QWidget::setTabOrder(visibleOilCheckBox, visibleWaterCheckBox);
        QWidget::setTabOrder(visibleWaterCheckBox, visibleGasCheckBox);
        QWidget::setTabOrder(visibleGasCheckBox, visibleOilWetCheckBox);
        QWidget::setTabOrder(visibleOilWetCheckBox, visibleWaterWetCheckBox);
        QWidget::setTabOrder(visibleWaterWetCheckBox, resetRadioButton);
        QWidget::setTabOrder(resetRadioButton, rotateCheckBox);
        QWidget::setTabOrder(rotateCheckBox, xCutCheckBox);
        QWidget::setTabOrder(xCutCheckBox, yCutCheckBox);
        QWidget::setTabOrder(yCutCheckBox, zCutCheckBox);
        QWidget::setTabOrder(zCutCheckBox, pore3DCheckBox);
        QWidget::setTabOrder(pore3DCheckBox, node3DCheckBox);
        QWidget::setTabOrder(node3DCheckBox, lightCheckBox);
        QWidget::setTabOrder(lightCheckBox, minYAxis);
        QWidget::setTabOrder(minYAxis, maxYAxis);
        QWidget::setTabOrder(maxYAxis, minXAxis);
        QWidget::setTabOrder(minXAxis, maxXAxis);

        retranslateUi(MainWindow);

        networkTabBox->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        tabWidget_3->setCurrentIndex(0);
        depletionSecondPhase->setCurrentIndex(1);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "numSCAL", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Network Properties", Q_NULLPTR));
        regularRadioButton->setText(QApplication::translate("MainWindow", "Regular &Network (simple bond  model)", Q_NULLPTR));
        extractedRadioButton->setText(QApplication::translate("MainWindow", "E&xtracted Network", Q_NULLPTR));
        calibratedRadioButton->setText(QApplication::translate("MainWindow", "Regular Networ&k (pore-throat model)", Q_NULLPTR));
        pathToExtractedLabel->setText(QApplication::translate("MainWindow", "Path to Extracted Network", Q_NULLPTR));
        loadNetworkButton->setText(QApplication::translate("MainWindow", "Load Network", Q_NULLPTR));
        pathToExtractedLineEdit->setText(QApplication::translate("MainWindow", "Extracted Networks/", Q_NULLPTR));
        rockPrefixLabel->setText(QApplication::translate("MainWindow", "Rock Prefix", Q_NULLPTR));
        networkRunningLabel->setText(QString());
        extendCheckBox->setText(QApplication::translate("MainWindow", "Extend", Q_NULLPTR));
        loadFromFileRadioButton->setText(QApplication::translate("MainWindow", "&Load  from File", Q_NULLPTR));
        oneDirection->setText(QApplication::translate("MainWindow", "1D", Q_NULLPTR));
        rockList->clear();
        rockList->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Berea", Q_NULLPTR)
         << QApplication::translate("MainWindow", "A1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "C1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "F42A", Q_NULLPTR)
         << QApplication::translate("MainWindow", "F42B", Q_NULLPTR)
         << QApplication::translate("MainWindow", "F42C", Q_NULLPTR)
         << QApplication::translate("MainWindow", "LV60A", Q_NULLPTR)
         << QApplication::translate("MainWindow", "LV60B", Q_NULLPTR)
         << QApplication::translate("MainWindow", "LV60C", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S1", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S2", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S3", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S4", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S5", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S6", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S7", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S8", Q_NULLPTR)
         << QApplication::translate("MainWindow", "S9", Q_NULLPTR)
        );
        rockList->setCurrentText(QApplication::translate("MainWindow", "Berea", Q_NULLPTR));
        extensionNumber->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        label_x->setText(QApplication::translate("MainWindow", "x", Q_NULLPTR));
        networkTabBox->setTabText(networkTabBox->indexOf(tab), QApplication::translate("MainWindow", "Source", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Network Size", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Seed", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        NxSpinBox->setToolTip(QApplication::translate("MainWindow", "number of elements in X direction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        NxSpinBox->setText(QApplication::translate("MainWindow", "40", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        NySpinBox->setToolTip(QApplication::translate("MainWindow", "number of elements in Y direction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        NySpinBox->setText(QApplication::translate("MainWindow", "40", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        NzSpinBox->setToolTip(QApplication::translate("MainWindow", "number of elements in Z direction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        NzSpinBox->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        label_25->setText(QApplication::translate("MainWindow", "Nx", Q_NULLPTR));
        label_27->setText(QApplication::translate("MainWindow", "Ny", Q_NULLPTR));
        label_51->setText(QApplication::translate("MainWindow", "Nz", Q_NULLPTR));
        seed->setText(QApplication::translate("MainWindow", "50", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Pore Size Distribution", Q_NULLPTR));
        psdRayleighRadioButton->setText(QApplication::translate("MainWindow", "Rayleigh", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Max Radius", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        minRadiusLineEdit->setToolTip(QApplication::translate("MainWindow", "um", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        minRadiusLineEdit->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        psdUniformRadioButton->setText(QApplication::translate("MainWindow", "U&niform", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        maxRadiusLineEdit->setToolTip(QApplication::translate("MainWindow", "um", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        maxRadiusLineEdit->setText(QApplication::translate("MainWindow", "20", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Min Radius", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        rayleighParamLineEdit->setToolTip(QApplication::translate("MainWindow", "lambda (um)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        rayleighParamLineEdit->setText(QApplication::translate("MainWindow", "10", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        volConsRadiusLineEdit->setToolTip(QApplication::translate("MainWindow", "volume constant (3Rs approach)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        volConsRadiusLineEdit->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "Vol. Cons.", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "Vol. Exp.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        volExpLineEdit->setToolTip(QApplication::translate("MainWindow", "volume exponent (3Rs approach)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        volExpLineEdit->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        label_17->setText(QApplication::translate("MainWindow", "Cond. Cons.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        condConsLineEdit->setToolTip(QApplication::translate("MainWindow", "conductance constant (3Rs approach)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        condConsLineEdit->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        label_19->setText(QApplication::translate("MainWindow", "Cond. Exp.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        condExpLineEdit->setToolTip(QApplication::translate("MainWindow", "conductance exponent (3Rs approach)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        condExpLineEdit->setText(QApplication::translate("MainWindow", "4", Q_NULLPTR));
        psdTriangularRadioButton->setText(QApplication::translate("MainWindow", "Triang.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        triangularParameter->setToolTip(QApplication::translate("MainWindow", "c (a=minRadius , b=maxRadius) (um)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        triangularParameter->setText(QApplication::translate("MainWindow", "10", Q_NULLPTR));
        psdNormalRadioButton->setText(QApplication::translate("MainWindow", "TN", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        muNormalParameter->setToolTip(QApplication::translate("MainWindow", "u (mean) (um)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        muNormalParameter->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        sigmaNormalParameter->setToolTip(QApplication::translate("MainWindow", "sigma (variance) (um)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        sigmaNormalParameter->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Connectivity", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        lengthLineEdit->setToolTip(QApplication::translate("MainWindow", "um", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        lengthLineEdit->setText(QApplication::translate("MainWindow", "100", Q_NULLPTR));
        lengthLineEdit->setPlaceholderText(QApplication::translate("MainWindow", "um", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "Distort", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Length", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "Z", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "A. Ratio", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        zSpinBox->setToolTip(QApplication::translate("MainWindow", "coordination number", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        zSpinBox->setText(QApplication::translate("MainWindow", "6", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        distortionSpinBox->setToolTip(QApplication::translate("MainWindow", "distortion factor", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        distortionSpinBox->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        aspectRatioSpinBox->setToolTip(QApplication::translate("MainWindow", "aspect ratio (node/throat)", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        aspectRatioSpinBox->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        networkTabBox->setTabText(networkTabBox->indexOf(geometryTab), QApplication::translate("MainWindow", "Geometry", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Wettability Type", Q_NULLPTR));
        wwSpinBox->setText(QApplication::translate("MainWindow", "Water Wet", Q_NULLPTR));
        owSpinBox->setText(QApplication::translate("MainWindow", "Oil Wet", Q_NULLPTR));
        mwlSpinBox->setText(QApplication::translate("MainWindow", "Mi&x Wet Large", Q_NULLPTR));
        fwSpinBox->setText(QApplication::translate("MainWindow", "Fractional Wet", Q_NULLPTR));
        mwsSpinBox->setText(QApplication::translate("MainWindow", "Mix Wet Small", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Water Contact Angle", Q_NULLPTR));
        waterWetLabel->setText(QApplication::translate("MainWindow", "WW", Q_NULLPTR));
        oilWetLabel->setText(QApplication::translate("MainWindow", "OW", Q_NULLPTR));
        to1Label->setText(QApplication::translate("MainWindow", "to", Q_NULLPTR));
        to2Label->setText(QApplication::translate("MainWindow", "to", Q_NULLPTR));
        oilWetFractionLabel->setText(QApplication::translate("MainWindow", "OW Pores Fraction", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "Shape Factor", Q_NULLPTR));
        shapeFactor->setText(QApplication::translate("MainWindow", "0.079", Q_NULLPTR));
        shapeFactor->setPlaceholderText(QApplication::translate("MainWindow", "um", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        minWWThetaSpinBox->setToolTip(QApplication::translate("MainWindow", "deg", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        minWWThetaSpinBox->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        maxWWThetaSpinBox->setToolTip(QApplication::translate("MainWindow", "deg", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        maxWWThetaSpinBox->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        minOWThetaSpinBox->setToolTip(QApplication::translate("MainWindow", "deg", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        minOWThetaSpinBox->setText(QApplication::translate("MainWindow", "180", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        maxOWThetaSpinBox->setToolTip(QApplication::translate("MainWindow", "deg", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        maxOWThetaSpinBox->setText(QApplication::translate("MainWindow", "180", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        owFractionSpinBox->setToolTip(QApplication::translate("MainWindow", "fraction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        owFractionSpinBox->setText(QApplication::translate("MainWindow", "0.5", Q_NULLPTR));
        networkTabBox->setTabText(networkTabBox->indexOf(tab_4), QApplication::translate("MainWindow", "Wettability", Q_NULLPTR));
        groupBox_21->setTitle(QApplication::translate("MainWindow", "Solver", Q_NULLPTR));
        choleskyRadioButton->setText(QApplication::translate("MainWindow", "Choles&ky", Q_NULLPTR));
        bicstabRadioButton->setText(QApplication::translate("MainWindow", "BICSTAB", Q_NULLPTR));
        calcPermCheckBox->setText(QApplication::translate("MainWindow", "Calc. Perm.", Q_NULLPTR));
        groupBox_24->setTitle(QApplication::translate("MainWindow", "Data Extraction", Q_NULLPTR));
        extractDataCheckBox->setText(QApplication::translate("MainWindow", "Extract Data", Q_NULLPTR));
        networkTabBox->setTabText(networkTabBox->indexOf(tab_8), QApplication::translate("MainWindow", "Misc", Q_NULLPTR));
        groupBox_14->setTitle(QApplication::translate("MainWindow", "Network Properties", Q_NULLPTR));
        label_16->setText(QApplication::translate("MainWindow", "Porosity", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "Permeabilty", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        porosityLabel->setToolTip(QApplication::translate("MainWindow", "%", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        porosityLabel->setText(QApplication::translate("MainWindow", "NA", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        permeabilityLabel->setToolTip(QApplication::translate("MainWindow", "mDarcy", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        permeabilityLabel->setText(QApplication::translate("MainWindow", "NA", Q_NULLPTR));
        label_35->setText(QApplication::translate("MainWindow", "Total Pores", Q_NULLPTR));
        totalPoresLabel->setText(QApplication::translate("MainWindow", "NA", Q_NULLPTR));
        totalNodesLabel->setText(QApplication::translate("MainWindow", "NA", Q_NULLPTR));
        label_38->setText(QApplication::translate("MainWindow", "Total Nodes", Q_NULLPTR));
        saveNetworkImageButton->setText(QApplication::translate("MainWindow", "Save Network Image", Q_NULLPTR));
        networkTabBox->setTabText(networkTabBox->indexOf(tab_5), QApplication::translate("MainWindow", "Results", Q_NULLPTR));
        loadNetworkStatusRadioButton->setText(QApplication::translate("MainWindow", "Render Networ&k Status from Files", Q_NULLPTR));
        pathToPhasePoresLineEdit->setText(QApplication::translate("MainWindow", "Results/Network_Status/phases_pores.txt", Q_NULLPTR));
        label_18->setText(QApplication::translate("MainWindow", "Pores", Q_NULLPTR));
        label_29->setText(QApplication::translate("MainWindow", "Nodes", Q_NULLPTR));
        pathToPhaseNodesLineEdit->setText(QApplication::translate("MainWindow", "Results/Network_Status/phases_nodes.txt", Q_NULLPTR));
        renderImagesRadioButton->setText(QApplication::translate("MainWindow", "Render a Video from Files", Q_NULLPTR));
        label_30->setText(QApplication::translate("MainWindow", "Total Images To Process", Q_NULLPTR));
        imagesCountLineEdit->setText(QApplication::translate("MainWindow", "1000", Q_NULLPTR));
        imagesCountLineEdit->setPlaceholderText(QApplication::translate("MainWindow", "um", Q_NULLPTR));
        renderNetworkButton->setText(QApplication::translate("MainWindow", "Render", Q_NULLPTR));
        renderStopButton->setText(QApplication::translate("MainWindow", "Stop Rendering", Q_NULLPTR));
        networkRenderingLabel->setText(QString());
        onlyPhasesCheckBox->setText(QApplication::translate("MainWindow", "Only phases", Q_NULLPTR));
        networkTabBox->setTabText(networkTabBox->indexOf(tab_15), QApplication::translate("MainWindow", "Post-processing", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Fluid Injection Simulation", Q_NULLPTR));
        PDCheckBox->setText(QApplication::translate("MainWindow", "Primary Drainage", Q_NULLPTR));
        PICheckBox->setText(QApplication::translate("MainWindow", "Primary Imbibtion", Q_NULLPTR));
        SDCheckBox->setText(QApplication::translate("MainWindow", "Secondary Drainage", Q_NULLPTR));
        SICheckBox->setText(QApplication::translate("MainWindow", "Secondary Imbibition", Q_NULLPTR));
        TDCheckBox->setText(QApplication::translate("MainWindow", "Tertiary Drainage", Q_NULLPTR));
        twoPhaseSimButton->setText(QApplication::translate("MainWindow", "Run Simulation", Q_NULLPTR));
        twoPhaseSimStopButton->setText(QApplication::translate("MainWindow", "Stop Simulation", Q_NULLPTR));
        twoPhaseRunningLabel->setText(QString());
        twoPhaseFlowSteady->setText(QApplication::translate("MainWindow", "SS (&2 Phases)", Q_NULLPTR));
        twoPhaseFlowUnSteady->setText(QApplication::translate("MainWindow", "&USS (Ganglion)", Q_NULLPTR));
        threePhaseFlowSteady->setText(QApplication::translate("MainWindow", "SS (3 Phases)", Q_NULLPTR));
        twoPhaseLoadFromFileRadioButton->setText(QApplication::translate("MainWindow", "&Load  from File", Q_NULLPTR));
        twoPhaseFlowUnSteadyFast->setText(QApplication::translate("MainWindow", "USS (Drainage)", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("MainWindow", "Cycles", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Parameters", Q_NULLPTR));
        label_60->setText(QApplication::translate("MainWindow", "Steps", Q_NULLPTR));
        twoPhaseStepsLineEdit->setText(QApplication::translate("MainWindow", "100", Q_NULLPTR));
        label_40->setText(QApplication::translate("MainWindow", "Seed", Q_NULLPTR));
        twoPhaseSeed->setText(QApplication::translate("MainWindow", "50", Q_NULLPTR));
        groupBox_20->setTitle(QApplication::translate("MainWindow", "Invasion Mecanism", Q_NULLPTR));
        waterWetLabel_2->setText(QApplication::translate("MainWindow", "WW Ang. Inc. after Aging", Q_NULLPTR));
        waterWetLabel_3->setText(QApplication::translate("MainWindow", "OW Ang. Inc. after Aging", Q_NULLPTR));
        waterFilmsCheckBox->setText(QApplication::translate("MainWindow", "Water Films", Q_NULLPTR));
        oilFilmsCheckBox->setText(QApplication::translate("MainWindow", "Oil Films", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseWWThetaIncSpinBox->setToolTip(QApplication::translate("MainWindow", "deg", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseWWThetaIncSpinBox->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseOWThetaIncSpinBox->setToolTip(QApplication::translate("MainWindow", "deg", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseOWThetaIncSpinBox->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_11), QApplication::translate("MainWindow", "SS (2 Ph.)", Q_NULLPTR));
        groupBox_25->setTitle(QApplication::translate("MainWindow", "WAG Cycle", Q_NULLPTR));
        threePhaseInitialWaterFlood->setText(QApplication::translate("MainWindow", "Initial Water Flood", Q_NULLPTR));
        threePhaseGasFlood->setText(QApplication::translate("MainWindow", "Gas Flood", Q_NULLPTR));
        threePhaseWaterFlood->setText(QApplication::translate("MainWindow", "Water Flood", Q_NULLPTR));
        threePhaseRepeatWAG->setText(QApplication::translate("MainWindow", "Repeat x", Q_NULLPTR));
        threePhaseCyclesNumber->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        groupBox_26->setTitle(QApplication::translate("MainWindow", "3 Phase experiment", Q_NULLPTR));
        threePhaseWAGCycle->setText(QApplication::translate("MainWindow", "WAG cycle", Q_NULLPTR));
        threePhaseCustomCycle->setText(QApplication::translate("MainWindow", "Custom", Q_NULLPTR));
        threePhaseStopAfterBreakthrough->setText(QApplication::translate("MainWindow", "Stop at Breakthrough", Q_NULLPTR));
        threePhaseNoSnapOff->setText(QApplication::translate("MainWindow", "No Snap-off", Q_NULLPTR));
        groupBox_27->setTitle(QApplication::translate("MainWindow", "Custom", Q_NULLPTR));
        label_81->setText(QApplication::translate("MainWindow", "Initial Phase", Q_NULLPTR));
        threePhaseSecondPhase->clear();
        threePhaseSecondPhase->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Oil", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Water", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Gas", Q_NULLPTR)
        );
        threePhaseSecondPhase->setCurrentText(QApplication::translate("MainWindow", "Oil", Q_NULLPTR));
        label_82->setText(QApplication::translate("MainWindow", "Second Phase", Q_NULLPTR));
        label_83->setText(QApplication::translate("MainWindow", "Invading Phase", Q_NULLPTR));
        threePhaseInvadingPhase->clear();
        threePhaseInvadingPhase->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Oil", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Water", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Gas", Q_NULLPTR)
        );
        threePhaseInvadingPhase->setCurrentText(QApplication::translate("MainWindow", "Oil", Q_NULLPTR));
        threePhaseInitialPhase->clear();
        threePhaseInitialPhase->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Oil", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Water", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Gas", Q_NULLPTR)
        );
        threePhaseInitialPhase->setCurrentText(QApplication::translate("MainWindow", "Oil", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_16), QApplication::translate("MainWindow", "SS (3 Ph.)", Q_NULLPTR));
        groupBox_10->setTitle(QApplication::translate("MainWindow", "Flow Properties", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhasePrecisionFactorLineEdit->setToolTip(QApplication::translate("MainWindow", "fraction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhasePrecisionFactorLineEdit->setText(QApplication::translate("MainWindow", "0.05", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseFlowRateLineEdit->setToolTip(QApplication::translate("MainWindow", "m^3/s", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseFlowRateLineEdit->setText(QApplication::translate("MainWindow", "1e-10", Q_NULLPTR));
        label_73->setText(QApplication::translate("MainWindow", "Flow Rate", Q_NULLPTR));
        label_75->setText(QApplication::translate("MainWindow", "Precision Factor", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseSimulationTimeLineEdit->setToolTip(QApplication::translate("MainWindow", "seconds", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseSimulationTimeLineEdit->setText(QApplication::translate("MainWindow", "100", Q_NULLPTR));
        label_80->setText(QApplication::translate("MainWindow", "Simulation Time", Q_NULLPTR));
        overrideByInjectedPVs->setText(QApplication::translate("MainWindow", "Injected  PVs", Q_NULLPTR));
        injectedPVs->setText(QApplication::translate("MainWindow", "10", Q_NULLPTR));
        rateBump->setText(QApplication::translate("MainWindow", "Rate Bump", Q_NULLPTR));
        rateBumpMultiplier->setPrefix(QApplication::translate("MainWindow", "x ", Q_NULLPTR));
        rateBumpTiming->setPrefix(QApplication::translate("MainWindow", "after %", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_14), QApplication::translate("MainWindow", "USS", Q_NULLPTR));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "LS/Surfactant Injection", Q_NULLPTR));
        oilWetLabel_3->setText(QApplication::translate("MainWindow", "IFT Change", Q_NULLPTR));
        twoPhaseSurfactantInjectionAfterBr->setText(QApplication::translate("MainWindow", "After Breakthrough", Q_NULLPTR));
        oilWetLabel_2->setText(QApplication::translate("MainWindow", "Angle Change", Q_NULLPTR));
        twoPhaseSurfactantInjection->setText(QApplication::translate("MainWindow", "Secondary Injection", Q_NULLPTR));
        twoPhaseSurfactantInjectionAfterHST->setText(QApplication::translate("MainWindow", "After 1/2 Sim. Time", Q_NULLPTR));
        label_47->setText(QApplication::translate("MainWindow", "Remove", Q_NULLPTR));
        twoPhaseSurfactantRemoveAfterBr->setText(QApplication::translate("MainWindow", "After Breakthrough", Q_NULLPTR));
        twoPhaseSurfactantRemoveAfterHST->setText(QApplication::translate("MainWindow", "After 1/2 Sim. Time", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseAngleChange->setToolTip(QApplication::translate("MainWindow", "ded", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseAngleChange->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseIFTChange->setToolTip(QApplication::translate("MainWindow", "dyn/cm", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseIFTChange->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_9), QApplication::translate("MainWindow", "LS/Surfactant", Q_NULLPTR));
        groupBox_23->setTitle(QApplication::translate("MainWindow", "Polymer Injection", Q_NULLPTR));
        twoPhasePolymerInjectionAfterBr->setText(QApplication::translate("MainWindow", "After Breakthrough", Q_NULLPTR));
        oilWetLabel_5->setText(QApplication::translate("MainWindow", "Viscosity Change", Q_NULLPTR));
        twoPhasePolymerInjection->setText(QApplication::translate("MainWindow", "Secondary Injection", Q_NULLPTR));
        twoPhasePolymerMicroscopicDiversion->setText(QApplication::translate("MainWindow", "Microscopic Diversion", Q_NULLPTR));
        oilWetLabel_7->setText(QApplication::translate("MainWindow", "Criti. Radius", Q_NULLPTR));
        twoPhasePolymerInjectionAfterHST->setText(QApplication::translate("MainWindow", "After 1/2 Sim. Time", Q_NULLPTR));
        twoPhasePolymerRemoveAfterBr->setText(QApplication::translate("MainWindow", "After Breakthrough", Q_NULLPTR));
        twoPhasePolymerRemoveAfterHST->setText(QApplication::translate("MainWindow", "After 1/2 Sim. Time", Q_NULLPTR));
        label_42->setText(QApplication::translate("MainWindow", "Remove", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhasePolymerCriticalRadius->setToolTip(QApplication::translate("MainWindow", "um", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhasePolymerCriticalRadius->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhasePolymerViscosityChange->setToolTip(QApplication::translate("MainWindow", "cP", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhasePolymerViscosityChange->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_13), QApplication::translate("MainWindow", "Polymer", Q_NULLPTR));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "Fluids Properties", Q_NULLPTR));
        label_20->setText(QApplication::translate("MainWindow", "Oil Viscosity", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseOilVisLineEdit->setToolTip(QApplication::translate("MainWindow", "cP", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseOilVisLineEdit->setText(QApplication::translate("MainWindow", "1.39", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseWaterVisLineEdit->setToolTip(QApplication::translate("MainWindow", "cP", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseWaterVisLineEdit->setText(QApplication::translate("MainWindow", "1.05", Q_NULLPTR));
        label_21->setText(QApplication::translate("MainWindow", "Water Viscosity", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseGasVisLineEdit->setToolTip(QApplication::translate("MainWindow", "cP", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseGasVisLineEdit->setText(QApplication::translate("MainWindow", "0.0187", Q_NULLPTR));
        label_22->setText(QApplication::translate("MainWindow", "Gas Viscosity", Q_NULLPTR));
        label_23->setText(QApplication::translate("MainWindow", "OW IFT", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseOWIFTLineEdit->setToolTip(QApplication::translate("MainWindow", "dyn/cm", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseOWIFTLineEdit->setText(QApplication::translate("MainWindow", "30", Q_NULLPTR));
        label_24->setText(QApplication::translate("MainWindow", "OG IFT", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseOGIFTLineEdit->setToolTip(QApplication::translate("MainWindow", "dyn/cm", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseOGIFTLineEdit->setText(QApplication::translate("MainWindow", "30", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseWGIFTLineEdit->setToolTip(QApplication::translate("MainWindow", "dyn/cm", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseWGIFTLineEdit->setText(QApplication::translate("MainWindow", "30", Q_NULLPTR));
        label_70->setText(QApplication::translate("MainWindow", "WG IFT", Q_NULLPTR));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "Initial Sate", Q_NULLPTR));
        label_26->setText(QApplication::translate("MainWindow", "Swi", Q_NULLPTR));
        twoPhaseSwRandomRadioButton->setText(QApplication::translate("MainWindow", "Random", Q_NULLPTR));
        twoPhaseSwBigRadioButton->setText(QApplication::translate("MainWindow", "Big", Q_NULLPTR));
        twoPhaseSwSmallRadioButton->setText(QApplication::translate("MainWindow", "Small", Q_NULLPTR));
        twoPhaseSwAPDRadioButton->setText(QApplication::translate("MainWindow", "After PD", Q_NULLPTR));
        twoPhaseAging->setText(QApplication::translate("MainWindow", "Aging", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhaseInitialSwSpinBox->setToolTip(QApplication::translate("MainWindow", "fraction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhaseInitialSwSpinBox->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("MainWindow", "Fluids", Q_NULLPTR));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "Misc.", Q_NULLPTR));
        twoPhaseRPermCheckBox->setText(QApplication::translate("MainWindow", "R. Permeabilities", Q_NULLPTR));
        twoPhasePostProcessCheckBox->setText(QApplication::translate("MainWindow", "Post-processing", Q_NULLPTR));
        twoPhaseGravityCheckBox->setText(QApplication::translate("MainWindow", "Gravity ON", Q_NULLPTR));
        twoPhaseGLineEdit->setText(QApplication::translate("MainWindow", "9.8", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhasePinViscousLineEdit->setToolTip(QApplication::translate("MainWindow", "Pa", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhasePinViscousLineEdit->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        twoPhaseViscousForcesCheckBox->setText(QApplication::translate("MainWindow", "Viscous Forces ON", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        twoPhasePoutViscousLineEdit->setToolTip(QApplication::translate("MainWindow", "Pa", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        twoPhasePoutViscousLineEdit->setText(QApplication::translate("MainWindow", "-10000", Q_NULLPTR));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_19), QApplication::translate("MainWindow", "Misc", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Depletion Simulation", Q_NULLPTR));
        depSimButton->setText(QApplication::translate("MainWindow", "Run Simulation", Q_NULLPTR));
        depSimStopButton->setText(QApplication::translate("MainWindow", "Stop Simulation", Q_NULLPTR));
        migrationCheckBox->setText(QApplication::translate("MainWindow", "Migration", Q_NULLPTR));
        injectionRadioButton->setText(QApplication::translate("MainWindow", "In&jection", Q_NULLPTR));
        diffusionRadioButton->setText(QApplication::translate("MainWindow", "Diffusion", Q_NULLPTR));
        growthCheckBox->setText(QApplication::translate("MainWindow", "Growth", Q_NULLPTR));
        depRunningLabel->setText(QString());
        threePhaseDepletion->setText(QApplication::translate("MainWindow", "3 Phases", Q_NULLPTR));
        depLoadFromFileRadioButton->setText(QApplication::translate("MainWindow", "Load  from File", Q_NULLPTR));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_7), QApplication::translate("MainWindow", "Cycles", Q_NULLPTR));
        groupBox_17->setTitle(QApplication::translate("MainWindow", "Depletion Parameters", Q_NULLPTR));
        label_61->setText(QApplication::translate("MainWindow", "BP Pressure", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depDepletionRateLineEdit->setToolTip(QApplication::translate("MainWindow", "psi/day", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depDepletionRateLineEdit->setText(QApplication::translate("MainWindow", "20", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depBPPressLineEdit->setToolTip(QApplication::translate("MainWindow", "psi", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depBPPressLineEdit->setText(QApplication::translate("MainWindow", "750", Q_NULLPTR));
        label_62->setText(QApplication::translate("MainWindow", "Dep. Rate", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depDepletionTimeLineEdit->setToolTip(QApplication::translate("MainWindow", "day", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depDepletionTimeLineEdit->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        label_63->setText(QApplication::translate("MainWindow", "Dep. Time", Q_NULLPTR));
        depDepletionStepsLabel->setText(QApplication::translate("MainWindow", "Dep. Steps", Q_NULLPTR));
        depDepletionStepsLineEdit->setText(QApplication::translate("MainWindow", "10", Q_NULLPTR));
        depOptimalStepCheckBox->setText(QApplication::translate("MainWindow", "Optimal Step", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depInjectionRateLineEdit->setToolTip(QApplication::translate("MainWindow", "ug/day", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depInjectionRateLineEdit->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        label_68->setText(QApplication::translate("MainWindow", "Injection Rate", Q_NULLPTR));
        label_41->setText(QApplication::translate("MainWindow", "Seed", Q_NULLPTR));
        depSeed->setText(QApplication::translate("MainWindow", "50", Q_NULLPTR));
        groupBox_18->setTitle(QApplication::translate("MainWindow", "Nucleation Parameters", Q_NULLPTR));
        depINRadioBox->setText(QApplication::translate("MainWindow", "Instantaneous", Q_NULLPTR));
        depPNRadioBox->setText(QApplication::translate("MainWindow", "Progressive", Q_NULLPTR));
        label_64->setText(QApplication::translate("MainWindow", "Bubbles Number", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depMinCavRadiusLineEdit->setToolTip(QApplication::translate("MainWindow", "um", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depMinCavRadiusLineEdit->setText(QApplication::translate("MainWindow", "0.1", Q_NULLPTR));
        label_65->setText(QApplication::translate("MainWindow", "Cavity Density", Q_NULLPTR));
        label_66->setText(QApplication::translate("MainWindow", "Min Cavity Radius", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depMaxCavRadiusLineEdit->setToolTip(QApplication::translate("MainWindow", "um", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depMaxCavRadiusLineEdit->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depCavityDensityLineEdit->setToolTip(QApplication::translate("MainWindow", "pores/cavity", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depCavityDensityLineEdit->setText(QApplication::translate("MainWindow", "200", Q_NULLPTR));
        label_67->setText(QApplication::translate("MainWindow", "Max Cavity Radius", Q_NULLPTR));
        depBubbleNumSpinBox->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        groupBox_19->setTitle(QApplication::translate("MainWindow", "Growth Parameters", Q_NULLPTR));
        depMultiFillRadioBox->setText(QApplication::translate("MainWindow", "Multi-Filling", Q_NULLPTR));
        depInvasionPercRadioBox->setText(QApplication::translate("MainWindow", "Inv. Perc.", Q_NULLPTR));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_12), QApplication::translate("MainWindow", "Parameters", Q_NULLPTR));
        groupBox_28->setTitle(QApplication::translate("MainWindow", "3 Phase experiment", Q_NULLPTR));
        label_84->setText(QApplication::translate("MainWindow", "Initial Phase", Q_NULLPTR));
        depletionSecondPhase->clear();
        depletionSecondPhase->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Oil", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Water", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Gas", Q_NULLPTR)
        );
        depletionSecondPhase->setCurrentText(QApplication::translate("MainWindow", "Water", Q_NULLPTR));
        label_85->setText(QApplication::translate("MainWindow", "Second Phase", Q_NULLPTR));
        depletionInitialPhase->clear();
        depletionInitialPhase->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Oil", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Water", Q_NULLPTR)
         << QApplication::translate("MainWindow", "Gas", Q_NULLPTR)
        );
        depletionInitialPhase->setCurrentText(QApplication::translate("MainWindow", "Oil", Q_NULLPTR));
        depletionNoSnapOff->setText(QApplication::translate("MainWindow", "No Snap-off", Q_NULLPTR));
        depletionStopAfterBreakthrough->setText(QApplication::translate("MainWindow", "Stop at Breakthrough", Q_NULLPTR));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_10), QApplication::translate("MainWindow", "3 Phases", Q_NULLPTR));
        groupBox_16->setTitle(QApplication::translate("MainWindow", "Fluids Properties", Q_NULLPTR));
        label_39->setText(QApplication::translate("MainWindow", "Oil Viscosity", Q_NULLPTR));
        label_43->setText(QApplication::translate("MainWindow", "Water Viscosity", Q_NULLPTR));
        label_44->setText(QApplication::translate("MainWindow", "Gas Viscosity", Q_NULLPTR));
        label_45->setText(QApplication::translate("MainWindow", "OW IFT", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depOWIFTLineEdit->setToolTip(QApplication::translate("MainWindow", "dyn/cm", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depOWIFTLineEdit->setText(QApplication::translate("MainWindow", "30", Q_NULLPTR));
        label_46->setText(QApplication::translate("MainWindow", "OG IFT", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depOGIFTLineEdit->setToolTip(QApplication::translate("MainWindow", "dyn/cm", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depOGIFTLineEdit->setText(QApplication::translate("MainWindow", "30", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depWGIFTLineEdit->setToolTip(QApplication::translate("MainWindow", "dyn/cm", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depWGIFTLineEdit->setText(QApplication::translate("MainWindow", "30", Q_NULLPTR));
        label_48->setText(QApplication::translate("MainWindow", "WG IFT", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depOilVisLineEdit->setToolTip(QApplication::translate("MainWindow", "cP", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depOilVisLineEdit->setText(QApplication::translate("MainWindow", "300", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depWaterVisLineEdit->setToolTip(QApplication::translate("MainWindow", "cP", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depWaterVisLineEdit->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depGasVisLineEdit->setToolTip(QApplication::translate("MainWindow", "cP", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depGasVisLineEdit->setText(QApplication::translate("MainWindow", "0.02", Q_NULLPTR));
        depLoadPressDependCheckBox->setText(QApplication::translate("MainWindow", "Override with Fixed Values", Q_NULLPTR));
        label_50->setText(QApplication::translate("MainWindow", "Oil Diff Coeff.", Q_NULLPTR));
        label_49->setText(QApplication::translate("MainWindow", "Water Diff Coeff.", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depOilDiffCoefLineEdit->setToolTip(QApplication::translate("MainWindow", "10e-5 cm2/s ", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depOilDiffCoefLineEdit->setText(QApplication::translate("MainWindow", "0.3", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depWaterDiffCoefLineEdit->setToolTip(QApplication::translate("MainWindow", "10e-5 cm2/s", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depWaterDiffCoefLineEdit->setText(QApplication::translate("MainWindow", "0.3", Q_NULLPTR));
        depPressDependDataPathLineEdit->setText(QApplication::translate("MainWindow", "Pressure Data/", Q_NULLPTR));
        depPressDependDataPathLineEdit->setPlaceholderText(QString());
        depPressDependDataPathLabel->setText(QApplication::translate("MainWindow", "Data Folder", Q_NULLPTR));
        groupBox_15->setTitle(QApplication::translate("MainWindow", "Initial State", Q_NULLPTR));
        label_28->setText(QApplication::translate("MainWindow", "Swi", Q_NULLPTR));
        depletionSwRandomRadioButton->setText(QApplication::translate("MainWindow", "Random", Q_NULLPTR));
        depletionSwBigRadioButton->setText(QApplication::translate("MainWindow", "Big", Q_NULLPTR));
        depletionSwSmallRadioButton->setText(QApplication::translate("MainWindow", "Small", Q_NULLPTR));
        depSwAPDRadioButton->setText(QApplication::translate("MainWindow", "After PD", Q_NULLPTR));
        twoPhaseAging_2->setText(QApplication::translate("MainWindow", "Aging", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depletionInitialSwSpinBox->setToolTip(QApplication::translate("MainWindow", "fraction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depletionInitialSwSpinBox->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        tabWidget_3->setTabText(tabWidget_3->indexOf(tab_6), QApplication::translate("MainWindow", "Fluids", Q_NULLPTR));
        groupBox_22->setTitle(QApplication::translate("MainWindow", "Misc.", Q_NULLPTR));
        depPostProcessCheckBox->setText(QApplication::translate("MainWindow", "Post-processing", Q_NULLPTR));
        depRPermCheckBox->setText(QApplication::translate("MainWindow", "R. Permeabilities", Q_NULLPTR));
        depGravityCheckBox->setText(QApplication::translate("MainWindow", "Gravity ON", Q_NULLPTR));
        depGLineEdit->setText(QApplication::translate("MainWindow", "9.8", Q_NULLPTR));
        depViscousForcesCheckBox->setText(QApplication::translate("MainWindow", "Viscous Forces ON", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depPinViscousLineEdit->setToolTip(QApplication::translate("MainWindow", "Pa", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depPinViscousLineEdit->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        depPoutViscousLineEdit->setToolTip(QApplication::translate("MainWindow", "Pa", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        depPoutViscousLineEdit->setText(QApplication::translate("MainWindow", "-10000", Q_NULLPTR));
        tabWidget_3->setTabText(tabWidget_3->indexOf(misc), QApplication::translate("MainWindow", "Misc", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Graphs", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_17), QApplication::translate("MainWindow", "3D Window", Q_NULLPTR));
        label_71->setText(QApplication::translate("MainWindow", "Title", Q_NULLPTR));
        label_37->setText(QApplication::translate("MainWindow", "X-Axis Title", Q_NULLPTR));
        yAxisTitle22->setText(QApplication::translate("MainWindow", "Y-Axis Title", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "Update", Q_NULLPTR));
        twoPhaseSaveCurveButton->setText(QApplication::translate("MainWindow", "Save Plot", Q_NULLPTR));
        label_33->setText(QApplication::translate("MainWindow", "Min X", Q_NULLPTR));
        minXAxis->setText(QString());
        label_34->setText(QApplication::translate("MainWindow", "Max X", Q_NULLPTR));
        maxXAxis->setText(QString());
        label_31->setText(QApplication::translate("MainWindow", "Min Y", Q_NULLPTR));
        label_32->setText(QApplication::translate("MainWindow", "Max Y", Q_NULLPTR));
        label_36->setText(QApplication::translate("MainWindow", "Tick Step", Q_NULLPTR));
        tickStep->setText(QString());
        pushButton_2->setText(QApplication::translate("MainWindow", "Add Plot", Q_NULLPTR));
        realTimeCheckBox->setText(QApplication::translate("MainWindow", "Real Time", Q_NULLPTR));
        plot->setText(QApplication::translate("MainWindow", "Plot", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "Clear Plots", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_18), QApplication::translate("MainWindow", "Curves", Q_NULLPTR));
        visiblePhases->setTitle(QApplication::translate("MainWindow", "Visible Pores", Q_NULLPTR));
        visibleOilCheckBox->setText(QApplication::translate("MainWindow", "Oil", Q_NULLPTR));
        visibleWaterCheckBox->setText(QApplication::translate("MainWindow", "Water", Q_NULLPTR));
        visibleGasCheckBox->setText(QApplication::translate("MainWindow", "Gas", Q_NULLPTR));
        visibleOilWetCheckBox->setText(QApplication::translate("MainWindow", "OW", Q_NULLPTR));
        visibleWaterWetCheckBox->setText(QApplication::translate("MainWindow", "WW", Q_NULLPTR));
        visiblePhases_2->setTitle(QApplication::translate("MainWindow", "View Angle", Q_NULLPTR));
        resetRadioButton->setText(QApplication::translate("MainWindow", "Reset &View", Q_NULLPTR));
        rotateCheckBox->setText(QApplication::translate("MainWindow", "Rotate", Q_NULLPTR));
        visiblePhases_4->setTitle(QApplication::translate("MainWindow", "Slicing", Q_NULLPTR));
        xCutCheckBox->setText(QString());
        yCutCheckBox->setText(QString());
        zCutCheckBox->setText(QString());
#ifndef QT_NO_TOOLTIP
        xCutSpinBox->setToolTip(QApplication::translate("MainWindow", "fraction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        xCutSpinBox->setText(QApplication::translate("MainWindow", "0.5", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        yCutSpinBox->setToolTip(QApplication::translate("MainWindow", "fraction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        yCutSpinBox->setText(QApplication::translate("MainWindow", "0.5", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        zCutSpinBox->setToolTip(QApplication::translate("MainWindow", "fraction", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        zCutSpinBox->setText(QApplication::translate("MainWindow", "0.5", Q_NULLPTR));
        label_52->setText(QApplication::translate("MainWindow", "X", Q_NULLPTR));
        label_53->setText(QApplication::translate("MainWindow", "Y", Q_NULLPTR));
        label_54->setText(QApplication::translate("MainWindow", "Z", Q_NULLPTR));
        visiblePhases_3->setTitle(QApplication::translate("MainWindow", "Enable 3D", Q_NULLPTR));
        pore3DCheckBox->setText(QApplication::translate("MainWindow", "3D Pores", Q_NULLPTR));
        node3DCheckBox->setText(QApplication::translate("MainWindow", "3D Nodes", Q_NULLPTR));
        lightCheckBox->setText(QApplication::translate("MainWindow", "Light", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
