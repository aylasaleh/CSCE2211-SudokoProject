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

    this->setFixedSize(485, 604);

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
            item->setForeground(QBrush(QColor(QColorConstants::Black)));  // set font color to black to show editable position
        }else
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
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);  //center text

            if (grid[i][j].isFixed) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable); //make fixed numbers non-editable
            } else {
                item->setFlags(item->flags() | Qt::ItemIsEditable); //allow editing of empty cells
            }
            ui->sudokuTable->setItem(i, j, item);
            if ((i / 3) % 2 == 0 && (j / 3) % 2 == 0) {
                item->setBackground(QBrush(QColor(230, 230, 250)));}    //sets the boxes colors
            item->setForeground(QBrush(QColorConstants::Svg::purple));  // set font color of non-editable
        }
    }

    ui->sudokuTable->setStyleSheet("QTableWidget::item:nth-child(1) { gridline-color: black;}");//trying to implement box gridlines but failed

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

    int useranswer[9][9];

    // extract the user's answers from the grid along with original answers
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {

            QTableWidgetItem* item = ui->sudokuTable->item(i, j);
            if (item && !item->text().isEmpty()) {
                //convert cell value to integer
                useranswer[i][j] = item->text().toInt();
            } else {
                // treat empty cells as 0s
                useranswer[i][j] = 0;

            }
        }
    }
    bool valid = true;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {

            // check if answers are not the same
            if (useranswer[i][j] != 0 && useranswer[i][j] != solvedsudoko[i][j]){
                qDebug() << "correct answer is" << solvedsudoko[i][j];
                valid = false;
                break;
            }

        }
        if (valid == false){
            break;
        }
    }

    if (valid == false){
        QMessageBox errorMessage(this);
        errorMessage.setText("Wrong Solution");
        errorMessage.exec();
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




