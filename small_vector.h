#ifndef SMALL_VECTOR
#define SMALL_VECTOR

#include "base_vector.h"
#include "utilities.h"
#include <iostream>


using std::cout;
using std::endl;                                                  

template <typename T>
struct small_vector : base_vector < T > {
    static const uint8_t capacity = 32 / sizeof(T);
    uint8_t _size;
    T data[capacity];

    small_vector() : _size(0) {}

    small_vector(size_t new_size) : _size(new_size) {
        my_assert(new_size <= capacity);  
    }

    small_vector(size_t new_size, T const & value) : _size(new_size) {
        my_assert(new_size <= capacity);
        for (size_t i = 0; i < new_size; ++i) data[i] = value;
    }

    small_vector(small_vector < T > const & sv) : _size(sv._size) {                    
        for (size_t i = 0; i < _size; ++i) {
            data[i] = sv.data[i];
        }
    }

    ~small_vector() {}

    small_vector < T > & operator= (small_vector < T > const & sv) {
        _size = sv._size;
        for (size_t i = 0; i < _size; ++i) {
            data[i] = sv.data[i];
        }  
        return *this;
    }

    void push_back(T const & value) {
        my_assert(_size < capacity);
        data[_size] = value;
        ++_size;   
    }

    void pop_back() {
        my_assert(_size > 0);
        --_size;
    }

    void clear() {
        _size = 0;
    }

    T & operator[] (size_t pos) {
        //cout << "s" << endl;
        my_assert(pos < _size);
        return data[pos];
    }

    T const & operator[] (size_t pos) const {
        my_assert(pos < _size);
        return data[pos];
    }

    T & back() {
        my_assert(_size > 0);
        return data[_size - 1];
    }

    bool is_full() const {
        return _size == capacity;
    }

    T const & back() const {
        my_assert(_size > 0);
        return data[_size - 1];
    }

    void reverse() {
        if (_size < 2) return;
        for (size_t i = 0; i < _size / 2; ++i) {
            std::swap(data[i], data[_size - i - 1]);
        }
    }

    size_t size() const {
        return _size;
    }    
};

#endif