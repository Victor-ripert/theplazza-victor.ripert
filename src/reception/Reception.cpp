/*
** EPITECH PROJECT, 2026
** Victor.ripert@epitech.eu
** File description:
** Plzza
*/

#include "../../include/reception/Reception.hpp"
#include "../../include/errors/PlazzaErrors.hpp"
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
        try {
            pizzaQueue = _parser.parseCommand(order);
        } catch (LowError &e) {
            std::cerr << "Invalid Order: " << order << std::endl;
            continue;
        }
        for (int i = 0; i < pizzaQueue.size(); i++) {
            std::cout << "Pizza: " << pizzaQueue[i].id << std::endl;
        }
    }
}