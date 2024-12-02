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

    // game music
    gameMusic = new QSoundEffect(this);
    gameMusic->setSource(QUrl("qrc:/Music Choices/Gameplay Theme - Microsoft Sudoku.wav"));
    PlayMusic();
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

void Welcome::PlayMusic()
{
    qDebug() << "playing music";
    gameMusic->setLoopCount(50);
    gameMusic->play();
}

void Welcome::MuteMusic()
{
    qDebug() << "Stopping music";
    gameMusic->stop();
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

    //connect signals for playing music
    connect(m, &MainWindow::PlayMusic, this, &Welcome::PlayMusic);
    connect(m, &MainWindow::MuteMusic, this, &Welcome::MuteMusic);

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

    //connect signals for playing music
    connect(m, &MainWindow::PlayMusic, this, &Welcome::PlayMusic);
    connect(m, &MainWindow::MuteMusic, this, &Welcome::MuteMusic);

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

    //connect signals for playing music
    connect(m, &MainWindow::PlayMusic, this, &Welcome::PlayMusic);
    connect(m, &MainWindow::MuteMusic, this, &Welcome::MuteMusic);


    m->show();
}


void Welcome::on_MuteMusic_stateChanged(int state)
{
    if (state == Qt::Checked){
        MuteMusic();
    }
    else if (state == Qt::Unchecked){
        PlayMusic();
    }
}

