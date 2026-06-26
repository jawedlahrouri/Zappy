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
    /**
     * @brief Forwards to @c ::poll(), reinterpreting @ref PollFd as @c pollfd.
     * @details The cast is safe because @ref PollFd is laid out identically to
     *          @c struct pollfd (int + two shorts).
     */
    int poll(std::vector<PollFd> &fds, int timeout) {
        return ::poll(reinterpret_cast<struct pollfd*>(fds.data()), fds.size(), timeout);
    }

    /**
     * @brief @c true when @c errno is @c EINTR (signal-interrupted syscall).
     */
    bool isInterrupted() {
        return errno == EINTR;
    }

    /**
     * @brief Returns @c strerror(errno) for the most recent failure.
     */
    std::string getLastError() {
        return std::strerror(errno);
    }
}

#endif
