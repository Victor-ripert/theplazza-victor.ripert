/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** ThreadPool
*/

#include "kitchen/ThreadPool.hpp"
#include <unistd.h>

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
    float bakingTime = 0;
    switch (pizza.type) {
        case PizzaType::Margarita: bakingTime = 1; break;
        case PizzaType::Regina: bakingTime = 2; break;
        case PizzaType::Americana: bakingTime = 2; break;
        case PizzaType::Fantasia: bakingTime = 4; break;
    }
    usleep(bakingTime * _multiplier * 1000000);
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
        _completedMutex.lock();
        _completedPizzas.push(pizza);
        _completedMutex.unlock();
    }
};

std::vector<Pizza> ThreadPool::getCompletedPizzas() {
    std::vector<Pizza> completed;
    _completedMutex.lock();
    while (!_completedPizzas.empty()) {
        completed.push_back(_completedPizzas.front());
        _completedPizzas.pop();
    }
    _completedMutex.unlock();
    return completed;
}

int ThreadPool::getQueueSize() {
    _queueMutex.lock();
    int size = _pizzaQueue.size();
    _queueMutex.unlock();
    return size;
}

void ThreadPool::endPool() {
    _running = false;
    _condition.notifyAll();
}