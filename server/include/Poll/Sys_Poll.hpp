/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll.hpp
*/

#ifndef SYS_POLL_HPP_
#define SYS_POLL_HPP_

#include <poll.h>

namespace SysPoll {
    int poll(struct pollfd *fds, nfds_t nfds, int timeout);
}

#endif /* !SYS_POLL_HPP_ */
