/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Kitchen
*/

#include "kitchen/Kitchen.hpp"
#include <chrono>
#include <iostream>
#include <sys/select.h>

Kitchen::Kitchen(int cooksCount, int restockTime, float multiplier, const std::string& ipcName)
    :_cooksCount(cooksCount), _restockTime(restockTime), _ipc(ipcName, true), _stock(_restockTime), _threadPool(_cooksCount, _stock, multiplier) {
        _stock.startRestockLoop();
};

Kitchen::~Kitchen() {
    _stock.stopRestockLoop();
    _threadPool.endPool();
};

void Kitchen::run() {
    auto lastActive = std::chrono::steady_clock::now();

    while (true)
    {
        int fd = _ipc.getFdIn();
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; // 100ms

        int retval = select(fd + 1, &rfds, nullptr, nullptr, &tv);

        if (retval == -1) {
            break;
        } else if (retval > 0) {
            std::string data = _ipc.receive();
            if (data == "STATUS") {
                KitchenStatus status;
                status.pizzaQueueSize = _threadPool.getQueueSize();
                status.activeCooks = _threadPool.getActiveCooks();
                auto stock = _stock.getCurrentStock();
                for (size_t i = 0; i < static_cast<size_t>(Ingredient::Count); ++i) {
                    status.stock[i] = stock[i];
                }
                _ipc << "STATUS " + Serializer::pack(status);
            } else if (!data.empty()) {
                Pizza pizza = _serializer.unpack(data);
                _threadPool.enqueuePizza(pizza);
                lastActive = std::chrono::steady_clock::now();
            }
        }

        // Check completed pizzas
        auto completed = _threadPool.getCompletedPizzas();
        for (const auto& pizza : completed) {
            _ipc << "DONE " + Serializer::pack(pizza);
            lastActive = std::chrono::steady_clock::now();
        }

        // Check inactivity
        if (_threadPool.getQueueSize() == 0 && _threadPool.getActiveCooks() == 0) {
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(now - lastActive).count() >= 5) {
                break;
            }
        } else {
            lastActive = std::chrono::steady_clock::now();
        }
    }
};

void Kitchen::checkInactivityTimeout() {

};