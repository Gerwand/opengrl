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
    QSpacerItem *verticalSpacer;
    QFrame *frame_2;
    QFormLayout *formLayout;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_7;
    QLabel *labBegin;
    QLabel *labEnd;
    QLabel *labPoints;
    QLabel *labLen;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *trackPreview;
    QFrame *frame;
    QHBoxLayout *_2;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SaveTrackDialog)
    {
        if (SaveTrackDialog->objectName().isEmpty())
            SaveTrackDialog->setObjectName(QString::fromUtf8("SaveTrackDialog"));
        SaveTrackDialog->resize(878, 540);
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
        inTrackName->setAcceptDrops(true);
        inTrackName->setInputMethodHints(Qt::ImhNone);
        inTrackName->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        inTrackName->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        inTrackName->setAcceptRichText(false);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, inTrackName);

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

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(1, QFormLayout::LabelRole, horizontalSpacer);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(2, QFormLayout::LabelRole, horizontalSpacer_4);

        label_3 = new QLabel(frame_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(4, QFormLayout::LabelRole, horizontalSpacer_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(5, QFormLayout::LabelRole, horizontalSpacer_5);

        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_4);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(7, QFormLayout::LabelRole, horizontalSpacer_3);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(8, QFormLayout::LabelRole, horizontalSpacer_6);

        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(9, QFormLayout::LabelRole, label_5);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(10, QFormLayout::LabelRole, horizontalSpacer_7);

        labBegin = new QLabel(frame_2);
        labBegin->setObjectName(QString::fromUtf8("labBegin"));

        formLayout->setWidget(1, QFormLayout::FieldRole, labBegin);

        labEnd = new QLabel(frame_2);
        labEnd->setObjectName(QString::fromUtf8("labEnd"));

        formLayout->setWidget(4, QFormLayout::FieldRole, labEnd);

        labPoints = new QLabel(frame_2);
        labPoints->setObjectName(QString::fromUtf8("labPoints"));

        formLayout->setWidget(7, QFormLayout::FieldRole, labPoints);

        labLen = new QLabel(frame_2);
        labLen->setObjectName(QString::fromUtf8("labLen"));

        formLayout->setWidget(10, QFormLayout::FieldRole, labLen);


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
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 601, 434));
        horizontalLayout_3 = new QHBoxLayout(scrollAreaWidgetContents_2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        trackPreview = new QLabel(scrollAreaWidgetContents_2);
        trackPreview->setObjectName(QString::fromUtf8("trackPreview"));
        trackPreview->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(trackPreview);

        scrollArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_2->addWidget(scrollArea);


        horizontalLayout->addWidget(groupBox);


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
        label_2->setText(QApplication::translate("SaveTrackDialog", "Pocz\304\205tek(L/P):", nullptr));
        label_3->setText(QApplication::translate("SaveTrackDialog", "Koniec(L/P):", nullptr));
        label_4->setText(QApplication::translate("SaveTrackDialog", "Punkty(L/P):", nullptr));
        label_5->setText(QApplication::translate("SaveTrackDialog", "D\305\202ugo\305\233\304\207:(L/P)", nullptr));
        labBegin->setText(QString());
        labEnd->setText(QString());
        labPoints->setText(QString());
        labLen->setText(QString());
        groupBox->setTitle(QApplication::translate("SaveTrackDialog", "\305\232cie\305\274ka", nullptr));
        trackPreview->setText(QString());
        okButton->setText(QApplication::translate("SaveTrackDialog", "Zapisz", nullptr));
        cancelButton->setText(QApplication::translate("SaveTrackDialog", "Anuluj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveTrackDialog: public Ui_SaveTrackDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVETRACKDIALOG_H
