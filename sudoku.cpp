#include "sudoku.h"
#include <iostream>
using namespace std; 

sudoku::sudoku()
{
    SIZE  = 9; //initilaize the size
    arr = new int[SIZE][SIZE]; //initialize the sudoku grid
}

sudoku::~sudoku()
{
    for(int i = 0; i < SIZE; i++){
      delete[] arr[i];
    delete[] arr;
}

void sudoku::create ()
{

} 

//This function assumes that our sudoku has been initialied
bool sudoku::solve()
{
	return solve(0,0);
} 

bool sudoku::solve(int index1, int index2){
    //check if the element in consideration is fixed
    if(arr[index1][index2].isFixed){
        return solveNext(index1, index2);
    }

    //if the element is empty and we can change it
    else{
        for(int i = 1; i <=SIZE; i++){//try all possible solutions one through nine
            arr[index1][index2] = i;
            //if the element does not violate existing elements
            if(checkRow(index1, i) && checkColumn(index2, i) && checkBox((index1 - index1%3), (index2-index2%3),i)){
                if(solveNext(index1, index2)){//if there is an actual solution which results then we are done
                    return true;
                }
            }
        }
        //we exited the loop and thus no element works, so reset the element in question and return false (no solution reached)
        arr[index1][index2] = 0;
        return false;
    }
}

bool sudoku::solveNext(int index1, int index2){
    //If we have reached the end of the sudoku then we have solved it
    if(index1 < 0 || index2 < 0 || index1 >= SIZE || index2 >= SIZE){ 
        return true;
    }

    //if it is solve by moving across each row moving down the columns
        if((index1+1) %SIZE ==0)
            return solve(((index1 + 1) % SIZE), index2 + 1);
        else
            return solve(((index1 + 1) % SIZE), index2);
    }
}

//prints the sudoku grid
void sudoku::print ()
{
    for(int i = 0; i < SIZE; i++){
      for(int j = 0; j < SIZE; j++){
            std::cout << arr[i][j] << " ";
          }
          std::cout << std::endl;
      }
} 

//checks if the row does not contain the same element
bool sudoku::checkRow(int indexRow, int valCheck)
{
    for(int i = 0; i < SIZE; i++){
        if(arr[indexRow][i] == valCheck){
          return false;
        }
    }
    return true;
}

//checks if the column does not contain the same element
bool sudoku::checkCol(int indexCol, int valCheck)
{
    for(int i = 0; i < SIZE; i++){
        if(arr[i][indexCol] == valCheck){
            return false;
        }
    }
    return true;
}

//checks if the box grid does not contain the same element
bool sudoku::checkBox(int indexBoxStart1, int indexBoxStart2, int valCheck)
{
    for(int i = indexBoxStart1; i<= indexBoxStart1+3; i++){
           for(int j = indexBoxStart2; j<= indexBoxStart2+3; j++){
             if(arr[i][j] == valCheck){
               return false;
             }
           }
    }
    return true;
} 
