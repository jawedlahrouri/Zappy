/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.cpp
*/

#include "ServerException.hpp"
#include "Poll/Poll.hpp"
#include "Poll/Sys_Poll.hpp"

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
    m_pollfds.push_back(pfd);
}

void Poll::removeFd(int fd)
{
    for (auto it = m_pollfds.begin(); it != m_pollfds.end(); ++it) {
        if (it->fd == fd) {
            m_pollfds.erase(it);
            break;
        }
    }
}

void Poll::addEvents(int fd, NetworkEvent event)
{
    for (auto &pfd : m_pollfds) {
        if (pfd.fd == fd) {
            pfd.events |= eventToSys(event);
            break;
        }
    }
}

void Poll::removeEvents(int fd, NetworkEvent event)
{
    for (auto &pfd : m_pollfds) {
        if (pfd.fd == fd) {
            pfd.events &= ~eventToSys(event);
            break;
        }
    }
}

int Poll::runPoll(int timeout)
{
    for (auto &pfd : m_pollfds) {
        pfd.revents = 0;
    }
    int ret = SysPoll::poll(m_pollfds, timeout);
    if (ret == -1 && SysPoll::isInterrupted()) {
        return 0;
    }
    return ret;
}

bool Poll::isReadable(size_t index) const
{
    if (index >= m_pollfds.size()) {
        return false;
    }
    return (m_pollfds[index].revents & SysPoll::EventIn) != 0;
}

bool Poll::isWritable(size_t index) const
{
    if (index >= m_pollfds.size()) {
        return false;
    }
    return (m_pollfds[index].revents & SysPoll::EventOut) != 0;
}

bool Poll::isError(size_t index) const
{
    if (index >= m_pollfds.size()) {
        return false;
    }
    return (m_pollfds[index].revents & (SysPoll::EventErr | SysPoll::EventHup)) != 0;
}

int Poll::getFd(size_t index) const
{
    if (index >= m_pollfds.size()) {
        return -1;
    }
    return m_pollfds[index].fd;
}

size_t Poll::getSize() const
{
    return m_pollfds.size();
}
