/********************************************************************************
** Form generated from reading UI file 'SaveTrackDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVETRACKDIALOG_H
#define UI_SAVETRACKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
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

class Ui_SaveTrackDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_4;
    QFormLayout *formLayout_2;
    QLabel *label;
    QTextEdit *inTrackName;
    QCheckBox *checkSaveLeft;
    QCheckBox *checkSaveRight;
    QLabel *label_6;
    QSpacerItem *verticalSpacer;
    QFrame *frame_2;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLabel *labLeftBegin;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_3;
    QLabel *labLeftEnd;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_4;
    QLabel *labLeftPoints;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_5;
    QLabel *labLeftLen;
    QSpacerItem *horizontalSpacer_7;
    QLabel *labRightBegin;
    QLabel *labRightEnd;
    QLabel *labRightPoints;
    QLabel *labRightLen;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *leftTrackPreview;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *rightTrackPreview;
    QFrame *frame;
    QHBoxLayout *_2;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SaveTrackDialog)
    {
        if (SaveTrackDialog->objectName().isEmpty())
            SaveTrackDialog->setObjectName(QString::fromUtf8("SaveTrackDialog"));
        SaveTrackDialog->resize(1360, 540);
        SaveTrackDialog->setMinimumSize(QSize(790, 540));
        SaveTrackDialog->setMaximumSize(QSize(1360, 540));
        SaveTrackDialog->setModal(true);
        verticalLayout = new QVBoxLayout(SaveTrackDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        frame_3 = new QFrame(SaveTrackDialog);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setMaximumSize(QSize(220, 16777215));
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

        inTrackName = new QTextEdit(frame_4);
        inTrackName->setObjectName(QString::fromUtf8("inTrackName"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(inTrackName->sizePolicy().hasHeightForWidth());
        inTrackName->setSizePolicy(sizePolicy);
        inTrackName->setMinimumSize(QSize(0, 0));
        inTrackName->setMaximumSize(QSize(16777215, 25));
        inTrackName->setAcceptRichText(false);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, inTrackName);

        checkSaveLeft = new QCheckBox(frame_4);
        checkSaveLeft->setObjectName(QString::fromUtf8("checkSaveLeft"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, checkSaveLeft);

        checkSaveRight = new QCheckBox(frame_4);
        checkSaveRight->setObjectName(QString::fromUtf8("checkSaveRight"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, checkSaveRight);

        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        formLayout_2->setItem(1, QFormLayout::LabelRole, verticalSpacer);


        verticalLayout_3->addWidget(frame_4);

        frame_2 = new QFrame(frame_3);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(frame_2);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        formLayout->setVerticalSpacing(9);
        formLayout->setContentsMargins(-1, 9, -1, -1);
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        labLeftBegin = new QLabel(frame_2);
        labLeftBegin->setObjectName(QString::fromUtf8("labLeftBegin"));

        formLayout->setWidget(0, QFormLayout::FieldRole, labLeftBegin);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(1, QFormLayout::LabelRole, horizontalSpacer);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(2, QFormLayout::LabelRole, horizontalSpacer_4);

        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

        labLeftEnd = new QLabel(frame_2);
        labLeftEnd->setObjectName(QString::fromUtf8("labLeftEnd"));

        formLayout->setWidget(3, QFormLayout::FieldRole, labLeftEnd);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(4, QFormLayout::LabelRole, horizontalSpacer_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(5, QFormLayout::LabelRole, horizontalSpacer_5);

        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_4);

        labLeftPoints = new QLabel(frame_2);
        labLeftPoints->setObjectName(QString::fromUtf8("labLeftPoints"));

        formLayout->setWidget(6, QFormLayout::FieldRole, labLeftPoints);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(7, QFormLayout::LabelRole, horizontalSpacer_3);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(8, QFormLayout::LabelRole, horizontalSpacer_6);

        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(9, QFormLayout::LabelRole, label_5);

        labLeftLen = new QLabel(frame_2);
        labLeftLen->setObjectName(QString::fromUtf8("labLeftLen"));

        formLayout->setWidget(9, QFormLayout::FieldRole, labLeftLen);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(10, QFormLayout::LabelRole, horizontalSpacer_7);

        labRightBegin = new QLabel(frame_2);
        labRightBegin->setObjectName(QString::fromUtf8("labRightBegin"));

        formLayout->setWidget(1, QFormLayout::FieldRole, labRightBegin);

        labRightEnd = new QLabel(frame_2);
        labRightEnd->setObjectName(QString::fromUtf8("labRightEnd"));

        formLayout->setWidget(4, QFormLayout::FieldRole, labRightEnd);

        labRightPoints = new QLabel(frame_2);
        labRightPoints->setObjectName(QString::fromUtf8("labRightPoints"));

        formLayout->setWidget(7, QFormLayout::FieldRole, labRightPoints);

        labRightLen = new QLabel(frame_2);
        labRightLen->setObjectName(QString::fromUtf8("labRightLen"));

        formLayout->setWidget(10, QFormLayout::FieldRole, labRightLen);


        verticalLayout_3->addWidget(frame_2);


        horizontalLayout->addWidget(frame_3);

        groupBox = new QGroupBox(SaveTrackDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        scrollArea = new QScrollArea(groupBox);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 532, 446));
        horizontalLayout_3 = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        leftTrackPreview = new QLabel(scrollAreaWidgetContents_2);
        leftTrackPreview->setObjectName(QString::fromUtf8("leftTrackPreview"));
        leftTrackPreview->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(leftTrackPreview);

        scrollArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_2->addWidget(scrollArea);


        horizontalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(SaveTrackDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        scrollArea_2 = new QScrollArea(groupBox_2);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 532, 446));
        horizontalLayout_2 = new QHBoxLayout(scrollAreaWidgetContents_3);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        rightTrackPreview = new QLabel(scrollAreaWidgetContents_3);
        rightTrackPreview->setObjectName(QString::fromUtf8("rightTrackPreview"));
        rightTrackPreview->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(rightTrackPreview);

        scrollArea_2->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_4->addWidget(scrollArea_2);


        horizontalLayout->addWidget(groupBox_2);


        verticalLayout->addLayout(horizontalLayout);

        frame = new QFrame(SaveTrackDialog);
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


        verticalLayout->addWidget(frame);


        retranslateUi(SaveTrackDialog);

        QMetaObject::connectSlotsByName(SaveTrackDialog);
    } // setupUi

    void retranslateUi(QDialog *SaveTrackDialog)
    {
        SaveTrackDialog->setWindowTitle(QApplication::translate("SaveTrackDialog", "Zapis \305\233ciezki", nullptr));
        label->setText(QApplication::translate("SaveTrackDialog", "Nazwa", nullptr));
        checkSaveLeft->setText(QApplication::translate("SaveTrackDialog", "Lewa trasa", nullptr));
        checkSaveRight->setText(QApplication::translate("SaveTrackDialog", "Prawa trasa", nullptr));
        label_6->setText(QApplication::translate("SaveTrackDialog", "Zapis do\n"
"bazy", nullptr));
        label_2->setText(QApplication::translate("SaveTrackDialog", "Pocz\304\205tek(L/P):", nullptr));
        labLeftBegin->setText(QString());
        label_3->setText(QApplication::translate("SaveTrackDialog", "Koniec(L/P):", nullptr));
        labLeftEnd->setText(QString());
        label_4->setText(QApplication::translate("SaveTrackDialog", "Punkty(L/P):", nullptr));
        labLeftPoints->setText(QString());
        label_5->setText(QApplication::translate("SaveTrackDialog", "D\305\202ugo\305\233\304\207:(L/P)", nullptr));
        labLeftLen->setText(QString());
        labRightBegin->setText(QString());
        labRightEnd->setText(QString());
        labRightPoints->setText(QString());
        labRightLen->setText(QString());
        groupBox->setTitle(QApplication::translate("SaveTrackDialog", "Lewa \305\233ciezka", nullptr));
        leftTrackPreview->setText(QString());
        groupBox_2->setTitle(QApplication::translate("SaveTrackDialog", "Prawa \305\233cie\305\274ka", nullptr));
        rightTrackPreview->setText(QString());
        okButton->setText(QApplication::translate("SaveTrackDialog", "Zapisz", nullptr));
        cancelButton->setText(QApplication::translate("SaveTrackDialog", "Anuluj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveTrackDialog: public Ui_SaveTrackDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVETRACKDIALOG_H
