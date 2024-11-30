#include "welcome.h"
#include "ui_welcome.h"
#include "sudoku.h"
#include "mainwindow.h"
#include <QRandomGenerator>

Welcome::Welcome(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Welcome)
{
    ui->setupUi(this);
    this->setWindowTitle("Welcome");
}

Welcome::~Welcome()
{
    delete ui;
}

char Welcome::randomLetterGenerator()
{
    int n = QRandomGenerator::global()->bounded(4);
    return 'A' + n;
}

void Welcome::on_easyB_clicked()
{
    //initialize a new Sudoku puzzle
    sudoku puzzle;
    //randomize puzzle
    puzzle.create(sudoku::Difficulty::Easy, toupper(randomLetterGenerator()));

    //hide this window
    hide();

    //create new mainwindow
    MainWindow* m = new MainWindow(this);

    //pass the puzzle to the main window
    m->getPuzzle(puzzle);

    //inorder to avoid creating endless windows when starting new games we will maintain 1 welcome window and closing main windows
    connect(m, &MainWindow::backToWelcome, this, &Welcome::show);
    m->show();
}


void Welcome::on_medB_clicked()
{
    //initialize a new Sudoku puzzle
    sudoku puzzle;
    //randomize puzzle
    puzzle.create(sudoku::Difficulty::Medium, toupper(randomLetterGenerator()));

    //hide this window
    hide();

    //create new mainwindow
    MainWindow* m = new MainWindow(this);

    //pass the puzzle to the main window
    m->getPuzzle(puzzle);
    connect(m, &MainWindow::backToWelcome, this, &Welcome::show);
    m->show();
}


void Welcome::on_hardB_clicked()
{
    //initialize a new Sudoku puzzle
    sudoku puzzle;
    //randomize puzzle
    puzzle.create(sudoku::Difficulty::Hard,toupper(randomLetterGenerator()));

    //hide this window
    hide();

    //create new mainwindow
    MainWindow* m = new MainWindow(this);

    //pass the puzzle to the main window
    m->getPuzzle(puzzle);
    connect(m, &MainWindow::backToWelcome, this, &Welcome::show);
    m->show();
}

