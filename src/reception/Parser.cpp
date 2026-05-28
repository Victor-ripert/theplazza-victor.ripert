#include "../../include/reception/Parser.hpp"
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>

std::string lower(std::string s) {
    for (char &c : s) {
        if (c >= 'A' && c <= 'Z') {
            c += 32;
        }
    }
    return s;
}

std::string trim(const std::string &s) {
    auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";
    auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

PizzaType parseType(const std::string &str) {
    static const std::unordered_map<std::string, PizzaType> types = {
        {"regina", PizzaType::Regina},
        {"margarita", PizzaType::Margarita},
        {"americana", PizzaType::Americana},
        {"fantasia", PizzaType::Fantasia}
    };
    auto strLowered = lower(str);
    auto it = types.find(strLowered);
    if (it == types.end())
        throw PizzaException("Unknown pizza type: " + str);
    return it->second;
}

PizzaSize parseSize(const std::string &str) {
    static const std::unordered_map<std::string, PizzaSize> sizes = {
        {"s", PizzaSize::S},
        {"m", PizzaSize::M},
        {"l", PizzaSize::L},
        {"xl", PizzaSize::XL},
        {"xxl", PizzaSize::XXL}
    };
    auto strLowered = lower(str);
    auto it = sizes.find(strLowered);
    if (it == sizes.end())
        throw PizzaException("Unknown pizza size: " + str);
    return it->second;
}

int parseQuantity(const std::string &raw) {
    if (raw.size() < 2 || (raw[0] != 'x' && raw[0] != 'X'))
        throw PizzaException("Invalid quantity: " + raw);

    int qty = 0;
    try {
        qty = std::stoi(raw.substr(1));
    } catch (...) {
        throw PizzaException("Invalid quantity number: " + raw);
    }
    if (qty <= 0)
        throw PizzaException("Quantity must be > 0");
    return qty;
}

std::vector<Pizza> Parser::parseCommand(const std::string &input) {
    std::vector<Pizza> pizzas;
    std::stringstream ss(input);
    std::string pizza;
    size_t nextId = 1;

    while (std::getline(ss, pizza, ';')) {
        pizza = trim(pizza);
        if (pizza.empty())
            continue;
        std::istringstream subss(pizza);
        std::string typeStr, sizeStr, qtyStr;
        if (!(subss >> typeStr >> sizeStr >> qtyStr))
            throw PizzaException("Invalid pizza format: " + pizza);
        const PizzaType type = parseType(typeStr);
        const PizzaSize size = parseSize(sizeStr);
        const int qty = parseQuantity(qtyStr);
        for (int i = 0; i < qty; i++) {
            pizzas.push_back(Pizza{type, size, nextId++});
        }
    }
    if (pizzas.empty())
        throw PizzaException("No valid pizza in command");
    return pizzas;
}