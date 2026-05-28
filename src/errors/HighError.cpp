/*
** EPITECH PROJECT, 2026
** Victor.ripert@epitech.eu
** File description:
** Plazza
*/

#include "../../include/errors/PlazzaErrors.hpp"

HighError::HighError(const std::string &str) : _msg(str) {}

const char *HighError::what() const noexcept {
    return _msg.c_str();
}