// final/include/gfx.hpp

#ifndef GFX_HPP
#define GFX_HPP

#include <iostream>

// USAGE: std::cout << color::red << "THIS IS RED!" << color::reset << "\n";
namespace color {
    constexpr const char* reset   = "\033[0m";
    constexpr const char* red     = "\033[31m";
    constexpr const char* green   = "\033[32m";
    constexpr const char* yellow  = "\033[33m";
    constexpr const char* blue    = "\033[34m";
    constexpr const char* magenta = "\033[35m";
    constexpr const char* cyan    = "\033[36m";
    constexpr const char* white   = "\033[37m";
}

constexpr const char* SEP = "========================================";
constexpr const char* SUB = "----------------------------------------";

#endif