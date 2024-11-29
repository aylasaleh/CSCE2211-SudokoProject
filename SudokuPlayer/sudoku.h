#ifndef SUDOKU_H
#define SUDOKU_H
#include <QTextStream>

class sudoku {
private:
    struct element{ //a struct to add a flag to each int
        int num;
        bool isFixed; //a flag to signify if number is set and permenant in the sudoku array
        element() : num(0) , isFixed(false){}
        element (int n, bool flag): num(n), isFixed(flag){}
    };

    static const int SIZE = 9;
    element arr[SIZE][SIZE]; //2D array of the Sudoku grid initialized to zeros

public:
    enum Difficulty{Easy, Medium, Hard};

    sudoku();

    ~sudoku();

    void create(Difficulty d, char version);

    bool solve();

    void print ();

    const element (&getGrid() const)[SIZE][SIZE];


private:
    bool solve(int index1, int index2);
    bool solveNext(int index1, int index2);

    bool checkRow(int indexRow, int indexCol, int valCheck);
    bool checkCol(int indexRow, int indexCol, int valCheck);
    bool checkBox(int index1, int index2, int indexBoxStart1, int indexBoxStart2, int valCheck);

    void generateSudoku(QTextStream &file, char ver);

};
#endif
