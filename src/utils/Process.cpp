/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Process
*/

#include "utils/Process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

Process::Process() : _pid(-1) {}

Process::~Process() {}

pid_t Process::spawn() {
    _pid = fork();
    return _pid;
}

void Process::wait() {
    if (_pid > 0) {
        waitpid(_pid, nullptr, 0);
        _pid = -1;
    }
}

void Process::terminate() {
    if (_pid > 0) {
        kill(_pid, SIGTERM);
        _pid = -1;
    }
}
