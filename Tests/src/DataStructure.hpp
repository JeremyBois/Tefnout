#ifndef __DATASTRUCTURE__HPP
#define __DATASTRUCTURE__HPP

#include <limits>
#include <ostream>

struct Simple
{
  public:
    int aInt = 3;
    float aFloat = 2;

    Simple() = default;
    Simple(int valInt, float valFloat);

    friend bool operator==(const Simple& first, const Simple& second);
    friend bool operator!=(const Simple& first, const Simple& second);

    friend std::ostream& operator<<(std::ostream& os, const Simple& simple);
};

struct Dynamic
{
  public:
    int aInt = std::numeric_limits<int>::max();
    float aFloat = std::numeric_limits<float>::max();
    int* dynamic = new int[33];

    Dynamic() = default;
    Dynamic(int valInt, float valFloat);
    ~Dynamic();

    // Copy swap idiom
    Dynamic(const Dynamic& other);
    Dynamic(Dynamic&& other);
    Dynamic& operator=(Dynamic other);
    friend void swap(Dynamic& first, Dynamic& second) noexcept;

    friend bool operator==(const Dynamic& first, const Dynamic& second);
    friend bool operator!=(const Dynamic& first, const Dynamic& second);

    friend std::ostream& operator<<(std::ostream& os, const Dynamic& dynamic);
};

#endif
