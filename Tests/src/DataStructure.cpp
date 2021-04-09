#include "DataStructure.hpp"


#include <utility>


    Simple::Simple(int valInt, float valFloat) : aInt{valInt}, aFloat{valFloat}
    {
    }

    bool operator==(const Simple& first, const Simple& second)
    {
        return first.aInt == second.aInt && first.aFloat == second.aFloat;
    }
    bool operator!=(const Simple& first, const Simple& second)
    {
        return first.aInt != second.aInt && first.aFloat == second.aFloat;
    }

    std::ostream& operator<<(std::ostream& os, const Simple& simple)
    {
        return os << "Simple<int=" << simple.aInt << ">- float=<" << simple.aFloat << ">";
    }


    Dynamic::~Dynamic()
    {
        // std::cout << "Deleted" << std::endl;
        delete[] dynamic;
    }

    Dynamic::Dynamic(int valInt, float valFloat) : aInt{valInt}, aFloat{valFloat}
    {
    }

    Dynamic::Dynamic(const Dynamic& other) : aInt(other.aInt), aFloat(other.aFloat)
    {
        std::copy(other.dynamic, other.dynamic + 33, dynamic);
    }

    Dynamic::Dynamic(Dynamic&& other) : Dynamic()
    {
        swap(*this, other);
    }

    // Let the compiler do the copy for us
    Dynamic& Dynamic::operator=(Dynamic other)
    {
        swap(*this, other);

        return *this;
    }

    void swap(Dynamic& first, Dynamic& second) noexcept
    {
        using std::swap;

        swap(first.aInt, second.aInt);
        swap(first.aFloat, second.aFloat);
        swap(first.dynamic, second.dynamic);
    }

    bool operator==(const Dynamic& first, const Dynamic& second)
    {
        return first.aInt == second.aInt && first.aFloat == second.aFloat;
    }
    bool operator!=(const Dynamic& first, const Dynamic& second)
    {
        return first.aInt != second.aInt || first.aFloat != second.aFloat;
    }

    std::ostream& operator<<(std::ostream& os, const Dynamic& dynamic)
    {
        return os << "Dynamic{int=" << dynamic.aInt << " float=" << dynamic.aFloat << "}";
    }
