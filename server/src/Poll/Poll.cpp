/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.cpp
*/

#include "Exception_Server.hpp"
#include "Poll/Poll.hpp"
#include "Poll/Sys_Poll.hpp"

void Poll::add_fd(int fd, short events)
{
    try {
    SysPoll::PollFd pfd{};
    pfd.fd = fd;
    pfd.events = events;
    pfd.revents = 0;
    _pollfds.push_back(pfd);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::add_fd: ") + e.what());
    }
}

void Poll::remove_fd(int fd)
{
    try {
    for (auto it = _pollfds.begin(); it != _pollfds.end(); ++it) {
        if (it->fd == fd) {
            _pollfds.erase(it);
            break;
        }
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::remove_fd: ") + e.what());
    }
}

void Poll::add_events(int fd, short events)
{
    try {
    for (auto &pfd : _pollfds) {
        if (pfd.fd == fd) {
            pfd.events |= events;
            break;
        }
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::add_events: ") + e.what());
    }
}

void Poll::remove_events(int fd, short events)
{
    try {
    for (auto &pfd : _pollfds) {
        if (pfd.fd == fd) {
            pfd.events &= ~events;
            break;
        }
    }
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::remove_events: ") + e.what());
    }
}

int Poll::run_poll(int timeout)
{
    try {
    return SysPoll::poll(_pollfds, timeout);
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::run_poll: ") + e.what());
    }
}

bool Poll::is_readable(size_t index) const
{
    try {
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & SysPoll::EventIn) != 0;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::is_readable: ") + e.what());
    }
}

bool Poll::is_writable(size_t index) const
{
    try {
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & SysPoll::EventOut) != 0;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::is_writable: ") + e.what());
    }
}

bool Poll::is_error(size_t index) const
{
    try {
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & (SysPoll::EventErr | SysPoll::EventHup)) != 0;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::is_error: ") + e.what());
    }
}

int Poll::get_fd(size_t index) const
{
    try {
    if (index >= _pollfds.size()) {
        return -1;
    }
    return _pollfds[index].fd;
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::get_fd: ") + e.what());
    }
}

size_t Poll::get_size() const
{
    try {
    return _pollfds.size();
    } catch (const std::exception &e) {
        throw Exception_Server(std::string("Poll::get_size: ") + e.what());
    }
}
