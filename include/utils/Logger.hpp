/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Logger
*/

#ifndef LOGGER_HPP_
    #define LOGGER_HPP_

#include <string>
#include <fstream>
#include "Mutex.hpp"

class Logger {
    public:
        Logger(const std::string& filename = "log.txt");
        ~Logger();

        void log(const std::string& message);

    private:
        std::string _filename;
        std::ofstream _file;
        Mutex _mutex;
};

#endif
