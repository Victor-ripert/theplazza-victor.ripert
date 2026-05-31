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
        IPCNetwork(const std::string& name);
        IPCNetwork(const std::string& name, bool reversed);
        ~IPCNetwork();

        void send(const std::string& message);
        std::string receive();

        IPCNetwork& operator<<(const std::string& message);
        IPCNetwork& operator>>(std::string& message);

    private:
        std::string _name;
        int _fd_in;
        int _fd_out;
        bool _reversed;
};

#endif
