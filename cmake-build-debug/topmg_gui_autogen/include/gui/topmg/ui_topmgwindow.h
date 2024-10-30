/********************************************************************************
** Form generated from reading UI file 'topmgwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPMGWINDOW_H
#define UI_TOPMGWINDOW_H

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

class Ui_TopmgWindow
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
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *combinedOutputEdit;
    QWidget *horizontalLayoutWidget_17;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_18;
    QLineEdit *modFileEdit;
    QPushButton *modFileButton;
    QWidget *tab;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QComboBox *fixedModComboBox;
    QLineEdit *fixedModFileEdit;
    QPushButton *fixedModFileButton;
    QWidget *horizontalLayoutWidget_21;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *decoyCheckBox;
    QCheckBox *wholeProteinCheckBox;
    QCheckBox *topfdFeatureCheckBox;
    QWidget *horizontalLayoutWidget_11;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_9;
    QLineEdit *errorToleranceEdit;
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
    QWidget *horizontalLayoutWidget_13;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_10;
    QLineEdit *formErrorToleranceEdit;
    QWidget *horizontalLayoutWidget_24;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *geneHTMLCheckBox;
    QCheckBox *keepDecoyCheckBox;
    QCheckBox *keepTempCheckBox;
    QWidget *horizontalLayoutWidget_22;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_23;
    QLineEdit *threadNumberEdit;
    QWidget *tab_2;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *activationComboBox;
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
    QWidget *horizontalLayoutWidget_12;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_14;
    QComboBox *numUnknownShiftComboBox;
    QWidget *horizontalLayoutWidget_23;
    QHBoxLayout *horizontalLayout_23;
    QCheckBox *asfDiagCheckBox;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_15;
    QLineEdit *maxGapLength;
    QLabel *label_16;
    QLineEdit *maxVarPTMGap;

    void setupUi(QMainWindow *TopmgWindow)
    {
        if (TopmgWindow->objectName().isEmpty())
            TopmgWindow->setObjectName(QString::fromUtf8("TopmgWindow"));
        TopmgWindow->resize(920, 700);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(TopmgWindow->sizePolicy().hasHeightForWidth());
        TopmgWindow->setSizePolicy(sizePolicy);
        TopmgWindow->setMinimumSize(QSize(920, 700));
        TopmgWindow->setMaximumSize(QSize(920, 700));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/toppic.ico"), QSize(), QIcon::Normal, QIcon::Off);
        TopmgWindow->setWindowIcon(icon);
        centralWidget = new QWidget(TopmgWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        outputTextBrowser = new QTextBrowser(centralWidget);
        outputTextBrowser->setObjectName(QString::fromUtf8("outputTextBrowser"));
        outputTextBrowser->setGeometry(QRect(20, 390, 881, 261));
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
        tabWidget->setGeometry(QRect(20, 10, 881, 371));
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        listWidget = new QListWidget(tab_4);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(14, 90, 850, 151));
        horizontalLayoutWidget = new QWidget(tab_4);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 10, 861, 33));
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

        horizontalLayoutWidget_5 = new QWidget(tab_4);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(10, 290, 861, 33));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(horizontalLayoutWidget_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(160, 0));

        horizontalLayout_5->addWidget(label_5);

        combinedOutputEdit = new QLineEdit(horizontalLayoutWidget_5);
        combinedOutputEdit->setObjectName(QString::fromUtf8("combinedOutputEdit"));

        horizontalLayout_5->addWidget(combinedOutputEdit);

        horizontalLayoutWidget_17 = new QWidget(tab_4);
        horizontalLayoutWidget_17->setObjectName(QString::fromUtf8("horizontalLayoutWidget_17"));
        horizontalLayoutWidget_17->setGeometry(QRect(10, 250, 861, 33));
        horizontalLayout_17 = new QHBoxLayout(horizontalLayoutWidget_17);
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        horizontalLayout_17->setContentsMargins(0, 0, 0, 0);
        label_18 = new QLabel(horizontalLayoutWidget_17);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setMinimumSize(QSize(160, 0));

        horizontalLayout_17->addWidget(label_18);

        modFileEdit = new QLineEdit(horizontalLayoutWidget_17);
        modFileEdit->setObjectName(QString::fromUtf8("modFileEdit"));
        modFileEdit->setEnabled(true);
        modFileEdit->setReadOnly(true);

        horizontalLayout_17->addWidget(modFileEdit);

        modFileButton = new QPushButton(horizontalLayoutWidget_17);
        modFileButton->setObjectName(QString::fromUtf8("modFileButton"));
        modFileButton->setEnabled(true);

        horizontalLayout_17->addWidget(modFileButton);

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

        horizontalLayout_4->addWidget(label_4);

        fixedModComboBox = new QComboBox(horizontalLayoutWidget_4);
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->setObjectName(QString::fromUtf8("fixedModComboBox"));

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
        horizontalLayoutWidget_21->setGeometry(QRect(10, 50, 381, 103));
        verticalLayout_3 = new QVBoxLayout(horizontalLayoutWidget_21);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        decoyCheckBox = new QCheckBox(horizontalLayoutWidget_21);
        decoyCheckBox->setObjectName(QString::fromUtf8("decoyCheckBox"));

        verticalLayout_3->addWidget(decoyCheckBox);

        wholeProteinCheckBox = new QCheckBox(horizontalLayoutWidget_21);
        wholeProteinCheckBox->setObjectName(QString::fromUtf8("wholeProteinCheckBox"));

        verticalLayout_3->addWidget(wholeProteinCheckBox);

        topfdFeatureCheckBox = new QCheckBox(horizontalLayoutWidget_21);
        topfdFeatureCheckBox->setObjectName(QString::fromUtf8("topfdFeatureCheckBox"));

        verticalLayout_3->addWidget(topfdFeatureCheckBox);

        horizontalLayoutWidget_11 = new QWidget(tab);
        horizontalLayoutWidget_11->setObjectName(QString::fromUtf8("horizontalLayoutWidget_11"));
        horizontalLayoutWidget_11->setGeometry(QRect(10, 160, 277, 33));
        horizontalLayout_11 = new QHBoxLayout(horizontalLayoutWidget_11);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        label_9 = new QLabel(horizontalLayoutWidget_11);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(210, 0));

        horizontalLayout_11->addWidget(label_9);

        errorToleranceEdit = new QLineEdit(horizontalLayoutWidget_11);
        errorToleranceEdit->setObjectName(QString::fromUtf8("errorToleranceEdit"));

        horizontalLayout_11->addWidget(errorToleranceEdit);

        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 230, 861, 101));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(20, 25, 151, 31));
        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(480, 25, 171, 31));
        verticalLayoutWidget = new QWidget(groupBox_3);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(150, 20, 171, 80));
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
        layoutWidget->setGeometry(QRect(620, 20, 171, 81));
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

        horizontalLayoutWidget_13 = new QWidget(tab);
        horizontalLayoutWidget_13->setObjectName(QString::fromUtf8("horizontalLayoutWidget_13"));
        horizontalLayoutWidget_13->setGeometry(QRect(490, 160, 294, 33));
        horizontalLayout_13 = new QHBoxLayout(horizontalLayoutWidget_13);
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(horizontalLayoutWidget_13);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(0, 0));

        horizontalLayout_13->addWidget(label_10);

        formErrorToleranceEdit = new QLineEdit(horizontalLayoutWidget_13);
        formErrorToleranceEdit->setObjectName(QString::fromUtf8("formErrorToleranceEdit"));

        horizontalLayout_13->addWidget(formErrorToleranceEdit);

        horizontalLayoutWidget_24 = new QWidget(tab);
        horizontalLayoutWidget_24->setObjectName(QString::fromUtf8("horizontalLayoutWidget_24"));
        horizontalLayoutWidget_24->setGeometry(QRect(490, 50, 381, 103));
        verticalLayout_4 = new QVBoxLayout(horizontalLayoutWidget_24);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        geneHTMLCheckBox = new QCheckBox(horizontalLayoutWidget_24);
        geneHTMLCheckBox->setObjectName(QString::fromUtf8("geneHTMLCheckBox"));
        geneHTMLCheckBox->setChecked(true);

        verticalLayout_4->addWidget(geneHTMLCheckBox);

        keepDecoyCheckBox = new QCheckBox(horizontalLayoutWidget_24);
        keepDecoyCheckBox->setObjectName(QString::fromUtf8("keepDecoyCheckBox"));

        verticalLayout_4->addWidget(keepDecoyCheckBox);

        keepTempCheckBox = new QCheckBox(horizontalLayoutWidget_24);
        keepTempCheckBox->setObjectName(QString::fromUtf8("keepTempCheckBox"));

        verticalLayout_4->addWidget(keepTempCheckBox);

        horizontalLayoutWidget_22 = new QWidget(tab);
        horizontalLayoutWidget_22->setObjectName(QString::fromUtf8("horizontalLayoutWidget_22"));
        horizontalLayoutWidget_22->setGeometry(QRect(10, 190, 277, 33));
        horizontalLayout_22 = new QHBoxLayout(horizontalLayoutWidget_22);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        horizontalLayout_22->setContentsMargins(0, 0, 0, 0);
        label_23 = new QLabel(horizontalLayoutWidget_22);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setMinimumSize(QSize(210, 0));

        horizontalLayout_22->addWidget(label_23);

        threadNumberEdit = new QLineEdit(horizontalLayoutWidget_22);
        threadNumberEdit->setObjectName(QString::fromUtf8("threadNumberEdit"));
        threadNumberEdit->setMaximumSize(QSize(110, 16777215));
        threadNumberEdit->setReadOnly(false);

        horizontalLayout_22->addWidget(threadNumberEdit);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        horizontalLayoutWidget_3 = new QWidget(tab_2);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(480, 90, 361, 33));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(250, 0));

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

        horizontalLayoutWidget_10 = new QWidget(tab_2);
        horizontalLayoutWidget_10->setObjectName(QString::fromUtf8("horizontalLayoutWidget_10"));
        horizontalLayoutWidget_10->setGeometry(QRect(480, 50, 361, 33));
        horizontalLayout_10 = new QHBoxLayout(horizontalLayoutWidget_10);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(horizontalLayoutWidget_10);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(250, 0));

        horizontalLayout_10->addWidget(label_8);

        maxModEdit = new QLineEdit(horizontalLayoutWidget_10);
        maxModEdit->setObjectName(QString::fromUtf8("maxModEdit"));
        maxModEdit->setMaximumSize(QSize(110, 16777215));
        maxModEdit->setReadOnly(false);

        horizontalLayout_10->addWidget(maxModEdit);

        horizontalLayoutWidget_9 = new QWidget(tab_2);
        horizontalLayoutWidget_9->setObjectName(QString::fromUtf8("horizontalLayoutWidget_9"));
        horizontalLayoutWidget_9->setGeometry(QRect(10, 10, 381, 33));
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
        numModComboBox->addItem(QString());
        numModComboBox->addItem(QString());
        numModComboBox->addItem(QString());
        numModComboBox->addItem(QString());
        numModComboBox->addItem(QString());
        numModComboBox->addItem(QString());
        numModComboBox->addItem(QString());
        numModComboBox->setObjectName(QString::fromUtf8("numModComboBox"));
        numModComboBox->setEditable(false);

        horizontalLayout_9->addWidget(numModComboBox);

        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 60, 381, 91));
        gridLayoutWidget = new QWidget(groupBox_2);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 26, 321, 61));
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

        horizontalLayoutWidget_12 = new QWidget(tab_2);
        horizontalLayoutWidget_12->setObjectName(QString::fromUtf8("horizontalLayoutWidget_12"));
        horizontalLayoutWidget_12->setGeometry(QRect(480, 10, 361, 33));
        horizontalLayout_12 = new QHBoxLayout(horizontalLayoutWidget_12);
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        label_14 = new QLabel(horizontalLayoutWidget_12);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(250, 0));

        horizontalLayout_12->addWidget(label_14);

        numUnknownShiftComboBox = new QComboBox(horizontalLayoutWidget_12);
        numUnknownShiftComboBox->addItem(QString());
        numUnknownShiftComboBox->addItem(QString());
        numUnknownShiftComboBox->addItem(QString());
        numUnknownShiftComboBox->setObjectName(QString::fromUtf8("numUnknownShiftComboBox"));
        numUnknownShiftComboBox->setEditable(false);

        horizontalLayout_12->addWidget(numUnknownShiftComboBox);

        horizontalLayoutWidget_23 = new QWidget(tab_2);
        horizontalLayoutWidget_23->setObjectName(QString::fromUtf8("horizontalLayoutWidget_23"));
        horizontalLayoutWidget_23->setGeometry(QRect(480, 130, 361, 33));
        horizontalLayout_23 = new QHBoxLayout(horizontalLayoutWidget_23);
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        horizontalLayout_23->setContentsMargins(0, 0, 0, 0);
        asfDiagCheckBox = new QCheckBox(horizontalLayoutWidget_23);
        asfDiagCheckBox->setObjectName(QString::fromUtf8("asfDiagCheckBox"));

        horizontalLayout_23->addWidget(asfDiagCheckBox);

        gridLayoutWidget_2 = new QWidget(tab_2);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 170, 381, 70));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_15 = new QLabel(gridLayoutWidget_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_2->addWidget(label_15, 0, 0, 1, 1);

        maxGapLength = new QLineEdit(gridLayoutWidget_2);
        maxGapLength->setObjectName(QString::fromUtf8("maxGapLength"));
        maxGapLength->setMaximumSize(QSize(110, 16777215));
        maxGapLength->setReadOnly(false);

        gridLayout_2->addWidget(maxGapLength, 0, 1, 1, 1);

        label_16 = new QLabel(gridLayoutWidget_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_2->addWidget(label_16, 1, 0, 1, 1);

        maxVarPTMGap = new QLineEdit(gridLayoutWidget_2);
        maxVarPTMGap->setObjectName(QString::fromUtf8("maxVarPTMGap"));
        maxVarPTMGap->setMaximumSize(QSize(110, 16777215));
        maxVarPTMGap->setReadOnly(false);

        gridLayout_2->addWidget(maxVarPTMGap, 1, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());
        TopmgWindow->setCentralWidget(centralWidget);
        tabWidget->raise();
        outputTextBrowser->raise();
        horizontalLayoutWidget_7->raise();

        retranslateUi(TopmgWindow);

        outputButton->setDefault(true);
        startButton->setDefault(true);
        tabWidget->setCurrentIndex(0);
        numModComboBox->setCurrentIndex(4);
        numUnknownShiftComboBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TopmgWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TopmgWindow)
    {
        TopmgWindow->setWindowTitle(QCoreApplication::translate("TopmgWindow", "TopMG", nullptr));
        outputTextBrowser->setHtml(QCoreApplication::translate("TopmgWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Cantarell';\">Click the Start button to process the spectrum file.</span></p></body></html>", nullptr));
        exitButton->setText(QCoreApplication::translate("TopmgWindow", "Exit", nullptr));
        clearButton->setText(QCoreApplication::translate("TopmgWindow", "Clear Input Files", nullptr));
        defaultButton->setText(QCoreApplication::translate("TopmgWindow", "Default Parameters", nullptr));
        outputButton->setText(QCoreApplication::translate("TopmgWindow", "Output Folder", nullptr));
        startButton->setText(QCoreApplication::translate("TopmgWindow", "Start", nullptr));
        label->setText(QCoreApplication::translate("TopmgWindow", "Database file: ", nullptr));
        databaseFileButton->setText(QCoreApplication::translate("TopmgWindow", "Select", nullptr));
        label_2->setText(QCoreApplication::translate("TopmgWindow", "Spectrum files: ", nullptr));
        addButton->setText(QCoreApplication::translate("TopmgWindow", "Add", nullptr));
        delButton->setText(QCoreApplication::translate("TopmgWindow", "Remove", nullptr));
        label_5->setText(QCoreApplication::translate("TopmgWindow", "Combined output file: ", nullptr));
        label_18->setText(QCoreApplication::translate("TopmgWindow", "Variable PTM file:", nullptr));
        modFileButton->setText(QCoreApplication::translate("TopmgWindow", "Select", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("TopmgWindow", "Files", nullptr));
        label_4->setText(QCoreApplication::translate("TopmgWindow", "Fixed modifications:", nullptr));
        fixedModComboBox->setItemText(0, QCoreApplication::translate("TopmgWindow", "NONE", nullptr));
        fixedModComboBox->setItemText(1, QCoreApplication::translate("TopmgWindow", "Carbamidomethylation on cysteine", nullptr));
        fixedModComboBox->setItemText(2, QCoreApplication::translate("TopmgWindow", "Carboxymethylation on cysteine", nullptr));
        fixedModComboBox->setItemText(3, QCoreApplication::translate("TopmgWindow", "A fixed modification file", nullptr));

        fixedModFileButton->setText(QCoreApplication::translate("TopmgWindow", "File", nullptr));
        decoyCheckBox->setText(QCoreApplication::translate("TopmgWindow", "Decoy database", nullptr));
        wholeProteinCheckBox->setText(QCoreApplication::translate("TopmgWindow", "Whole proteins only", nullptr));
        topfdFeatureCheckBox->setText(QCoreApplication::translate("TopmgWindow", "Missing MS1 feature file", nullptr));
        label_9->setText(QCoreApplication::translate("TopmgWindow", "Mass error tolerance (PPM):", nullptr));
        errorToleranceEdit->setText(QCoreApplication::translate("TopmgWindow", "10", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("TopmgWindow", "Cutoff settings", nullptr));
        label_11->setText(QCoreApplication::translate("TopmgWindow", "Spectrum level:", nullptr));
        label_13->setText(QCoreApplication::translate("TopmgWindow", "Proteoform level:", nullptr));
        cutoffSpectralTypeComboBox->setItemText(0, QCoreApplication::translate("TopmgWindow", "EVALUE", nullptr));
        cutoffSpectralTypeComboBox->setItemText(1, QCoreApplication::translate("TopmgWindow", "FDR", nullptr));

        cutoffSpectralValueEdit->setText(QCoreApplication::translate("TopmgWindow", "0.01", nullptr));
        cutoffProteoformTypeComboBox->setItemText(0, QCoreApplication::translate("TopmgWindow", "EVALUE", nullptr));
        cutoffProteoformTypeComboBox->setItemText(1, QCoreApplication::translate("TopmgWindow", "FDR", nullptr));

        cutoffProteoformValueEdit->setText(QCoreApplication::translate("TopmgWindow", "0.01", nullptr));
        label_10->setText(QCoreApplication::translate("TopmgWindow", "PrSM cluster error tolerance (Da):", nullptr));
        formErrorToleranceEdit->setText(QCoreApplication::translate("TopmgWindow", "1.2", nullptr));
        geneHTMLCheckBox->setText(QCoreApplication::translate("TopmgWindow", "Generate HTML files", nullptr));
        keepDecoyCheckBox->setText(QCoreApplication::translate("TopmgWindow", "Keep decoy identifications", nullptr));
        keepTempCheckBox->setText(QCoreApplication::translate("TopmgWindow", "Keep intermediate files", nullptr));
        label_23->setText(QCoreApplication::translate("TopmgWindow", "Thread number:", nullptr));
        threadNumberEdit->setText(QCoreApplication::translate("TopmgWindow", "1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("TopmgWindow", "Basic Parameters", nullptr));
        label_3->setText(QCoreApplication::translate("TopmgWindow", "Fragmentation:", nullptr));
        activationComboBox->setItemText(0, QCoreApplication::translate("TopmgWindow", "FILE", nullptr));
        activationComboBox->setItemText(1, QCoreApplication::translate("TopmgWindow", "CID", nullptr));
        activationComboBox->setItemText(2, QCoreApplication::translate("TopmgWindow", "ETD", nullptr));
        activationComboBox->setItemText(3, QCoreApplication::translate("TopmgWindow", "HCD", nullptr));
        activationComboBox->setItemText(4, QCoreApplication::translate("TopmgWindow", "MPD", nullptr));
        activationComboBox->setItemText(5, QCoreApplication::translate("TopmgWindow", "UVPD", nullptr));

        label_8->setText(QCoreApplication::translate("TopmgWindow", "Maximum mass shift (Da):", nullptr));
        maxModEdit->setText(QCoreApplication::translate("TopmgWindow", "500", nullptr));
        label_12->setText(QCoreApplication::translate("TopmgWindow", "Maximum number of variable PTMs:", nullptr));
        numModComboBox->setItemText(0, QCoreApplication::translate("TopmgWindow", "1", nullptr));
        numModComboBox->setItemText(1, QCoreApplication::translate("TopmgWindow", "2", nullptr));
        numModComboBox->setItemText(2, QCoreApplication::translate("TopmgWindow", "3", nullptr));
        numModComboBox->setItemText(3, QCoreApplication::translate("TopmgWindow", "4", nullptr));
        numModComboBox->setItemText(4, QCoreApplication::translate("TopmgWindow", "5", nullptr));
        numModComboBox->setItemText(5, QCoreApplication::translate("TopmgWindow", "6", nullptr));
        numModComboBox->setItemText(6, QCoreApplication::translate("TopmgWindow", "7", nullptr));
        numModComboBox->setItemText(7, QCoreApplication::translate("TopmgWindow", "8", nullptr));
        numModComboBox->setItemText(8, QCoreApplication::translate("TopmgWindow", "9", nullptr));
        numModComboBox->setItemText(9, QCoreApplication::translate("TopmgWindow", "10", nullptr));

        groupBox_2->setTitle(QCoreApplication::translate("TopmgWindow", "N-terminal forms", nullptr));
        NONECheckBox->setText(QCoreApplication::translate("TopmgWindow", "NONE", nullptr));
        NMECheckBox->setText(QCoreApplication::translate("TopmgWindow", "NME", nullptr));
        NMEACCheckBox->setText(QCoreApplication::translate("TopmgWindow", "NME_ACETYLATION", nullptr));
        MACCheckBox->setText(QCoreApplication::translate("TopmgWindow", "M_ACETYLATION", nullptr));
        label_14->setText(QCoreApplication::translate("TopmgWindow", "Maximum number of unknown mass shifts:", nullptr));
        numUnknownShiftComboBox->setItemText(0, QCoreApplication::translate("TopmgWindow", "0", nullptr));
        numUnknownShiftComboBox->setItemText(1, QCoreApplication::translate("TopmgWindow", "1", nullptr));
        numUnknownShiftComboBox->setItemText(2, QCoreApplication::translate("TopmgWindow", "2", nullptr));

        asfDiagCheckBox->setText(QCoreApplication::translate("TopmgWindow", "Use ASF-diagonal filtering", nullptr));
        label_15->setText(QCoreApplication::translate("TopmgWindow", "Maximum gap length in the graph:", nullptr));
        maxGapLength->setText(QCoreApplication::translate("TopmgWindow", "40", nullptr));
        label_16->setText(QCoreApplication::translate("TopmgWindow", "Maximum number of variable PTMs in a gap:", nullptr));
        maxVarPTMGap->setText(QCoreApplication::translate("TopmgWindow", "5", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("TopmgWindow", "Advanced Parameters", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TopmgWindow: public Ui_TopmgWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPMGWINDOW_H
