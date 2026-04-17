// final/export.cpp

#include "include/algorithm.h"
#include "include/memory.hpp"
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

std::string current_timestamp() {
    using clock = std::chrono::system_clock;
    auto now = clock::now();
    std::time_t t = clock::to_time_t(now);

    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void export_results_to_csv(const std::string& filename, int pop_number, double insertion_time,
    double bubble_time, double merge_time, const SortStats& i_stats, const SortStats& b_stats,
    const SortStats& m_stats) {
    bool file_exists = std::ifstream(filename).good();
    std::ofstream out(filename, std::ios::app);

    if (!file_exists) {
        out << "Timestamp,Population,Algorithm,Time(ms),"
               "Comparisons,Assignments,Operations,"
               "CurrentMemory(KB),PeakMemory(KB)\n";
    }

    double current_kb = TrackingAllocator<int>::current() / 1024.0;
    double peak_kb    = TrackingAllocator<int>::peak() / 1024.0;

    auto write_row = [&](const char* name, double time, const SortStats& s) {
        out << current_timestamp() << ","
            << pop_number << ","
            << name << ","
            << time << ","
            << s.comparisons << ","
            << s.assignments << ","
            << s.operations << ","
            << current_kb << ","
            << peak_kb << "\n";
    };

    write_row("Insertion Sort", insertion_time, i_stats);
    write_row("Bubble Sort",    bubble_time,    b_stats);
    write_row("Merge Sort",     merge_time,     m_stats);
}

