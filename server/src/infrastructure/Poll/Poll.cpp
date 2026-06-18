/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.cpp
*/

#include "Poll.hpp"
#include "ServerException.hpp"
#include <algorithm>

namespace {
    enum PollFlag : short {
        PollIn        = 1,
        PollOut       = 4,
        PollErr       = 8,
        PollHup       = 16,
        PollNval      = 32,
        PollErrorMask = PollErr | PollHup | PollNval
    };
}

static short eventToShort(NetworkEvent event)
{
    if (event == NetworkEvent::Input) {
        return PollIn;
    }
    if (event == NetworkEvent::Output) {
        return PollOut;
    }
    return 0;
}

Poll::Poll(const NetworkBackend::Symbols &symbols)
    : _symbols(symbols)
{
}

void Poll::addFd(int fd, NetworkEvent event)
{
    PollEntry entry{};
    entry.fd      = fd;
    entry.events  = eventToShort(event);
    entry.revents = 0;
    _pollfds.push_back(entry);
}

void Poll::removeFd(int fd)
{
    _pollfds.erase(
        std::remove_if(_pollfds.begin(), _pollfds.end(),
            [fd](const PollEntry &e) { return e.fd == fd; }),
        _pollfds.end());
}

void Poll::addEvents(int fd, NetworkEvent event)
{
    for (auto &entry : _pollfds) {
        if (entry.fd == fd) {
            entry.events |= eventToShort(event);
            break;
        }
    }
}

void Poll::removeEvents(int fd, NetworkEvent event)
{
    for (auto &entry : _pollfds) {
        if (entry.fd == fd) {
            entry.events &= static_cast<short>(~eventToShort(event));
            break;
        }
    }
}

int Poll::runPoll(int timeout)
{
    const unsigned int count = static_cast<unsigned int>(_pollfds.size());
    std::vector<int>   fds(count);
    std::vector<short> events(count);
    std::vector<short> revents(count, 0);

    for (unsigned int i = 0; i < count; ++i) {
        fds[i]    = _pollfds[i].fd;
        events[i] = _pollfds[i].events;
    }

    int ret = _symbols.poll(fds.data(), events.data(), revents.data(), count, timeout);

    if (ret == -1) {
        if (_symbols.isInterrupted()) {
            return 0;
        }
        throw ServerException(std::string("Poll failed: ") + _symbols.pollGetLastError());
    }

    for (unsigned int i = 0; i < count; ++i) {
        _pollfds[i].revents = revents[i];
    }
    return ret;
}

bool Poll::isReadable(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & PollIn) != 0;
}

bool Poll::isWritable(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & PollOut) != 0;
}

bool Poll::isError(size_t index) const
{
    if (index >= _pollfds.size()) {
        return false;
    }
    return (_pollfds[index].revents & PollErrorMask) != 0;
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
