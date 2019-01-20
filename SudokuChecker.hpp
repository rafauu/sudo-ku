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

    bool nthRowIsCorrect(const Sudoku& sudoku, uint row_index) const
    {
        auto nthRow = [this, row_index](uint index){ return index/gridSize == row_index; };
        return hasUniqueElements(sudoku.getSpecificPart(nthRow));
    }

    bool nthColIsCorrect(const Sudoku& sudoku, uint col_index) const
    {
        auto nthCol = [this, col_index](uint index){ return index%gridSize == col_index; };
        return hasUniqueElements(sudoku.getSpecificPart(nthCol));
    }

    bool nthSqrIsCorrect(const Sudoku& sudoku, uint sqr_index) const
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
        return hasUniqueElements(sudoku.getSpecificPart(nthSqr));
    }
};
