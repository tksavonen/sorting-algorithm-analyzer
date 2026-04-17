// final/include/algorithm.h

#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <iostream>

struct Metrics {
    long long comparisons = 0;
    long long memory_bytes = 0;
    double time_us = 0;
};

struct SortStats {
    long long comparisons = 0;
    long long assignments = 0;
    long long operations = 0;
    void reset() {
        comparisons = assignments = operations = 0;
    }
};

void linear_search(int pop_number, int key);
void binary_search(int pop_number, int key);
void heap_sort(int pop_number);
void insertion_sort(int pop_number);
void performance_test(int pop_number);

#endif