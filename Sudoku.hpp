#pragma once
#include <string>
#include <vector>
#include "FlattenVector.hpp"
#include "Cell.hpp"

namespace
{
    uint charToUint(char ch) { return static_cast<uint>(ch-'0'); }
}

class Sudoku
{
public:
    Sudoku(const std::string& sudokuGrid)
      : actualIndex{0}
      , sudokuVector(sudokuGrid.size())
    {
        for(const auto& value : sudokuGrid)
        {
            sudokuVector.emplace_back(isdigit(value)
                                      ? Cell(charToUint(value), false)
                                      : Cell());
        }
    }
    void display()
    {
        std::cout << sudokuVector;
    }
    std::vector<Cell> getSpecificPart(const auto& predicate) const
    {
        return sudokuVector.getSpecificPart(predicate);
    }
    void goToNextEmptyCell() { while(actualCell().value) { ++actualIndex; } }
    void goToPreviousFilledCell() { while(not actualCell().value or not actualCell().isModifiable) { --actualIndex; } }
    void cleanActualCell() { actualCell() = Cell(); }
    void setLowestPossibleValueInActualCell() { actualCell().changeValue(minValueInCell); }
    void incrementValueInActualCell() { actualCell().increment(); }
    bool isMaxValueSet() const { return actualCell().value and *actualCell().value == maxValueInCell; }
    bool isIndexValid() const { return actualIndex < cellQuantity; }
private:
    uint actualIndex;
    FlattenVector<Cell> sudokuVector;

    static constexpr uint cellQuantity{81};
    static constexpr uint minValueInCell{1};
    static constexpr uint maxValueInCell{9};

    Cell& actualCell() { return sudokuVector[actualIndex]; }
    const Cell& actualCell() const { return sudokuVector[actualIndex]; }
};

