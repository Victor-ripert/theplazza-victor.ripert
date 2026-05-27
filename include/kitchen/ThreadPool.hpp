/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** ThreadPool
*/

/*
 * Manages a fixed number of cook threads within a kitchen.
 * Handles the queue of incoming pizzas and dispatches them to idle cooks using condition variables.
 */

#ifndef THREADPOOL_HPP_
    #define THREADPOOL_HPP_

#include <vector>
#include <queue>
#include <memory>
#include "../utils/Thread.hpp"
#include "../utils/Mutex.hpp"
#include "../utils/ConditionVariable.hpp"
#include "../common/Pizza.hpp"
#include "Stock.hpp"

class ThreadPool {
    public:
        ThreadPool(int size, Stock& stock, float multiplier);
        ~ThreadPool();

        void enqueuePizza(const Pizza& pizza);
        int getActiveCooks();

    private:
        void workerRoutine();
        void cook(Pizza &pizza);

        std::vector<std::unique_ptr<Thread>> _workers;
        std::queue<Pizza> _pizzaQueue;
        Mutex _queueMutex;
        Mutex _activeMutex;
        ConditionVariable _condition;

        Stock& _stockReference;
        float _multiplier;
        int _activeCooks = 0;
        bool _running;
};

#endif
