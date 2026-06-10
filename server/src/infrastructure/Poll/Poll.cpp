/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.cpp
*/

#include "ServerException.hpp"
#include "Poll/Poll.hpp"
#include "Poll/Sys_Poll.hpp"

void Poll::add_fd(int fd, short events)
{
    SysPoll::PollFd pfd{};
    pfd.fd = fd;
    pfd.events = events;
    pfd.revents = 0;
    _pollfds.push_back(pfd);
}

void Poll::remove_fd(int fd)
{
    for (auto it = _pollfds.begin(); it != _pollfds.end(); ++it) {
        if (it->fd == fd) {
            _pollfds.erase(it);
            break;
        }
    }
}

void Poll::add_events(int fd, short events)
{
    for (auto &pfd : _pollfds) {
        if (pfd.fd == fd) {
            pfd.events |= events;
            break;
        }
    }
}

void Poll::remove_events(int fd, short events)
{
    for (auto &pfd : _pollfds) {
        if (pfd.fd == fd) {
            pfd.events &= ~events;
            break;
        }
    }
}

int Poll::run_poll(int timeout)
{
    return SysPoll::poll(_pollfds, timeout);
}

bool Poll::is_readable(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & SysPoll::EventIn) != 0;
}

bool Poll::is_writable(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & SysPoll::EventOut) != 0;
}

bool Poll::is_error(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & (SysPoll::EventErr | SysPoll::EventHup)) != 0;
}

int Poll::get_fd(size_t index) const
{
    if (index >= _pollfds.size()) {
        return -1;
    }
    return _pollfds[index].fd;
}

size_t Poll::get_size() const
{
    return _pollfds.size();
}
