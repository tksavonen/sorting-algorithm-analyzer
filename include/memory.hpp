// final/include/memory.hpp

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <memory>
#include <vector>
#include <atomic>
#include <cstddef>

template <typename T>
class TrackingAllocator {
public:
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    template <typename U>
    struct rebind {
        using other = TrackingAllocator<U>;
    };

    // ------ CONSTRUCTORS ------
    TrackingAllocator() noexcept = default;

    template <typename U>
    TrackingAllocator(const TrackingAllocator<U>&) noexcept {}

    // allocation
    T* allocate(std::size_t n) {
    size_type bytes = n * sizeof(T);
    size_type current = allocated.fetch_add(bytes) + bytes;
    size_type peak = peak_allocated.load();
    while (current > peak &&
           !peak_allocated.compare_exchange_weak(peak, current)) {
    }

    return std::allocator<T>().allocate(n);
}

    // deallocation
    void deallocate(T* p, std::size_t n) noexcept {
        size_type bytes = n * sizeof(T);
        allocated -= bytes;
        std::allocator<T>().deallocate(p, n);
    }

    // comparison operators
    bool operator==(const TrackingAllocator&) const noexcept {
        return true;
    }

    bool operator!=(const TrackingAllocator&) const noexcept {
        return false;
    }

    // static tracking variables
    static std::atomic<size_type> allocated;
    static std::atomic<size_type> peak_allocated;

    // ------ HELPER FUNCTIONS ------
    static void reset() {
        allocated = 0;
        peak_allocated = 0;
    }

    static size_type current() {
        return allocated.load();
    }

    static size_type peak() {
        return peak_allocated.load();
    }
};

template <typename T>
std::atomic<typename TrackingAllocator<T>::size_type>
    TrackingAllocator<T>::allocated{0};

template <typename T>
std::atomic<typename TrackingAllocator<T>::size_type>
    TrackingAllocator<T>::peak_allocated{0};

template <typename T, typename Alloc>
void merge(std::vector<T, Alloc>& v, int left, int mid, int right, SortStats& stats) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<T> L(n1), R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = v[left + i];
        stats.assignments++;
        stats.operations++;
    }

    for (int j = 0; j < n2; j++) {
        R[j] = v[mid + 1 + j];
        stats.assignments++;
        stats.operations++;
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        stats.comparisons++; 

        if (L[i] <= R[j]) {
            v[k++] = L[i++];
            stats.assignments++;
            stats.operations++;
        } else {
            v[k++] = R[j++];
            stats.assignments++;
            stats.operations++;
        }
    }

    while (i < n1) {
        v[k++] = L[i++];
        stats.assignments++;
        stats.operations++;
    }

    while (j < n2) {
        v[k++] = R[j++];
        stats.assignments++;
        stats.operations++;
    }
}

template <typename Allocator = std::allocator<int>>
std::vector<int, Allocator> generate_array_data(int n) {
    std::vector<int, Allocator> v;
    v.reserve(n);

    for (int i = 0; i < n; ++i)
        v.push_back(i * 2);

    return v;
}

template <typename T, typename Alloc>
void merge_sort_recursive(std::vector<T, Alloc>& v, int left, int right, SortStats& stats) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    merge_sort_recursive(v, left, mid, stats);
    merge_sort_recursive(v, mid + 1, right, stats);

    merge(v, left, mid, right, stats);
}

#endif