#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>

template <typename T, uint rowSize>
class FlattenVector
{
public:
    FlattenVector<T, rowSize>(uint size) { vec.reserve(size); };
    T& operator[](uint index) { return vec[index]; }
    const T& operator[](uint index) const { return vec[index]; }
    void emplace_back(T&& element) { vec.emplace_back(std::move(element)); }
    auto size() const { return vec.size(); }

    template <typename Range>
    auto getFilledElements(Range&& range) const
    {
        auto indexToValue = [this](uint index){ return vec[index]; };
        auto hasValue = [](const T& element){ return element; };

        using namespace boost::adaptors;
        return boost::copy_range<decltype(vec)>(range | transformed(indexToValue)
                                                      | filtered(hasValue));
    }

    friend std::ostream& operator<<(std::ostream& os, const FlattenVector<T, rowSize>& flattenVec)
    {
        for(auto index : boost::irange(0lu, flattenVec.vec.size()))
        {
            os << flattenVec.vec[index] << " ";
            if((index + 1) % rowSize == 0)
                os << std::endl;
        }
        os << std::endl;
        return os;
    }

private:
    std::vector<T> vec;
};
