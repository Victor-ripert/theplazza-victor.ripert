/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Mutex
*/

#include "utils/Mutex.hpp"

Mutex::Mutex() {
    pthread_mutex_init(&_mutex, nullptr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&_mutex);
}

void Mutex::lock() {
    pthread_mutex_lock(&_mutex);
}

void Mutex::unlock() {
    pthread_mutex_unlock(&_mutex);
}

pthread_mutex_t *Mutex::getNativeHandle() {
    return &_mutex;
}
