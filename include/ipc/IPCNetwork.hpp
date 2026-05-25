/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** IPCNetwork
*/

/*
 * Abstracts the underlying Inter-Process Communication mechanism (e.g., message queues or pipes).
 * Provides a clean interface for the Reception and Kitchens to send and receive serialized data.
 */

#ifndef IPCNETWORK_HPP_
    #define IPCNETWORK_HPP_

#include <string>

class IPCNetwork {
    public:
        IPCNetwork();
        ~IPCNetwork();

        void send(const std::string& message);
        std::string receive();

        IPCNetwork& operator<<(const std::string& message);
        IPCNetwork& operator>>(std::string& message);
};

#endif
