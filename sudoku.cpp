#include "sudoku.h"
#include <iostream>
#include <string>

//constructor does not require any specfic or default implmenetation
sudoku::sudoku()
{}

//no pointers used, nothing to delete or handle manually..
sudoku::~sudoku()
{}

//creates a sudoku based on difficulty
//Each "sudoku" file has different sudokus, each described by a "version" from A-D (that is atm, but we can increase amount of sudokus)
void sudoku::create(Difficulty d, char version)
{
    //handle differently according to difficulty level described in "create"

    if(d == Difficulty::Easy){
        std::ifstream fin;
        fin.open("EasySudokus.txt", std::ios::in);
        generateSudoku(fin, version);
        fin.close();
    }

     else if(d == Difficulty::Medium){
         std::ifstream fin;
         fin.open("MediumSudokus.txt", std::ios::in);
         generateSudoku(fin, version);
         fin.close();
      }

      else{
          std::ifstream fin;
          fin.open("HardSudokus.txt", std::ios::in);
          generateSudoku(fin, version);
          fin.close();
      }

}

//generates a sudoku based on difficulty level and version required
void sudoku::generateSudoku(std::ifstream &file, char ver){
        if(!file.is_open()){
            std::cout<<"File Not Opened in generateSudoku function"<<std::endl;
            file.close();
            return;
        }

        std::string line;
        char ch = '0';
        file >> ch; //read in the first character to check version

        //skip to next "version"
        while(ver != ch && !file.eof()){
            for(int i = 0; i <= 9; i++){
                std::getline(file, line);
            }
            file >> ch;
        }

        //the specified "version" does not exist
        if(ver != ch){
            std::cout << "Wrong Input in generateSoduku function." <<std::endl;
            file.close();
            return;
        }

        //read in all numbers into the sudoku array
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                file >> ch;
                int num = ch - '0';
                element el(num, (num==0)?false:true);
                arr[i][j] = el;
            }
            std::getline(file, line);
        }
        file.close();
    }

//This function assumes that our sudoku has been initialied and solves recursively using backtracking
bool sudoku::solve()
{
	return solve(0,0);
} 

bool sudoku::solve(int index1, int index2){
    //check if out of bounds
    if (index1 < 0 || index2 < 0 || index1 >= 9 || index2 >= 9) {
        return true;
    }

    //check if element in context is fixed, if yes do not change it.
    if(arr[index1][index2].isFixed){
        return solveNext(index1, index2);
    }

    //if the element is not fixed and we can change it
    else{
        for(int i = 1; i <= SIZE; i++){//try all possible solutions one through nine
            arr[index1][index2].num = i;

            //if the element does not violate existing elements
            if(checkRow(index1, index2, i) && checkCol(index1, index2, i) && checkBox(index1, index2, (index1 - index1%3), (index2-index2%3),i)){
                if(solveNext(index1, index2)){ //if there is an actual solution which results then we are done
                    return true;
                }
            }
        }
        //we exited the loop and thus no element works, so reset the element in question and return false (no solution reached)
        arr[index1][index2].num = 0;
        return false;
    }
}

bool sudoku::solveNext(int index1, int index2){
    //If we have reached the end of the sudoku then we have solved it
    if(index1 < 0 || index2 < 0 || index1 >= SIZE || index2 >= SIZE){ 
        return true;
    }

    //if it is, solve by moving across each row moving down the columns
        if((index1+1) %SIZE ==0)
            return solve(((index1 + 1) % SIZE), index2 + 1);
        else
            return solve(((index1 + 1) % SIZE), index2);

}

//prints the sudoku grid
void sudoku::print ()
{
    for(int i = 0; i < SIZE; i++){
      for(int j = 0; j < SIZE; j++){
            std::cout << arr[i][j].num << " ";
          }
          std::cout << std::endl;
      }
} 

//checks if the row does not contain the same element
bool sudoku::checkRow(int indexRow, int indexCol, int valCheck)
{
    for(int i = 0; i < SIZE; i++){
        if (i == indexCol) continue;
        if(arr[indexRow][i].num == valCheck){
          return false;
        }
    }
    return true;
}

//checks if the column does not contain the same element
bool sudoku::checkCol(int indexRow, int indexCol, int valCheck)
{
    for(int i = 0; i < SIZE; i++){
        if (i == indexRow) continue;
        if(arr[i][indexCol].num == valCheck){
            return false;
        }
    }
    return true;
}

//checks if the box grid does not contain the same element
bool sudoku::checkBox(int indexRow, int indexCol, int indexBoxStart1, int indexBoxStart2, int valCheck)
{
    for(int i = indexBoxStart1; i<= indexBoxStart1+2; i++){
        for(int j = indexBoxStart2; j<= indexBoxStart2+2; j++){
            if (i == indexRow && j== indexCol)continue;
            if(arr[i][j].num == valCheck){
               return false;
             }
           }
    }
    return true;
} 
