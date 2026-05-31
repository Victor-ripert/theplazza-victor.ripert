/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Stock
*/

#include "kitchen/Stock.hpp"
#include <unistd.h>

Stock::Stock(int restockTimeMs) : _restockTimeMs(restockTimeMs){
    _ingredients.fill(5);
};

Stock::~Stock() {

};

Stock::Recipe Stock::getRecipeForPizza(PizzaType type) {
    Recipe recipe{};

    switch (type) {
        case PizzaType::Margarita:
            recipe[static_cast<size_t>(Ingredient::Dough)] = 1;
            recipe[static_cast<size_t>(Ingredient::Tomato)] = 1;
            recipe[static_cast<size_t>(Ingredient::Gruyere)] = 1;
            break;

        case PizzaType::Regina:
            recipe[static_cast<size_t>(Ingredient::Dough)] = 1;
            recipe[static_cast<size_t>(Ingredient::Tomato)] = 1;
            recipe[static_cast<size_t>(Ingredient::Gruyere)] = 1;
            recipe[static_cast<size_t>(Ingredient::Ham)] = 1;
            recipe[static_cast<size_t>(Ingredient::Mushrooms)] = 1;
            break;

        case PizzaType::Americana:
            recipe[static_cast<size_t>(Ingredient::Dough)] = 1;
            recipe[static_cast<size_t>(Ingredient::Tomato)] = 1;
            recipe[static_cast<size_t>(Ingredient::Gruyere)] = 1;
            recipe[static_cast<size_t>(Ingredient::Steak)] = 1;
            break;

        case PizzaType::Fantasia:
            recipe[static_cast<size_t>(Ingredient::Dough)] = 1;
            recipe[static_cast<size_t>(Ingredient::Tomato)] = 1;
            recipe[static_cast<size_t>(Ingredient::Eggplant)] = 1;
            recipe[static_cast<size_t>(Ingredient::GoatCheese)] = 1;
            recipe[static_cast<size_t>(Ingredient::ChiefLove)] = 1;
            break;

        default:
            break;
    }
    return recipe;
}

bool Stock::canTakeIngredients(const Pizza& pizza) {
    Recipe recipe = getRecipeForPizza(pizza.type);
    for (size_t i = 0; i < static_cast<size_t>(Ingredient::Count); ++i) {
        if (recipe[i] > _ingredients[i])
            return false;
    }
    return true;
}

void Stock::consumeIngredients(const Pizza& pizza) {
    Recipe recipe = getRecipeForPizza(pizza.type);
    for (size_t i = 0; i < static_cast<size_t>(Ingredient::Count); ++i) {
        _ingredients[i] -= recipe[i];
    }
}

void Stock::waitAndTakeIngredients(const Pizza& pizza) {
    _mutex.lock();
    while (!canTakeIngredients(pizza) && _isRunning) {
        _stockCondition.wait(_mutex);
    }
    if (_isRunning) {
        consumeIngredients(pizza);
    }
    _mutex.unlock();
}

void Stock::restockLoop()
{
    while (_isRunning) {
        usleep(_restockTimeMs * 1000);

        if (!_isRunning)
            break;

        _mutex.lock();
        regenerate();
        _mutex.unlock();

        _stockCondition.notifyAll();
    }
}

void Stock::startRestockLoop() {
    _isRunning = true;
    _restockThread.start([this] {restockLoop();});
}

void Stock::stopRestockLoop() {
    _isRunning = false;
};

std::array<int, static_cast<size_t>(Ingredient::Count)> Stock::getCurrentStock() {
    _mutex.lock();
    std::array<int, static_cast<size_t>(Ingredient::Count)> ingredients = _ingredients;
    _mutex.unlock();
    return ingredients;
};

void Stock::regenerate() {
    for(auto & ingredient : _ingredients) {
        ingredient += 1;
    }
};