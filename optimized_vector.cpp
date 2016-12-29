#ifndef OPTIMIZED_VECTOR
#define OPTIMIZED_VECTOR


#include "utilities.cpp"
#include "small_vector.cpp"
#include "large_vector.cpp"
#include <cassert>

using std::cout;
using std::endl;

template <typename T>

struct optimized_vector {
    bool is_large;   
    union {
        large_vector < T > lv;
        small_vector < T > sv;
    };

    optimized_vector() : is_large(false), sv() {}
                                                                         
    optimized_vector(size_t new_size) {
        if (small_vector < T >::capacity < new_size) {
            is_large = true;
            lv.construct_on_bad_memory(large_vector < T > (new_size));
        }
        else {
            is_large = false;
            sv = small_vector < T > (new_size);
        }
    }

    optimized_vector(size_t new_size, T const & value) {
        if (small_vector < T >::capacity < new_size) {
            is_large = true;
            lv.construct_on_bad_memory(large_vector < T > (new_size, value));
        }
        else {
            is_large = false;
            sv = small_vector < T > (new_size, value);
        }
    }
    
    optimized_vector(optimized_vector < T > const & other) : is_large(other.is_large) {
        
        if (other.is_large) {
            lv.construct_on_bad_memory(other.lv);
            //lv = other.lv;
        }
        else {
            sv = other.sv;
        }
    }

    optimized_vector < T > & operator= (optimized_vector < T > const & other) {        
        //cout << "pol" << endl;
        if (other.is_large) {
            if (!is_large) {
                lv.construct_on_bad_memory(other.lv);       
            }
            else {
                lv = other.lv;
            }                   
        }
        else {
            if (is_large) {
                (& lv) -> ~large_vector < T >();
            }
            sv = other.sv;
        }
        is_large = other.is_large;
        return *this;
    }

    ~optimized_vector() {
        if (is_large) {
            (& lv) -> ~large_vector < T >();  
        }
    }

    void push_back(T const & value) {
        if (is_large) {
            lv.push_back(value);
        }
        else {
            if (sv.is_full()) {
                //std::cout << "lo2" << std::endl;
                small_vector < T > tmp = sv;
                
                
                lv.construct_on_bad_memory(large_vector < T > (tmp));
                /*
                std::cout << "hello2" << endl;
                for (int i = 0; i < (int) lv.size(); ++i) {
                    std::cout << lv[i] << " ";
                }
                std::cout << std::endl;
                */
                is_large = true;
                lv.push_back(value);
            }
            else {
                sv.push_back(value);
            }
        }
    }

    void pop_back() {
        if (is_large) {
            lv.pop_back();            
        }
        else {
            sv.pop_back();
        }
        //v -> pop_back();
    }

    void clear() {
        if (is_large) {
            (& lv) -> ~large_vector < T >();
        }
        is_large = false;
        sv = small_vector < T >();
    }

    T & operator[] (size_t pos) {
        if (is_large) {
            return lv[pos];
        }
        else {
            return sv[pos];
        }
        //return v -> operator[] (pos);
    }

    T const & operator[] (size_t pos) const {
        if (is_large) {
            return lv[pos];
        }
        else {
            return sv[pos];
        }
    }

    T & back() {
        if (is_large) {
            return lv.back();
        }
        else {
            return sv.back();
        }
    }

    T const & back() const {
        if (is_large) {
            return lv.back();
        }
        else {
            return sv.back();
        }

    }

    size_t size() const {
        if (is_large) {
            return lv.size();
        }
        else {
            return sv.size();
        }

    }

    void reverse() {
        if (is_large) {
            return lv.reverse();
        }
        else {
            return sv.reverse();
        }

    }

    void resize(size_t new_size) {
        size_t sz = size();
        while (sz < new_size) {
            ++sz;
            push_back(T ());
        }

        while (sz > new_size) {
            --sz;
            pop_back();
        }
    }
};

#endif