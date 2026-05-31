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
#include <cstdint>

IPCNetwork::IPCNetwork() : _name(""), _isChild(false), _fd_in(-1), _fd_out(-1) {}

IPCNetwork::IPCNetwork(const std::string& name, bool isChild) : _name(name), _isChild(isChild), _fd_in(-1), _fd_out(-1) {
    std::string name_req = "/tmp/" + _name + "_req";
    std::string name_res = "/tmp/" + _name + "_res";
    
    mkfifo(name_req.c_str(), 0666);
    mkfifo(name_res.c_str(), 0666);
}

IPCNetwork::~IPCNetwork() {
    if (_fd_in != -1) close(_fd_in);
    if (_fd_out != -1) close(_fd_out);
    
    if (!_isChild) {
        unlink(("/tmp/" + _name + "_req").c_str());
        unlink(("/tmp/" + _name + "_res").c_str());
    }
}

void IPCNetwork::send(const std::string& message) {
    if (_fd_out == -1) {
        std::string fifo_out = _isChild ? "/tmp/" + _name + "_res" : "/tmp/" + _name + "_req";
        _fd_out = open(fifo_out.c_str(), O_WRONLY);
    }
    if (_fd_out != -1) {
        uint32_t size = message.size();
        write(_fd_out, &size, sizeof(size));
        write(_fd_out, message.c_str(), size);
    }
}

std::string IPCNetwork::receive() {
    int fd = getFdIn();
    if (fd != -1) {
        uint32_t size = 0;
        ssize_t bytes = read(fd, &size, sizeof(size));
        if (bytes <= 0)
            return "";
        std::vector<char> buffer(size);
        bytes = read(fd, buffer.data(), size);
        if (bytes <= 0)
            return "";
        return std::string(buffer.begin(), buffer.end());
    }
    return "";
}

IPCNetwork& IPCNetwork::operator>>(std::string& message) {
    message = receive();
    return *this;
}

IPCNetwork& IPCNetwork::operator<<(const std::string& message) {
    send(message);
    return *this;
}

int IPCNetwork::getFdIn() {
    if (_fd_in == -1) {
        std::string fifo_in = _isChild ? "/tmp/" + _name + "_req" : "/tmp/" + _name + "_res";
        _fd_in = open(fifo_in.c_str(), O_RDONLY | O_NONBLOCK);
    }
    return _fd_in;
}
