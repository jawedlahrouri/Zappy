/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll.hpp
*/

#ifndef SYS_POLL_HPP_
#define SYS_POLL_HPP_

#include <vector>
#include <string>

namespace SysPoll {
    enum Event : short {
        EventIn = 1,
        EventOut = 4,
        EventErr = 8,
        EventHup = 16,
    };

    struct PollFd {
        int fd;
        short events;
        short revents;
    };

    int poll(std::vector<PollFd> &fds, int timeout);
    bool isInterrupted();
    std::string getLastError();

}

#endif /* !SYS_POLL_HPP_ */
