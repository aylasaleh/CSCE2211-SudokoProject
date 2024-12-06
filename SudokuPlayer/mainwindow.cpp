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
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentDigit(0),
    countdownTimer(new QTimer(this)),
    remainingTime(0),
    incorrectAttempts(0),
    autoCheckEnabled(false)
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

    // Score label
    scoreLabel = new QLabel(this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    scoreLabel->setText("Score: 0");
    ui->verticalLayout->addWidget(scoreLabel); // Add score label to the layout

    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateTimer);

    // set up the table dimensions
    ui->sudokuTable->setRowCount(9);
    ui->sudokuTable->setColumnCount(9);

    // connect digit buttons to slot
    connect(ui->digitButton1, &QPushButton::clicked, this, [this]() { setDigit(1); });
    connect(ui->digitButton2, &QPushButton::clicked, this, [this]() { setDigit(2); });
    connect(ui->digitButton3, &QPushButton::clicked, this, [this]() { setDigit(3); });
    connect(ui->digitButton4, &QPushButton::clicked, this, [this]() { setDigit(4); });
    connect(ui->digitButton5, &QPushButton::clicked, this, [this]() { setDigit(5); });
    connect(ui->digitButton6, &QPushButton::clicked, this, [this]() { setDigit(6); });
    connect(ui->digitButton7, &QPushButton::clicked, this, [this]() { setDigit(7); });
    connect(ui->digitButton8, &QPushButton::clicked, this, [this]() { setDigit(8); });
    connect(ui->digitButton9, &QPushButton::clicked, this, [this]() { setDigit(9); });

    // Connect table cell click to insert the digit
    connect(ui->sudokuTable, &QTableWidget::cellClicked, this, &MainWindow::onCellClicked);

    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadPuzzle);
    connect(ui->autoCheckCheckBox, &QCheckBox::toggled, this, &MainWindow::onAutoCheckToggled);

    this->setFixedSize(486, 632);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getPuzzle(sudoku p)
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
    // This to make auto check  only be available in easy mode
    // In case  the difficulty is not easy we show a message once and revert the checkbox.
    if (currentDifficulty == sudoku::Difficulty::Easy) {
        // In Easy mode, we can enable or disable auto-check based on user input
        autoCheckEnabled = checked;
    } else {
        // Temporarily block signals to avoid recursive calls when changing the checkbox state
        ui->autoCheckCheckBox->blockSignals(true);
        ui->autoCheckCheckBox->setChecked(false);
        ui->autoCheckCheckBox->blockSignals(false);

        autoCheckEnabled = false;
        QMessageBox::information(this, "Not Available", "Auto check is only available in Easy mode.");
    }
}


void MainWindow::onCellClicked(int row, int col) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QTableWidgetItem* item = ui->sudokuTable->item(i, j);
            if (!item) continue;

            // Skip fixed cells to avoid overwriting their background color
            if (puzzle.getGrid()[i][j].isFixed) {
                item->setBackground(QBrush(QColor(230, 240, 255)));
                continue;
            }

            // Highlight the selected cell's row and column
            if (i == row || j == col) {
                item->setBackground(QBrush(QColor(173, 216, 230)));  // Light blue for selection
            } else {
                item->setBackground(QBrush(((i / 3) + (j / 3)) % 2 == 0 ? QColor(240, 240, 240) : QColor(255, 255, 255)));
            }
        }
    }

    QTableWidgetItem* selectedItem = ui->sudokuTable->item(row, col);
    if (!selectedItem) return;

    // Check if the cell is editable
    if (selectedItem->flags() & Qt::ItemIsEditable) {
        if (currentDigit != 0) {
            selectedItem->setText(QString::number(currentDigit));  // to set the digit
            // If autoCheckEnabled is true (which only happens in Easy mode), automatically check solution
            if (autoCheckEnabled) {
                onCheckSolution();
            }
        } else {
            selectedItem->setText("");  // to erase the digit
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

    ui->sudokuTable->horizontalHeader()->setVisible(false); // Hide the horizontal header
    ui->sudokuTable->verticalHeader()->setVisible(false);   // Hide the vertical header
    ui->sudokuTable->setShowGrid(true);                     // Show grid lines

    for (int i = 0; i < 9; ++i) {
        ui->sudokuTable->setColumnWidth(i, 51); // Set column width to 51 pixels for a uniform look
        ui->sudokuTable->setRowHeight(i, 51);   // Set row height to 51 pixels for a uniform look
    }

    qDebug() << "Loaded a new Sudoku puzzle."; // Debug message indicating the puzzle has been loaded

    // Solve the Sudoku (for internal checking)
    puzzle.solve();

    // Copy solved Sudoku into an array for checking
    const auto& sudokosolved = puzzle.getGrid();
    puzzle.print(); // Print the solved puzzle to console for debugging
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            solvedsudoko[i][j] = sudokosolved[i][j].num; // Store solved values for later checks
        }
    }

    // Reeset score display to 0 at the start of a new puzzle
    scoreLabel->setText("Score: 0");
}

void MainWindow::onCheckSolution() {
    // this deducts one life before checking the solution in these modes
    if (currentDifficulty == sudoku::Difficulty::Medium || currentDifficulty == sudoku::Difficulty::Hard) {
        deductLife();
        // If no lives remain after deduction then game over:)
    }

    bool isComplete = true; // Flag to check if all cells are filled
    bool valid = true;      // Flag to check if the filled solution is correct

    // Check each cell for completeness and correctness
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            QTableWidgetItem* item = ui->sudokuTable->item(i, j);
            if (!item) continue; // Skip if no item found

            int userValue = item->text().isEmpty() ? 0 : item->text().toInt();
            bool isFixed = puzzle.getGrid()[i][j].isFixed;

            // If the cell was originally fixed, restore its style
            if (isFixed) {
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(0, 0, 139))); // Dark blue text for fixed cells
                item->setBackground(QBrush(QColor(230, 240, 255))); // Light background for fixed cells
                continue;
            }

            // If the cell is filled but incorrect
            if (userValue != 0 && userValue != solvedsudoko[i][j]) {
                valid = false; // Mark solution as invalid
                QFont font = item->font();
                font.setBold(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(255, 0, 0)));     // Red text for incorrect cell
                item->setBackground(QBrush(QColor(255, 192, 192)));   // Light red background for incorrect cell
            } else if (userValue == 0) {
                // If the cell is empty, the puzzle is incomplete
                isComplete = false;
            } else {
                // if the cell matches the solved puzzle style it as correct
                QFont font = item->font();
                font.setItalic(true);
                item->setFont(font);
                item->setForeground(QBrush(QColor(34, 139, 34)));    //Green text for correct cell
                item->setBackground(QBrush(QColor(255, 255, 255)));  // White background for correct cell
            }
        }
    }

    if (isComplete) {
        if (valid) {
            // Puzzle is complete and valid
            countdownTimer->stop(); // Stop the timer as the game is completed

            int baseScore;
            int livesBonus = 0;
            // Determine base score and lives bonus based on difficulty
            switch (currentDifficulty) {
            case sudoku::Difficulty::Easy:
                baseScore = 500;
                livesBonus = 0; // In Easy mode no bonus for remaining lives (they are infinity)
                break;
            case sudoku::Difficulty::Medium:
                baseScore = 1000;
                livesBonus = lives * 50; // Bonus per remaining life in medium
                break;
            case sudoku::Difficulty::Hard:
                baseScore = 1500;
                livesBonus = lives * 100; // Bonus per remaining life in hard
                break;
            }

            int timeBonus = remainingTime;   // Bonus for remaining time
            int penalty = incorrectAttempts * 50;   // penalty for incorrect attempts
            int finalScore = baseScore + timeBonus + livesBonus - penalty;

            scoreLabel->setText(QString("Score: %1").arg(finalScore)); // Update score display

            // Displaying a message when correctly solving with score breakdown
            QMessageBox::information(this, "Success",
                                     QString("Congratulations, you solved the Sudoku!\n\n"
                                             "Score Details:\n"
                                             "Base Score: %1\n"
                                             "Time Bonus: +%2\n"
                                             "Lives Bonus: +%3\n"
                                             "Incorrect Attempts Penalty: -%4\n\n"
                                             "Final Score: %5")
                                         .arg(baseScore)
                                         .arg(timeBonus)
                                         .arg(livesBonus)
                                         .arg(penalty)
                                         .arg(finalScore));

        } else {
            // Puzzle is complete but invalid
            incorrectAttempts++; // Increase the count of incorrect attempts
            QMessageBox::warning(this, "Incorrect Solution", "Your solution is incorrect. Try again!");

            // if no lives are remaining then game over :)
            if (lives == 0) {
                QMessageBox::critical(this, "Game Over", "You have no lives left. Returning to the main menu.");
                on_loadButton_clicked(); // Return to the main menu
            }
        }
    } else {

        // removed the old puzzle is not complete to match the new lives and score logic
    }
}


void MainWindow::on_checkButton_clicked()
{
    onCheckSolution();
}

void MainWindow::on_loadButton_clicked()
{
    emit backToWelcome();
    this->close();
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
    remainingTime = minutes * 60;
    timerLabel->setText(QString("Time Left: %1:%2")
                            .arg(remainingTime / 60, 2, 10, QChar('0'))
                            .arg(remainingTime % 60, 2, 10, QChar('0')));
    countdownTimer->start(1000);
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
        on_loadButton_clicked();
    }
}

void MainWindow::stopTimer() {
    countdownTimer->stop();
}

void MainWindow::initializeLives(int initialLives) {
    lives = initialLives;

    // set up the lives label
    livesLabel = new QLabel(this);
    livesLabel->setAlignment(Qt::AlignCenter);
    livesLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: red;");
    ui->verticalLayout->addWidget(livesLabel);
    updateLivesLabel();
}

void MainWindow::updateLivesLabel() {
    if (lives == -1) {
        livesLabel->setText("Lives: ∞");
    } else {
        livesLabel->setText(QString("Lives: %1").arg(lives));
    }
}

void MainWindow::deductLife() {
    if (lives != -1) {
        --lives;
        updateLivesLabel();

        if (lives <= 0) {
            QMessageBox::critical(this, "Game Over", "You have no lives left. Game Over!");
            on_loadButton_clicked();
        }
    }
}

void MainWindow::setDifficulty(sudoku::Difficulty diff) {
    currentDifficulty = diff;
}
