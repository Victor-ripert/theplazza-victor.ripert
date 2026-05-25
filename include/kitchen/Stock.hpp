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

class Stock {
    public:
        Stock(int restockTimeMs);
        ~Stock();

        bool takeIngredients(const std::map<std::string, int>& required);
        void startRestockLoop();
        void stopRestockLoop();
        std::map<std::string, int> getCurrentStock();

    private:
        void regenerate();

        int _restockTimeMs;
        bool _isRunning;
        std::map<std::string, int> _ingredients;
        Mutex _mutex;
};

#endif
