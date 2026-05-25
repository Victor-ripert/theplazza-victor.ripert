/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Pizza
*/

/*
 * Defines the core data structures and enumerations (PizzaType, PizzaSize) used to represent a pizza
 * across the entire system. This acts as the shared language between the Reception and the Kitchens.
 */

#ifndef PIZZA_HPP_
    #define PIZZA_HPP_

#include <cstddef>

enum class PizzaType {
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8
};

enum class PizzaSize {
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16
};

struct Pizza {
    PizzaType type;
    PizzaSize size;
    size_t id;
};

#endif
