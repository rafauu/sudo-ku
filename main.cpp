#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <cctype>
#include <numeric>
#include <set>
#include <boost/range/adaptors.hpp>
#include <boost/range/irange.hpp>
 
template <typename T>
class FlattenVector
{
public:
	FlattenVector<T>(size_t rows,
	                 size_t cols)
	: rows{rows}
	, cols{cols}
	{ vec.reserve(rows*cols); };
	T& operator[](size_t index) { return vec[index]; }
	const T& operator[](size_t index) const { return vec[index]; }
	void emplace_back(T&& element) { vec.emplace_back(std::move(element)); }
	bool checkNthRowCorrectness(size_t row)
	{
	    std::vector<size_t> indexes(rows*cols);
	    std::iota(indexes.begin(), indexes.end(), 0);
 
	    auto nthRow = [this, row](size_t index){ return index/3 == row; };//9
	    auto indexToValueFromVector = [this](size_t index){ return vec[index]; };
	    //auto hasValue = [this](T element){ return bool(element.value); };
 
	    using namespace boost::adaptors;
        auto temp = boost::copy_range<std::vector<T>>(indexes | filtered(nthRow)
                                                              | transformed(indexToValueFromVector));
                                                              //| filtered(hasValue));
 
        return temp.size() == std::set<T>(temp.begin(), temp.end()).size();
	}
 
	bool checkNthColCorrectness(size_t row)
	{
	    std::vector<size_t> indexes(rows*cols);
	    std::iota(indexes.begin(), indexes.end(), 0);
 
	    auto nthRow = [this, row](size_t index){ return index%3 == row; };//9
	    auto indexToValueFromVector = [this](size_t index){ return vec[index]; };
	    //auto hasValue = [this](T element){ return bool(element.value); };
 
	    using namespace boost::adaptors;
        auto temp = boost::copy_range<std::vector<T>>(indexes | filtered(nthRow)
                                                              | transformed(indexToValueFromVector));
                                                              //| filtered(hasValue));
 
        return temp.size() == std::set<T>(temp.begin(), temp.end()).size();
	}
 
	bool checkNthSqrCorrectness(size_t row)
	{
	    std::vector<size_t> indexes(rows*cols);
	    std::iota(indexes.begin(), indexes.end(), 0);
 
	    auto nthRow = [this, row](size_t index){ return index<9; };//9
	    auto indexToValueFromVector = [this](size_t index){ return vec[index]; };
	    //auto hasValue = [this](T element){ return bool(element.value); };
 
	    using namespace boost::adaptors;
        auto temp = boost::copy_range<std::vector<T>>(indexes | filtered(nthRow)
                                                              | transformed(indexToValueFromVector));
                                                              //| filtered(hasValue));
 
        display();
        return temp.size() == std::set<T>(temp.begin(), temp.end()).size();
	}
 
	void display() const
	{
	    using namespace boost::adaptors;
 
	    for(const auto& el : vec | indexed(0))
	    {
	        if(el.index() % cols == 0) std::cout << std::endl;
	        std::cout << el.value();
	    }
	}
private:
    size_t rows;
    size_t cols;
	std::vector<T> vec;
};
 
struct Cell
{
    Cell(std::optional<size_t> value = std::nullopt,
         bool isModifiable = true)
    : value{value}
    , isModifiable{isModifiable}
    {}
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell)
    {
        os << (cell.value ? *(cell.value) : static_cast<?>(' '));
        return os;
    }
    bool operator==(const Cell& rhs) const { return value == rhs.value; }
    bool operator<(const Cell& rhs) const { return value < rhs.value; }
    void increment() { if(value) ++(*value); }
    void changeValue(size_t new_value) { /*value.emplace(new_value);*/ value = (new_value); }
 
	std::optional<size_t> value;
	bool isModifiable;
};
 
class SudokuSolver
{
public:
	SudokuSolver(const std::string& sudokuGrid)
	: actualIndex{0}
	, sudokuVector(3, 3)//99
	{
	    for(const auto& value : sudokuGrid)
	    {
	        sudokuVector.emplace_back(isdigit(value)
	                                  ? Cell(static_cast<size_t>(value-'0'), false)
	                                  : Cell());
	    }
	}
	void solve()
	{
		while(actualIndex < 9)//while(actualIndex < 81)
		{
		    std::cout << "*" << actualIndex << "*";
		    //break;//remove this
		    //std::cout <<"^^"<<isok()<<"^^";
		    if(isok())
		    {
		        std::cout << "isok";
		        //break;//remove this
		        goToNextModifiableCell();
		        std::cout << "***" << actualIndex << "***";
		        setLowestPossibleValueInActualCell();
		        std::cout << *actualCell().value;
		        display();
		        break;
		    }
		    else if(not isok() and
		            actualCell().value and
		            *actualCell().value < 9)
		    {
		        std::cout<<"dwa";
		        //break;//remove this
		        incrementValueInActualCell();
            }
	        else
		    {
		        //break;//remove this
		        std::cout << "trzy";
		        cleanActualCell();
		        goToPreviousModifiableCell();
		    }
		    //break;
		}
	}
	void display()
	{
		sudokuVector.display();
	}
private:
	size_t actualIndex;
	FlattenVector<Cell> sudokuVector;
 
    bool isok()//fix this
    {
        for(auto i : boost::irange<size_t>(0, 3))//9
        {
            std::cout <<""<<std::endl;
            if(not (sudokuVector.checkNthRowCorrectness(i) and
                    sudokuVector.checkNthColCorrectness(i) and
                    sudokuVector.checkNthSqrCorrectness(i)))
            {
                std::cout <<"ddd";
                return false;
            }
        }
        return true;
    }
    Cell& actualCell() { return sudokuVector[actualIndex]; }
    const Cell& actualCell() const { return sudokuVector[actualIndex]; }
 
    void goToNextModifiableCell() { while(not actualCell().isModifiable) { ++actualIndex; } }
    void goToPreviousModifiableCell() { while(not actualCell().isModifiable) { --actualIndex; } }
    void cleanActualCell() { actualCell() = Cell(); }
    //void setLowestPossibleValueInActualCell() { actualCell() = Cell(1); }
    void setLowestPossibleValueInActualCell() { actualCell().changeValue(1); }//sudokuVector[actualIndex].value.emplace('5'); std::cout << "$$"<<actualIndex<<sudokuVector[4]; }
    void incrementValueInActualCell() { actualCell().increment(); }
};
 
int main()
{
    const std::string sudokuGrid{"123"
                                 "4 6"
                                 "789"};
	SudokuSolver sudokuSolver(sudokuGrid);
	sudokuSolver.display();
	sudokuSolver.solve();
	sudokuSolver.display();
	return 0;
}
