#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "welcome.h"
#include "sudoku.h"
#include <QTableWidgetItem>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>
#include <QCloseEvent>

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


void MainWindow::onCellClicked(int row, int col) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QTableWidgetItem* item = ui->sudokuTable->item(i, j);
            if (!item) continue;

            // Skip fixed cells to avoid overwriting their background color
            if (puzzle.getGrid()[i][j].isFixed) {
                item->setBackground(QBrush(QColor(230, 240, 255)));  // Keep fixed cells' color
                continue;
            }

            // Highlight the selected cell's row and column
            if (i == row || j == col) {
                item->setBackground(QBrush(QColor(173, 216, 230)));  // Light blue for selection
            } else {
                // Restore alternating 3x3 grid colors for editable cells
                item->setBackground(QBrush(((i / 3) + (j / 3)) % 2 == 0 ? QColor(240, 240, 240) : QColor(255, 255, 255)));
            }
        }
    }

    QTableWidgetItem* selectedItem = ui->sudokuTable->item(row, col);
    if (!selectedItem) return;

    // Check if the cell is editable
    if (selectedItem->flags() & Qt::ItemIsEditable) {
        if (currentDigit != 0) {
            selectedItem->setText(QString::number(currentDigit));  // Set the digit
            if (autoCheckEnabled) {
                onCheckSolution();  // Automatically check the solution
            }
        } else {
            selectedItem->setText("");  // Erase the digit
        }
    } else {
        qDebug() << "Cannot edit fixed cells.";
    }
}




void MainWindow::onLoadPuzzle() {
    // Clear the existing table
    ui->sudokuTable->clear();

    // Set up the table dimensions again, just in case
    ui->sudokuTable->setRowCount(9);
    ui->sudokuTable->setColumnCount(9);
    ui->sudokuTable->setStyleSheet("background-color: white;");

    // Get the Sudoku grid
    const auto& grid = puzzle.getGrid();

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            // Populate table
            int value = grid[i][j].num;
            QTableWidgetItem* item = new QTableWidgetItem(value == 0 ? "" : QString::number(value));

            QFont font = item->font();
            font.setPointSize(14);  // Set font size to 14
            if (grid[i][j].isFixed) {
                // Explicitly style fixed cells
                font.setBold(true);  // Bold font
                item->setFont(font);
                item->setForeground(QBrush(QColor(0, 0, 139)));  // Dark blue text
                item->setBackground(QBrush(QColor(230, 240, 255)));   // Light beige background
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);  // Non-editable
            } else {
                font.setItalic(false);  // Non-italic for editable cells
                item->setFont(font);
                item->setForeground(QBrush(QColor(0, 0, 0)));  // Black text
                item->setBackground(QBrush(QColor(255, 255, 255)));  // White background
                item->setFlags(item->flags() | Qt::ItemIsEditable);  // Allow editing
            }

            item->setTextAlignment(Qt::AlignCenter);  // Center text
            ui->sudokuTable->setItem(i, j, item);

            // Set alternating 3x3 grid colors
            if (((i / 3) + (j / 3)) % 2 == 0) {
                if (!grid[i][j].isFixed) {
                    item->setBackground(QBrush(QColor(240, 240, 240)));  // Light gray for editable cells
                }
            }
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

    // Solve the Sudoku
    puzzle.solve();

    // Copy solved Sudoku
    const auto& sudokosolved = puzzle.getGrid();
    puzzle.print();

    // Copy answers of Sudoku into an array
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            solvedsudoko[i][j] = sudokosolved[i][j].num;
        }
    }
}




void MainWindow::onCheckSolution() {
    bool isComplete = true;
    bool valid = true;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QTableWidgetItem* item = ui->sudokuTable->item(i, j);
            if (!item) continue;

            int userValue = item->text().isEmpty() ? 0 : item->text().toInt();
            bool isFixed = puzzle.getGrid()[i][j].isFixed;

            // Fixed cells
            if (isFixed) {
                // Ensure fixed cells retain their color and style
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(0, 0, 139)));  // Dark blue text
                item->setBackground(QBrush(QColor(230, 240, 255)));  // Light grayish blue background
                continue;  // Skip further checks for fixed cells
            }

            // Editable cells
            if (userValue != 0 && userValue != solvedsudoko[i][j]) {
                // Incorrect value
                valid = false;
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(255, 0, 0)));  // Red text
                item->setBackground(QBrush(QColor(255, 192, 192)));  // Light red background
            } else if (userValue != 0 && userValue == solvedsudoko[i][j]) {
                // Correct value
                QFont font = item->font();
                font.setItalic(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(34, 139, 34)));  // Green text
                // Ensure proper background for vertical/horizontal highlights
                if (item->background().color() != QColor(173, 216, 230)) {  // Light blue for selection
                    item->setBackground(QBrush(QColor(255, 255, 255)));  // Default white
                }
            } else {
                // Empty cell
                QFont font = item->font();
                font.setBold(false);
                font.setItalic(false);
                item->setFont(font);
                item->setForeground(QBrush(QColor(0, 0, 0)));  // Black text
                if (item->background().color() != QColor(173, 216, 230)) {  // Light blue for selection
                    item->setBackground(QBrush(QColor(255, 255, 255)));  // Default white
                }
            }

            if (userValue == 0) {
                isComplete = false;  // Puzzle is incomplete
            }
        }
    }

    // Check for completion and validity
    if (valid && isComplete) {
        QMessageBox::information(this, "Success", "Congratulations, you solved the Sudoku!");
    } else if (!isComplete) {
        if (autoCheckEnabled) return;  // Allow auto-check without showing a message for incomplete puzzles
        QMessageBox::warning(this, "Incomplete", "The puzzle is not complete.");
    } else {
        QMessageBox::warning(this, "Invalid", "The solution is invalid.");
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

void MainWindow::on_eraseButton_clicked()
{
    currentDigit = 0;
    // QTC_TEMP
}


void MainWindow::on_MuteMusic_stateChanged(int state)
{
    if (state == Qt::Checked){
        emit MuteMusic();
    }
    else if (state == Qt::Unchecked){
        emit PlayMusic();
    }
}

// override close application function to stop the sound when exiting application
void MainWindow::closeEvent(QCloseEvent *event)
{
    emit backToWelcome();
    this->close();
}
