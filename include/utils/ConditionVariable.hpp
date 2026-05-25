/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** ConditionVariable
*/

/*
 * Encapsulates POSIX condition variables.
 * Allows threads to safely suspend execution and wait to be notified when a specific state changes
 * (e.g., waking up a cook when an order arrives).
 */

#ifndef CONDITIONVARIABLE_HPP_
    #define CONDITIONVARIABLE_HPP_

#include <pthread.h>
#include "Mutex.hpp"

class ConditionVariable {
    public:
        ConditionVariable();
        ~ConditionVariable();
        void wait(Mutex& mutex);
        void notifyOne();
        void notifyAll();

    private:
        pthread_cond_t _cond;
};

#endif
