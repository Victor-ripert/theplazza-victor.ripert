/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Serializer
*/

#include "ipc/Serializer.hpp"
#include <sstream>

std::string Serializer::pack(const Pizza& pizza) {
    std::stringstream ss;
    ss << static_cast<int>(pizza.type) << " "
       << static_cast<int>(pizza.size) << " "
       << pizza.id;
    return ss.str();
}

Pizza Serializer::unpack(const std::string& data) {
    std::stringstream ss(data);
    int type, size;
    size_t id;

    ss >> type >> size >> id;
    return Pizza(static_cast<PizzaType>(type), static_cast<PizzaSize>(size), id);
}
