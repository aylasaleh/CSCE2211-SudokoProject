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

    public:

    sudoku(); 
    ~sudoku(); 
    void create (); 
    void solve(); 
    void print (); 
    bool checkRow(); 
    bool checkCol(); 
    bool checkBox(); 

}; 

#endif