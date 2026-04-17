// final/include/export.h

#ifndef EXPORT_H
#define EXPORT_H

#include <fstream>
#include "algorithm.h"

void export_results_to_csv(const std::string& filename, int pop_number,
    double insertion_time, double bubble_time, double merge_time, const SortStats& i_stats,
    const SortStats& b_stats, const SortStats& m_stats);

#endif