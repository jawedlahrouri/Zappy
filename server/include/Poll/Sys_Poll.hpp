/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll.hpp
*/

#ifndef SYS_POLL_HPP_
#define SYS_POLL_HPP_

#include <vector>

namespace SysPoll {
    enum Event : short {
        EventIn = 1 << 0,
        EventOut = 1 << 1,
        EventErr = 1 << 2,
        EventHup = 1 << 3,
    };

    struct PollFd {
        int fd;
        short events;
        short revents;
    };

    int poll(std::vector<PollFd> &fds, int timeout);
}

#endif /* !SYS_POLL_HPP_ */
