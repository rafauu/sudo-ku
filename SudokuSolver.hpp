#pragma once
#include "Sudoku.hpp"
#include "SudokuChecker.hpp"

class SudokuSolver
{
public:
    SudokuSolver(Sudoku& sudokuToSolve)
      : sudoku(sudokuToSolve)
      , sudokuChecker(sudoku)
    {}
    void solve()
    {
        while(true)
        {
            if(sudokuChecker.isSudokuCorrect())
            {
                sudoku.goToNextEmptyCell();
                if(not sudoku.isIndexValid()) break;
                sudoku.setLowestPossibleValueInActualCell();
            }
            else
            {
                while(sudoku.isMaxValueSet())
                {
                    sudoku.cleanActualCell();
                    sudoku.goToPreviousFilledCell();
                }
                sudoku.incrementValueInActualCell();
            }
        }
    }
private:
    Sudoku& sudoku;
    SudokuChecker sudokuChecker;
};
