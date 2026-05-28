/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Stock
*/

/*
 * Manages a thread-safe inventory of ingredients for a kitchen.
 * Includes the background loop logic to automatically regenerate ingredients at the interval specified at startup.
 */

#ifndef STOCK_HPP_
    #define STOCK_HPP_

#include <map>
#include <string>
#include "../utils/Mutex.hpp"
#include "../common/Pizza.hpp"
#include "../utils/Thread.hpp"
#include "../utils/ConditionVariable.hpp"
#include <array>

class Stock {
    public:
        Stock(int restockTimeMs);
        ~Stock();

        using Recipe = std::array<int, static_cast<size_t>(Ingredient::Count)>;
        void waitAndTakeIngredients(const Pizza& pizza);
        void startRestockLoop();
        void stopRestockLoop();
        std::array<int, static_cast<size_t>(Ingredient::Count)> getCurrentStock();
        Recipe getRecipeForPizza(PizzaType type);

    private:
        void regenerate();
        void consumeIngredients(const Pizza& pizza);
        bool canTakeIngredients(const Pizza& pizza);
        void restockLoop();

        int _restockTimeMs;
        bool _isRunning;
        Thread _restockThread;
        ConditionVariable _stockCondition;
        std::array<int, static_cast<size_t>(Ingredient::Count)> _ingredients;
        Mutex _mutex;
};

#endif
