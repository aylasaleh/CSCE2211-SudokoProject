#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "sudoku.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getPuzzle (sudoku p);

signals:
    void backToWelcome(); // Signal to notify return to the Welcome window

private slots:
    void onLoadPuzzle();
    void onCheckSolution();
    void setDigit(int digit);
    void onCellClicked(int row, int column);

    void on_checkButton_clicked();

    void on_loadButton_clicked();

    void on_eraseB_clicked();

private:
    Ui::MainWindow *ui;
    QTableWidget *sudokuTable;
    sudoku puzzle;
    void updateTable();
    int solvedsudoko[9][9];
    void showMessage(const QString &message);
    int currentDigit;
};

#endif // MAINWINDOW_H