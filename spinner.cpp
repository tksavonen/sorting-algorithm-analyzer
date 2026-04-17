// final/spinner.cpp

#include "include/spinner.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

std::atomic<bool> running = {true};

void spinner_thread() {
    std::vector<std::string> frames = {"⠋","⠙","⠹","⠸","⠼","⠴","⠦","⠧","⠇","⠏"};
    int i = 0;

    while (running) {
        std::cout << "\rProcessing " << frames[i % frames.size()] << std::flush;
        i++;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}