/********************************************************************************
** Form generated from reading UI file 'topdiffdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPDIFFDIALOG_H
#define UI_TOPDIFFDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TopDiffDialog
{
public:
    QWidget *centralWidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *addButton;
    QPushButton *delButton;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *precErrorEdit;
    QWidget *horizontalLayoutWidget_7;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *exitButton;
    QPushButton *clearButton;
    QPushButton *defaultButton;
    QPushButton *outputButton;
    QPushButton *startButton;
    QTextBrowser *outputTextBrowser;
    QListWidget *listWidget;
    QWidget *horizontalLayoutWidget_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *outputEdit;
    QWidget *horizontalLayoutWidget_8;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_6;
    QComboBox *toolComboBox;

    void setupUi(QMainWindow *TopDiffDialog)
    {
        if (TopDiffDialog->objectName().isEmpty())
            TopDiffDialog->setObjectName(QString::fromUtf8("TopDiffDialog"));
        TopDiffDialog->resize(920, 625);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(TopDiffDialog->sizePolicy().hasHeightForWidth());
        TopDiffDialog->setSizePolicy(sizePolicy);
        TopDiffDialog->setMinimumSize(QSize(920, 625));
        TopDiffDialog->setMaximumSize(QSize(920, 625));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/toppic.ico"), QSize(), QIcon::Normal, QIcon::Off);
        TopDiffDialog->setWindowIcon(icon);
        centralWidget = new QWidget(TopDiffDialog);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(40, 10, 321, 39));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(105, 16777215));

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

        horizontalLayoutWidget_4 = new QWidget(centralWidget);
        horizontalLayoutWidget_4->setObjectName(QString::fromUtf8("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(40, 270, 361, 36));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(horizontalLayoutWidget_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(110, 0));

        horizontalLayout_4->addWidget(label_4);

        precErrorEdit = new QLineEdit(horizontalLayoutWidget_4);
        precErrorEdit->setObjectName(QString::fromUtf8("precErrorEdit"));
        precErrorEdit->setMinimumSize(QSize(0, 0));
        precErrorEdit->setMaximumSize(QSize(70, 16777215));

        horizontalLayout_4->addWidget(precErrorEdit);

        horizontalLayoutWidget_7 = new QWidget(centralWidget);
        horizontalLayoutWidget_7->setObjectName(QString::fromUtf8("horizontalLayoutWidget_7"));
        horizontalLayoutWidget_7->setGeometry(QRect(40, 570, 861, 41));
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
        outputTextBrowser->setGeometry(QRect(40, 360, 861, 201));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(40, 50, 861, 171));
        horizontalLayoutWidget_6 = new QWidget(centralWidget);
        horizontalLayoutWidget_6->setObjectName(QString::fromUtf8("horizontalLayoutWidget_6"));
        horizontalLayoutWidget_6->setGeometry(QRect(40, 310, 861, 36));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget_6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_6);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_6->addWidget(label_3);

        outputEdit = new QLineEdit(horizontalLayoutWidget_6);
        outputEdit->setObjectName(QString::fromUtf8("outputEdit"));

        horizontalLayout_6->addWidget(outputEdit);

        horizontalLayoutWidget_8 = new QWidget(centralWidget);
        horizontalLayoutWidget_8->setObjectName(QString::fromUtf8("horizontalLayoutWidget_8"));
        horizontalLayoutWidget_8->setGeometry(QRect(40, 230, 251, 31));
        horizontalLayout_8 = new QHBoxLayout(horizontalLayoutWidget_8);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(horizontalLayoutWidget_8);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_8->addWidget(label_6);

        toolComboBox = new QComboBox(horizontalLayoutWidget_8);
        toolComboBox->addItem(QString());
        toolComboBox->addItem(QString());
        toolComboBox->setObjectName(QString::fromUtf8("toolComboBox"));

        horizontalLayout_8->addWidget(toolComboBox);

        TopDiffDialog->setCentralWidget(centralWidget);

        retranslateUi(TopDiffDialog);

        startButton->setDefault(true);


        QMetaObject::connectSlotsByName(TopDiffDialog);
    } // setupUi

    void retranslateUi(QMainWindow *TopDiffDialog)
    {
        TopDiffDialog->setWindowTitle(QCoreApplication::translate("TopDiffDialog", "Topdiff", nullptr));
        label->setText(QCoreApplication::translate("TopDiffDialog", "Spectrum files: ", nullptr));
        addButton->setText(QCoreApplication::translate("TopDiffDialog", "Add", nullptr));
        delButton->setText(QCoreApplication::translate("TopDiffDialog", "Remove", nullptr));
        label_4->setText(QCoreApplication::translate("TopDiffDialog", "Precursor mass error tolerance (Dalton):", nullptr));
        precErrorEdit->setText(QCoreApplication::translate("TopDiffDialog", "1.2", nullptr));
        exitButton->setText(QCoreApplication::translate("TopDiffDialog", "Exit", nullptr));
        clearButton->setText(QCoreApplication::translate("TopDiffDialog", "Clear", nullptr));
        defaultButton->setText(QCoreApplication::translate("TopDiffDialog", "Default", nullptr));
        outputButton->setText(QCoreApplication::translate("TopDiffDialog", "Output Folder", nullptr));
        startButton->setText(QCoreApplication::translate("TopDiffDialog", "Start", nullptr));
        outputTextBrowser->setHtml(QCoreApplication::translate("TopDiffDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Cantarell';\">Click the Start button to merge identification files.</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("TopDiffDialog", "Output file:", nullptr));
        outputEdit->setText(QCoreApplication::translate("TopDiffDialog", "sample_diff.tsv", nullptr));
        label_6->setText(QCoreApplication::translate("TopDiffDialog", "Database search tool:", nullptr));
        toolComboBox->setItemText(0, QCoreApplication::translate("TopDiffDialog", "toppic", nullptr));
        toolComboBox->setItemText(1, QCoreApplication::translate("TopDiffDialog", "topmg", nullptr));

        toolComboBox->setCurrentText(QCoreApplication::translate("TopDiffDialog", "toppic", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TopDiffDialog: public Ui_TopDiffDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPDIFFDIALOG_H
