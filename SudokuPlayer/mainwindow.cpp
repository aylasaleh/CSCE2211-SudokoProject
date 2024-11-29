#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sudoku.h"
#include <QTableWidgetItem>
#include <QDebug>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), currentDigit(0) //initialize current digit to 0 (no selection)
{
    ui->setupUi(this);

    //set up the table dimensions
    ui->sudokuTable->setRowCount(9);
    ui->sudokuTable->setColumnCount(9);

    onLoadPuzzle();


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
    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::onCheckSolution);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDigit(int digit)
{
    currentDigit = digit;
    qDebug() << "Selected digit:" << currentDigit;
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
        }
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

    //initialize a new Sudoku puzzle
    sudoku puzzle;
    puzzle.create(sudoku::Difficulty::Easy, 'A');

    //get the Sudoku grid
    const auto& grid = puzzle.getGrid();

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            qDebug() << "Grid[" << i << "][" << j << "] = " << grid[i][j].num;

            //populate table
            int value = grid[i][j].num;
            QTableWidgetItem* item = new QTableWidgetItem(value == 0 ? "" : QString::number(value));
            if (grid[i][j].isFixed) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); //make fixed numbers non-editable
            } else {
                item->setFlags(item->flags() | Qt::ItemIsEditable); //allow editing of empty cells
            }
            ui->sudokuTable->setItem(i, j, item);
        }
    }

    ui->sudokuTable->horizontalHeader()->setVisible(false);
    ui->sudokuTable->verticalHeader()->setVisible(false);
    ui->sudokuTable->setShowGrid(true);

    for (int i = 0; i < 9; ++i) {
        ui->sudokuTable->setColumnWidth(i, 40);
        ui->sudokuTable->setRowHeight(i, 40);
    }

    qDebug() << "Loaded a new Sudoku puzzle.";
}

void MainWindow::onCheckSolution()
{
    qDebug() << "Saif will implement check the solution. For now lets pretend it is checking..." ;
}
