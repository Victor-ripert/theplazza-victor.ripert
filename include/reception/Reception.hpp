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

#include <vector>
#include <string>
#include "Parser.hpp"
#include "../common/Pizza.hpp"
#include "../ipc/IPCNetwork.hpp"
#include "../utils/Process.hpp"

class Reception {
    public:
        Reception(float multiplier, int cooksPerKitchen, int restockTime);
        ~Reception();

        void runShell();
        void displayStatus();

    private:
        void dispatchOrder(const std::vector<Pizza>& pizzas);
        void spawnNewKitchen();

        float _multiplier;
        int _cooksPerKitchen;
        int _restockTime;

        Parser _parser;
        std::vector<Process*> _kitchenProcesses;
        std::vector<IPCNetwork*> _kitchenIPCs;
};

#endif
