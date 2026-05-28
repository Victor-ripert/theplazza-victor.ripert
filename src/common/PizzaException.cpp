/*
** EPITECH PROJECT, 2026
** Victor.ripert@epitech.eu
** File description:
** Plazza
*/

#include "../../include/common/PizzaException.hpp"

PizzaException::PizzaException(const std::string &str) : _msg(str) {}

const char *PizzaException::what() const noexcept {
    return _msg.c_str();
}