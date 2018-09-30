#pragma once
#include <iostream>
#include <optional>

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
