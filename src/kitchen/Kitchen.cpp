/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Kitchen
*/

#include "kitchen/Kitchen.hpp"

Kitchen::Kitchen(int cooksCount, int restockTime, float multiplier)
    :_cooksCount(cooksCount), _restockTime(restockTime), _multiplier(multiplier), _stock(_restockTime), _threadPool(_cooksCount, _stock, multiplier) {
        _stock.startRestockLoop();
        _lastActivityTime = std::chrono::steady_clock::now();
};

Kitchen::~Kitchen() {
    _stock.stopRestockLoop();
    _threadPool.endPool();
};

void Kitchen::run() {
    std::string pizza_data;
    Pizza pizza;
    while (_running) {
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