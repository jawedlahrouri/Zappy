/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll.cpp
*/

#include "Poll/Sys_Poll.hpp"
#include <poll.h>

namespace SysPoll {
    int poll(std::vector<PollFd> &fds, int timeout)
    {
        std::vector<struct pollfd> sys_fds;
        sys_fds.reserve(fds.size());

        for (const auto &fd : fds) {
            struct pollfd pfd = {fd.fd, 0, 0};
            if (fd.events & EventIn) pfd.events |= POLLIN;
            if (fd.events & EventOut) pfd.events |= POLLOUT;
            sys_fds.push_back(pfd);
        }

        int ret = ::poll(sys_fds.data(), sys_fds.size(), timeout);

        for (size_t i = 0; i < fds.size(); ++i) {
            fds[i].revents = 0;
        }

        if (ret > 0) {
            for (size_t i = 0; i < fds.size(); ++i) {
                if (sys_fds[i].revents & POLLIN) fds[i].revents |= EventIn;
                if (sys_fds[i].revents & POLLOUT) fds[i].revents |= EventOut;
                if (sys_fds[i].revents & POLLERR) fds[i].revents |= EventErr;
                if (sys_fds[i].revents & POLLHUP) fds[i].revents |= EventHup;
            }
        }

        return ret;
    }
}
