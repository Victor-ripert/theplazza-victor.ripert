/*
** EPITECH PROJECT, 2026
** Victor.ripert@epitech.eu
** File description:
** Plzza
*/

#include "reception/Reception.hpp"
#include "errors/PlazzaErrors.hpp"
#include <iostream>
#include <unistd.h>

Reception::Reception(float multiplier, int cooksPerKitchen, int restockTime) : _multiplier(multiplier), _cooksPerKitchen(cooksPerKitchen), _restockTime(restockTime) {
    _parser = Parser();
}

Reception::~Reception() {}

void Reception::runShell() {
    std::string order;
    std::vector<Pizza> pizzaQueue;
    while (true) {
        if (!std::getline(std::cin, order)) {
            throw HighError("Invalid order");
        }
        if (order == "exit")
            return;
        try {
            pizzaQueue = _parser.parseCommand(order);
            checkCloseKitchen();
            dispatchOrder(pizzaQueue);
        } catch (LowError &e) {
            std::cerr << "Invalid Order: " << order << std::endl;
            continue;
        }
        for (int i = 0; i < static_cast<int>(pizzaQueue.size()); i++) {
            std::cout << "Pizza: " << pizzaQueue[i].id << std::endl;
        }
    }
}

void Reception::checkCloseKitchen() {
    for (size_t i = 0; i < _kitchenProcesses.size(); ) {
        if (!_kitchenProcesses[i]->isAlive()) {
            _kitchenProcesses.erase(_kitchenProcesses.begin() + i);
            _kitchenIPCs.erase(_kitchenIPCs.begin() + i);
        } else {
            ++i;
        }
    }
}

void sendPizzaToKitchen(std::vector<Pizza>& pizzas, IPCNetwork& kitchenIpc) {
    if (pizzas.size() <= 0)
        return;
    std::string serializedPizza;
    Pizza pizza = pizzas.back();
    pizzas.pop_back();
    serializedPizza = Serializer::pack(pizza);
    kitchenIpc << serializedPizza;
}

void Reception::dispatchOrder(std::vector<Pizza>& pizzas) {
    KitchenStatus status;
    for (size_t i = 0; i < _kitchenIPCs.size() && !pizzas.empty(); i++) {
        std::string statusStr;
        *_kitchenIPCs[i] >> statusStr;
        std::cout << "After ipc >> status" << std::endl;
        KitchenStatus status = Serializer::unpackStatus(statusStr);
        if (status.pizzaQueueSize >= 2 * _cooksPerKitchen)
            continue;
        while (status.pizzaQueueSize < 2 * _cooksPerKitchen && !pizzas.empty()) {
            sendPizzaToKitchen(pizzas, *_kitchenIPCs[i]);
            status.pizzaQueueSize++;
        }
    }
    if (!pizzas.empty()) {
        for (size_t i = 0; i < pizzas.size() / (_cooksPerKitchen * 2); i++) {
            spawnNewKitchen();
        }
        dispatchOrder(pizzas);
    }
}

void Reception::spawnNewKitchen() {
    auto new_process = std::make_unique<Process>();
    pid_t pid = new_process->spawn();

    if (pid == 0) {
        Kitchen kitchen(_cooksPerKitchen, _restockTime, _multiplier, getpid());
        kitchen.run();
        exit(0);
    } else if (pid > 0) {
        _kitchenProcesses.push_back(std::move(new_process));
        _kitchenIPCs.push_back(std::make_unique<IPCNetwork>(std::to_string(pid)));
    } else {
        throw HighError("Failed to spawn kitchen");
    }
}
