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
    auto sudoku{std::make_unique<Sudoku>(sudokuGrid)};
    auto sudokuChecker{std::make_unique<SudokuChecker>()};
    auto sudokuSolver{std::make_unique<SudokuSolver>(std::move(sudoku),
                                                     std::move(sudokuChecker))};
    sudokuSolver->solve();
    sudokuSolver->display();
    return 0;
}
