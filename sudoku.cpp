#include "sudoku.h"
#include <iostream>
using namespace std; 

sudoku::sudoku()
{

}

sudoku::~sudoku()
{

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
        for(int i = 1; i <=9; i++){//try all possible solutions one through nine
            solArr[index1][index2] = i;
            //if the element does not violate existing elements
            if(checkRow(index1, i) && checkColumn(index2, i) && checkBox((index1 - index1%3), (index2-index2%3),i)){
                if(solveNext(index1, index2)){//if there is an actual solution which results then we are done
                    return true;
                }
            }
        }
        //we exited the loop and thus no element works, so reset the element in question and return false (no solution reached)
        solArr[index1][index2] = 0;
        return false;
    }
}

bool sudoku::solveNext(int index1, int index2){
    //If we have reached the end of the sudoku then we have solved it
    if(index1 < 0 || index2 < 0 || index1 >= 9 || index2 >= 9){ 
        return true;
    }

    //if it is solve by moving across each row moving down the columns
        if((index1+1) %9 ==0)
            return solve(((index1 + 1) % 9), index2 + 1);
        else
            return solve(((index1 + 1) % 9), index2);
    }

}
void sudoku::print ()
{

} 

bool sudoku::checkRow()
{

}

bool sudoku::checkCol()
{

}

bool sudoku::checkBox()
{

} 
