#include "Stock.hpp"

Stock::Stock(int restockTimeMs) : _restockTimeMs(restockTimeMs) {
    _ingredients.fill(5);
};

Stock::~Stock() {

};

Stock::Recipe Stock::getRecipeForPizza(PizzaType type) {
    Recipe recipe;

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

bool Stock::takeIngredients(const Pizza& pizza) {
    Recipe recipe = getRecipeForPizza(pizza.type);
    _mutex.lock();
    for (int i = 0; i < static_cast<size_t>(Ingredient::Count); i++) {
        if (recipe[i] > _ingredients[i]) {
            _mutex.unlock();
            return false;
        }
    }

    for (int i = 0; i < static_cast<size_t>(Ingredient::Count); i++) {
        _ingredients[i] -= recipe[i];
    }
    _mutex.unlock();
    return true;
};

void Stock::startRestockLoop() {
    while (_isRunning)
    {
        //sleep for time
        if(!_isRunning) {
            break;
        }
        _mutex.lock();
        regenerate();
        _mutex.unlock();
    }
};

void Stock::stopRestockLoop() {
    _isRunning = false;
};

std::array<int, static_cast<size_t>(Ingredient::Count)> Stock::getCurrentStock() {
    return _ingredients;
};

void Stock::regenerate() {
    for(auto & ingredient : _ingredients) {
        ingredient += 1;
    }
};