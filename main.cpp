#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <cctype>
#include <numeric>
#include <set>
#include <algorithm>
#include <iomanip> // setw
#include <boost/range/adaptors.hpp>
#include <boost/range/irange.hpp>

template <typename T>
class FlattenVector
{
public:
	FlattenVector<T>(uint rows, uint cols)
	: rows{rows}, cols{cols}
	{
	    vec.reserve(rows*cols);
	};
	
	T& operator[](uint index) { return vec[index]; }
	const T& operator[](uint index) const { return vec[index]; }
	void emplace_back(T&& element) { vec.emplace_back(std::move(element)); }
	size_t size() const { return vec.size(); }
	std::vector<T> getSpecificPart(const auto& predicate)
	{
	    std::vector<uint> indexes(vec.size());
	    std::iota(indexes.begin(), indexes.end(), 0);
	    auto indexToValueFromVector = [this](uint index){ return vec[index]; };
	    using namespace boost::adaptors;
	    return boost::copy_range<std::vector<T>>(indexes | filtered(predicate)
                                                         | transformed(indexToValueFromVector));
	}
	void display() const
	{
	    using namespace boost::adaptors;
	    for(const auto& el : vec | indexed(0))
	    {
	        if(el.index() % cols == 0) std::cout << std::endl;
	        std::cout << std::setw(2) << el.value() << " "; //remove setw and iomanip
	    }
	    std::cout << std::endl;//remove
	}
	
	const uint rows;
    const uint cols;
    
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
 
class SudokuSolver
{
public:
	SudokuSolver(const std::string& sudokuGrid)
	: actualIndex{0}
	, sudokuVector(std::sqrt(sudokuGrid.size()), std::sqrt(sudokuGrid.size()))
	{
	    for(const auto& value : sudokuGrid)
	    {
	        sudokuVector.emplace_back(isdigit(value)
	                                  ? Cell(charToUint(value), false)
	                                  : Cell());
	    }
	}
	void solve()
	{
	    while(true)
		{
////////////std::cout << "*" << actualIndex << "*";
		    if(allCellsAreCorrect())
		    {
////////////////std::cout << "isok";
		        goToNextModifiableCell();
		        if(actualIndex >= sudokuVector.size()) break;
////////////////std::cout << "***" << actualIndex << "***";
		        setLowestPossibleValueInActualCell();
		    }
		    else
		    {
////////////////std::cout << "notok";
                while(actualCell().value and
                      *actualCell().value == 9)//magic number
                {
                    cleanActualCell();
                    goToPreviousModifiableCell();
                }
                incrementValueInActualCell();
		    }
		    display();
		}
	}
	void display()
	{
		sudokuVector.display();
	}
private:
	uint actualIndex;
	FlattenVector<Cell> sudokuVector;
	
	bool nthRowIsCorrect(uint row_index)
	{
	    auto nthRow = [this, row_index](uint index){ return index/sudokuVector.rows == row_index; };
	    return hasUniqueElements(sudokuVector.getSpecificPart(nthRow));
	}
	
	bool nthColIsCorrect(uint col_index)
	{
	    auto nthCol = [this, col_index](uint index){ return index%sudokuVector.cols == col_index; };
	    return hasUniqueElements(sudokuVector.getSpecificPart(nthCol));
	}
	
	bool nthSqrIsCorrect(uint sqr_index)
	{
	    auto nthSqr = [this, sqr_index](uint index)
	    {
	        auto row = index/sudokuVector.rows;
	        auto col = index%sudokuVector.cols;
	        return row == std::clamp(row, sqr_index/3, sqr_index/3+3-1) and
	               col == std::clamp(col, sqr_index%3, sqr_index%3+3-1);   //magic numbers
	    };
	    return hasUniqueElements(sudokuVector.getSpecificPart(nthSqr));
	}
	
	//add SudokuChecker
 
    bool allCellsAreCorrect()
    {
        for(auto i : boost::irange(0u, sudokuVector.rows))
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
    Cell& actualCell() { return sudokuVector[actualIndex]; }
    const Cell& actualCell() const { return sudokuVector[actualIndex]; }
 
    void goToNextModifiableCell() { do { ++actualIndex; } while(not actualCell().isModifiable); }
    void goToPreviousModifiableCell() { do { --actualIndex; } while(not actualCell().isModifiable); }
    void cleanActualCell() { actualCell() = Cell(); }
    void setLowestPossibleValueInActualCell() { actualCell().changeValue(1); }//magic number
    void incrementValueInActualCell() { actualCell().increment(); }
};
 
int main()
{
    const std::string sudokuGrid{"7  2 4 1 "
                                 "95  6  3 "
                                 "2  1 8 57"
                                 "  3  5   "
                                 "    7    "
                                 "1 23 9   "
                                 "         "
                                 "         "
                                 "         "};
	SudokuSolver sudokuSolver(sudokuGrid);
	sudokuSolver.solve();
	sudokuSolver.display();
	return 0;
}
