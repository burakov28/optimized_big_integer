#ifndef LARGE_VECTOR
#define LARGE_VECTOR
#include "small_vector.h"
#include "base_vector.h"
#include "utilities.h"

using std::endl;
using std::cout;

template <typename T>
struct large_vector : base_vector < T > {
    static const size_t MIN_CAPACITY = 32 / sizeof(T);
    
    struct storage {
        uint32_t num_ptr;
        T data[];
    };

    size_t _size, capacity;
    T * data;

    large_vector() : _size(0), capacity(MIN_CAPACITY) {
        data = allocate(MIN_CAPACITY);
    }

    large_vector(size_t new_size) : _size(new_size), capacity(std::max(MIN_CAPACITY, new_size)) {
        data = allocate(capacity);
    }

    large_vector(size_t new_size, T const & value) : _size(new_size), capacity(std::max(MIN_CAPACITY, new_size)) {
        data = allocate(capacity);
        for (size_t i = 0; i < _size; ++i) {
            data[i] = value;
        }
    }

    large_vector(large_vector < T > const & lv) : _size(lv._size), capacity(lv.capacity), data(lv.data) {
        lv.get_num_ptr() += 1;        
    }

    large_vector(small_vector < T > const & sv) : _size(sv.size()), capacity(MIN_CAPACITY) {
        data = allocate(MIN_CAPACITY);
        for (size_t i = 0; i < sv.size(); ++i) {
            data[i] = sv[i];
        }
    }

    large_vector < T > & operator= (large_vector < T > const & other) {
        
        _size = other._size;
        capacity = other.capacity;

        uint32_t & num_ptr = get_num_ptr();
        --num_ptr;
        if (num_ptr == 0) {
            free(to_storage(data));
        }
        data = other.data;
        ++(other.get_num_ptr());
        return *this;
    }

    void construct_on_bad_memory(large_vector < T > const & other) {
        //cout << "hell" << endl;
        _size = other._size;
        capacity = other.capacity;
        data = other.data;
        ++(other.get_num_ptr());
    }

    ~large_vector() {
        uint32_t & num_ptr = get_num_ptr();
        --num_ptr;
        if (num_ptr == 0) {
            free(to_storage(data));
        }
    }

    void push_back(T const & value) {
        uint32_t & num_ptr = get_num_ptr();
        T * new_data;
        if (num_ptr != 1) {
            --num_ptr;
            size_t need = capacity;
            if (_size == capacity) {
                need <<= 1;
            }
            new_data = allocate(need);
            for (size_t i = 0; i < _size; ++i) {
                new_data[i] = data[i];
            }
            data = new_data;
            capacity = need;
        }
        else {
            if (_size == capacity) {
                capacity <<= 1;
                new_data = allocate(capacity);
                //cout << "start" << endl;
                for (size_t i = 0; i < _size; ++i) {
                    new_data[i] = data[i];                    
                }
                
                free(to_storage(data));
                data = new_data;
            }
        }
        data[_size] = value;
        ++_size;
        for (size_t i = 0; i < _size; ++i) {
            //cout << data[i] << " ";
        }
         
        //cout << endl;
        //cout << "end" << endl;
    }

    void pop_back() {
        my_assert(_size > 0);
        --_size;

        if (_size * 4 < capacity) {
            change_capacity(_size * 2);
        }
    }

    void clear() {
        _size = 0;
        change_capacity(MIN_CAPACITY);  
    }

    size_t size() const {
        return _size;
    }

    T & operator[] (size_t pos) {
        //cout << "great" << endl;
        my_assert(pos < _size);
        if (get_num_ptr() > 1) {
            change_capacity(capacity);
        }
        return data[pos];
    }

    T const & operator[] (size_t pos) const {
        my_assert(pos < _size);
        return data[pos];
    }

    T & back() {
        my_assert(_size > 0);
        if (get_num_ptr() > 1) {
            change_capacity(capacity);
        }
        return data[_size - 1];
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


    void change_capacity(size_t new_cap) {
        new_cap = std::max(new_cap, MIN_CAPACITY);
        uint32_t & num_ptr = get_num_ptr();
        --num_ptr;

        T * new_data = allocate(new_cap);
        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = data[i];
        }
        capacity = new_cap;
        if (num_ptr == 0) {
            free(to_storage(data));
        }
        data = new_data;

    }

    uint32_t & get_num_ptr() const {
        return to_storage(data) -> num_ptr;
    }
    
    T * allocate(size_t new_cap) const {
        storage * st = (storage *) malloc(sizeof(T) * new_cap + 4);
        st -> num_ptr = 1;
        return to_data(st);         
    }

    T * to_data(storage const * st) const {
        return (T *) (((char *) st) + 4);
    }

    storage * to_storage(T const * d) const {
        return (storage *) (((char *) d) - 4);
    }
};

#endif