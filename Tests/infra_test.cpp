/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Infrastructure Stress Test
*/

#include "utils/Logger.hpp"
#include "utils/Thread.hpp"
#include <vector>
#include <string>
#include <iostream>

void stress_routine(Logger &logger, int thread_id) {
    for (int i = 0; i < 100; ++i) {
        logger.log("Thread " + std::to_string(thread_id) + " - message " + std::to_string(i));
    }
}

int main() {
    Logger logger("test_log.txt");
    std::vector<Thread*> threads;
    int num_threads = 50;

    std::cout << "Starting stress test with " << num_threads << " threads..." << std::endl;

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(new Thread([&logger, i]() {
            stress_routine(logger, i);
        }));
    }

    for (auto thread : threads) {
        thread->join();
        delete thread;
    }

    std::cout << "Stress test finished. Check test_log.txt for results." << std::endl;
    return 0;
}
