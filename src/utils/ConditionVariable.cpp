/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** ConditionVariable
*/

#include "utils/ConditionVariable.hpp"

ConditionVariable::ConditionVariable() {
    pthread_cond_init(&_cond, nullptr);
}

ConditionVariable::~ConditionVariable() {
    pthread_cond_destroy(&_cond);
}

void ConditionVariable::wait(Mutex& mutex) {
    pthread_cond_wait(&_cond, mutex.getNativeHandle());
}

void ConditionVariable::notifyOne() {
    pthread_cond_signal(&_cond);
}

void ConditionVariable::notifyAll() {
    pthread_cond_broadcast(&_cond);
}
