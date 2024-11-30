#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcome.h"
#include "sudoku.h"
#include <QTableWidgetItem>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), currentDigit(0) //initialize current digit to 0 (no selection)
{
    ui->setupUi(this);

    //set up the table dimensions
    ui->sudokuTable->setRowCount(9);
    ui->sudokuTable->setColumnCount(9);



    //connect digit buttons to slot
    connect(ui->digitButton1, &QPushButton::clicked, this, [this]() { setDigit(1); });
    connect(ui->digitButton2, &QPushButton::clicked, this, [this]() { setDigit(2); });
    connect(ui->digitButton3, &QPushButton::clicked, this, [this]() { setDigit(3); });
    connect(ui->digitButton4, &QPushButton::clicked, this, [this]() { setDigit(4); });
    connect(ui->digitButton5, &QPushButton::clicked, this, [this]() { setDigit(5); });
    connect(ui->digitButton6, &QPushButton::clicked, this, [this]() { setDigit(6); });
    connect(ui->digitButton7, &QPushButton::clicked, this, [this]() { setDigit(7); });
    connect(ui->digitButton8, &QPushButton::clicked, this, [this]() { setDigit(8); });
    connect(ui->digitButton9, &QPushButton::clicked, this, [this]() { setDigit(9); });

    //connect table cell click to insert the digit
    connect(ui->sudokuTable, &QTableWidget::cellClicked, this, &MainWindow::onCellClicked);

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadPuzzle);
    connect(ui->autoCheckCheckBox, &QCheckBox::toggled, this, &MainWindow::onAutoCheckToggled);

    this->setFixedSize(486, 632);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getPuzzle (sudoku p)
{
    puzzle = p;
    onLoadPuzzle();
}

void MainWindow::setDigit(int digit)
{
    currentDigit = digit;
    //qDebug() << "Selected digit:" << currentDigit;
}

void MainWindow::onAutoCheckToggled(bool checked)
{
    autoCheckEnabled = checked;
}

void MainWindow::on_eraseB_clicked()
{
    currentDigit = 0;
}

void MainWindow::onCellClicked(int row, int col)
{
    //check if the current cell is editable (empty)
    QTableWidgetItem *item = ui->sudokuTable->item(row, col);
    if (!item) {
        item = new QTableWidgetItem();
        ui->sudokuTable->setItem(row, col, item);
    }

    //if the item is not fixed, allow the user to set the digit
    if (item->flags() & Qt::ItemIsEditable) {
        if (currentDigit != 0) {
            item->setText(QString::number(currentDigit));  //set the digit
            QFont font = item->font();
            font.setBold(false); //non-fixed cells not bold unless error
            item->setFont(font);
            item->setForeground(QBrush(QColor(Qt::black)));
            if (autoCheckEnabled) {
                onCheckSolution();  //automatically check after each entry
            }
        } else
            item ->setText("");//erasing
    } else {
        qDebug() << "Cannot edit fixed cells.";
    }
}

void MainWindow::onLoadPuzzle()
{
    //clear the existing table
    ui->sudokuTable->clear();

    //set up the table dimensions again, just in case
    ui->sudokuTable->setRowCount(9);
    ui->sudokuTable->setColumnCount(9);
    ui -> sudokuTable->setStyleSheet("background-color: white;");

    //get the Sudoku grid
    const auto& grid = puzzle.getGrid();

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            //qDebug() << "Grid[" << i << "][" << j << "] = " << grid[i][j].num;

            //populate table
            int value = grid[i][j].num;
            QTableWidgetItem* item = new QTableWidgetItem(value == 0 ? "" : QString::number(value));

            QFont font = item->font();
            font.setPointSize(14);  // set font size to 14
            if(grid[i][j].isFixed){
                font.setBold(true);  //bold fixed cells
                item->setForeground(QBrush(QColorConstants::Svg::purple));
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); //make non-editable
            } else {
                item->setFlags(item->flags() | Qt::ItemIsEditable); //allow editing
            }

            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);  //center text

            ui->sudokuTable->setItem(i, j, item);

            if ((i / 3) % 2 == 0 && (j / 3) % 2 == 0) {
                item->setBackground(QBrush(QColor(230, 230, 250)));
            }    //sets the boxes colors
            item->setForeground(QBrush(QColorConstants::Svg::purple));  // set font color of non-editable
        }
    }

    ui->sudokuTable->horizontalHeader()->setVisible(false);
    ui->sudokuTable->verticalHeader()->setVisible(false);
    ui->sudokuTable->setShowGrid(true);

    for (int i = 0; i < 9; ++i) {
        ui->sudokuTable->setColumnWidth(i, 51);
        ui->sudokuTable->setRowHeight(i, 51);
    }

    qDebug() << "Loaded a new Sudoku puzzle.";

    // solve the sudoko
    puzzle.solve();

    //copy solved sudoko
    const auto& sudokosolved = puzzle.getGrid();
    puzzle.print();

    //copy answers of sudoko into an array
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            solvedsudoko[i][j] = sudokosolved[i][j].num;
        }

    }
}

void MainWindow::onCheckSolution()
{
    bool isComplete = true;
    bool valid = true;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QTableWidgetItem* item = ui->sudokuTable->item(i, j);
            if (!item) continue;

            int userValue = item->text().isEmpty() ? 0 : item->text().toInt();
            bool isFixed = puzzle.getGrid()[i][j].isFixed;

            if (isFixed) {
                //reset fixed cells to purple bold
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QBrush(QColorConstants::Svg::purple));
            } else {
                //editable cells
                if (userValue != 0 && userValue != solvedsudoko[i][j]) {
                    //incorrect value
                    valid = false;
                    QFont font = item->font();
                    font.setBold(true);
                    item->setFont(font);
                    item->setForeground(QBrush(QColor(Qt::red)));
                } else {
                    //correct or empty value
                    QFont font = item->font();
                    font.setBold(false);
                    item->setFont(font);
                    item->setForeground(userValue == 0 ? QBrush(QColor(Qt::black)) : QBrush(QColorConstants::Svg::purple));
                }
            }

            if (userValue == 0) {
                isComplete = false;  //puzzle is incomplete
            }
        }
    }

    if (autoCheckEnabled) {
        return;
    } else {
        // If auto-check is disabled, check for both completion and validity
        if (valid && isComplete) {
            QMessageBox::information(this, "Success", "Congratulations, you solved the Sudoku!");
        } else if (!isComplete) {
            QMessageBox::warning(this, "Incomplete", "The puzzle is not complete.");
        } else {
            QMessageBox::warning(this, "Invalid", "The solution is invalid.");
        }
    }
}

void MainWindow::on_checkButton_clicked()
{
    onCheckSolution();
}


void MainWindow::on_loadButton_clicked()
{
    emit backToWelcome(); // emit signal to the connect in the welcome pages
    this->close();        // close mainwindow
}




