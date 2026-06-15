/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.hpp
*/

#ifndef POLL_HPP_
#define POLL_HPP_

#include "IPoll.hpp"
#include "Sys_Poll.hpp"

class Poll : public IPoll {
    public:
        Poll() = default;
        ~Poll() override = default;

        void addFd(int fd, NetworkEvent event) override;
        void removeFd(int fd) override;
        void addEvents(int fd, NetworkEvent event) override;
        void removeEvents(int fd, NetworkEvent event) override;
        int runPoll(int timeout = -1) override;

        bool isReadable(size_t index) const override;
        bool isWritable(size_t index) const override;
        bool isError(size_t index) const override;

        int getFd(size_t index) const override;
        size_t getSize() const override;

    private:
        std::vector<SysPoll::PollFd> _pollfds;
};

#endif /* !POLL_HPP_ */
