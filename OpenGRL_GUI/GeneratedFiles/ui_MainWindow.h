/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QComboBox *comboBox;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *saveProfileButton;
    QGroupBox *trackBox;
    QGridLayout *gridLayout_2;
    QPushButton *recordButton;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_12;
    QFrame *frame_3;
    QPushButton *saveGestureButton;
    QLabel *label;
    QSlider *certainty;
    QLabel *certaintyValue;
    QGroupBox *groupBox_8;
    QVBoxLayout *verticalLayout_10;
    QLabel *debugLog;
    QFrame *frame;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_6;
    QLabel *depthImageBox;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_3;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_7;
    QLabel *handImageBox;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QVBoxLayout *verticalLayout_8;
    QLabel *skeletonImageBox;
    QGroupBox *groupBox_7;
    QVBoxLayout *verticalLayout_5;
    QScrollArea *scrollArea_4;
    QWidget *scrollAreaWidgetContents_4;
    QVBoxLayout *verticalLayout_9;
    QLabel *matchedImageBox;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(1154, 722);
        MainWindowClass->setDocumentMode(false);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_11 = new QVBoxLayout(centralWidget);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMinimumSize(QSize(300, 0));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(frame_2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        comboBox = new QComboBox(groupBox_2);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 30, 131, 22));

        verticalLayout->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(frame_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setSizeIncrement(QSize(0, 0));
        horizontalLayout = new QHBoxLayout(groupBox_4);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        saveProfileButton = new QPushButton(groupBox_4);
        saveProfileButton->setObjectName(QString::fromUtf8("saveProfileButton"));

        horizontalLayout->addWidget(saveProfileButton);


        verticalLayout->addWidget(groupBox_4);

        trackBox = new QGroupBox(frame_2);
        trackBox->setObjectName(QString::fromUtf8("trackBox"));
        gridLayout_2 = new QGridLayout(trackBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        recordButton = new QPushButton(trackBox);
        recordButton->setObjectName(QString::fromUtf8("recordButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(recordButton->sizePolicy().hasHeightForWidth());
        recordButton->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(recordButton, 0, 0, 1, 1);


        verticalLayout->addWidget(trackBox);

        groupBox_3 = new QGroupBox(frame_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_12 = new QVBoxLayout(groupBox_3);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        frame_3 = new QFrame(groupBox_3);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        saveGestureButton = new QPushButton(frame_3);
        saveGestureButton->setObjectName(QString::fromUtf8("saveGestureButton"));
        saveGestureButton->setGeometry(QRect(80, 0, 93, 28));
        sizePolicy.setHeightForWidth(saveGestureButton->sizePolicy().hasHeightForWidth());
        saveGestureButton->setSizePolicy(sizePolicy);

        verticalLayout_12->addWidget(frame_3);

        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_12->addWidget(label);

        certainty = new QSlider(groupBox_3);
        certainty->setObjectName(QString::fromUtf8("certainty"));
        sizePolicy.setHeightForWidth(certainty->sizePolicy().hasHeightForWidth());
        certainty->setSizePolicy(sizePolicy);
        certainty->setMaximumSize(QSize(250, 16777215));
        certainty->setMaximum(250);
        certainty->setValue(70);
        certainty->setOrientation(Qt::Horizontal);
        certainty->setTickPosition(QSlider::TicksBelow);
        certainty->setTickInterval(0);

        verticalLayout_12->addWidget(certainty);

        certaintyValue = new QLabel(groupBox_3);
        certaintyValue->setObjectName(QString::fromUtf8("certaintyValue"));
        certaintyValue->setAlignment(Qt::AlignCenter);

        verticalLayout_12->addWidget(certaintyValue);


        verticalLayout->addWidget(groupBox_3);

        groupBox_8 = new QGroupBox(frame_2);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        verticalLayout_10 = new QVBoxLayout(groupBox_8);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        debugLog = new QLabel(groupBox_8);
        debugLog->setObjectName(QString::fromUtf8("debugLog"));
        debugLog->setScaledContents(true);

        verticalLayout_10->addWidget(debugLog);


        verticalLayout->addWidget(groupBox_8);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 2);
        verticalLayout->setStretch(3, 2);

        horizontalLayout_2->addWidget(frame_2);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_5 = new QGroupBox(frame);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_2 = new QVBoxLayout(groupBox_5);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        scrollArea = new QScrollArea(groupBox_5);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 370, 259));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        depthImageBox = new QLabel(scrollAreaWidgetContents);
        depthImageBox->setObjectName(QString::fromUtf8("depthImageBox"));
        depthImageBox->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(depthImageBox);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);


        gridLayout->addWidget(groupBox_5, 0, 0, 1, 1);

        groupBox_6 = new QGroupBox(frame);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        verticalLayout_3 = new QVBoxLayout(groupBox_6);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        scrollArea_2 = new QScrollArea(groupBox_6);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 370, 259));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        handImageBox = new QLabel(scrollAreaWidgetContents_2);
        handImageBox->setObjectName(QString::fromUtf8("handImageBox"));
        handImageBox->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(handImageBox);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_3->addWidget(scrollArea_2);


        gridLayout->addWidget(groupBox_6, 0, 1, 1, 1);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        scrollArea_3 = new QScrollArea(groupBox);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 370, 259));
        verticalLayout_8 = new QVBoxLayout(scrollAreaWidgetContents_3);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        skeletonImageBox = new QLabel(scrollAreaWidgetContents_3);
        skeletonImageBox->setObjectName(QString::fromUtf8("skeletonImageBox"));
        skeletonImageBox->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(skeletonImageBox);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_4->addWidget(scrollArea_3);


        gridLayout->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_7 = new QGroupBox(frame);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        verticalLayout_5 = new QVBoxLayout(groupBox_7);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        scrollArea_4 = new QScrollArea(groupBox_7);
        scrollArea_4->setObjectName(QString::fromUtf8("scrollArea_4"));
        scrollArea_4->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 370, 259));
        verticalLayout_9 = new QVBoxLayout(scrollAreaWidgetContents_4);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        matchedImageBox = new QLabel(scrollAreaWidgetContents_4);
        matchedImageBox->setObjectName(QString::fromUtf8("matchedImageBox"));
        matchedImageBox->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(matchedImageBox);

        scrollArea_4->setWidget(scrollAreaWidgetContents_4);

        verticalLayout_5->addWidget(scrollArea_4);


        gridLayout->addWidget(groupBox_7, 1, 1, 1, 1);


        horizontalLayout_2->addWidget(frame);


        verticalLayout_11->addLayout(horizontalLayout_2);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1154, 26));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindowClass", "Przechwytywanie", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindowClass", "Microsoft Kinect - One", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindowClass", "Microsoft Kinect - 360", nullptr));

        groupBox_4->setTitle(QApplication::translate("MainWindowClass", "Narz\304\231dzia", nullptr));
        saveProfileButton->setText(QApplication::translate("MainWindowClass", "Zapisz profilowanie", nullptr));
        trackBox->setTitle(QApplication::translate("MainWindowClass", "\305\232cie\305\274ka", nullptr));
        recordButton->setText(QApplication::translate("MainWindowClass", "Nagrywaj", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindowClass", "Gesty", nullptr));
        saveGestureButton->setText(QApplication::translate("MainWindowClass", "Nagrywaj", nullptr));
        label->setText(QApplication::translate("MainWindowClass", "Certainty setup:", nullptr));
        certaintyValue->setText(QString());
        groupBox_8->setTitle(QApplication::translate("MainWindowClass", "Logger", nullptr));
        debugLog->setText(QString());
        groupBox_5->setTitle(QApplication::translate("MainWindowClass", "GroupBox", nullptr));
        depthImageBox->setText(QString());
        groupBox_6->setTitle(QApplication::translate("MainWindowClass", "GroupBox", nullptr));
        handImageBox->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindowClass", "GroupBox", nullptr));
        skeletonImageBox->setText(QString());
        groupBox_7->setTitle(QApplication::translate("MainWindowClass", "GroupBox", nullptr));
        matchedImageBox->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
