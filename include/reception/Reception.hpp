/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** Reception
*/

/*
 * The main controller for the parent process.
 * Manages the user interface, tracks the state of all child processes,
 * and load-balances incoming pizza orders across active kitchens.
 */

#ifndef RECEPTION_HPP_
    #define RECEPTION_HPP_

#include "Parser.hpp"
#include <vector>
#include <string>
#include "../utils/Process.hpp"
#include "../ipc/IPCNetwork.hpp"
#include "../ipc/Serializer.hpp"
#include "../utils/Logger.hpp"

class Reception {
    struct KitchenProxy {
        Process* process;
        IPCNetwork* ipc;
        KitchenStatus lastStatus;
        std::string name;
        bool active;
    };

    public:
        Reception(float multiplier, int cooksPerKitchen, int restockTime);
        ~Reception();

        void runShell();

    private:
        void dispatchPizza(const Pizza& pizza);
        void updateKitchens();
        void spawnKitchen();
        void displayStatus();
        void logPizza(const Pizza& pizza);

        float _multiplier;
        int _cooksPerKitchen;
        int _restockTime;

        Parser _parser;
        Logger _logger;
        std::vector<KitchenProxy> _kitchens;
        size_t _kitchenCounter = 0;
};

#endif
