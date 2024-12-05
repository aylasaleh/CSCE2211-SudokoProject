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

Welcome::~Welcome() {
    if (gameMusic) {
        gameMusic->stop();
        delete gameMusic;
    }
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

void Welcome::on_easyB_clicked() {
    sudoku puzzle;
    puzzle.create(sudoku::Difficulty::Easy, toupper(randomLetterGenerator()));
    hide();

    MainWindow* m = new MainWindow(this);
    m->getPuzzle(puzzle);
    m->initializeTimer(15);      // 15 minutes for Easy
    m->initializeLives(-1);      // Infinite lives for Easy
    connect(m, &MainWindow::backToWelcome, this, &Welcome::show);
    connect(m, &MainWindow::PlayMusic, this, &Welcome::PlayMusic);
    connect(m, &MainWindow::MuteMusic, this, &Welcome::MuteMusic);
    m->show();
}

void Welcome::on_medB_clicked() {
    sudoku puzzle;
    puzzle.create(sudoku::Difficulty::Medium, toupper(randomLetterGenerator()));
    hide();

    MainWindow* m = new MainWindow(this);
    m->getPuzzle(puzzle);
    m->initializeTimer(10);      // 10 minutes for Medium
    m->initializeLives(10);      // 10 lives for Medium
    connect(m, &MainWindow::backToWelcome, this, &Welcome::show);
    connect(m, &MainWindow::PlayMusic, this, &Welcome::PlayMusic);
    connect(m, &MainWindow::MuteMusic, this, &Welcome::MuteMusic);
    m->show();
}

void Welcome::on_hardB_clicked() {
    sudoku puzzle;
    puzzle.create(sudoku::Difficulty::Hard, toupper(randomLetterGenerator()));
    hide();

    MainWindow* m = new MainWindow(this);
    m->getPuzzle(puzzle);
    m->initializeTimer(7);       // 7 minutes for Hard
    m->initializeLives(3);       // 3 lives for Hard
    connect(m, &MainWindow::backToWelcome, this, &Welcome::show);
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

