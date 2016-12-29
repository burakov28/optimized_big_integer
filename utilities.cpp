#pragma once
#ifndef UTILITIES
#define UTILITIES

#include <iostream>
#include <cstdlib>

static void my_assert(bool expr) {
    if (!expr) {
        std::cout << "Assertion failed" << std::endl;
        exit(0);
    }
}

#endif