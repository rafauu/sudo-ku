#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include <boost/range/irange.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>
#include "Sudoku.hpp"
#include "CommonDefinitions.hpp"

namespace
{
    template<typename T>
    bool hasUniqueElements(const std::vector<T>& vec)
    {
        return vec.size() == std::set<T>(vec.cbegin(), vec.cend()).size();
    }
}

class SudokuChecker
{
public:
    SudokuChecker() = default;

    bool isSudokuCorrect(const Sudoku& sudoku) const
    {
        return boost::algorithm::all_of(boost::irange(0u, gridSize),
                                        [this, &sudoku](uint index)
                                        {
                                            return nthRowIsCorrect(sudoku, index) and
                                                   nthColIsCorrect(sudoku, index) and
                                                   nthSqrIsCorrect(sudoku, index);
                                        });
    }
private:
    static constexpr uint latinSqrSize{3};

    bool nthRowIsCorrect(const Sudoku& sudoku, uint rowIndex) const
    {
        return hasUniqueElements(
                   sudoku.getFilledElements(
                       boost::irange(gridSize * rowIndex, gridSize * (rowIndex + 1))));
    }

    bool nthColIsCorrect(const Sudoku& sudoku, uint colIndex) const
    {
        return hasUniqueElements(
                   sudoku.getFilledElements(
                       boost::irange(colIndex, gridElements, gridSize)));
    }

    bool nthSqrIsCorrect(const Sudoku& sudoku, uint sqrIndex) const
    {
        using namespace boost::adaptors;
        auto nthSqr = [this, sqrIndex](uint index)
        {
            auto row = index/gridSize;
            auto col = index%gridSize;
            auto latinSqrRowIndex = latinSqrSize*(sqrIndex/latinSqrSize);
            auto latinSqrColIndex = latinSqrSize*(sqrIndex%latinSqrSize);
            return row == std::clamp(row, latinSqrRowIndex, latinSqrRowIndex + latinSqrSize - 1) and
                   col == std::clamp(col, latinSqrColIndex, latinSqrColIndex + latinSqrSize - 1);
        };
        return hasUniqueElements(
                   sudoku.getFilledElements(
                       boost::irange(0u, gridElements) | filtered(nthSqr)));
    }
};
