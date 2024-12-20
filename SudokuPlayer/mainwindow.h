#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "sudoku.h"
#include <QTimer>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getPuzzle(sudoku p);
    void initializeTimer(int minutes);
    void initializeLives(int initialLives);
    void setDifficulty(sudoku::Difficulty diff);

signals:
    void backToWelcome(); // Signal to notify return to the Welcome window
    void MuteMusic();     // Signal to mute music
    void PlayMusic();     // Signal to play music

private slots:
    void onLoadPuzzle();
    void onCheckSolution();
    void setDigit(int digit);
    void onCellClicked(int row, int column);
    void on_checkButton_clicked();
    void on_loadButton_clicked();
    void onAutoCheckToggled(bool checked);
    void on_eraseButton_clicked();
    void on_MuteMusic_stateChanged(int state);

private:
    Ui::MainWindow *ui;
    QTableWidget *sudokuTable;
    sudoku puzzle;
    int solvedsudoko[9][9];

    int currentDigit;
    bool autoCheckEnabled;

    QTimer *countdownTimer;
    QLabel *timerLabel;
    QLabel *livesLabel;
    QLabel *scoreLabel;

    int remainingTime;
    int lives;
    int incorrectAttempts;
    sudoku::Difficulty currentDifficulty;

    void closeEvent(QCloseEvent *event) override;
    void updateTable();
    void showMessage(const QString &message);
    void updateTimer();
    void stopTimer();
    void updateLivesLabel();
    void deductLife();
};

#endif // MAINWINDOW_H
