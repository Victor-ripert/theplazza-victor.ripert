/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Thread
*/

/*
 * Wrapper for POSIX threads.
 * Manages the lifecycle of a single thread, including starting routines, joining, and detaching.
 */

#ifndef THREAD_HPP_
    #define THREAD_HPP_

#include <pthread.h>
#include <functional>

class Thread {
    public:
        Thread();
        Thread(std::function<void()> routine);
        ~Thread();
        void start(std::function<void()> routine);
        void join();
        void detach();

    private:
        pthread_t _thread;
        bool _isJoinable;
};

#endif
