/********************************************************************************
** Form generated from reading UI file 'topindexdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPINDEXDIALOG_H
#define UI_TOPINDEXDIALOG_H

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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TopIndexDialog
{
public:
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *exitButton;
    QPushButton *clearButton;
    QPushButton *defaultButton;
    QPushButton *outputButton;
    QPushButton *startButton;
    QTextBrowser *outputTextBrowser;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *databaseFileEdit;
    QPushButton *databaseFileButton;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QComboBox *fixedModComboBox;
    QLineEdit *fixedModFileEdit;
    QPushButton *fixedModFileButton;
    QWidget *horizontalLayoutWidget_21;
    QHBoxLayout *horizontalLayout_21;
    QCheckBox *decoyCheckBox;
    QWidget *horizontalLayoutWidget_22;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_23;
    QLineEdit *threadNumberEdit;
    QWidget *horizontalLayoutWidget_14;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_15;
    QLineEdit *errorToleranceEdit_2;
    QGroupBox *groupBox_2;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QCheckBox *NONECheckBox;
    QCheckBox *NMECheckBox;
    QCheckBox *NMEACCheckBox;
    QCheckBox *MACCheckBox;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QWidget *horizontalLayoutWidget_8;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;

    void setupUi(QMainWindow *TopIndexDialog)
    {
        if (TopIndexDialog->objectName().isEmpty())
            TopIndexDialog->setObjectName(QString::fromUtf8("TopIndexDialog"));
        TopIndexDialog->resize(920, 522);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(TopIndexDialog->sizePolicy().hasHeightForWidth());
        TopIndexDialog->setSizePolicy(sizePolicy);
        TopIndexDialog->setMinimumSize(QSize(920, 510));
        TopIndexDialog->setMaximumSize(QSize(920, 710));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/toppic.ico"), QSize(), QIcon::Normal, QIcon::Off);
        TopIndexDialog->setWindowIcon(icon);
        centralWidget = new QWidget(TopIndexDialog);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutWidget_7 = new QWidget(centralWidget);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(30, 460, 861, 41));
        horizontalLayout_7 = new QHBoxLayout(horizontalLayoutWidget_7);
        horizontalLayout_7->setSpacing(30);
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

        outputTextBrowser = new QTextBrowser(centralWidget);
        outputTextBrowser->setObjectName(QString::fromUtf8("outputTextBrowser"));
        outputTextBrowser->setGeometry(QRect(30, 230, 861, 221));
        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(180, 20, 711, 39));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        databaseFileEdit = new QLineEdit(horizontalLayoutWidget_2);
        databaseFileEdit->setObjectName(QString::fromUtf8("databaseFileEdit"));
        databaseFileEdit->setEnabled(true);
        databaseFileEdit->setMaximumSize(QSize(650, 16777215));
        databaseFileEdit->setReadOnly(true);

        horizontalLayout_3->addWidget(databaseFileEdit);

        databaseFileButton = new QPushButton(horizontalLayoutWidget_2);
        databaseFileButton->setObjectName(QString::fromUtf8("databaseFileButton"));
        databaseFileButton->setMaximumSize(QSize(85, 16777215));

        horizontalLayout_3->addWidget(databaseFileButton);

        horizontalLayoutWidget_5 = new QWidget(centralWidget);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(180, 70, 711, 32));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        fixedModComboBox = new QComboBox(horizontalLayoutWidget_5);
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->addItem(QString());
        fixedModComboBox->setObjectName(QString::fromUtf8("fixedModComboBox"));

        horizontalLayout_5->addWidget(fixedModComboBox);

        fixedModFileEdit = new QLineEdit(horizontalLayoutWidget_5);
        fixedModFileEdit->setObjectName(QString::fromUtf8("fixedModFileEdit"));
        fixedModFileEdit->setEnabled(false);
        fixedModFileEdit->setReadOnly(true);

        horizontalLayout_5->addWidget(fixedModFileEdit);

        fixedModFileButton = new QPushButton(horizontalLayoutWidget_5);
        fixedModFileButton->setObjectName(QString::fromUtf8("fixedModFileButton"));
        fixedModFileButton->setEnabled(false);
        fixedModFileButton->setMaximumSize(QSize(85, 16777215));

        horizontalLayout_5->addWidget(fixedModFileButton);

        horizontalLayoutWidget_21 = new QWidget(centralWidget);
        horizontalLayoutWidget_21->setObjectName(QString::fromUtf8("horizontalLayoutWidget_21"));
        horizontalLayoutWidget_21->setGeometry(QRect(560, 110, 151, 33));
        horizontalLayout_21 = new QHBoxLayout(horizontalLayoutWidget_21);
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        horizontalLayout_21->setContentsMargins(0, 0, 0, 0);
        decoyCheckBox = new QCheckBox(horizontalLayoutWidget_21);
        decoyCheckBox->setObjectName(QString::fromUtf8("decoyCheckBox"));

        horizontalLayout_21->addWidget(decoyCheckBox);

        horizontalLayoutWidget_22 = new QWidget(centralWidget);
        horizontalLayoutWidget_22->setObjectName(QString::fromUtf8("horizontalLayoutWidget_22"));
        horizontalLayoutWidget_22->setGeometry(QRect(560, 150, 331, 33));
        horizontalLayout_22 = new QHBoxLayout(horizontalLayoutWidget_22);
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        horizontalLayout_22->setContentsMargins(0, 0, 0, 0);
        label_23 = new QLabel(horizontalLayoutWidget_22);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        horizontalLayout_22->addWidget(label_23, 0, Qt::AlignLeft);

        threadNumberEdit = new QLineEdit(horizontalLayoutWidget_22);
        threadNumberEdit->setObjectName(QString::fromUtf8("threadNumberEdit"));
        threadNumberEdit->setMinimumSize(QSize(110, 0));
        threadNumberEdit->setMaximumSize(QSize(110, 16777215));
        threadNumberEdit->setReadOnly(false);

        horizontalLayout_22->addWidget(threadNumberEdit);

        horizontalLayoutWidget_14 = new QWidget(centralWidget);
        horizontalLayoutWidget_14->setObjectName(QString::fromUtf8("horizontalLayoutWidget_14"));
        horizontalLayoutWidget_14->setGeometry(QRect(560, 190, 331, 31));
        horizontalLayout_14 = new QHBoxLayout(horizontalLayoutWidget_14);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        label_15 = new QLabel(horizontalLayoutWidget_14);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_14->addWidget(label_15, 0, Qt::AlignLeft);

        errorToleranceEdit_2 = new QLineEdit(horizontalLayoutWidget_14);
        errorToleranceEdit_2->setObjectName(QString::fromUtf8("errorToleranceEdit_2"));
        errorToleranceEdit_2->setMinimumSize(QSize(110, 0));
        errorToleranceEdit_2->setMaximumSize(QSize(110, 16777215));
        errorToleranceEdit_2->setFrame(true);

        horizontalLayout_14->addWidget(errorToleranceEdit_2, 0, Qt::AlignRight);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(30, 120, 341, 91));
        gridLayoutWidget = new QWidget(groupBox_2);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(9, 17, 321, 70));
        gridLayout = new QGridLayout(gridLayoutWidget);
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

        horizontalLayoutWidget_3 = new QWidget(centralWidget);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(30, 20, 151, 39));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        horizontalLayoutWidget_8 = new QWidget(centralWidget);
        horizontalLayoutWidget_8->setObjectName(QString::fromUtf8("horizontalLayoutWidget_8"));
        horizontalLayoutWidget_8->setGeometry(QRect(30, 70, 151, 32));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget_8);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(horizontalLayoutWidget_8);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        TopIndexDialog->setCentralWidget(centralWidget);

        retranslateUi(TopIndexDialog);

        startButton->setDefault(true);


        QMetaObject::connectSlotsByName(TopIndexDialog);
    } // setupUi

    void retranslateUi(QMainWindow *TopIndexDialog)
    {
        TopIndexDialog->setWindowTitle(QCoreApplication::translate("TopIndexDialog", "TopIndex", nullptr));
        exitButton->setText(QCoreApplication::translate("TopIndexDialog", "Exit", nullptr));
        clearButton->setText(QCoreApplication::translate("TopIndexDialog", "Clear", nullptr));
        defaultButton->setText(QCoreApplication::translate("TopIndexDialog", "Default", nullptr));
        outputButton->setText(QCoreApplication::translate("TopIndexDialog", "Output Folder", nullptr));
        startButton->setText(QCoreApplication::translate("TopIndexDialog", "Start", nullptr));
        outputTextBrowser->setHtml(QCoreApplication::translate("TopIndexDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Cantarell';\">Click the Start button to generate index files.</span></p></body></html>", nullptr));
        databaseFileButton->setText(QCoreApplication::translate("TopIndexDialog", "Select", nullptr));
        fixedModComboBox->setItemText(0, QCoreApplication::translate("TopIndexDialog", "NONE", nullptr));
        fixedModComboBox->setItemText(1, QCoreApplication::translate("TopIndexDialog", "Carbamidomethylation on cysteine", nullptr));
        fixedModComboBox->setItemText(2, QCoreApplication::translate("TopIndexDialog", "Carboxymethylation on cysteine", nullptr));
        fixedModComboBox->setItemText(3, QCoreApplication::translate("TopIndexDialog", "A fixed modification file", nullptr));

        fixedModFileButton->setText(QCoreApplication::translate("TopIndexDialog", "File", nullptr));
        decoyCheckBox->setText(QCoreApplication::translate("TopIndexDialog", "Decoy database", nullptr));
        label_23->setText(QCoreApplication::translate("TopIndexDialog", "Thread number:", nullptr));
        threadNumberEdit->setText(QCoreApplication::translate("TopIndexDialog", "1", nullptr));
        label_15->setText(QCoreApplication::translate("TopIndexDialog", "Mass error tolerance (PPM):", nullptr));
        errorToleranceEdit_2->setText(QCoreApplication::translate("TopIndexDialog", "15", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("TopIndexDialog", "N-terminal forms", nullptr));
        NONECheckBox->setText(QCoreApplication::translate("TopIndexDialog", "NONE", nullptr));
        NMECheckBox->setText(QCoreApplication::translate("TopIndexDialog", "NME", nullptr));
        NMEACCheckBox->setText(QCoreApplication::translate("TopIndexDialog", "NME_ACETYLATION", nullptr));
        MACCheckBox->setText(QCoreApplication::translate("TopIndexDialog", "M_ACETYLATION", nullptr));
        label_3->setText(QCoreApplication::translate("TopIndexDialog", "Database file: ", nullptr));
        label_7->setText(QCoreApplication::translate("TopIndexDialog", "Fixed modifications:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TopIndexDialog: public Ui_TopIndexDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPINDEXDIALOG_H
