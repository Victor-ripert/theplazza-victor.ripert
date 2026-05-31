/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Kitchen
*/

#include "kitchen/Kitchen.hpp"

#include <iostream>

Kitchen::Kitchen(int cooksCount, int restockTime, float multiplier, pid_t pid)
    :_cooksCount(cooksCount), _restockTime(restockTime), _multiplier(multiplier), _stock(_restockTime), _threadPool(_cooksCount, _stock, multiplier) {
        _stock.startRestockLoop();
        _lastActivityTime = std::chrono::steady_clock::now();
        _ipc = IPCNetwork(std::to_string(pid));
        _running = true;
};

Kitchen::~Kitchen() {
    _stock.stopRestockLoop();
    _threadPool.endPool();
    std::cout << "Destroyed" << std::endl;
};

void Kitchen::sendStatus() {
    KitchenStatus status;
    status.pizzaQueueSize = _threadPool.getQueueSize();
    status.activeCooks = _threadPool.getActiveCooks();
    // status.stock = _stock.getCurrentStock();
    for (size_t i = 0; i < static_cast<size_t>(Ingredient::Count); ++i) {
        status.stock[i] = 0;
    }
    std::string serializedStatus = Serializer::packStatus(status);
    _ipc << serializedStatus;
}

void Kitchen::run() {
    std::string pizza_data;
    Pizza pizza;
    std::cout << "Initialized" << std::endl;
    while (_running) {
        sendStatus();
        _ipc >> pizza_data;
        pizza = _serializer.unpack(pizza_data);
        _threadPool.enqueuePizza(pizza);
        _lastActivityTime = std::chrono::steady_clock::now();
    }
};

void Kitchen::checkInactivityTimeout() {
    while (_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        auto elapsed = std::chrono::steady_clock::now() - _lastActivityTime;

        if (elapsed >= std::chrono::seconds(5)) {
            _running = false;
            break;
        }
    }
}