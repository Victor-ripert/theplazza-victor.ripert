/*
** EPITECH PROJECT, 2026
** Victor.ripert@epitech.eu
** File description:
** Plazza
*/

#include "../../include/errors/PlazzaErrors.hpp"

LowError::LowError(const std::string &str) : _msg(str) {}

const char *LowError::what() const noexcept {
    return _msg.c_str();
}