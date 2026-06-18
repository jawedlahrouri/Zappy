/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll_Linux.cpp — Linux poll backend (POSIX poll)
** Compiled as a shared library: zappy_network_linux.so
*/

#include <poll.h>
#include <cerrno>
#include <cstring>
#include <vector>

namespace {
    static char s_errorBuf[256];
}

extern "C" {

/*
** Flat array interface for poll().
** fds     : array of file descriptors
** events  : array of requested events (parallel to fds)
** revents : array of returned events (parallel to fds, written by poll)
** count   : number of entries in fds / events / revents
** timeout : milliseconds (-1 = block indefinitely)
*/
int net_poll(int *fds, short *events, short *revents, unsigned int count, int timeout)
{
    std::vector<struct pollfd> pfdBuf(count);

    for (unsigned int i = 0; i < count; ++i) {
        pfdBuf[i].fd      = fds[i];
        pfdBuf[i].events  = events[i];
        pfdBuf[i].revents = 0;
    }

    int ret = ::poll(pfdBuf.data(), static_cast<nfds_t>(count), timeout);

    for (unsigned int i = 0; i < count; ++i) {
        revents[i] = pfdBuf[i].revents;
    }
    return ret;
}

int net_isInterrupted()
{
    return (errno == EINTR) ? 1 : 0;
}

const char *net_pollGetLastError()
{
    std::strncpy(s_errorBuf, std::strerror(errno), sizeof(s_errorBuf) - 1);
    s_errorBuf[sizeof(s_errorBuf) - 1] = '\0';
    return s_errorBuf;
}

} /* extern "C" */
