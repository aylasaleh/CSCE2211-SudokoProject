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
    : QMainWindow(parent), ui(new Ui::MainWindow), currentDigit(0), countdownTimer(new QTimer(this)), remainingTime(0)
{
    ui->setupUi(this);

    // resizing to prevent scrollbars
    ui->sudokuTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->sudokuTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->sudokuTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->sudokuTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->sudokuTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    // Timer label
    timerLabel = new QLabel(this);
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    ui->verticalLayout->addWidget(timerLabel); // Add timer label to the UI layout

    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateTimer);


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
    bool isComplete = true;  // Flag to determine if the puzzle is fully filled
    bool valid = true;       // Flag to determine if the solution is correct

    // Iterate through the grid to check each cell
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QTableWidgetItem* item = ui->sudokuTable->item(i, j);
            if (!item) continue;

            int userValue = item->text().isEmpty() ? 0 : item->text().toInt();
            bool isFixed = puzzle.getGrid()[i][j].isFixed;

            // Fixed cells - retain their styling and skip checks
            if (isFixed) {
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(0, 0, 139)));  // Dark blue text
                item->setBackground(QBrush(QColor(230, 240, 255)));  // Light grayish blue background
                continue;
            }

            // Check if the cell is incorrect
            if (userValue != 0 && userValue != solvedsudoko[i][j]) {
                valid = false; // The solution is invalid
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(255, 0, 0)));  // Red text
                item->setBackground(QBrush(QColor(255, 192, 192)));  // Light red background
            } else if (userValue == 0) {
                // If a cell is empty, mark the puzzle as incomplete
                isComplete = false;
            } else {
                // Correct value - reset styling
                QFont font = item->font();
                font.setItalic(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(34, 139, 34)));  // Green text
                item->setBackground(QBrush(QColor(255, 255, 255)));  // Default white background
            }
        }
    }

    // Puzzle completeness and validity checks
    if (isComplete) {
        if (valid) {
            // Puzzle is complete and valid
            countdownTimer->stop(); // Stop the timer
            QMessageBox::information(this, "Success", "Congratulations, you solved the Sudoku!");
        } else {
            // Puzzle is complete but invalid
            deductLife(); // Deduct a life for an incorrect solution
            QMessageBox::warning(this, "Incorrect Solution", "Your solution is incorrect. Try again!");

            // If lives reach zero, show Game Over message
            if (lives == 0) {
                QMessageBox::critical(this, "Game Over", "You have no lives left. Returning to the main menu.");
                on_loadButton_clicked(); // Return to the welcome screen
            }
        }
    } else {
        // Puzzle is incomplete
        if (!autoCheckEnabled) {
            QMessageBox::warning(this, "Incomplete Puzzle", "The puzzle is not complete. Please finish it before checking.");
        }
        // No action if auto-check is enabled
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

void MainWindow::initializeTimer(int minutes) {
    remainingTime = minutes * 60; // Convert minutes to seconds
    timerLabel->setText(QString("Time Left: %1:%2")
                            .arg(remainingTime / 60, 2, 10, QChar('0'))
                            .arg(remainingTime % 60, 2, 10, QChar('0')));
    countdownTimer->start(1000); // Update every second
}

void MainWindow::updateTimer() {
    if (remainingTime > 0) {
        --remainingTime;
        timerLabel->setText(QString("Time Left: %1:%2")
                                .arg(remainingTime / 60, 2, 10, QChar('0'))
                                .arg(remainingTime % 60, 2, 10, QChar('0')));
    } else {
        countdownTimer->stop();
        QMessageBox::warning(this, "Time's Up", "You ran out of time! Game over.");
        on_loadButton_clicked(); // Return to the welcome screen
    }
}

void MainWindow::stopTimer() {
    countdownTimer->stop();
}

void MainWindow::initializeLives(int initialLives) {
    lives = initialLives;

    // Set up the lives label
    livesLabel = new QLabel(this);
    livesLabel->setAlignment(Qt::AlignCenter);
    livesLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: red;");
    ui->verticalLayout->addWidget(livesLabel); // Add the label to the layout
    updateLivesLabel();
}

void MainWindow::updateLivesLabel() {
    if (lives == -1) {
        livesLabel->setText("Lives: ∞"); // Infinity symbol for easy level
    } else {
        livesLabel->setText(QString("Lives: %1").arg(lives));
    }
}

void MainWindow::deductLife() {
    if (lives != -1) { // Do not deduct lives for easy mode
        --lives;
        updateLivesLabel();

        if (lives <= 0) {
            QMessageBox::critical(this, "Game Over", "You have no lives left. Game Over!");
            on_loadButton_clicked(); // Return to the welcome screen
        }
    }
}
