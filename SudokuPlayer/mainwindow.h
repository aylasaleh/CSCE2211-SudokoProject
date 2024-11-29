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

private slots:
    void onLoadPuzzle();
    void onCheckSolution();
    void setDigit(int digit);
    void onCellClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    QTableWidget *sudokuTable;
    sudoku puzzle;
    void updateTable();
    void showMessage(const QString &message);
    int currentDigit;
};

#endif // MAINWINDOW_H
