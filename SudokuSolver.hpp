#pragma once
#include "Sudoku.hpp"
#include "SudokuChecker.hpp"

class SudokuSolver
{
public:
    SudokuSolver(std::unique_ptr<Sudoku> sudoku,
                 std::unique_ptr<SudokuChecker> sudokuChecker)
        : sudoku(std::move(sudoku))
        , sudokuChecker(std::move(sudokuChecker))
    {}

    void solve()
    {
        while(true)
        {
            if(sudokuChecker->isSudokuCorrect(*sudoku))
            {
                sudoku->goToNextEmptyCell();
                if(not sudoku->isIndexValid()) break;
                sudoku->setLowestPossibleValueInActualCell();
            }
            else
            {
                while(sudoku->isMaxValueSet())
                {
                    sudoku->cleanActualCell();
                    sudoku->goToPreviousFilledCell();
                }
                sudoku->incrementValueInActualCell();
            }
        }
    }

    void display() const
    {
        sudoku->display();
    }

private:
    std::unique_ptr<Sudoku> sudoku;
    std::unique_ptr<SudokuChecker> sudokuChecker;
};
