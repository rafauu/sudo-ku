#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <optional>
#include <cctype>
#include <numeric>
#include <set>
#include <algorithm>
#include <boost/range/adaptors.hpp>
#include <boost/range/irange.hpp>

template <typename T>
class FlattenVector
{
public:
    FlattenVector<T>(uint size) { vec.reserve(size); };
    T& operator[](uint index) { return vec[index]; }
    const T& operator[](uint index) const { return vec[index]; }
    void emplace_back(T&& element) { vec.emplace_back(std::move(element)); }
    size_t size() const { return vec.size(); }
    std::vector<T> getSpecificPart(const auto& predicate) const
    {
        std::vector<uint> indexes(vec.size());
        std::iota(indexes.begin(), indexes.end(), 0);
        auto indexToValueFromVector = [this](uint index){ return vec[index]; };
        using namespace boost::adaptors;
        return boost::copy_range<std::vector<T>>(indexes | filtered(predicate)
                                                         | transformed(indexToValueFromVector));
    }
    friend std::ostream& operator<<(std::ostream& os, const FlattenVector<T>& flattenVec)
    {
        std::copy(flattenVec.vec.cbegin(),
                  flattenVec.vec.cend(),
                  std::ostream_iterator<T>(os, " "));
        std::cout << std::endl;
        return os;
    }

private:
    std::vector<T> vec;
};

struct Cell
{
    Cell(std::optional<uint> value = std::nullopt,
         bool isModifiable = true)
      : value{value}
      , isModifiable{isModifiable}
    {}
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell)
    {
        os << (cell.value ? *(cell.value) : ' ');
        return os;
    }
    bool operator==(const Cell& rhs) const { return value == rhs.value; }
    bool operator<(const Cell& rhs) const { return value < rhs.value; }
    operator bool() const { return static_cast<bool>(value); }
    void increment() { if(value) ++(*value); }
    void changeValue(uint new_value) { value.emplace(new_value); }
 
    std::optional<uint> value;
    bool isModifiable;
};

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
    void goToNextEmptyCell() { while(actualCell().value) { ++actualIndex; }; }
    void goToPreviousModifiableCell() { do { --actualIndex; } while(not actualCell().isModifiable); }
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
                    sudoku.goToPreviousModifiableCell();
                }
                sudoku.incrementValueInActualCell();
            }
        }
    }
private:
    Sudoku& sudoku;
    SudokuChecker sudokuChecker;
};

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
