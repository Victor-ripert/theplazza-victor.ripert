/*
** EPITECH PROJECT, 2026
** plazza
** File description:
** IPCNetwork
*/

#include "ipc/IPCNetwork.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <vector>

IPCNetwork::IPCNetwork() : _name(""), _fd_in(-1), _fd_out(-1) {}

IPCNetwork::IPCNetwork(const std::string& name) : _name(name), _fd_in(-1), _fd_out(-1) {
    std::string name_in = _name + "_in";
    std::string name_out = _name + "_out";
    
    mkfifo(name_in.c_str(), 0666);
    mkfifo(name_out.c_str(), 0666);
}

IPCNetwork::~IPCNetwork() {
    if (_fd_in != -1) close(_fd_in);
    if (_fd_out != -1) close(_fd_out);
    
    unlink((_name + "_in").c_str());
    unlink((_name + "_out").c_str());
}

void IPCNetwork::send(const std::string& message) {
    if (_fd_out == -1) {
        // Open the "out" pipe for writing. 
        // This blocks until the other side opens it for reading.
        _fd_out = open((_name + "_out").c_str(), O_WRONLY);
    }
    if (_fd_out != -1) {
        uint32_t size = message.size();
        write(_fd_out, &size, sizeof(size));
        write(_fd_out, message.c_str(), size);
    }
}

std::string IPCNetwork::receive() {
    if (_fd_in == -1) {
        // Open the "in" pipe for reading.
        // This blocks until the other side opens it for writing.
        _fd_in = open((_name + "_in").c_str(), O_RDONLY);
    }
    if (_fd_in != -1) {
        uint32_t size = 0;
        ssize_t bytes = read(_fd_in, &size, sizeof(size));
        if (bytes <= 0)
            return "";
        std::vector<char> buffer(size);
        bytes = read(_fd_in, buffer.data(), size);
        if (bytes <= 0)
            return "";
        return std::string(buffer.begin(), buffer.end());
    }
    return "";
}

IPCNetwork& IPCNetwork::operator<<(const std::string& message) {
    send(message);
    return *this;
}

IPCNetwork& IPCNetwork::operator>>(std::string& message) {
    message = receive();
    return *this;
}
