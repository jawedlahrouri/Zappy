/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll.cpp
*/

#ifndef _WIN32

#include "Sys_Poll.hpp"
#include <poll.h>
#include <cerrno>
#include <cstring>

namespace SysPoll {
    int poll(std::vector<PollFd> &fds, int timeout) {
        return ::poll(reinterpret_cast<struct pollfd*>(fds.data()), fds.size(), timeout);
    }

    bool isInterrupted() {
        return errno == EINTR;
    }

    std::string getLastError() {
        return std::strerror(errno);
    }
}

#endif
