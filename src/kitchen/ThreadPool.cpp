/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** ThreadPool
*/

#include "kitchen/ThreadPool.hpp"

ThreadPool::ThreadPool(int size, Stock& stock, float multiplier)
    : _stockReference(stock), _multiplier(multiplier), _running(true)
{
    _workers.reserve(size);
    for (int i = 0; i < size; ++i) {
        _workers.emplace_back(std::make_unique<Thread>());
        _workers[i]->start([this] { workerRoutine(); });
    }
}

ThreadPool::~ThreadPool() {
    _running = false;
    _condition.notifyAll();
    for (auto& worker : _workers)
        worker->join();
};

void ThreadPool::enqueuePizza(const Pizza& pizza) {
    _queueMutex.lock();
    _pizzaQueue.push(pizza);
    _queueMutex.unlock();
    _condition.notifyOne();
};

int ThreadPool::getActiveCooks() {
    _activeMutex.lock();
    int active = _activeCooks;
    _activeMutex.unlock();
    return active;
};

void ThreadPool::cook(Pizza &pizza) {
    _stockReference.waitAndTakeIngredients(pizza);
};


void ThreadPool::workerRoutine() {
    while (_running)
    {
        _queueMutex.lock();
        while (_pizzaQueue.empty() && _running)
        {
            _condition.wait(_queueMutex);
        }
        if (!_running) {
            return;
        }
        Pizza pizza = _pizzaQueue.front();
        _pizzaQueue.pop();
        _queueMutex.unlock();
        _activeMutex.lock();
            _activeCooks += 1;
        _activeMutex.unlock();
        cook(pizza);
        _activeMutex.lock();
            _activeCooks -= 1;
        _activeMutex.unlock();
    }
};

void ThreadPool::endPool() {
    if (_running)
        _running = false;
}