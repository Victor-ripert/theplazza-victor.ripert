/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Mutex
*/

/*
 * Encapsulates POSIX mutexes to provide a secure, object-oriented locking mechanism.
 * Used to prevent race conditions when multiple threads access shared resources.
 */

#ifndef MUTEX_HPP_
    #define MUTEX_HPP_

#include <pthread.h>

class Mutex {
    public:
        Mutex();
        ~Mutex();
        void lock();
        void unlock();
        pthread_mutex_t *getNativeHandle();

    private:
        pthread_mutex_t _mutex;
};

#endif
