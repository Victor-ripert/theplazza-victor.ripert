/*
** EPITECH PROJECT, 2026
** Victor.ripert@epitech.eu
** File description:
** Plzza
*/

#include "reception/Reception.hpp"
#include "errors/PlazzaErrors.hpp"
#include "kitchen/Kitchen.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sys/select.h>
#include <unistd.h>

Reception::Reception(float multiplier, int cooksPerKitchen, int restockTime) : _multiplier(multiplier), _cooksPerKitchen(cooksPerKitchen), _restockTime(restockTime), _logger("plazza.log") {
    _parser = Parser();
}

Reception::~Reception() {
    for (auto& k : _kitchens) {
        if (k.active) {
            k.process->terminate();
        }
        delete k.process;
        delete k.ipc;
    }
}

void Reception::logPizza(const Pizza& pizza) {
    std::string type;
    switch (pizza.type) {
        case PizzaType::Margarita: type = "margarita"; break;
        case PizzaType::Regina: type = "regina"; break;
        case PizzaType::Americana: type = "americana"; break;
        case PizzaType::Fantasia: type = "fantasia"; break;
    }
    std::string size;
    switch (pizza.size) {
        case PizzaSize::S: size = "S"; break;
        case PizzaSize::M: size = "M"; break;
        case PizzaSize::L: size = "L"; break;
        case PizzaSize::XL: size = "XL"; break;
        case PizzaSize::XXL: size = "XXL"; break;
    }
    std::string msg = "Pizza ready: " + type + " " + size + " (id: " + std::to_string(pizza.id) + ")";
    std::cout << msg << std::endl;
    _logger.log(msg);
}

void Reception::updateKitchens() {
    for (auto it = _kitchens.begin(); it != _kitchens.end(); ) {
        if (!it->process->isAlive()) {
            it->active = false;
            delete it->process;
            delete it->ipc;
            it = _kitchens.erase(it);
            continue;
        }

        int fd = it->ipc->getFdIn();
        if (fd == -1) {
            ++it;
            continue;
        }
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        struct timeval tv = {0, 0};

        if (select(fd + 1, &rfds, nullptr, nullptr, &tv) > 0) {
            std::string data = it->ipc->receive();
            if (data.substr(0, 5) == "DONE ") {
                Pizza p = Serializer::unpack(data.substr(5));
                logPizza(p);
            } else if (data.substr(0, 7) == "STATUS ") {
                it->lastStatus = Serializer::unpackStatus(data.substr(7));
            }
        }
        ++it;
    }
}

void Reception::spawnKitchen() {
    std::string name = "k" + std::to_string(_kitchenCounter++);
    KitchenProxy kp;
    kp.process = new Process();
    kp.ipc = new IPCNetwork(name, false);
    kp.name = name;
    kp.active = true;
    kp.lastStatus = {0, 0, {5, 5, 5, 5, 5, 5, 5, 5, 5}};

    pid_t pid = kp.process->spawn();
    if (pid == 0) {
        Kitchen kitchen(_cooksPerKitchen, _restockTime, _multiplier, name);
        kitchen.run();
        exit(0);
    } else {
        _kitchens.push_back(kp);
    }
}

void Reception::dispatchPizza(const Pizza& pizza) {
    for (auto& k : _kitchens) {
        *(k.ipc) << "STATUS";
    }
    updateKitchens();

    for (auto& k : _kitchens) {
        if (k.lastStatus.pizzaQueueSize < 2 * _cooksPerKitchen) {
            *(k.ipc) << Serializer::pack(pizza);
            k.lastStatus.pizzaQueueSize++;
            return;
        }
    }
    spawnKitchen();
    *(_kitchens.back().ipc) << Serializer::pack(pizza);
    _kitchens.back().lastStatus.pizzaQueueSize++;
}

void Reception::displayStatus() {
    for (auto& k : _kitchens) {
        *(k.ipc) << "STATUS";
    }
    updateKitchens();
    // Wait a bit more for status?
    usleep(10000);
    updateKitchens();

    for (auto& k : _kitchens) {
        std::cout << "Kitchen " << k.name << ":" << std::endl;
        std::cout << "  Queue: " << k.lastStatus.pizzaQueueSize << std::endl;
        std::cout << "  Active Cooks: " << k.lastStatus.activeCooks << std::endl;
        std::cout << "  Stock: ";
        for (int i = 0; i < static_cast<int>(Ingredient::Count); ++i) {
            std::cout << k.lastStatus.stock[i] << " ";
        }
        std::cout << std::endl;
    }
}

void Reception::runShell() {
    std::string line;
    while (true) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        struct timeval tv = {0, 100000}; // 100ms

        int retval = select(1, &rfds, nullptr, nullptr, &tv);
        updateKitchens();

        if (retval > 0) {
            if (!std::getline(std::cin, line))
                break;
            if (line == "exit")
                break;
            if (line == "status") {
                displayStatus();
                continue;
            }
            try {
                auto pizzas = _parser.parseCommand(line);
                for (const auto& p : pizzas) {
                    dispatchPizza(p);
                }
            } catch (LowError &e) {
                std::cerr << e.what() << std::endl;
            }
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
    std::cout << "Before loop kitchepIpcs" << std::endl;
    for (auto& kitchenIpc : _kitchenIPCs) {
        std::string statusStr;
        (*kitchenIpc) >> statusStr;
        std::cout << statusStr << std::endl;
        status = Serializer::unpackStatus(statusStr);
        if (status.pizzaQueueSize >= 2 * _cooksPerKitchen)
            continue;
        for (int i = 0; i < _cooksPerKitchen - status.pizzaQueueSize; i++) {
            sendPizzaToKitchen(pizzas, *kitchenIpc);
        }
    }
    if (pizzas.size() > 0) {
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
        Kitchen kitchen(_cooksPerKitchen, _restockTime, _multiplier, pid);
        kitchen.run();
        exit(0);
    } else if (pid > 0) {
        _kitchenProcesses.push_back(std::move(new_process));
        _kitchenIPCs.push_back(std::make_unique<IPCNetwork>(std::to_string(pid)));
    } else {
        throw HighError("Failed to spawn kitchen");
    }
}