#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include <boost/range/irange.hpp>
#include <boost/range/adaptors.hpp>
#include "Sudoku.hpp"

namespace
{
    template<typename T>
    std::vector<T> filterOutElementsWithoutValues(const std::vector<T>& vec)
    {
        auto hasValue = [](const T& element){ return element; };
        using namespace boost::adaptors;
        return boost::copy_range<std::vector<T>>(vec | filtered(hasValue));
    }

    template<typename T>
    bool hasUniqueElements(const std::vector<T>& vec)
    {
        auto temp = filterOutElementsWithoutValues(vec);
        return temp.size() == std::set<T>(temp.begin(), temp.end()).size();
    }
}

class SudokuChecker
{
public:
    SudokuChecker(const Sudoku& sudokuVector)
      : sudokuVector{sudokuVector}
    {}
    bool isSudokuCorrect() const
    {
        for(auto i : boost::irange(0u, gridSize))
        {
            if(not nthRowIsCorrect(i) or
               not nthColIsCorrect(i) or
               not nthSqrIsCorrect(i))
            {
                return false;
            }
        }
        return true;
    }
private:
    const Sudoku& sudokuVector;

    static constexpr uint latinSqrSize{3};
    static constexpr uint gridSize{9};

    bool nthRowIsCorrect(uint row_index) const
    {
        auto nthRow = [this, row_index](uint index){ return index/gridSize == row_index; };
        return hasUniqueElements(sudokuVector.getSpecificPart(nthRow));
    }

    bool nthColIsCorrect(uint col_index) const
    {
        auto nthCol = [this, col_index](uint index){ return index%gridSize == col_index; };
        return hasUniqueElements(sudokuVector.getSpecificPart(nthCol));
    }

    bool nthSqrIsCorrect(uint sqr_index) const
    {
        auto nthSqr = [this, sqr_index](uint index)
        {
            auto row = index/gridSize;
            auto col = index%gridSize;
            auto latinSqrRowIndex = latinSqrSize*(sqr_index/latinSqrSize);
            auto latinSqrColIndex = latinSqrSize*(sqr_index%latinSqrSize);
            return row == std::clamp(row, latinSqrRowIndex, latinSqrRowIndex + latinSqrSize - 1) and
                   col == std::clamp(col, latinSqrColIndex, latinSqrColIndex + latinSqrSize - 1);
        };
        return hasUniqueElements(sudokuVector.getSpecificPart(nthSqr));
    }
};
