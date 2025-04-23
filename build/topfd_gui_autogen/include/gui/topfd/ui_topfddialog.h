/********************************************************************************
** Form generated from reading UI file 'topfddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPFDDIALOG_H
#define UI_TOPFDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TopFDDialog
{
public:
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *addButton;
    QPushButton *delButton;
    QWidget *horizontalLayoutWidget_13;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_10;
    QComboBox *activationComboBox;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QLineEdit *maxChargeEdit;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *maxMassEdit;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *mzErrorEdit;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *ms1snRatioEdit;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *exitButton;
    QPushButton *clearButton;
    QPushButton *defaultButton;
    QPushButton *outputButton;
    QPushButton *startButton;
    QWidget *horizontalLayoutWidget_8;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_6;
    QLineEdit *windowSizeEdit;
    QTextBrowser *outputTextBrowser;
    QWidget *horizontalLayoutWidget_9;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *ms2snRatioEdit;
    QListWidget *listWidget;
    QWidget *horizontalLayoutWidget_10;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QLineEdit *threadNumberEdit;
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget_11;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *geneHTMLCheckBox;
    QWidget *horizontalLayoutWidget_12;
    QHBoxLayout *horizontalLayout_11;
    QCheckBox *disableFilteringCheckBox;
    QWidget *horizontalLayoutWidget_14;
    QHBoxLayout *horizontalLayout_12;
    QCheckBox *missLevelOneCheckBox;
    QWidget *horizontalLayoutWidget_15;
    QHBoxLayout *horizontalLayout_14;
    QCheckBox *envCNNCheckBox;

    void setupUi(QMainWindow *TopFDDialog)
    {
        if (TopFDDialog->objectName().isEmpty())
            TopFDDialog->setObjectName(QString::fromUtf8("TopFDDialog"));
        TopFDDialog->resize(965, 610);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(TopFDDialog->sizePolicy().hasHeightForWidth());
        TopFDDialog->setSizePolicy(sizePolicy);
        TopFDDialog->setMinimumSize(QSize(965, 610));
        TopFDDialog->setMaximumSize(QSize(965, 610));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/toppic.ico"), QSize(), QIcon::Normal, QIcon::Off);
        TopFDDialog->setWindowIcon(icon);
        centralWidget = new QWidget(TopFDDialog);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(30, 10, 436, 50));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        addButton = new QPushButton(horizontalLayoutWidget);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setMinimumSize(QSize(100, 0));
        addButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(addButton);

        delButton = new QPushButton(horizontalLayoutWidget);
        delButton->setObjectName(QString::fromUtf8("delButton"));
        delButton->setMinimumSize(QSize(100, 0));
        delButton->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(delButton);

        horizontalLayoutWidget_13 = new QWidget(centralWidget);
        horizontalLayoutWidget_13->setObjectName(QString::fromUtf8("horizontalLayoutWidget_13"));
        horizontalLayoutWidget_13->setGeometry(QRect(620, 290, 311, 41));
        horizontalLayout_13 = new QHBoxLayout(horizontalLayoutWidget_13);
        horizontalLayout_13->setSpacing(7);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setSizeConstraint(QLayout::SetNoConstraint);
        horizontalLayout_13->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(horizontalLayoutWidget_13);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_13->addWidget(label_10);

        activationComboBox = new QComboBox(horizontalLayoutWidget_13);
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->addItem(QString());
        activationComboBox->setObjectName(QString::fromUtf8("activationComboBox"));
        activationComboBox->setEnabled(true);
        activationComboBox->setMinimumSize(QSize(0, 0));
        activationComboBox->setMaximumSize(QSize(75, 16777215));
        activationComboBox->setIconSize(QSize(20, 20));
        activationComboBox->setFrame(true);

        horizontalLayout_13->addWidget(activationComboBox);

        horizontalLayoutWidget_2 = new QWidget(centralWidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(30, 170, 241, 44));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(0, 0));
        label_2->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout_6->addWidget(label_2);

        maxChargeEdit = new QLineEdit(horizontalLayoutWidget_2);
        maxChargeEdit->setObjectName(QString::fromUtf8("maxChargeEdit"));
        maxChargeEdit->setEnabled(true);
        maxChargeEdit->setMinimumSize(QSize(70, 0));
        maxChargeEdit->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_6->addWidget(maxChargeEdit, 0, Qt::AlignRight);

        horizontalLayoutWidget_3 = new QWidget(centralWidget);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(30, 210, 241, 41));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(0, 0));

        horizontalLayout_3->addWidget(label_3);

        maxMassEdit = new QLineEdit(horizontalLayoutWidget_3);
        maxMassEdit->setObjectName(QString::fromUtf8("maxMassEdit"));
        maxMassEdit->setMinimumSize(QSize(70, 0));
        maxMassEdit->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_3->addWidget(maxMassEdit, 0, Qt::AlignRight);

        horizontalLayoutWidget_4 = new QWidget(centralWidget);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(620, 210, 311, 41));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(horizontalLayoutWidget_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(0, 0));

        horizontalLayout_4->addWidget(label_4);

        mzErrorEdit = new QLineEdit(horizontalLayoutWidget_4);
        mzErrorEdit->setObjectName(QString::fromUtf8("mzErrorEdit"));
        mzErrorEdit->setMinimumSize(QSize(40, 0));
        mzErrorEdit->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_4->addWidget(mzErrorEdit);

        horizontalLayoutWidget_5 = new QWidget(centralWidget);
        horizontalLayoutWidget_5->setObjectName(QString::fromUtf8("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(300, 170, 271, 44));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(horizontalLayoutWidget_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(110, 0));

        horizontalLayout_5->addWidget(label_5);

        ms1snRatioEdit = new QLineEdit(horizontalLayoutWidget_5);
        ms1snRatioEdit->setObjectName(QString::fromUtf8("ms1snRatioEdit"));
        ms1snRatioEdit->setMinimumSize(QSize(0, 0));
        ms1snRatioEdit->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_5->addWidget(ms1snRatioEdit, 0, Qt::AlignRight);

        horizontalLayoutWidget_7 = new QWidget(centralWidget);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(30, 550, 901, 50));
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

        horizontalLayoutWidget_8 = new QWidget(centralWidget);
        horizontalLayoutWidget_8->setObjectName(QString::fromUtf8("horizontalLayoutWidget_8"));
        horizontalLayoutWidget_8->setGeometry(QRect(620, 170, 311, 44));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_8);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(horizontalLayoutWidget_8);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(120, 0));

        horizontalLayout_2->addWidget(label_6);

        windowSizeEdit = new QLineEdit(horizontalLayoutWidget_8);
        windowSizeEdit->setObjectName(QString::fromUtf8("windowSizeEdit"));
        windowSizeEdit->setMinimumSize(QSize(0, 0));
        windowSizeEdit->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_2->addWidget(windowSizeEdit, 0, Qt::AlignRight);

        outputTextBrowser = new QTextBrowser(centralWidget);
        outputTextBrowser->setObjectName(QString::fromUtf8("outputTextBrowser"));
        outputTextBrowser->setGeometry(QRect(30, 360, 901, 181));
        horizontalLayoutWidget_9 = new QWidget(centralWidget);
        horizontalLayoutWidget_9->setObjectName(QString::fromUtf8("horizontalLayoutWidget_9"));
        horizontalLayoutWidget_9->setGeometry(QRect(300, 210, 271, 41));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget_9);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(horizontalLayoutWidget_9);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        ms2snRatioEdit = new QLineEdit(horizontalLayoutWidget_9);
        ms2snRatioEdit->setObjectName(QString::fromUtf8("ms2snRatioEdit"));
        ms2snRatioEdit->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_8->addWidget(ms2snRatioEdit);

        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(30, 60, 901, 111));
        horizontalLayoutWidget_10 = new QWidget(centralWidget);
        horizontalLayoutWidget_10->setObjectName(QString::fromUtf8("horizontalLayoutWidget_10"));
        horizontalLayoutWidget_10->setGeometry(QRect(620, 250, 311, 41));
        horizontalLayout_9 = new QHBoxLayout(horizontalLayoutWidget_10);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(horizontalLayoutWidget_10);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(110, 0));

        horizontalLayout_9->addWidget(label_8);

        threadNumberEdit = new QLineEdit(horizontalLayoutWidget_10);
        threadNumberEdit->setObjectName(QString::fromUtf8("threadNumberEdit"));
        threadNumberEdit->setMinimumSize(QSize(0, 0));
        threadNumberEdit->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_9->addWidget(threadNumberEdit);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 270, 541, 81));
        horizontalLayoutWidget_11 = new QWidget(groupBox);
        horizontalLayoutWidget_11->setObjectName(QString::fromUtf8("horizontalLayoutWidget_11"));
        horizontalLayoutWidget_11->setGeometry(QRect(270, 20, 201, 25));
        horizontalLayout_10 = new QHBoxLayout(horizontalLayoutWidget_11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        geneHTMLCheckBox = new QCheckBox(horizontalLayoutWidget_11);
        geneHTMLCheckBox->setObjectName(QString::fromUtf8("geneHTMLCheckBox"));
        geneHTMLCheckBox->setAcceptDrops(false);
        geneHTMLCheckBox->setCheckable(true);
        geneHTMLCheckBox->setChecked(true);

        horizontalLayout_10->addWidget(geneHTMLCheckBox);

        horizontalLayoutWidget_12 = new QWidget(groupBox);
        horizontalLayoutWidget_12->setObjectName(QString::fromUtf8("horizontalLayoutWidget_12"));
        horizontalLayoutWidget_12->setGeometry(QRect(270, 50, 201, 25));
        horizontalLayout_11 = new QHBoxLayout(horizontalLayoutWidget_12);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        disableFilteringCheckBox = new QCheckBox(horizontalLayoutWidget_12);
        disableFilteringCheckBox->setObjectName(QString::fromUtf8("disableFilteringCheckBox"));
        disableFilteringCheckBox->setAcceptDrops(false);
        disableFilteringCheckBox->setCheckable(true);
        disableFilteringCheckBox->setChecked(false);

        horizontalLayout_11->addWidget(disableFilteringCheckBox);

        horizontalLayoutWidget_14 = new QWidget(groupBox);
        horizontalLayoutWidget_14->setObjectName(QString::fromUtf8("horizontalLayoutWidget_14"));
        horizontalLayoutWidget_14->setGeometry(QRect(10, 20, 201, 25));
        horizontalLayout_12 = new QHBoxLayout(horizontalLayoutWidget_14);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        horizontalLayout_12->setContentsMargins(0, 0, 0, 0);
        missLevelOneCheckBox = new QCheckBox(horizontalLayoutWidget_14);
        missLevelOneCheckBox->setObjectName(QString::fromUtf8("missLevelOneCheckBox"));
        missLevelOneCheckBox->setAcceptDrops(false);
        missLevelOneCheckBox->setCheckable(true);
        missLevelOneCheckBox->setChecked(false);

        horizontalLayout_12->addWidget(missLevelOneCheckBox);

        horizontalLayoutWidget_15 = new QWidget(groupBox);
        horizontalLayoutWidget_15->setObjectName(QString::fromUtf8("horizontalLayoutWidget_15"));
        horizontalLayoutWidget_15->setGeometry(QRect(10, 50, 201, 25));
        horizontalLayout_14 = new QHBoxLayout(horizontalLayoutWidget_15);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        envCNNCheckBox = new QCheckBox(horizontalLayoutWidget_15);
        envCNNCheckBox->setObjectName(QString::fromUtf8("envCNNCheckBox"));
        envCNNCheckBox->setMinimumSize(QSize(150, 0));
        envCNNCheckBox->setAcceptDrops(false);
        envCNNCheckBox->setCheckable(true);
        envCNNCheckBox->setChecked(false);

        horizontalLayout_14->addWidget(envCNNCheckBox);

        TopFDDialog->setCentralWidget(centralWidget);
        listWidget->raise();
        horizontalLayoutWidget->raise();
        horizontalLayoutWidget_13->raise();
        horizontalLayoutWidget_2->raise();
        horizontalLayoutWidget_3->raise();
        horizontalLayoutWidget_4->raise();
        horizontalLayoutWidget_5->raise();
        horizontalLayoutWidget_7->raise();
        horizontalLayoutWidget_8->raise();
        outputTextBrowser->raise();
        horizontalLayoutWidget_9->raise();
        horizontalLayoutWidget_10->raise();
        groupBox->raise();

        retranslateUi(TopFDDialog);

        startButton->setDefault(true);


        QMetaObject::connectSlotsByName(TopFDDialog);
    } // setupUi

    void retranslateUi(QMainWindow *TopFDDialog)
    {
        TopFDDialog->setWindowTitle(QCoreApplication::translate("TopFDDialog", "TopFD", nullptr));
        label->setText(QCoreApplication::translate("TopFDDialog", "Spectrum files: ", nullptr));
        addButton->setText(QCoreApplication::translate("TopFDDialog", "Add", nullptr));
        delButton->setText(QCoreApplication::translate("TopFDDialog", "Remove", nullptr));
        label_10->setText(QCoreApplication::translate("TopFDDialog", "Fragmentation:", nullptr));
        activationComboBox->setItemText(0, QCoreApplication::translate("TopFDDialog", "FILE", nullptr));
        activationComboBox->setItemText(1, QCoreApplication::translate("TopFDDialog", "CID", nullptr));
        activationComboBox->setItemText(2, QCoreApplication::translate("TopFDDialog", "ETD", nullptr));
        activationComboBox->setItemText(3, QCoreApplication::translate("TopFDDialog", "HCD", nullptr));
        activationComboBox->setItemText(4, QCoreApplication::translate("TopFDDialog", "MPD", nullptr));
        activationComboBox->setItemText(5, QCoreApplication::translate("TopFDDialog", "UVPD", nullptr));

        label_2->setText(QCoreApplication::translate("TopFDDialog", "Maximum charge: ", nullptr));
        maxChargeEdit->setText(QCoreApplication::translate("TopFDDialog", "30", nullptr));
        label_3->setText(QCoreApplication::translate("TopFDDialog", "Maximum mass (Da): ", nullptr));
        maxMassEdit->setText(QCoreApplication::translate("TopFDDialog", "100000", nullptr));
        label_4->setText(QCoreApplication::translate("TopFDDialog", "M/z error: ", nullptr));
        mzErrorEdit->setText(QCoreApplication::translate("TopFDDialog", "0.02", nullptr));
        label_5->setText(QCoreApplication::translate("TopFDDialog", "MS1 S/N ratio: ", nullptr));
        ms1snRatioEdit->setText(QCoreApplication::translate("TopFDDialog", "3.0", nullptr));
        exitButton->setText(QCoreApplication::translate("TopFDDialog", "Exit", nullptr));
        clearButton->setText(QCoreApplication::translate("TopFDDialog", "Clear", nullptr));
        defaultButton->setText(QCoreApplication::translate("TopFDDialog", "Default", nullptr));
        outputButton->setText(QCoreApplication::translate("TopFDDialog", "Output Folder", nullptr));
        startButton->setText(QCoreApplication::translate("TopFDDialog", "Start", nullptr));
        label_6->setText(QCoreApplication::translate("TopFDDialog", "Precursor window size (m/z): ", nullptr));
        windowSizeEdit->setText(QCoreApplication::translate("TopFDDialog", "3.0", nullptr));
        outputTextBrowser->setHtml(QCoreApplication::translate("TopFDDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Cantarell';\">Click the Start button to process the spectrum file.</span></p></body></html>", nullptr));
        label_7->setText(QCoreApplication::translate("TopFDDialog", "MS2 S/N ratio:", nullptr));
        ms2snRatioEdit->setText(QCoreApplication::translate("TopFDDialog", "1.0", nullptr));
        label_8->setText(QCoreApplication::translate("TopFDDialog", "Thread number:", nullptr));
        threadNumberEdit->setText(QCoreApplication::translate("TopFDDialog", "1", nullptr));
        groupBox->setTitle(QCoreApplication::translate("TopFDDialog", "Additional settings", nullptr));
        geneHTMLCheckBox->setText(QCoreApplication::translate("TopFDDialog", "Generate HTML files", nullptr));
        disableFilteringCheckBox->setText(QCoreApplication::translate("TopFDDialog", "Disable final filtering", nullptr));
        missLevelOneCheckBox->setText(QCoreApplication::translate("TopFDDialog", "Missing MS1 spectra", nullptr));
        envCNNCheckBox->setText(QCoreApplication::translate("TopFDDialog", "Use EnvCNN for scoring", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TopFDDialog: public Ui_TopFDDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPFDDIALOG_H
