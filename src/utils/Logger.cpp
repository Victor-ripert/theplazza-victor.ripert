/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Logger
*/

#include "utils/Logger.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>

Logger::Logger(const std::string& filename) : _filename(filename) {
    _file.open(_filename, std::ios::app);
}

Logger::~Logger() {
    if (_file.is_open()) {
        _file.close();
    }
}

void Logger::log(const std::string& message) {
    _mutex.lock();
    if (_file.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        _file << std::put_time(std::localtime(&in_time_t), "[%Y-%m-%d %H:%M:%S] ")
              << message << std::endl;
        _file.flush();
    }
    _mutex.unlock();
}
