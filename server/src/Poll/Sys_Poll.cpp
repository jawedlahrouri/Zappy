/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll.cpp
*/

#include "Poll/Sys_Poll.hpp"

namespace SysPoll {
    int poll(struct pollfd *fds, nfds_t nfds, int timeout)
    {
        return ::poll(fds, nfds, timeout);
    }
}
