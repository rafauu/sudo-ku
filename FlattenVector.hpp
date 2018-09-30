#pragma once
#include <iostream>
#include <vector>
#include <numeric>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

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
        boost::copy(flattenVec.vec,
                    std::ostream_iterator<T>(os, " "));
        std::cout << std::endl;
        return os;
    }

private:
    std::vector<T> vec;
};
