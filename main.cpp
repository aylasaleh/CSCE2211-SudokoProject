#include "Sudoku.cpp"
#include <iostream>
using namespace std;

int main(){
  sudoku s;
  s.create(sudoku::Difficulty::Easy, 'D');
  s.print();
  cout << endl << endl;
  s.solve();
  s.print();

  return 0;
}