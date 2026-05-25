/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** kitchen
*/

/*
 * The main controller for a child process.
 * Listens for incoming orders via the IPC network, manages the local Stock, oversees the ThreadPool,
 * and handles the inactivity timeout shutdown sequence.
 */

#ifndef KITCHEN_HPP_
    #define KITCHEN_HPP_

#include "../ipc/IPCNetwork.hpp"
#include "ThreadPool.hpp"
#include "Stock.hpp"

class Kitchen {
    public:
        Kitchen(int cooksCount, int restockTime, float multiplier);
        ~Kitchen();

        void run();

    private:
        void checkInactivityTimeout();

        int _cooksCount;
        int _restockTime;
        float _multiplier;

        IPCNetwork _ipc;
        Stock _stock;
        ThreadPool _threadPool;
};

#endif
