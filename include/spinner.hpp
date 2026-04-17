// final/include/spinner.hpp

#ifndef SPINNER_HPP
#define SPINNER_HPP

#include <atomic>

extern std::atomic<bool> running;

void spinner_thread();

#endif