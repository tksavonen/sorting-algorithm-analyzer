// final/algorithm.cpp

#include "include/algorithm.h"
#include "include/memory.hpp"
#include "include/gfx.hpp"
#include "include/spinner.hpp"
#include "include/export.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

// ------ HELPER FUNCTIONS ------
std::vector<int> generate_data(int n) {
    std::vector<int> v(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);

    for (int i = 0; i < n; i++)
        v[i] = dis(gen);

    return v;
}

double measure_time(std::function<void()> func, int trials = 2) {
    double total = 0;

    for (int i = 0; i < trials; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();

        total += std::chrono::duration<double, std::micro>(end - start).count();
    }

    return total / trials; // avg
}

void display_stats(std::chrono::high_resolution_clock::time_point end, 
    std::chrono::high_resolution_clock::time_point start) {
    std::cout << color::blue << SUB << color::reset << "\n";
    std::cout << "Operation took "
              << std::chrono::duration<double, std::micro>(end - start).count()
              << " µs.\n";
    std::cout << color::blue << SUB << color::reset << "\n";

    std::cout << color::cyan << "Memory Usage\n" << color::reset;
    std::cout << "Current:" << std::setw(17)    << TrackingAllocator<int>::current() / 1024.0 << " KB\n";
    std::cout << "Peak:"    << std::setw(20)    << TrackingAllocator<int>::peak() / 1024.0 << " KB\n";
    
    std::cout << color::blue << SEP << color::reset << "\n\n";
}

// ------ SEARCH ALGORITHMS ------
void linear_search(int pop_number, int key) {
    TrackingAllocator<int>::reset();
    auto v = generate_array_data<TrackingAllocator<int>>(pop_number);

    auto start = std::chrono::high_resolution_clock::now();
    auto it = std::find(v.begin(), v.end(), key);

    int index = -1;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == key) {
            index = i;
            break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "\n" << color::blue << SEP << "\n";
    std::cout << "          Linear Search Result\n";
    std::cout << SEP << color::reset << "\n";

    if (it != v.end())
        std::cout << color::green << "Key found at position " << (it - v.begin()) + 1 << "!" 
        << "\n" << color::reset;
    else
        std::cout << color::red << "Key not found." << "\n" << color::reset;

    display_stats(end, start);
}

void binary_search(int pop_number, int key) {
    TrackingAllocator<int>::reset();
    auto v = generate_array_data<TrackingAllocator<int>>(pop_number);
    sort(v.begin(), v.end());

    auto start = std::chrono::high_resolution_clock::now();
    int left = 0, right = v.size() - 1;
    int index = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
   
        if (v[mid] == key) {
            index = mid;
            break;
        }
  
        if (v[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;     
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "\n" << color::blue << SEP << "\n";
    std::cout << "        Binary Search Result\n";
    std::cout << SEP << color::reset << "\n";

    if (index != -1)
        std::cout << color::green << "Key found at position " << index + 1 << "!" 
        << "\n" << color::reset;
    else
        std::cout << color::red << "Key not found." << "\n" << color::reset;

    display_stats(end, start);
}

// ------ SORTING ALGORITHMS ------
void heap_sort(int pop_number) {
    TrackingAllocator<int>::reset();
    auto v = generate_array_data<TrackingAllocator<int>>(pop_number);

    std::make_heap(v.begin(), v.end());
    auto start = std::chrono::high_resolution_clock::now();
    std::sort_heap(v.begin(), v.end());
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "\n" << color::blue << SEP << "\n";
    std::cout << "        Heap Sort Result\n";
    std::cout << SEP << color::reset << "\n";

    std::cout << color::green << "Heap sort completed.\n" << color::reset;

    display_stats(end, start);
}

void insertion_sort(int pop_number) {
    TrackingAllocator<int>::reset();
    auto v = generate_array_data<TrackingAllocator<int>>(pop_number);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i < pop_number; i++) {
        int current = v[i];
        int j = i - 1;

        while (j >= 0 && v[j] > current) {
            v[j + 1] = v[j];
            j--;
        }

        v[j + 1] = current;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "\n" << color::blue << SEP << "\n";
    std::cout << "        Insertion Sort Result\n";
    std::cout << SEP << color::reset << "\n";

    std::cout << color::green << "Insertion sort completed.\n" << color::reset;

    display_stats(end, start);
}

void performance_test(int pop_number) {
    auto original = generate_array_data<TrackingAllocator<int>>(pop_number);
    std::shuffle(original.begin(), original.end(), std::mt19937(std::random_device{}()));

    TrackingAllocator<int>::reset();
    SortStats i_stats, b_stats, m_stats;
    i_stats.reset(); b_stats.reset(); m_stats.reset();

    running = true;
    std::thread t(spinner_thread);

    double insertion_time = measure_time([&]() {
        auto v = original;

        for (int i = 1; i < pop_number; i++) {
        int current = v[i];
        i_stats.assignments++;
        int j = i - 1;
        i_stats.assignments++;

        while (true) {
            i_stats.comparisons++;
            if (j < 0) break;

            i_stats.comparisons++; 
            if (v[j] <= current) break;

            v[j + 1] = v[j];
            j--;

            i_stats.assignments++;
            i_stats.operations++;
        }

        v[j + 1] = current;
        i_stats.assignments++;
    }
    });

    double bubble_time = measure_time([&]() {
        auto v = original;
        for (int i = 0; i < pop_number - 1; i++) {
            for (int j = 0; j < pop_number - i - 1; j++) {
                b_stats.comparisons++;
                if (v[j] > v[j + 1]) {
                    std::swap(v[j], v[j + 1]);
                    b_stats.assignments += 3;
                }
            }
        }
    });

    double merge_time = measure_time([&]() {
        auto v = original;
        merge_sort_recursive(v, 0, pop_number - 1, m_stats);
    });

    running = false;
    t.join();

    // results
    std::cout << "\n" << color::blue << SEP << "\n";
    std::cout << "      Sorting Performance Results\n";
    std::cout << SEP << color::reset << "\n";

    double fastest = std::min({insertion_time, bubble_time, merge_time});
    double slowest = std::max({insertion_time, bubble_time, merge_time});
    double ratio = slowest / fastest;

    auto getColor = [&](double t) {
        if (t == fastest) return color::green;
        if (t == slowest) return color::red;
        return color::yellow;
    };

    std::cout << std::left
            << std::setw(18) << "Algorithm"
            << std::setw(15) << "Time (µs)"
            << std::setw(15) << "Assignments"
            << std::setw(15) << "Comparisons"
            << "\n";

    std::cout << color::blue << SUB << color::reset << "\n";

    std::cout << getColor(insertion_time)
            << std::setw(18) << "Insertion Sort"
            << std::setw(15) << insertion_time
            << std::setw(15) << i_stats.assignments
            << std::setw(15) << i_stats.comparisons
            << color::reset << "\n";

    std::cout << getColor(bubble_time)
            << std::setw(18) << "Bubble Sort"
            << std::setw(15) << bubble_time
            << std::setw(15) << b_stats.assignments
            << std::setw(15) << b_stats.comparisons
            << color::reset << "\n";

    std::cout << getColor(merge_time)
            << std::setw(18) << "Merge Sort"
            << std::setw(15) << merge_time
            << std::setw(15) << m_stats.assignments
            << std::setw(15) << m_stats.comparisons
            << color::reset << "\n";

    std::cout << color::blue << SUB << color::reset << "\n";

    std::string fastest_name =
        (fastest == insertion_time) ? "Insertion Sort" :
        (fastest == bubble_time)    ? "Bubble Sort"    : "Merge Sort";

    std::string slowest_name =
        (slowest == insertion_time) ? "Insertion Sort" :
        (slowest == bubble_time)    ? "Bubble Sort"    : "Merge Sort";

    std::cout << color::green << "Fastest: " << fastest_name << "\n";
    std::cout << color::red   << "Slowest: " << slowest_name << "\n";
    std::cout << color::reset;
    std::cout << slowest_name << " was " << ratio << "x slower than " << fastest_name << ".\n";

    std::cout << color::blue << SUB << color::reset << "\n";

    std::cout << color::cyan << "Memory Usage\n" << color::reset;

    std::cout << std::setw(20) << "Current:"
            << TrackingAllocator<int>::current() / 1024.0 << " KB\n";

    std::cout << std::setw(20) << "Peak:"
            << TrackingAllocator<int>::peak() / 1024.0 << " KB\n";

    std::cout << color::blue << SEP << color::reset << "\n\n";
    std::cout << "Saved to results.csv.\n\n";

    export_results_to_csv("results.csv", pop_number, insertion_time, bubble_time,
        merge_time, i_stats, b_stats, m_stats);

}
