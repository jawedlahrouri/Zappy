/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.cpp
*/

#include "ServerException.hpp"
#include "Poll.hpp"
#include "Sys_Poll.hpp"
#include <algorithm>

static short eventToSys(NetworkEvent event) {
    if (event == NetworkEvent::Input) {
        return SysPoll::EventIn;
    }
    if (event == NetworkEvent::Output) {
        return SysPoll::EventOut;
    }
    return 0;
}

void Poll::addFd(int fd, NetworkEvent event)
{
    SysPoll::PollFd pfd{};
    pfd.fd = fd;
    pfd.events = eventToSys(event);
    pfd.revents = 0;
    _pollfds.push_back(pfd);
}

void Poll::removeFd(int fd)
{
    _pollfds.erase(
        std::remove_if(_pollfds.begin(), _pollfds.end(),
            [fd](const SysPoll::PollFd& pfd) { return pfd.fd == fd; }),
        _pollfds.end());
}

void Poll::addEvents(int fd, NetworkEvent event)
{
    for (auto &pfd : _pollfds) {
        if (pfd.fd == fd) {
            pfd.events |= eventToSys(event);
            break;
        }
    }
}

void Poll::removeEvents(int fd, NetworkEvent event)
{
    for (auto &pfd : _pollfds) {
        if (pfd.fd == fd) {
            pfd.events &= ~eventToSys(event);
            break;
        }
    }
}

int Poll::runPoll(int timeout)
{
    for (auto &pfd : _pollfds) {
        pfd.revents = 0;
    }
    int ret = SysPoll::poll(_pollfds, timeout);
    if (ret == -1 && SysPoll::isInterrupted()) {
        return 0;
    }
    return ret;
}

bool Poll::isReadable(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & SysPoll::EventIn) != 0;
}

bool Poll::isWritable(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & SysPoll::EventOut) != 0;
}

bool Poll::isError(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & (SysPoll::EventErr | SysPoll::EventHup | SysPoll::EventNval)) != 0;
}

int Poll::getFd(size_t index) const
{
    if (index >= _pollfds.size()) {
        return -1;
    }
    return _pollfds[index].fd;
}

size_t Poll::getSize() const
{
    return _pollfds.size();
}
