/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IPoll.hpp
*/

#ifndef IPOLL_HPP_
#define IPOLL_HPP_

#include <cstddef>

enum class NetworkEvent {
    Input,
    Output
};

class IPoll {
    public:
        virtual ~IPoll() = default;

        virtual void addFd(int fd, NetworkEvent event) = 0;
        virtual void removeFd(int fd) = 0;
        virtual void addEvents(int fd, NetworkEvent event) = 0;
        virtual void removeEvents(int fd, NetworkEvent event) = 0;
        virtual int runPoll(int timeout = -1) = 0;

        virtual bool isReadable(size_t index) const = 0;
        virtual bool isWritable(size_t index) const = 0;
        virtual bool isError(size_t index) const = 0;

        virtual int getFd(size_t index) const = 0;
        virtual size_t getSize() const = 0;
};

#endif /* !IPOLL_HPP_ */
