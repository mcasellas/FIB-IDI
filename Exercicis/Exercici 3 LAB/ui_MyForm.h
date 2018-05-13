/********************************************************************************
** Form generated from reading UI file 'MyForm.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "MyLabel.h"

QT_BEGIN_NAMESPACE

class Ui_MyForm
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber_2;
    QHBoxLayout *horizontalLayout;
    QDial *dial_2;
    QDial *dial;
    QHBoxLayout *horizontalLayout_3;
    MyLabel *label;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QPushButton *pushButton;

    void setupUi(QWidget *MyForm)
    {
        if (MyForm->objectName().isEmpty())
            MyForm->setObjectName(QStringLiteral("MyForm"));
        MyForm->resize(568, 403);
        gridLayout = new QGridLayout(MyForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lcdNumber = new QLCDNumber(MyForm);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lcdNumber->sizePolicy().hasHeightForWidth());
        lcdNumber->setSizePolicy(sizePolicy);
        lcdNumber->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_2->addWidget(lcdNumber);

        lcdNumber_2 = new QLCDNumber(MyForm);
        lcdNumber_2->setObjectName(QStringLiteral("lcdNumber_2"));
        sizePolicy.setHeightForWidth(lcdNumber_2->sizePolicy().hasHeightForWidth());
        lcdNumber_2->setSizePolicy(sizePolicy);
        lcdNumber_2->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));

        horizontalLayout_2->addWidget(lcdNumber_2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        dial_2 = new QDial(MyForm);
        dial_2->setObjectName(QStringLiteral("dial_2"));
        dial_2->setMaximum(23);

        horizontalLayout->addWidget(dial_2);

        dial = new QDial(MyForm);
        dial->setObjectName(QStringLiteral("dial"));
        dial->setMaximum(59);

        horizontalLayout->addWidget(dial);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new MyLabel(MyForm);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
"color: rgb(255, 255, 255);"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        radioButton = new QRadioButton(MyForm);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        verticalLayout->addWidget(radioButton);

        radioButton_2 = new QRadioButton(MyForm);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setChecked(true);

        verticalLayout->addWidget(radioButton_2);

        pushButton = new QPushButton(MyForm);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(pushButton);


        horizontalLayout_3->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(MyForm);
        QObject::connect(dial, SIGNAL(valueChanged(int)), lcdNumber_2, SLOT(display(int)));
        QObject::connect(dial_2, SIGNAL(valueChanged(int)), lcdNumber, SLOT(display(int)));
        QObject::connect(radioButton, SIGNAL(toggled(bool)), label, SLOT(activarAlarma()));
        QObject::connect(label, SIGNAL(activaCheck(bool)), radioButton_2, SLOT(setChecked(bool)));
        QObject::connect(dial_2, SIGNAL(valueChanged(int)), label, SLOT(rebreHora(int)));
        QObject::connect(dial, SIGNAL(valueChanged(int)), label, SLOT(rebreMinut(int)));
        QObject::connect(pushButton, SIGNAL(clicked()), label, SLOT(reset()));
        QObject::connect(label, SIGNAL(assignaZero(QString)), lcdNumber, SLOT(display(QString)));
        QObject::connect(label, SIGNAL(assignaZero(QString)), lcdNumber_2, SLOT(display(QString)));
        QObject::connect(radioButton_2, SIGNAL(toggled(bool)), label, SLOT(desactivarAlarma()));

        QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
        MyForm->setWindowTitle(QApplication::translate("MyForm", "Form", nullptr));
        label->setText(QApplication::translate("MyForm", "Alarma Desactivada", nullptr));
        radioButton->setText(QApplication::translate("MyForm", "ON", nullptr));
        radioButton_2->setText(QApplication::translate("MyForm", "OFF", nullptr));
        pushButton->setText(QApplication::translate("MyForm", "Reset", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFORM_H
