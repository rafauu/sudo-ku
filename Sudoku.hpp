#pragma once
#include <string>
#include <vector>
#include "FlattenVector.hpp"
#include "Cell.hpp"
#include "CommonDefinitions.hpp"

namespace
{
    uint charToUint(char ch) { return static_cast<uint>(ch-'0'); }
}

class Sudoku
{
public:
    Sudoku(const std::string& sudokuGrid) :
        actualIndex{0},
        cellsVector(sudokuGrid.size())
    {
        for(const auto& value : sudokuGrid)
        {
            cellsVector.emplace_back(isdigit(value)
                                     ? Cell(charToUint(value), false)
                                     : Cell());
        }
    }
    void display()
    {
        std::cout << cellsVector;
    }
    template <typename Range>
    auto getFilledElements(Range&& indices) const
    {
        return cellsVector.getFilledElements(std::forward<Range>(indices));
    }
    void goToNextEmptyCell() { while(actualCell().value) { ++actualIndex; } }
    void goToPreviousFilledCell() { while(not actualCell().value or not actualCell().isModifiable) { --actualIndex; } }
    void cleanActualCell() { actualCell() = Cell(); }
    void setLowestPossibleValueInActualCell() { actualCell().changeValue(minValueInCell); }
    void incrementValueInActualCell() { actualCell().increment(); }
    bool isMaxValueSet() const { return actualCell().value and *actualCell().value == maxValueInCell; }
    bool isIndexValid() const { return actualIndex < gridElements; }
private:
    uint actualIndex;
    FlattenVector<Cell, gridSize> cellsVector;

    static constexpr uint minValueInCell{1};
    static constexpr uint maxValueInCell{9};

    Cell& actualCell() { return cellsVector[actualIndex]; }
    const Cell& actualCell() const { return cellsVector[actualIndex]; }
};

