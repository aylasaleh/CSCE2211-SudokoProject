/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *digitButtonLayout;
    QPushButton *digitButton1;
    QPushButton *digitButton2;
    QPushButton *digitButton3;
    QPushButton *digitButton4;
    QPushButton *digitButton5;
    QPushButton *digitButton6;
    QPushButton *digitButton7;
    QPushButton *digitButton8;
    QPushButton *digitButton9;
    QTableWidget *sudokuTable;
    QHBoxLayout *buttonLayout;
    QPushButton *loadButton;
    QPushButton *checkButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(400, 450);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        digitButtonLayout = new QHBoxLayout();
        digitButtonLayout->setObjectName("digitButtonLayout");
        digitButton1 = new QPushButton(centralwidget);
        digitButton1->setObjectName("digitButton1");

        digitButtonLayout->addWidget(digitButton1);

        digitButton2 = new QPushButton(centralwidget);
        digitButton2->setObjectName("digitButton2");

        digitButtonLayout->addWidget(digitButton2);

        digitButton3 = new QPushButton(centralwidget);
        digitButton3->setObjectName("digitButton3");

        digitButtonLayout->addWidget(digitButton3);

        digitButton4 = new QPushButton(centralwidget);
        digitButton4->setObjectName("digitButton4");

        digitButtonLayout->addWidget(digitButton4);

        digitButton5 = new QPushButton(centralwidget);
        digitButton5->setObjectName("digitButton5");

        digitButtonLayout->addWidget(digitButton5);

        digitButton6 = new QPushButton(centralwidget);
        digitButton6->setObjectName("digitButton6");

        digitButtonLayout->addWidget(digitButton6);

        digitButton7 = new QPushButton(centralwidget);
        digitButton7->setObjectName("digitButton7");

        digitButtonLayout->addWidget(digitButton7);

        digitButton8 = new QPushButton(centralwidget);
        digitButton8->setObjectName("digitButton8");

        digitButtonLayout->addWidget(digitButton8);

        digitButton9 = new QPushButton(centralwidget);
        digitButton9->setObjectName("digitButton9");

        digitButtonLayout->addWidget(digitButton9);


        verticalLayout->addLayout(digitButtonLayout);

        sudokuTable = new QTableWidget(centralwidget);
        sudokuTable->setObjectName("sudokuTable");
        sudokuTable->setGeometry(QRect(20, 20, 400, 400));
        sudokuTable->setRowCount(9);
        sudokuTable->setColumnCount(9);
       // sudokuTable->setHorizontalHeaderVisible(false);
      //  sudokuTable->setVerticalHeaderVisible(false);
        sudokuTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        sudokuTable->setSelectionMode(QAbstractItemView::NoSelection);

        verticalLayout->addWidget(sudokuTable);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        loadButton = new QPushButton(centralwidget);
        loadButton->setObjectName("loadButton");

        buttonLayout->addWidget(loadButton);

        checkButton = new QPushButton(centralwidget);
        checkButton->setObjectName("checkButton");

        buttonLayout->addWidget(checkButton);


        verticalLayout->addLayout(buttonLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Sudoku Game", nullptr));
        digitButton1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        digitButton2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        digitButton3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        digitButton4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        digitButton5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        digitButton6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        digitButton7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        digitButton8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        digitButton9->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "Load Puzzle", nullptr));
        checkButton->setText(QCoreApplication::translate("MainWindow", "Check Solution", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
