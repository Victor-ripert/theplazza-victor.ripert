/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** main
*/

#include <iostream>
#include "../include/reception/Reception.hpp"
#include "../include/errors/PlazzaErrors.hpp"


int main (int argc, char **argv)
{
    float multiplier = 0;
    int cooksPerKitchen = 0, restockTime = 0;
    try {
        if (argc != 4) {
            throw HighError("Not enough / too many arguments");
        }
        try {
            multiplier = std::stof(argv[1]);
            cooksPerKitchen = std::stoi(argv[2]);
            restockTime = std::stoi(argv[3]);
        } catch (...) {
            throw HighError("Ivalid argument");
        }
        if (multiplier <= 0 || cooksPerKitchen <= 0 || restockTime <= 0) {
        throw HighError("Invalid Argument");
        }
        Reception reception(multiplier, cooksPerKitchen, restockTime);
        reception.runShell();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return 84;
    }
    return 0;
}
