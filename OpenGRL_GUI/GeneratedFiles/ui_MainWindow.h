/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QComboBox *comboBox;
    QGroupBox *groupBox_4;
    QGroupBox *trackBox;
    QGridLayout *gridLayout_2;
    QPushButton *recordButton;
    QPushButton *saveTrackButton;
    QGroupBox *groupBox_3;
    QPushButton *exrButton;
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
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(1154, 722);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setMinimumSize(QSize(300, 0));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox_2 = new QGroupBox(frame_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        comboBox = new QComboBox(groupBox_2);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(10, 30, 131, 22));

        verticalLayout->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(frame_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setSizeIncrement(QSize(0, 0));

        verticalLayout->addWidget(groupBox_4);

        trackBox = new QGroupBox(frame_2);
        trackBox->setObjectName(QStringLiteral("trackBox"));
        gridLayout_2 = new QGridLayout(trackBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        recordButton = new QPushButton(trackBox);
        recordButton->setObjectName(QStringLiteral("recordButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(recordButton->sizePolicy().hasHeightForWidth());
        recordButton->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(recordButton, 0, 0, 1, 1);

        saveTrackButton = new QPushButton(trackBox);
        saveTrackButton->setObjectName(QStringLiteral("saveTrackButton"));

        gridLayout_2->addWidget(saveTrackButton, 1, 0, 1, 1);


        verticalLayout->addWidget(trackBox);

        groupBox_3 = new QGroupBox(frame_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        exrButton = new QPushButton(groupBox_3);
        exrButton->setObjectName(QStringLiteral("exrButton"));
        exrButton->setGeometry(QRect(100, 40, 75, 23));

        verticalLayout->addWidget(groupBox_3);

        groupBox_8 = new QGroupBox(frame_2);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        verticalLayout_10 = new QVBoxLayout(groupBox_8);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        debugLog = new QLabel(groupBox_8);
        debugLog->setObjectName(QStringLiteral("debugLog"));
        debugLog->setScaledContents(true);

        verticalLayout_10->addWidget(debugLog);


        verticalLayout->addWidget(groupBox_8);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 2);
        verticalLayout->setStretch(3, 2);

        horizontalLayout_2->addWidget(frame_2);

        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox_5 = new QGroupBox(frame);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        verticalLayout_2 = new QVBoxLayout(groupBox_5);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        scrollArea = new QScrollArea(groupBox_5);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 379, 277));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        depthImageBox = new QLabel(scrollAreaWidgetContents);
        depthImageBox->setObjectName(QStringLiteral("depthImageBox"));
        depthImageBox->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(depthImageBox);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_2->addWidget(scrollArea);


        gridLayout->addWidget(groupBox_5, 0, 0, 1, 1);

        groupBox_6 = new QGroupBox(frame);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        verticalLayout_3 = new QVBoxLayout(groupBox_6);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        scrollArea_2 = new QScrollArea(groupBox_6);
        scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 379, 277));
        verticalLayout_7 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        handImageBox = new QLabel(scrollAreaWidgetContents_2);
        handImageBox->setObjectName(QStringLiteral("handImageBox"));
        handImageBox->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(handImageBox);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_3->addWidget(scrollArea_2);


        gridLayout->addWidget(groupBox_6, 0, 1, 1, 1);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        scrollArea_3 = new QScrollArea(groupBox);
        scrollArea_3->setObjectName(QStringLiteral("scrollArea_3"));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QStringLiteral("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 379, 276));
        verticalLayout_8 = new QVBoxLayout(scrollAreaWidgetContents_3);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        skeletonImageBox = new QLabel(scrollAreaWidgetContents_3);
        skeletonImageBox->setObjectName(QStringLiteral("skeletonImageBox"));
        skeletonImageBox->setAlignment(Qt::AlignCenter);

        verticalLayout_8->addWidget(skeletonImageBox);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_4->addWidget(scrollArea_3);


        gridLayout->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_7 = new QGroupBox(frame);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        verticalLayout_5 = new QVBoxLayout(groupBox_7);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        scrollArea_4 = new QScrollArea(groupBox_7);
        scrollArea_4->setObjectName(QStringLiteral("scrollArea_4"));
        scrollArea_4->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QStringLiteral("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 379, 276));
        verticalLayout_9 = new QVBoxLayout(scrollAreaWidgetContents_4);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        matchedImageBox = new QLabel(scrollAreaWidgetContents_4);
        matchedImageBox->setObjectName(QStringLiteral("matchedImageBox"));
        matchedImageBox->setAlignment(Qt::AlignCenter);

        verticalLayout_9->addWidget(matchedImageBox);

        scrollArea_4->setWidget(scrollAreaWidgetContents_4);

        verticalLayout_5->addWidget(scrollArea_4);


        gridLayout->addWidget(groupBox_7, 1, 1, 1, 1);


        horizontalLayout_2->addWidget(frame);


        horizontalLayout->addLayout(horizontalLayout_2);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1154, 21));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
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

        groupBox_4->setTitle(QApplication::translate("MainWindowClass", "Ekstrakcja", nullptr));
        trackBox->setTitle(QApplication::translate("MainWindowClass", "\305\232cie\305\274ka", nullptr));
        recordButton->setText(QApplication::translate("MainWindowClass", "Nagrywaj", nullptr));
        saveTrackButton->setText(QApplication::translate("MainWindowClass", "Zapisz baz\304\231", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindowClass", "Klasyfikacja", nullptr));
        exrButton->setText(QApplication::translate("MainWindowClass", "OpenEXR", nullptr));
        groupBox_8->setTitle(QApplication::translate("MainWindowClass", "Logger", nullptr));
        debugLog->setText(QString());
        groupBox_5->setTitle(QApplication::translate("MainWindowClass", "GroupBox", nullptr));
        depthImageBox->setText(QApplication::translate("MainWindowClass", "TextLabel", nullptr));
        groupBox_6->setTitle(QApplication::translate("MainWindowClass", "GroupBox", nullptr));
        handImageBox->setText(QApplication::translate("MainWindowClass", "TextLabel", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindowClass", "GroupBox", nullptr));
        skeletonImageBox->setText(QApplication::translate("MainWindowClass", "TextLabel", nullptr));
        groupBox_7->setTitle(QApplication::translate("MainWindowClass", "GroupBox", nullptr));
        matchedImageBox->setText(QApplication::translate("MainWindowClass", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
