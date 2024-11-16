#ifndef SUDOKU_H
#define SUDOKU_H
#include <iostream>
using namespace std; 

class sudoku {
    private:
    struct element{ //a struct to add a flag to each int
        int num; 
        bool isFixed; //a flag to signify if number is set and permenant in the sudoku array
        element (int n, bool flag): num(n), isFixed(flag){}
    }; 
    element arr[9][9]; //2D array of the Sudoku grid
    element solArr[9][9]; //2D array which must be initialized to arr and will contain the solution

    public:

    sudoku(); 
    ~sudoku(); 
    void create (); 
    bool solve(); 
    void print (); 

    private:
    bool solve(int index1, int index2);
    bool solveNext(int index1, int index2);
    bool checkRow(int indexRow, int valCheck); 
    bool checkCol(int indexCol, int valCheck); 
    bool checkBox(int indexBoxStart1, int indexBoxStart2, int valCheck); 

}; 

#endif