/*
** EPITECH PROJECT, 2026
** Victor.ripert@epitech.eu
** File description:
** Plzza
*/

#include "reception/Reception.hpp"
#include "errors/PlazzaErrors.hpp"
#include <iostream>

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
        } catch (LowError &e) {
            std::cerr << "Invalid Order: " << order << std::endl;
            continue;
        }
        for (int i = 0; i < static_cast<int>(pizzaQueue.size()); i++) {
            std::cout << "Pizza: " << pizzaQueue[i].id << std::endl;
        }
    }
}

void sendPizzaToKitchen(std::vector<Pizza>& pizzas, IPCNetwork *kitchenIpc) {
    if (pizzas.size() <= 0)
        return;
        std::string serializedPizza;
    Pizza pizza = pizzas.back();
    pizzas.pop_back();
    serializedPizza = Serializer::pack(pizza);
    (*kitchenIpc) << serializedPizza;
}

void Reception::dispatchOrder(std::vector<Pizza>& pizzas) {
    int nbPizzaInQueue = 0;
    for (IPCNetwork *kitchenIpc : _kitchenIPCs) {
        std::string pizzaQueue;
        (*kitchenIpc) >> pizzaQueue;
        nbPizzaInQueue = std::stoi(pizzaQueue);
        if (nbPizzaInQueue >= 2 * _cooksPerKitchen)
            continue;
        for (int i = 0; i < _cooksPerKitchen - nbPizzaInQueue; i++) {
            sendPizzaToKitchen(pizzas, kitchenIpc);
        }
    }
    if (pizzas.size() > 0) {
        for (int i = 0; i < pizzas.size() / (_cooksPerKitchen * 2); i++) {
            spawnNewKitchen();
        }
        dispatchOrder(pizzas);
    }
}

void Reception::spawnNewKitchen() {
    Process *new_process = new Process();
    pid_t pid = new_process->spawn();

    if (pid == 0) {
        Kitchen kitchen(_cooksPerKitchen, _restockTime, _multiplier);
        kitchen.run();
        exit(0);
    } else if (pid > 0) {
        _kitchenProcesses.push_back(new_process);
        IPCNetwork *ipc = new IPCNetwork(std::to_string(pid));
        _kitchenIPCs.push_back(ipc);
    } else {
        throw HighError("Failed to spawn kitchen");
    }
}