/********************************************************************************
** Form generated from reading UI file 'SaveGestureDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEGESTUREDIALOG_H
#define UI_SAVEGESTUREDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SaveGestureDialog
{
public:
    QGridLayout *gridLayout_4;
    QFrame *frame;
    QHBoxLayout *_2;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_4;
    QFormLayout *formLayout_2;
    QLabel *label;
    QTextEdit *inGestureName;
    QSpacerItem *verticalSpacer;
    QFrame *frame_2;
    QGridLayout *gridLayout;
    QCheckBox *checkHand2;
    QCheckBox *checkHand5;
    QCheckBox *checkHand6;
    QCheckBox *checkHand1;
    QCheckBox *checkHand4;
    QCheckBox *checkHand3;
    QCheckBox *checkHand7;
    QCheckBox *checkHand8;
    QCheckBox *checkHand9;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QScrollArea *scrollArea_6;
    QWidget *scrollAreaWidgetContents_11;
    QHBoxLayout *horizontalLayout_7;
    QLabel *handPreview9;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *handPreview8;
    QScrollArea *scrollArea_5;
    QWidget *scrollAreaWidgetContents_10;
    QHBoxLayout *horizontalLayout_6;
    QLabel *handPreview3;
    QScrollArea *scrollArea_7;
    QWidget *scrollAreaWidgetContents_12;
    QHBoxLayout *horizontalLayout_9;
    QLabel *handPreview7;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_7;
    QHBoxLayout *horizontalLayout_2;
    QLabel *handPreview2;
    QLabel *handPreview1_4;
    QScrollArea *scrollArea_8;
    QWidget *scrollAreaWidgetContents_13;
    QHBoxLayout *horizontalLayout_8;
    QLabel *handPreview4;
    QScrollArea *scrollArea_4;
    QWidget *scrollAreaWidgetContents_9;
    QHBoxLayout *horizontalLayout_5;
    QLabel *handPreview5;
    QScrollArea *scrollArea_9;
    QWidget *scrollAreaWidgetContents_15;
    QGridLayout *gridLayout_3;
    QLabel *handPreview1;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_8;
    QHBoxLayout *horizontalLayout_4;
    QLabel *handPreview6;

    void setupUi(QDialog *SaveGestureDialog)
    {
        if (SaveGestureDialog->objectName().isEmpty())
            SaveGestureDialog->setObjectName(QString::fromUtf8("SaveGestureDialog"));
        SaveGestureDialog->resize(951, 600);
        SaveGestureDialog->setMinimumSize(QSize(790, 600));
        SaveGestureDialog->setMaximumSize(QSize(1360, 600));
        SaveGestureDialog->setModal(true);
        gridLayout_4 = new QGridLayout(SaveGestureDialog);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        frame = new QFrame(SaveGestureDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMaximumSize(QSize(16777215, 40));
        _2 = new QHBoxLayout(frame);
        _2->setSpacing(6);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setSizeConstraint(QLayout::SetDefaultConstraint);
        _2->setContentsMargins(1, 1, 1, 1);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _2->addItem(spacerItem);

        okButton = new QPushButton(frame);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        _2->addWidget(okButton);

        cancelButton = new QPushButton(frame);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        _2->addWidget(cancelButton);


        gridLayout_4->addWidget(frame, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame_3 = new QFrame(SaveGestureDialog);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setMaximumSize(QSize(260, 16777215));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame_4 = new QFrame(frame_3);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setMaximumSize(QSize(16777215, 16777215));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        formLayout_2 = new QFormLayout(frame_4);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label = new QLabel(frame_4);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        inGestureName = new QTextEdit(frame_4);
        inGestureName->setObjectName(QString::fromUtf8("inGestureName"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(inGestureName->sizePolicy().hasHeightForWidth());
        inGestureName->setSizePolicy(sizePolicy);
        inGestureName->setMinimumSize(QSize(0, 0));
        inGestureName->setMaximumSize(QSize(16777215, 25));
        inGestureName->setAcceptDrops(true);
        inGestureName->setInputMethodHints(Qt::ImhNone);
        inGestureName->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        inGestureName->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        inGestureName->setAcceptRichText(false);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, inGestureName);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        formLayout_2->setItem(1, QFormLayout::LabelRole, verticalSpacer);


        verticalLayout_3->addWidget(frame_4);

        frame_2 = new QFrame(frame_3);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        checkHand2 = new QCheckBox(frame_2);
        checkHand2->setObjectName(QString::fromUtf8("checkHand2"));

        gridLayout->addWidget(checkHand2, 0, 1, 1, 1);

        checkHand5 = new QCheckBox(frame_2);
        checkHand5->setObjectName(QString::fromUtf8("checkHand5"));

        gridLayout->addWidget(checkHand5, 1, 1, 1, 1);

        checkHand6 = new QCheckBox(frame_2);
        checkHand6->setObjectName(QString::fromUtf8("checkHand6"));

        gridLayout->addWidget(checkHand6, 1, 2, 1, 1);

        checkHand1 = new QCheckBox(frame_2);
        checkHand1->setObjectName(QString::fromUtf8("checkHand1"));

        gridLayout->addWidget(checkHand1, 0, 0, 1, 1);

        checkHand4 = new QCheckBox(frame_2);
        checkHand4->setObjectName(QString::fromUtf8("checkHand4"));

        gridLayout->addWidget(checkHand4, 1, 0, 1, 1);

        checkHand3 = new QCheckBox(frame_2);
        checkHand3->setObjectName(QString::fromUtf8("checkHand3"));

        gridLayout->addWidget(checkHand3, 0, 2, 1, 1);

        checkHand7 = new QCheckBox(frame_2);
        checkHand7->setObjectName(QString::fromUtf8("checkHand7"));

        gridLayout->addWidget(checkHand7, 9, 0, 1, 1);

        checkHand8 = new QCheckBox(frame_2);
        checkHand8->setObjectName(QString::fromUtf8("checkHand8"));

        gridLayout->addWidget(checkHand8, 9, 1, 1, 1);

        checkHand9 = new QCheckBox(frame_2);
        checkHand9->setObjectName(QString::fromUtf8("checkHand9"));

        gridLayout->addWidget(checkHand9, 9, 2, 1, 1);


        verticalLayout_3->addWidget(frame_2);


        horizontalLayout->addWidget(frame_3);

        groupBox = new QGroupBox(SaveGestureDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        scrollArea_6 = new QScrollArea(groupBox);
        scrollArea_6->setObjectName(QString::fromUtf8("scrollArea_6"));
        scrollArea_6->setWidgetResizable(true);
        scrollAreaWidgetContents_11 = new QWidget();
        scrollAreaWidgetContents_11->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_11"));
        scrollAreaWidgetContents_11->setGeometry(QRect(0, 0, 201, 159));
        horizontalLayout_7 = new QHBoxLayout(scrollAreaWidgetContents_11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        handPreview9 = new QLabel(scrollAreaWidgetContents_11);
        handPreview9->setObjectName(QString::fromUtf8("handPreview9"));
        handPreview9->setAlignment(Qt::AlignCenter);

        horizontalLayout_7->addWidget(handPreview9);

        scrollArea_6->setWidget(scrollAreaWidgetContents_11);

        gridLayout_2->addWidget(scrollArea_6, 2, 2, 1, 1);

        scrollArea = new QScrollArea(groupBox);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 201, 159));
        horizontalLayout_3 = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        handPreview8 = new QLabel(scrollAreaWidgetContents_2);
        handPreview8->setObjectName(QString::fromUtf8("handPreview8"));
        handPreview8->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(handPreview8);

        scrollArea->setWidget(scrollAreaWidgetContents_2);

        gridLayout_2->addWidget(scrollArea, 2, 1, 1, 1);

        scrollArea_5 = new QScrollArea(groupBox);
        scrollArea_5->setObjectName(QString::fromUtf8("scrollArea_5"));
        scrollArea_5->setWidgetResizable(true);
        scrollAreaWidgetContents_10 = new QWidget();
        scrollAreaWidgetContents_10->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_10"));
        scrollAreaWidgetContents_10->setGeometry(QRect(0, 0, 201, 159));
        horizontalLayout_6 = new QHBoxLayout(scrollAreaWidgetContents_10);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        handPreview3 = new QLabel(scrollAreaWidgetContents_10);
        handPreview3->setObjectName(QString::fromUtf8("handPreview3"));
        handPreview3->setAlignment(Qt::AlignCenter);

        horizontalLayout_6->addWidget(handPreview3);

        scrollArea_5->setWidget(scrollAreaWidgetContents_10);

        gridLayout_2->addWidget(scrollArea_5, 0, 2, 1, 1);

        scrollArea_7 = new QScrollArea(groupBox);
        scrollArea_7->setObjectName(QString::fromUtf8("scrollArea_7"));
        scrollArea_7->setWidgetResizable(true);
        scrollAreaWidgetContents_12 = new QWidget();
        scrollAreaWidgetContents_12->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_12"));
        scrollAreaWidgetContents_12->setGeometry(QRect(0, 0, 201, 159));
        horizontalLayout_9 = new QHBoxLayout(scrollAreaWidgetContents_12);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        handPreview7 = new QLabel(scrollAreaWidgetContents_12);
        handPreview7->setObjectName(QString::fromUtf8("handPreview7"));
        handPreview7->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(handPreview7);

        scrollArea_7->setWidget(scrollAreaWidgetContents_12);

        gridLayout_2->addWidget(scrollArea_7, 2, 0, 1, 1);

        scrollArea_2 = new QScrollArea(groupBox);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_7 = new QWidget();
        scrollAreaWidgetContents_7->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_7"));
        scrollAreaWidgetContents_7->setGeometry(QRect(0, 0, 201, 159));
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents_7);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        handPreview2 = new QLabel(scrollAreaWidgetContents_7);
        handPreview2->setObjectName(QString::fromUtf8("handPreview2"));
        handPreview2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(handPreview2);

        scrollArea_2->setWidget(scrollAreaWidgetContents_7);

        gridLayout_2->addWidget(scrollArea_2, 0, 1, 1, 1);

        handPreview1_4 = new QLabel(groupBox);
        handPreview1_4->setObjectName(QString::fromUtf8("handPreview1_4"));
        handPreview1_4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(handPreview1_4, 0, 3, 1, 1);

        scrollArea_8 = new QScrollArea(groupBox);
        scrollArea_8->setObjectName(QString::fromUtf8("scrollArea_8"));
        scrollArea_8->setWidgetResizable(true);
        scrollAreaWidgetContents_13 = new QWidget();
        scrollAreaWidgetContents_13->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_13"));
        scrollAreaWidgetContents_13->setGeometry(QRect(0, 0, 201, 158));
        horizontalLayout_8 = new QHBoxLayout(scrollAreaWidgetContents_13);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        handPreview4 = new QLabel(scrollAreaWidgetContents_13);
        handPreview4->setObjectName(QString::fromUtf8("handPreview4"));
        handPreview4->setAlignment(Qt::AlignCenter);

        horizontalLayout_8->addWidget(handPreview4);

        scrollArea_8->setWidget(scrollAreaWidgetContents_13);

        gridLayout_2->addWidget(scrollArea_8, 1, 0, 1, 1);

        scrollArea_4 = new QScrollArea(groupBox);
        scrollArea_4->setObjectName(QString::fromUtf8("scrollArea_4"));
        scrollArea_4->setWidgetResizable(true);
        scrollAreaWidgetContents_9 = new QWidget();
        scrollAreaWidgetContents_9->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_9"));
        scrollAreaWidgetContents_9->setGeometry(QRect(0, 0, 201, 158));
        horizontalLayout_5 = new QHBoxLayout(scrollAreaWidgetContents_9);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        handPreview5 = new QLabel(scrollAreaWidgetContents_9);
        handPreview5->setObjectName(QString::fromUtf8("handPreview5"));
        handPreview5->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(handPreview5);

        scrollArea_4->setWidget(scrollAreaWidgetContents_9);

        gridLayout_2->addWidget(scrollArea_4, 1, 1, 1, 1);

        scrollArea_9 = new QScrollArea(groupBox);
        scrollArea_9->setObjectName(QString::fromUtf8("scrollArea_9"));
        scrollArea_9->setWidgetResizable(true);
        scrollAreaWidgetContents_15 = new QWidget();
        scrollAreaWidgetContents_15->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_15"));
        scrollAreaWidgetContents_15->setGeometry(QRect(0, 0, 201, 159));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents_15);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        handPreview1 = new QLabel(scrollAreaWidgetContents_15);
        handPreview1->setObjectName(QString::fromUtf8("handPreview1"));
        handPreview1->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(handPreview1, 0, 0, 1, 1);

        scrollArea_9->setWidget(scrollAreaWidgetContents_15);

        gridLayout_2->addWidget(scrollArea_9, 0, 0, 1, 1);

        scrollArea_3 = new QScrollArea(groupBox);
        scrollArea_3->setObjectName(QString::fromUtf8("scrollArea_3"));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_8 = new QWidget();
        scrollAreaWidgetContents_8->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_8"));
        scrollAreaWidgetContents_8->setGeometry(QRect(0, 0, 201, 158));
        horizontalLayout_4 = new QHBoxLayout(scrollAreaWidgetContents_8);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        handPreview6 = new QLabel(scrollAreaWidgetContents_8);
        handPreview6->setObjectName(QString::fromUtf8("handPreview6"));
        handPreview6->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(handPreview6);

        scrollArea_3->setWidget(scrollAreaWidgetContents_8);

        gridLayout_2->addWidget(scrollArea_3, 1, 2, 1, 1);


        horizontalLayout->addWidget(groupBox);


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(SaveGestureDialog);

        QMetaObject::connectSlotsByName(SaveGestureDialog);
    } // setupUi

    void retranslateUi(QDialog *SaveGestureDialog)
    {
        SaveGestureDialog->setWindowTitle(QApplication::translate("SaveGestureDialog", "Zapis \305\233ciezki", nullptr));
        okButton->setText(QApplication::translate("SaveGestureDialog", "Zapisz", nullptr));
        cancelButton->setText(QApplication::translate("SaveGestureDialog", "Anuluj", nullptr));
        label->setText(QApplication::translate("SaveGestureDialog", "Nazwa", nullptr));
        checkHand2->setText(QApplication::translate("SaveGestureDialog", "2", nullptr));
        checkHand5->setText(QApplication::translate("SaveGestureDialog", "5", nullptr));
        checkHand6->setText(QApplication::translate("SaveGestureDialog", "6", nullptr));
        checkHand1->setText(QApplication::translate("SaveGestureDialog", "1", nullptr));
        checkHand4->setText(QApplication::translate("SaveGestureDialog", "4", nullptr));
        checkHand3->setText(QApplication::translate("SaveGestureDialog", "3", nullptr));
        checkHand7->setText(QApplication::translate("SaveGestureDialog", "7", nullptr));
        checkHand8->setText(QApplication::translate("SaveGestureDialog", "8", nullptr));
        checkHand9->setText(QApplication::translate("SaveGestureDialog", "9", nullptr));
        groupBox->setTitle(QApplication::translate("SaveGestureDialog", "Gest", nullptr));
        handPreview9->setText(QString());
        handPreview8->setText(QString());
        handPreview3->setText(QString());
        handPreview7->setText(QString());
        handPreview2->setText(QString());
        handPreview1_4->setText(QString());
        handPreview4->setText(QString());
        handPreview5->setText(QString());
        handPreview1->setText(QString());
        handPreview6->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SaveGestureDialog: public Ui_SaveGestureDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEGESTUREDIALOG_H
