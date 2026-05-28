/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Thread
*/

#include "utils/Thread.hpp"

Thread::Thread() : _thread(0), _isJoinable(false) {}

Thread::Thread(std::function<void()> routine) : _thread(0), _isJoinable(false) {
    start(routine);
}

Thread::~Thread() {
    if (_isJoinable) {
        pthread_detach(_thread);
    }
}

static void *thread_routine(void *arg) {
    auto *routine = static_cast<std::function<void()>*>(arg);
    (*routine)();
    delete routine;
    return nullptr;
}

void Thread::start(std::function<void()> routine) {
    auto *routine_ptr = new std::function<void()>(routine);
    if (pthread_create(&_thread, nullptr, thread_routine, routine_ptr) != 0) {
        delete routine_ptr;
        return;
    }
    _isJoinable = true;
}

void Thread::join() {
    if (_isJoinable) {
        pthread_join(_thread, nullptr);
        _isJoinable = false;
    }
}

void Thread::detach() {
    if (_isJoinable) {
        pthread_detach(_thread);
        _isJoinable = false;
    }
}
