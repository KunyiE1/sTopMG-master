/********************************************************************************
** Form generated from reading UI file 'toppicwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPPICWINDOW_H
#define UI_TOPPICWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ToppicWindow
{
public:
    QWidget *centralWidget;
    QTextBrowser *outputTextBrowser;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *exitButton;
    QPushButton *clearButton;
    QPushButton *defaultButton;
    QPushButton *outputButton;
    QPushButton *startButton;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QListWidget *listWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *databaseFileEdit;
    QPushButton *databaseFileButton;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QPushButton *addButton;
    QPushButton *delButton;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_7;
    QLineEdit *combinedOutputEdit;
    QWidget *tab;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QComboBox *fixedModComboBox;
    QLineEdit *fixedModFileEdit;
    QPushButton *fixedModFileButton;
    QWidget *horizontalLayoutWidget_21;
    QHBoxLayout *horizontalLayout_21;
    QCheckBox *decoyCheckBox;
    QWidget *horizontalLayoutWidget_11;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_9;
    QLineEdit *errorToleranceEdit;
    QWidget *horizontalLayoutWidget_13;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_14;
    QLineEdit *formErrorToleranceEdit;
    QWidget *horizontalLayoutWidget_22;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_23;
    QLineEdit *threadNumberEdit;
    QWidget *horizontalLayoutWidget_14;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_15;
    QLineEdit *varPtmNumEdit;
    QWidget *horizontalLayoutWidget_19;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_20;
    QLineEdit *varPtmFileEdit;
    QPushButton *varPtmFileButton;
    QWidget *horizontalLayoutWidget_25;
    QHBoxLayout *horizontalLayout_25;
    QCheckBox *approxSpectraCheckBox;
    QWidget *horizontalLayoutWidget_26;
    QHBoxLayout *horizontalLayout_26;
    QCheckBox *topfdFeatureCheckBox;
    QGroupBox *groupBox_3;
    QLabel *label_11;
    QLabel *label_13;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QComboBox *cutoffSpectralTypeComboBox;
    QLineEdit *cutoffSpectralValueEdit;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QComboBox *cutoffProteoformTypeComboBox;
    QLineEdit *cutoffProteoformValueEdit;
    QWidget *tab_2;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *activationComboBox;
    QWidget *horizontalLayoutWidget_15;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_16;
    QLineEdit *numCombinedEdit;
    QWidget *horizontalLayoutWidget_10;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QLineEdit *maxModEdit;
    QWidget *horizontalLayoutWidget_9;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_12;
    QComboBox *numModComboBox;
    QGroupBox *groupBox_2;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QCheckBox *NONECheckBox;
    QCheckBox *NMECheckBox;
    QCheckBox *NMEACCheckBox;
    QCheckBox *MACCheckBox;
    QGroupBox *groupBox_4;
    QWidget *horizontalLayoutWidget_17;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_5;
    QLineEdit *modFileEdit;
    QPushButton *modFileButton;
    QWidget *horizontalLayoutWidget_18;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_6;
    QLineEdit *miscoreThresholdEdit;
    QWidget *horizontalLayoutWidget_12;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_10;
    QLineEdit *minModEdit;
    QWidget *horizontalLayoutWidget_24;
    QHBoxLayout *horizontalLayout_24;
    QCheckBox *keepDecoyCheckBox;
    QWidget *horizontalLayoutWidget_23;
    QHBoxLayout *horizontalLayout_23;
    QCheckBox *geneHTMLCheckBox;
    QWidget *horizontalLayoutWidget_27;
    QHBoxLayout *horizontalLayout_28;
    QCheckBox *lookupTableCheckBox;
    QWidget *horizontalLayoutWidget_28;
    QHBoxLayout *horizontalLayout_29;
    QCheckBox *keepTempCheckBox;

    void setupUi(QMainWindow *ToppicWindow)
    {
        if (ToppicWindow->objectName().isEmpty())
            ToppicWindow->setObjectName(QString::fromUtf8("ToppicWindow"));
        ToppicWindow->resize(920, 700);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(ToppicWindow->sizePolicy().hasHeightForWidth());
        ToppicWindow->setSizePolicy(sizePolicy);
        ToppicWindow->setMinimumSize(QSize(920, 700));
        ToppicWindow->setMaximumSize(QSize(920, 700));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/toppic.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ToppicWindow->setWindowIcon(icon);
        centralWidget = new QWidget(ToppicWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        outputTextBrowser = new QTextBrowser(centralWidget);
        outputTextBrowser->setObjectName(QString::fromUtf8("outputTextBrowser"));
        outputTextBrowser->setGeometry(QRect(20, 410, 881, 241));
        horizontalLayoutWidget_7 = new QWidget(centralWidget);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(20, 650, 881, 55));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_7->setSpacing(30);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        exitButton = new QPushButton(horizontalLayoutWidget_7);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));

        horizontalLayout_7->addWidget(exitButton);

        clearButton = new QPushButton(horizontalLayoutWidget_7);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));

        horizontalLayout_7->addWidget(clearButton);

        defaultButton = new QPushButton(horizontalLayoutWidget_7);
        defaultButton->setObjectName(QString::fromUtf8("defaultButton"));

        horizontalLayout_7->addWidget(defaultButton);

        outputButton = new QPushButton(horizontalLayoutWidget_7);
        outputButton->setObjectName(QString::fromUtf8("outputButton"));
        outputButton->setEnabled(false);

        horizontalLayout_7->addWidget(outputButton);

        startButton = new QPushButton(horizontalLayoutWidget_7);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setEnabled(true);
        startButton->setAcceptDrops(false);
        startButton->setCheckable(false);
        startButton->setAutoDefault(true);
        startButton->setFlat(false);

        horizontalLayout_7->addWidget(startButton);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 10, 881, 391));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        listWidget = new QListWidget(tab_4);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 90, 851, 221));
        horizontalLayoutWidget = new QWidget(tab_4);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 851, 33));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(115, 0));

        horizontalLayout->addWidget(label);

        databaseFileEdit = new QLineEdit(horizontalLayoutWidget);
        databaseFileEdit->setObjectName(QString::fromUtf8("databaseFileEdit"));
        databaseFileEdit->setEnabled(true);
        databaseFileEdit->setMaximumSize(QSize(650, 16777215));
        databaseFileEdit->setReadOnly(true);

        horizontalLayout->addWidget(databaseFileEdit);

        databaseFileButton = new QPushButton(horizontalLayoutWidget);
        databaseFileButton->setObjectName(QString::fromUtf8("databaseFileButton"));
        databaseFileButton->setMaximumSize(QSize(85, 16777215));

        horizontalLayout->addWidget(databaseFileButton);

        horizontalLayoutWidget_2 = new QWidget(tab_4);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 50, 351, 33));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(115, 0));

        horizontalLayout_2->addWidget(label_2);

        addButton = new QPushButton(horizontalLayoutWidget_2);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        horizontalLayout_2->addWidget(addButton);

        delButton = new QPushButton(horizontalLayoutWidget_2);
        delButton->setObjectName(QString::fromUtf8("delButton"));

        horizontalLayout_2->addWidget(delButton);

        horizontalLayoutWidget_6 = new QWidget(tab_4);
        horizontalLayoutWidget_6->setObjectName(QString::fromUtf8("horizontalLayoutWidget_6"));
        horizontalLayoutWidget_6->setGeometry(QRect(10, 320, 851, 31));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_6);
        horizontalLayout_6->setSpacing(30);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(horizontalLayoutWidget_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_6->addWidget(label_7);

        combinedOutputEdit = new QLineEdit(horizontalLayoutWidget_6);
        combinedOutputEdit->setObjectName(QString::fromUtf8("combinedOutputEdit"));
        combinedOutputEdit->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(combinedOutputEdit->sizePolicy().hasHeightForWidth());
        combinedOutputEdit->setSizePolicy(sizePolicy1);
        combinedOutputEdit->setMaximumSize(QSize(4000000, 25));
        combinedOutputEdit->setReadOnly(false);

        horizontalLayout_6->addWidget(combinedOutputEdit);

        tabWidget->addTab(tab_4, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayoutWidget_4 = new QWidget(tab);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(10, 10, 861, 33));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(horizontalLayoutWidget_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(160, 0));

        horizontalLayout_4->addWidget(label_4);

        fixedModComboBox = new QComboBox(horizontalLayoutWidget_4);
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->setObjectName(QString::fromUtf8("fixedModComboBox"));
        fixedModComboBox->setMinimumSize(QSize(270, 0));

        horizontalLayout_4->addWidget(fixedModComboBox);

        fixedModFileEdit = new QLineEdit(horizontalLayoutWidget_4);
        fixedModFileEdit->setObjectName(QString::fromUtf8("fixedModFileEdit"));
        fixedModFileEdit->setEnabled(false);
        fixedModFileEdit->setReadOnly(true);

        horizontalLayout_4->addWidget(fixedModFileEdit);

        fixedModFileButton = new QPushButton(horizontalLayoutWidget_4);
        fixedModFileButton->setObjectName(QString::fromUtf8("fixedModFileButton"));
        fixedModFileButton->setEnabled(false);
        fixedModFileButton->setMaximumSize(QSize(85, 16777215));

        horizontalLayout_4->addWidget(fixedModFileButton);

        horizontalLayoutWidget_21 = new QWidget(tab);
        horizontalLayoutWidget_21->setObjectName(QString::fromUtf8("horizontalLayoutWidget_21"));
        horizontalLayoutWidget_21->setGeometry(QRect(10, 130, 261, 31));
        horizontalLayout_21 = new QHBoxLayout(horizontalLayoutWidget_21);
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        horizontalLayout_21->setContentsMargins(0, 0, 0, 0);
        decoyCheckBox = new QCheckBox(horizontalLayoutWidget_21);
        decoyCheckBox->setObjectName(QString::fromUtf8("decoyCheckBox"));
        decoyCheckBox->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_21->addWidget(decoyCheckBox);

        horizontalLayoutWidget_11 = new QWidget(tab);
        horizontalLayoutWidget_11->setObjectName(QString::fromUtf8("horizontalLayoutWidget_11"));
        horizontalLayoutWidget_11->setGeometry(QRect(10, 170, 261, 31));
        horizontalLayout_11 = new QHBoxLayout(horizontalLayoutWidget_11);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(horizontalLayoutWidget_11);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(160, 0));

        horizontalLayout_11->addWidget(label_9);

        errorToleranceEdit = new QLineEdit(horizontalLayoutWidget_11);
        errorToleranceEdit->setObjectName(QString::fromUtf8("errorToleranceEdit"));

        horizontalLayout_11->addWidget(errorToleranceEdit);

        horizontalLayoutWidget_13 = new QWidget(tab);
        horizontalLayoutWidget_13->setObjectName(QString::fromUtf8("horizontalLayoutWidget_13"));
        horizontalLayoutWidget_13->setGeometry(QRect(480, 170, 294, 31));
        horizontalLayout_13 = new QHBoxLayout(horizontalLayoutWidget_13);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        label_14 = new QLabel(horizontalLayoutWidget_13);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_13->addWidget(label_14);

        formErrorToleranceEdit = new QLineEdit(horizontalLayoutWidget_13);
        formErrorToleranceEdit->setObjectName(QString::fromUtf8("formErrorToleranceEdit"));

        horizontalLayout_13->addWidget(formErrorToleranceEdit);

        horizontalLayoutWidget_22 = new QWidget(tab);
        horizontalLayoutWidget_22->setObjectName(QString::fromUtf8("horizontalLayoutWidget_22"));
        horizontalLayoutWidget_22->setGeometry(QRect(10, 210, 261, 33));
        horizontalLayout_22 = new QHBoxLayout(horizontalLayoutWidget_22);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        horizontalLayout_22->setContentsMargins(0, 0, 0, 0);
        label_23 = new QLabel(horizontalLayoutWidget_22);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setMinimumSize(QSize(160, 0));

        horizontalLayout_22->addWidget(label_23);

        threadNumberEdit = new QLineEdit(horizontalLayoutWidget_22);
        threadNumberEdit->setObjectName(QString::fromUtf8("threadNumberEdit"));
        threadNumberEdit->setMaximumSize(QSize(110, 16777215));
        threadNumberEdit->setReadOnly(false);

        horizontalLayout_22->addWidget(threadNumberEdit);

        horizontalLayoutWidget_14 = new QWidget(tab);
        horizontalLayoutWidget_14->setObjectName(QString::fromUtf8("horizontalLayoutWidget_14"));
        horizontalLayoutWidget_14->setGeometry(QRect(10, 90, 261, 31));
        horizontalLayout_14 = new QHBoxLayout(horizontalLayoutWidget_14);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        label_15 = new QLabel(horizontalLayoutWidget_14);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(160, 0));

        horizontalLayout_14->addWidget(label_15);

        varPtmNumEdit = new QLineEdit(horizontalLayoutWidget_14);
        varPtmNumEdit->setObjectName(QString::fromUtf8("varPtmNumEdit"));

        horizontalLayout_14->addWidget(varPtmNumEdit);

        horizontalLayoutWidget_19 = new QWidget(tab);
        horizontalLayoutWidget_19->setObjectName(QString::fromUtf8("horizontalLayoutWidget_19"));
        horizontalLayoutWidget_19->setGeometry(QRect(10, 50, 861, 33));
        horizontalLayout_19 = new QHBoxLayout(horizontalLayoutWidget_19);
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        horizontalLayout_19->setContentsMargins(0, 0, 0, 0);
        label_20 = new QLabel(horizontalLayoutWidget_19);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setMinimumSize(QSize(160, 0));

        horizontalLayout_19->addWidget(label_20);

        varPtmFileEdit = new QLineEdit(horizontalLayoutWidget_19);
        varPtmFileEdit->setObjectName(QString::fromUtf8("varPtmFileEdit"));
        varPtmFileEdit->setEnabled(true);
        varPtmFileEdit->setReadOnly(true);

        horizontalLayout_19->addWidget(varPtmFileEdit);

        varPtmFileButton = new QPushButton(horizontalLayoutWidget_19);
        varPtmFileButton->setObjectName(QString::fromUtf8("varPtmFileButton"));
        varPtmFileButton->setEnabled(true);

        horizontalLayout_19->addWidget(varPtmFileButton);

        horizontalLayoutWidget_25 = new QWidget(tab);
        horizontalLayoutWidget_25->setObjectName(QString::fromUtf8("horizontalLayoutWidget_25"));
        horizontalLayoutWidget_25->setGeometry(QRect(480, 90, 391, 33));
        horizontalLayout_25 = new QHBoxLayout(horizontalLayoutWidget_25);
        horizontalLayout_25->setSpacing(6);
        horizontalLayout_25->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        horizontalLayout_25->setContentsMargins(0, 0, 0, 0);
        approxSpectraCheckBox = new QCheckBox(horizontalLayoutWidget_25);
        approxSpectraCheckBox->setObjectName(QString::fromUtf8("approxSpectraCheckBox"));
        approxSpectraCheckBox->setMinimumSize(QSize(230, 0));

        horizontalLayout_25->addWidget(approxSpectraCheckBox);

        horizontalLayoutWidget_26 = new QWidget(tab);
        horizontalLayoutWidget_26->setObjectName(QString::fromUtf8("horizontalLayoutWidget_26"));
        horizontalLayoutWidget_26->setGeometry(QRect(480, 130, 391, 31));
        horizontalLayout_26 = new QHBoxLayout(horizontalLayoutWidget_26);
        horizontalLayout_26->setSpacing(6);
        horizontalLayout_26->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));
        horizontalLayout_26->setContentsMargins(0, 0, 0, 0);
        topfdFeatureCheckBox = new QCheckBox(horizontalLayoutWidget_26);
        topfdFeatureCheckBox->setObjectName(QString::fromUtf8("topfdFeatureCheckBox"));
        topfdFeatureCheckBox->setMinimumSize(QSize(230, 0));

        horizontalLayout_26->addWidget(topfdFeatureCheckBox);

        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 250, 861, 111));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(20, 30, 151, 31));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(470, 30, 171, 31));
        verticalLayoutWidget = new QWidget(groupBox_3);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(140, 30, 171, 71));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        cutoffSpectralTypeComboBox = new QComboBox(verticalLayoutWidget);
        cutoffSpectralTypeComboBox->addItem(QString());
        cutoffSpectralTypeComboBox->addItem(QString());
        cutoffSpectralTypeComboBox->setObjectName(QString::fromUtf8("cutoffSpectralTypeComboBox"));
        cutoffSpectralTypeComboBox->setEditable(false);

        verticalLayout->addWidget(cutoffSpectralTypeComboBox);

        cutoffSpectralValueEdit = new QLineEdit(verticalLayoutWidget);
        cutoffSpectralValueEdit->setObjectName(QString::fromUtf8("cutoffSpectralValueEdit"));
        cutoffSpectralValueEdit->setReadOnly(false);

        verticalLayout->addWidget(cutoffSpectralValueEdit);

        layoutWidget = new QWidget(groupBox_3);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(610, 30, 171, 71));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        cutoffProteoformTypeComboBox = new QComboBox(layoutWidget);
        cutoffProteoformTypeComboBox->addItem(QString());
        cutoffProteoformTypeComboBox->addItem(QString());
        cutoffProteoformTypeComboBox->setObjectName(QString::fromUtf8("cutoffProteoformTypeComboBox"));
        cutoffProteoformTypeComboBox->setEditable(false);

        verticalLayout_2->addWidget(cutoffProteoformTypeComboBox);

        cutoffProteoformValueEdit = new QLineEdit(layoutWidget);
        cutoffProteoformValueEdit->setObjectName(QString::fromUtf8("cutoffProteoformValueEdit"));
        cutoffProteoformValueEdit->setReadOnly(false);

        verticalLayout_2->addWidget(cutoffProteoformValueEdit);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayoutWidget_3 = new QWidget(tab_2);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 50, 341, 33));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        activationComboBox = new QComboBox(horizontalLayoutWidget_3);
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->setObjectName(QString::fromUtf8("activationComboBox"));
        activationComboBox->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_3->addWidget(activationComboBox);

        horizontalLayoutWidget_15 = new QWidget(tab_2);
        horizontalLayoutWidget_15->setObjectName(QString::fromUtf8("horizontalLayoutWidget_15"));
        horizontalLayoutWidget_15->setGeometry(QRect(500, 90, 361, 33));
        horizontalLayout_15 = new QHBoxLayout(horizontalLayoutWidget_15);
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(0, 0, 0, 0);
        label_16 = new QLabel(horizontalLayoutWidget_15);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_15->addWidget(label_16);

        numCombinedEdit = new QLineEdit(horizontalLayoutWidget_15);
        numCombinedEdit->setObjectName(QString::fromUtf8("numCombinedEdit"));
        numCombinedEdit->setMaximumSize(QSize(110, 16777215));
        numCombinedEdit->setReadOnly(false);

        horizontalLayout_15->addWidget(numCombinedEdit);

        horizontalLayoutWidget_10 = new QWidget(tab_2);
        horizontalLayoutWidget_10->setObjectName(QString::fromUtf8("horizontalLayoutWidget_10"));
        horizontalLayoutWidget_10->setGeometry(QRect(500, 50, 361, 33));
        horizontalLayout_10 = new QHBoxLayout(horizontalLayoutWidget_10);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(horizontalLayoutWidget_10);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_10->addWidget(label_8);

        maxModEdit = new QLineEdit(horizontalLayoutWidget_10);
        maxModEdit->setObjectName(QString::fromUtf8("maxModEdit"));
        maxModEdit->setMaximumSize(QSize(110, 16777215));
        maxModEdit->setReadOnly(false);

        horizontalLayout_10->addWidget(maxModEdit);

        horizontalLayoutWidget_9 = new QWidget(tab_2);
        horizontalLayoutWidget_9->setObjectName(QString::fromUtf8("horizontalLayoutWidget_9"));
        horizontalLayoutWidget_9->setGeometry(QRect(10, 10, 341, 33));
        horizontalLayout_9 = new QHBoxLayout(horizontalLayoutWidget_9);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_12 = new QLabel(horizontalLayoutWidget_9);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_9->addWidget(label_12);

        numModComboBox = new QComboBox(horizontalLayoutWidget_9);
        numModComboBox->addItem(QString());
        numModComboBox->addItem(QString());
        numModComboBox->addItem(QString());
        numModComboBox->setObjectName(QString::fromUtf8("numModComboBox"));
        numModComboBox->setMaximumSize(QSize(110, 16777215));
        numModComboBox->setEditable(false);

        horizontalLayout_9->addWidget(numModComboBox);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 90, 341, 91));
        gridLayoutWidget = new QWidget(groupBox_2);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(9, 17, 321, 70));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        NONECheckBox = new QCheckBox(gridLayoutWidget);
        NONECheckBox->setObjectName(QString::fromUtf8("NONECheckBox"));
        NONECheckBox->setEnabled(true);
        NONECheckBox->setChecked(true);
        NONECheckBox->setAutoRepeat(false);

        gridLayout->addWidget(NONECheckBox, 0, 0, 1, 1);

        NMECheckBox = new QCheckBox(gridLayoutWidget);
        NMECheckBox->setObjectName(QString::fromUtf8("NMECheckBox"));
        NMECheckBox->setChecked(true);

        gridLayout->addWidget(NMECheckBox, 1, 0, 1, 1);

        NMEACCheckBox = new QCheckBox(gridLayoutWidget);
        NMEACCheckBox->setObjectName(QString::fromUtf8("NMEACCheckBox"));
        NMEACCheckBox->setChecked(true);

        gridLayout->addWidget(NMEACCheckBox, 0, 1, 1, 1);

        MACCheckBox = new QCheckBox(gridLayoutWidget);
        MACCheckBox->setObjectName(QString::fromUtf8("MACCheckBox"));
        MACCheckBox->setChecked(true);

        gridLayout->addWidget(MACCheckBox, 1, 1, 1, 1);

        groupBox_4 = new QGroupBox(tab_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 250, 851, 91));
        horizontalLayoutWidget_17 = new QWidget(groupBox_4);
        horizontalLayoutWidget_17->setObjectName(QString::fromUtf8("horizontalLayoutWidget_17"));
        horizontalLayoutWidget_17->setGeometry(QRect(10, 20, 831, 33));
        horizontalLayout_17 = new QHBoxLayout(horizontalLayoutWidget_17);
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        horizontalLayout_17->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(horizontalLayoutWidget_17);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(200, 0));

        horizontalLayout_17->addWidget(label_5);

        modFileEdit = new QLineEdit(horizontalLayoutWidget_17);
        modFileEdit->setObjectName(QString::fromUtf8("modFileEdit"));
        modFileEdit->setEnabled(true);
        modFileEdit->setReadOnly(true);

        horizontalLayout_17->addWidget(modFileEdit);

        modFileButton = new QPushButton(horizontalLayoutWidget_17);
        modFileButton->setObjectName(QString::fromUtf8("modFileButton"));
        modFileButton->setEnabled(true);

        horizontalLayout_17->addWidget(modFileButton);

        horizontalLayoutWidget_18 = new QWidget(groupBox_4);
        horizontalLayoutWidget_18->setObjectName(QString::fromUtf8("horizontalLayoutWidget_18"));
        horizontalLayoutWidget_18->setGeometry(QRect(10, 60, 331, 33));
        horizontalLayout_18 = new QHBoxLayout(horizontalLayoutWidget_18);
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        horizontalLayout_18->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(horizontalLayoutWidget_18);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(200, 0));

        horizontalLayout_18->addWidget(label_6);

        miscoreThresholdEdit = new QLineEdit(horizontalLayoutWidget_18);
        miscoreThresholdEdit->setObjectName(QString::fromUtf8("miscoreThresholdEdit"));
        miscoreThresholdEdit->setReadOnly(false);

        horizontalLayout_18->addWidget(miscoreThresholdEdit);

        horizontalLayoutWidget_12 = new QWidget(tab_2);
        horizontalLayoutWidget_12->setObjectName(QString::fromUtf8("horizontalLayoutWidget_12"));
        horizontalLayoutWidget_12->setGeometry(QRect(500, 10, 361, 33));
        horizontalLayout_12 = new QHBoxLayout(horizontalLayoutWidget_12);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(horizontalLayoutWidget_12);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_12->addWidget(label_10);

        minModEdit = new QLineEdit(horizontalLayoutWidget_12);
        minModEdit->setObjectName(QString::fromUtf8("minModEdit"));
        minModEdit->setMaximumSize(QSize(110, 16777215));
        minModEdit->setReadOnly(false);

        horizontalLayout_12->addWidget(minModEdit);

        horizontalLayoutWidget_24 = new QWidget(tab_2);
        horizontalLayoutWidget_24->setObjectName(QString::fromUtf8("horizontalLayoutWidget_24"));
        horizontalLayoutWidget_24->setGeometry(QRect(10, 210, 341, 33));
        horizontalLayout_24 = new QHBoxLayout(horizontalLayoutWidget_24);
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        horizontalLayout_24->setContentsMargins(0, 0, 0, 0);
        keepDecoyCheckBox = new QCheckBox(horizontalLayoutWidget_24);
        keepDecoyCheckBox->setObjectName(QString::fromUtf8("keepDecoyCheckBox"));
        keepDecoyCheckBox->setMinimumSize(QSize(120, 0));

        horizontalLayout_24->addWidget(keepDecoyCheckBox);

        horizontalLayoutWidget_23 = new QWidget(tab_2);
        horizontalLayoutWidget_23->setObjectName(QString::fromUtf8("horizontalLayoutWidget_23"));
        horizontalLayoutWidget_23->setGeometry(QRect(10, 180, 341, 31));
        horizontalLayout_23 = new QHBoxLayout(horizontalLayoutWidget_23);
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        horizontalLayout_23->setContentsMargins(0, 0, 0, 0);
        geneHTMLCheckBox = new QCheckBox(horizontalLayoutWidget_23);
        geneHTMLCheckBox->setObjectName(QString::fromUtf8("geneHTMLCheckBox"));
        geneHTMLCheckBox->setMinimumSize(QSize(120, 0));
        geneHTMLCheckBox->setChecked(true);

        horizontalLayout_23->addWidget(geneHTMLCheckBox);

        horizontalLayoutWidget_27 = new QWidget(tab_2);
        horizontalLayoutWidget_27->setObjectName(QString::fromUtf8("horizontalLayoutWidget_27"));
        horizontalLayoutWidget_27->setGeometry(QRect(500, 180, 361, 31));
        horizontalLayout_28 = new QHBoxLayout(horizontalLayoutWidget_27);
        horizontalLayout_28->setSpacing(6);
        horizontalLayout_28->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_28->setObjectName(QString::fromUtf8("horizontalLayout_28"));
        horizontalLayout_28->setContentsMargins(0, 0, 0, 0);
        lookupTableCheckBox = new QCheckBox(horizontalLayoutWidget_27);
        lookupTableCheckBox->setObjectName(QString::fromUtf8("lookupTableCheckBox"));

        horizontalLayout_28->addWidget(lookupTableCheckBox);

        horizontalLayoutWidget_28 = new QWidget(tab_2);
        horizontalLayoutWidget_28->setObjectName(QString::fromUtf8("horizontalLayoutWidget_28"));
        horizontalLayoutWidget_28->setGeometry(QRect(500, 210, 361, 33));
        horizontalLayout_29 = new QHBoxLayout(horizontalLayoutWidget_28);
        horizontalLayout_29->setSpacing(6);
        horizontalLayout_29->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_29->setObjectName(QString::fromUtf8("horizontalLayout_29"));
        horizontalLayout_29->setContentsMargins(0, 0, 0, 0);
        keepTempCheckBox = new QCheckBox(horizontalLayoutWidget_28);
        keepTempCheckBox->setObjectName(QString::fromUtf8("keepTempCheckBox"));

        horizontalLayout_29->addWidget(keepTempCheckBox);

        tabWidget->addTab(tab_2, QString());
        ToppicWindow->setCentralWidget(centralWidget);
        tabWidget->raise();
        outputTextBrowser->raise();
        horizontalLayoutWidget_7->raise();

        retranslateUi(ToppicWindow);

        outputButton->setDefault(true);
        startButton->setDefault(true);
        tabWidget->setCurrentIndex(2);
        numModComboBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ToppicWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ToppicWindow)
    {
        ToppicWindow->setWindowTitle(QCoreApplication::translate("ToppicWindow", "TopPIC", nullptr));
        outputTextBrowser->setHtml(QCoreApplication::translate("ToppicWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Cantarell';\">Click the Start button to process the spectrum file.</span></p></body></html>", nullptr));
        exitButton->setText(QCoreApplication::translate("ToppicWindow", "Exit", nullptr));
        clearButton->setText(QCoreApplication::translate("ToppicWindow", "Clear Input Files", nullptr));
        defaultButton->setText(QCoreApplication::translate("ToppicWindow", "Default Parameters", nullptr));
        outputButton->setText(QCoreApplication::translate("ToppicWindow", "Output Folder", nullptr));
        startButton->setText(QCoreApplication::translate("ToppicWindow", "Start", nullptr));
        label->setText(QCoreApplication::translate("ToppicWindow", "Database file: ", nullptr));
        databaseFileButton->setText(QCoreApplication::translate("ToppicWindow", "Select", nullptr));
        label_2->setText(QCoreApplication::translate("ToppicWindow", "Spectrum files: ", nullptr));
        addButton->setText(QCoreApplication::translate("ToppicWindow", "Add", nullptr));
        delButton->setText(QCoreApplication::translate("ToppicWindow", "Remove", nullptr));
        label_7->setText(QCoreApplication::translate("ToppicWindow", "Combined output file:", nullptr));
        combinedOutputEdit->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("ToppicWindow", "Files", nullptr));
        label_4->setText(QCoreApplication::translate("ToppicWindow", "Fixed modifications:", nullptr));
        fixedModComboBox->setItemText(0, QCoreApplication::translate("ToppicWindow", "NONE", nullptr));
        fixedModComboBox->setItemText(1, QCoreApplication::translate("ToppicWindow", "Carbamidomethylation on cysteine", nullptr));
        fixedModComboBox->setItemText(2, QCoreApplication::translate("ToppicWindow", "Carboxymethylation on cysteine", nullptr));
        fixedModComboBox->setItemText(3, QCoreApplication::translate("ToppicWindow", "A fixed modification file", nullptr));

        fixedModFileButton->setText(QCoreApplication::translate("ToppicWindow", "File", nullptr));
        decoyCheckBox->setText(QCoreApplication::translate("ToppicWindow", "Decoy database", nullptr));
        label_9->setText(QCoreApplication::translate("ToppicWindow", "Mass error tolerance (PPM):", nullptr));
        errorToleranceEdit->setText(QCoreApplication::translate("ToppicWindow", "10", nullptr));
        label_14->setText(QCoreApplication::translate("ToppicWindow", "PrSM cluster error tolerance (Da):", nullptr));
        formErrorToleranceEdit->setText(QCoreApplication::translate("ToppicWindow", "1.2", nullptr));
        label_23->setText(QCoreApplication::translate("ToppicWindow", "Thread number:", nullptr));
        threadNumberEdit->setText(QCoreApplication::translate("ToppicWindow", "1", nullptr));
        label_15->setText(QCoreApplication::translate("ToppicWindow", "Max variable PTM number: ", nullptr));
        varPtmNumEdit->setText(QCoreApplication::translate("ToppicWindow", "3", nullptr));
        label_20->setText(QCoreApplication::translate("ToppicWindow", "Variable PTM file:", nullptr));
        varPtmFileButton->setText(QCoreApplication::translate("ToppicWindow", "File", nullptr));
        approxSpectraCheckBox->setText(QCoreApplication::translate("ToppicWindow", "Use approximate spectra in protein filtering", nullptr));
        topfdFeatureCheckBox->setText(QCoreApplication::translate("ToppicWindow", "Missing MS1 feature file", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("ToppicWindow", "Cutoff settings", nullptr));
        label_11->setText(QCoreApplication::translate("ToppicWindow", "Spectrum level:", nullptr));
        label_13->setText(QCoreApplication::translate("ToppicWindow", "Proteoform level:", nullptr));
        cutoffSpectralTypeComboBox->setItemText(0, QCoreApplication::translate("ToppicWindow", "EVALUE", nullptr));
        cutoffSpectralTypeComboBox->setItemText(1, QCoreApplication::translate("ToppicWindow", "FDR", nullptr));

        cutoffSpectralValueEdit->setText(QCoreApplication::translate("ToppicWindow", "0.01", nullptr));
        cutoffProteoformTypeComboBox->setItemText(0, QCoreApplication::translate("ToppicWindow", "EVALUE", nullptr));
        cutoffProteoformTypeComboBox->setItemText(1, QCoreApplication::translate("ToppicWindow", "FDR", nullptr));

        cutoffProteoformValueEdit->setText(QCoreApplication::translate("ToppicWindow", "0.01", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("ToppicWindow", "Basic Parameters", nullptr));
        label_3->setText(QCoreApplication::translate("ToppicWindow", "Fragmentation:", nullptr));
        activationComboBox->setItemText(0, QCoreApplication::translate("ToppicWindow", "FILE", nullptr));
        activationComboBox->setItemText(1, QCoreApplication::translate("ToppicWindow", "CID", nullptr));
        activationComboBox->setItemText(2, QCoreApplication::translate("ToppicWindow", "ETD", nullptr));
        activationComboBox->setItemText(3, QCoreApplication::translate("ToppicWindow", "HCD", nullptr));
        activationComboBox->setItemText(4, QCoreApplication::translate("ToppicWindow", "MPD", nullptr));
        activationComboBox->setItemText(5, QCoreApplication::translate("ToppicWindow", "UVPD", nullptr));

        label_16->setText(QCoreApplication::translate("ToppicWindow", "Nummber of combined spectra:", nullptr));
        numCombinedEdit->setText(QCoreApplication::translate("ToppicWindow", "1", nullptr));
        label_8->setText(QCoreApplication::translate("ToppicWindow", "Maximum mass shift (Da):", nullptr));
        maxModEdit->setText(QCoreApplication::translate("ToppicWindow", "500", nullptr));
        label_12->setText(QCoreApplication::translate("ToppicWindow", "Maximum number of mass shifts:", nullptr));
        numModComboBox->setItemText(0, QCoreApplication::translate("ToppicWindow", "0", nullptr));
        numModComboBox->setItemText(1, QCoreApplication::translate("ToppicWindow", "1", nullptr));
        numModComboBox->setItemText(2, QCoreApplication::translate("ToppicWindow", "2", nullptr));

        groupBox_2->setTitle(QCoreApplication::translate("ToppicWindow", "N-terminal forms", nullptr));
        NONECheckBox->setText(QCoreApplication::translate("ToppicWindow", "NONE", nullptr));
        NMECheckBox->setText(QCoreApplication::translate("ToppicWindow", "NME", nullptr));
        NMEACCheckBox->setText(QCoreApplication::translate("ToppicWindow", "NME_ACETYLATION", nullptr));
        MACCheckBox->setText(QCoreApplication::translate("ToppicWindow", "M_ACETYLATION", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("ToppicWindow", "Modification localization    ", nullptr));
        label_5->setText(QCoreApplication::translate("ToppicWindow", "PTM file for localization:", nullptr));
        modFileButton->setText(QCoreApplication::translate("ToppicWindow", "File", nullptr));
        label_6->setText(QCoreApplication::translate("ToppicWindow", "MIScore threshold:", nullptr));
        miscoreThresholdEdit->setText(QCoreApplication::translate("ToppicWindow", "0.45", nullptr));
        label_10->setText(QCoreApplication::translate("ToppicWindow", "Minimum mass shift (Da):", nullptr));
        minModEdit->setText(QCoreApplication::translate("ToppicWindow", "-500", nullptr));
        keepDecoyCheckBox->setText(QCoreApplication::translate("ToppicWindow", "Keep decoy identifications", nullptr));
        geneHTMLCheckBox->setText(QCoreApplication::translate("ToppicWindow", "Generate HTML files", nullptr));
        lookupTableCheckBox->setText(QCoreApplication::translate("ToppicWindow", "Lookup table for E-value computation", nullptr));
        keepTempCheckBox->setText(QCoreApplication::translate("ToppicWindow", "Keep intermediate files", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("ToppicWindow", "Advanced Parameters", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ToppicWindow: public Ui_ToppicWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPPICWINDOW_H
