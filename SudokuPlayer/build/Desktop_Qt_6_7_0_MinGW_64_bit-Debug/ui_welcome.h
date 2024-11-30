/********************************************************************************
** Form generated from reading UI file 'welcome.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOME_H
#define UI_WELCOME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Welcome
{
public:
    QPushButton *easyB;
    QPushButton *medB;
    QPushButton *hardB;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *Welcome)
    {
        if (Welcome->objectName().isEmpty())
            Welcome->setObjectName("Welcome");
        Welcome->resize(400, 300);
        easyB = new QPushButton(Welcome);
        easyB->setObjectName("easyB");
        easyB->setGeometry(QRect(20, 150, 100, 32));
        medB = new QPushButton(Welcome);
        medB->setObjectName("medB");
        medB->setGeometry(QRect(150, 150, 100, 32));
        hardB = new QPushButton(Welcome);
        hardB->setObjectName("hardB");
        hardB->setGeometry(QRect(270, 150, 100, 32));
        label = new QLabel(Welcome);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 60, 161, 21));
        label_2 = new QLabel(Welcome);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 100, 291, 16));

        retranslateUi(Welcome);

        QMetaObject::connectSlotsByName(Welcome);
    } // setupUi

    void retranslateUi(QWidget *Welcome)
    {
        Welcome->setWindowTitle(QCoreApplication::translate("Welcome", "Form", nullptr));
        easyB->setText(QCoreApplication::translate("Welcome", "Easy", nullptr));
        medB->setText(QCoreApplication::translate("Welcome", "Medium", nullptr));
        hardB->setText(QCoreApplication::translate("Welcome", "Hard", nullptr));
        label->setText(QCoreApplication::translate("Welcome", "Welcome to Sudoku Solver", nullptr));
        label_2->setText(QCoreApplication::translate("Welcome", "Please choose a diffiuclty level to start playing :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Welcome: public Ui_Welcome {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOME_H
