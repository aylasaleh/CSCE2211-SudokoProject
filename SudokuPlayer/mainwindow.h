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
    void getPuzzle (sudoku p);
    void initializeTimer(int minutes);
    void initializeLives(int initialLives);



signals:
    void backToWelcome(); // Signal to notify return to the Welcome window
    void MuteMusic(); // Signal to mute music
    void PlayMusic();

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
    void updateTable();
    int solvedsudoko[9][9];
    void showMessage(const QString &message);
    int currentDigit;
    bool autoCheckEnabled;
    void closeEvent(QCloseEvent *event) override;
    QTimer *countdownTimer;
    QLabel *timerLabel;
    int remainingTime;
    void updateTimer();
    void stopTimer();
    int lives;
    QLabel *livesLabel;
    void updateLivesLabel();
    void deductLife();


};

#endif // MAINWINDOW_H
