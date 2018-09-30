#include <string>
#include "Sudoku.hpp"
#include "SudokuSolver.hpp"

int main()
{
    const std::string sudokuGrid{"  3 2 6  "
                                 "9  3 5  1"
                                 "  18 64  "
                                 "  81 29  "
                                 "7       8"
                                 "  67 82  "
                                 "  26 95  "
                                 "8  2 3  9"
                                 "  5 1 3  "};
    Sudoku sudoku(sudokuGrid);
    SudokuSolver sudokuSolver(sudoku);
    sudokuSolver.solve();
    sudoku.display();
    return 0;
}
