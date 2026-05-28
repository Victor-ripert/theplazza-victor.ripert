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
};

Kitchen::~Kitchen() {

};

void Kitchen::run() {
    std::string pizza_data;
    Pizza pizza;
    while (true)
    {
        _ipc >> pizza_data;
        pizza = _serializer.unpack(pizza_data);
        _threadPool.enqueuePizza(pizza);
    }
};

void Kitchen::checkInactivityTimeout() {

};