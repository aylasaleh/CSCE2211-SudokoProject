#include "sudoku.h"
#include <iostream>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QChar>
#include <qDebug>

// Constructor
sudoku::sudoku() {}

// Destructor
sudoku::~sudoku() {}

// Creates a Sudoku based on difficulty and version.
void sudoku::create(Difficulty d, char version) {
    QString fileName;

    if (d == Difficulty::Easy) {
        fileName = ":/games/EasySudokus.txt";
    } else if (d == Difficulty::Medium) {
        fileName = ":/games/MediumSudokus.txt";
    } else {
        fileName = ":/games/HardSudokus.txt";
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open" << fileName;
        return;
    }

    QTextStream in(&file);
    generateSudoku(in, version);
}

// Generates Sudoku grid based on difficulty and version.
void sudoku::generateSudoku(QTextStream &file, char ver) {
    if (file.atEnd()) {
        qDebug() << "File is empty in generateSudoku";
        return;
    }

    QString line;
    QChar ch;

    //read the first character to check the version
    file >> ch;

    qDebug() << "Looking for version:" << ver;

    //skip to the required version
    while (ver != ch.toLatin1() && !file.atEnd()) {
        for (int i = 0; i < 9; i++) { //read 9 lines to skip a version
            line = file.readLine();
            qDebug() << "Raw line read:" << line;
            line = line.trimmed(); //trim whitespace
            if (line.isEmpty()) {
                qDebug() << "Empty line detected, skipping.";
                continue; //skip empty lines
            }
        }
        file >> ch; //read the next version identifier
        qDebug() << "Skipped version, now at:" << ch;
    }

    //if the specified version is not found
    if (ver != ch.toLatin1()) {
        qDebug() << "Specified version does not exist.";
        return;
    }

    qDebug() << "Found version" << ver << "at the correct position.";

    //read numbers into the Sudoku grid
    for (int i = 0; i < 9; i++) {
        line = file.readLine().trimmed(); // trim whitespace
        qDebug() << "Line read:" << line;

        //check if the line is empty after trimming
        if (line.isEmpty()) {
            qDebug() << "Empty line detected, skipping.";
            i--; // Decrement counter to retry this row
            continue; // Skip empty lines
        }

        //check line length and ensure it's exactly 9 characters
        if (line.length() != 9) {
            qDebug() << "Invalid line length in sudoku file, expected 9 characters but got:" << line.length();
            return;
        }

        //check if the line contains only digits and is exactly 9 characters long
        for (int j = 0; j < 9; j++) {
            int num = line[j].digitValue(); //convert QChar to int
            if (num < 0 || num > 9) {      //validate digit value
                qDebug() << "Invalid character in sudoku file: " << line[j];
                return;
            }
            element el(num, num != 0);
            arr[i][j] = el;
        }
    }

    qDebug() << "Sudoku puzzle loaded successfully.";
}


// Solves the Sudoku using backtracking.
bool sudoku::solve() {
    return solve(0, 0);
}

bool sudoku::solve(int index1, int index2) {
    // If we reach the end, the Sudoku is solved.
    if (index1 == SIZE) {
        return true;
    }

    // Calculate next cell.
    int nextRow = (index2 == SIZE - 1) ? index1 + 1 : index1;
    int nextCol = (index2 == SIZE - 1) ? 0 : index2 + 1;

    // Skip fixed elements.
    if (arr[index1][index2].isFixed) {
        return solve(nextRow, nextCol);
    }

    // Try all possible values.
    for (int i = 1; i <= SIZE; ++i) {
        arr[index1][index2].num = i;

        if (checkRow(index1, index2, i) && checkCol(index1, index2, i) &&
            checkBox(index1, index2, index1 - index1 % 3, index2 - index2 % 3, i)) {
            if (solve(nextRow, nextCol)) {
                return true;
            }
        }
    }

    // Backtrack if no value works.
    arr[index1][index2].num = 0;
    return false;
}

// Prints the Sudoku grid.
void sudoku::print() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << arr[i][j].num << " ";
        }
        std::cout << std::endl;
    }
}

// Checks if the row does not contain duplicates.
bool sudoku::checkRow(int indexRow, int indexCol, int valCheck) {
    for (int i = 0; i < SIZE; ++i) {
        if (i != indexCol && arr[indexRow][i].num == valCheck) {
            return false;
        }
    }
    return true;
}

// Checks if the column does not contain duplicates.
bool sudoku::checkCol(int indexRow, int indexCol, int valCheck) {
    for (int i = 0; i < SIZE; ++i) {
        if (i != indexRow && arr[i][indexCol].num == valCheck) {
            return false;
        }
    }
    return true;
}

// Checks if the 3x3 box does not contain duplicates.
bool sudoku::checkBox(int indexRow, int indexCol, int indexBoxStart1, int indexBoxStart2, int valCheck) {
    for (int i = indexBoxStart1; i < indexBoxStart1 + 3; ++i) {
        for (int j = indexBoxStart2; j < indexBoxStart2 + 3; ++j) {
            if ((i != indexRow || j != indexCol) && arr[i][j].num == valCheck) {
                return false;
            }
        }
    }
    return true;
}

// Returns the current grid.
const sudoku::element (&sudoku::getGrid() const)[SIZE][SIZE] {
    return arr;
}
