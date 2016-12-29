#ifndef BASE_VECTOR
#define BASE_VECTOR


#include <cstdlib>

template <typename T>

struct base_vector {
    virtual T & operator[] (size_t) = 0;
    virtual T const & operator[] (size_t) const = 0;
    virtual T & back() = 0;
    virtual T const & back() const = 0;
    virtual size_t size() const = 0;
    virtual void reverse() = 0;
    virtual void push_back(T const &) = 0;
    virtual void pop_back() = 0;
    virtual void clear() = 0;
};

#endif