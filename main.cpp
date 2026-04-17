// final/main.cpp

#include "include/algorithm.h"
#include "include/gfx.hpp"
#include <iostream>

int main() {
    std::cout << "\n";
    std::cout << color::blue << SEP << "\n";
    std::cout << "           FINAL TIRA PROJECT\n";
    std::cout << SEP << color::reset << "\n";

    for (;;) {
        std::cout << color::cyan << "Select an operation\n" << color::reset;
        std::cout << SUB << "\n";

        std::cout << "  [1] Linear Search\n";
        std::cout << "  [2] Binary Search\n";
        std::cout << "  [3] Insertion Sort (N^2)\n";
        std::cout << "  [4] Heap Sort (NlogN)\n";
        std::cout << "  [5] Sorting Performance Comparison\n";
        std::cout << "  [6] Quit\n";

        std::cout << SUB << "\n";

        int input;
        while (true) {
            std::cout << color::yellow << "Your choice > " << color::reset;
            if (std::cin >> input && input >= 1 && input <= 6) break;

            std::cout << color::red << "Invalid choice. Try again.\n" << color::reset;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (input == 6) {
            std::cout << "\n" << color::green << "Exiting...\n" << color::reset;
            break;
        }

        std::cout << "\n" << color::blue << SUB << color::reset << "\n";

        int pop_number;
        while (true) {
            std::cout << "Enter array size: ";
            if (std::cin >> pop_number && pop_number > 0 && pop_number <= 5'000'000) 
                break;

            std::cout << color::red << "Invalid size. Must be between 1 and 5,000,000.\n" << color::reset;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        int key = 0;
        while (true && input <= 2) {
            std::cout << "Search key: ";
            if (std::cin >> key) 
                break;

            std::cout << color::red << "Invalid key. Try again.\n" << color::reset;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << color::blue << SUB << color::reset << "\n";

        switch (input) {
            case 1:
                linear_search(pop_number, key);
                break;
            case 2:
                binary_search(pop_number, key);
                break;
            case 3:
                insertion_sort(pop_number);
                break;
            case 4:
                heap_sort(pop_number);
                break;
            case 5:
                std::cout << "\nPlease wait.\n";
                performance_test(pop_number);
                break;
            default:
                std::cout << color::red << "Unknown input.\n" << color::reset;
        }
    }

    return 0;
}