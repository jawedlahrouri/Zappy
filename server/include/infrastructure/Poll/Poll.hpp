/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.hpp
*/

#ifndef POLL_HPP_
#define POLL_HPP_

#include <vector>
#include "Poll/Sys_Poll.hpp"

class Poll {
    public:
        Poll() = default;
        ~Poll() = default;

        void add_fd(int fd, short events);
        void remove_fd(int fd);
        void add_events(int fd, short events);
        void remove_events(int fd, short events);
        int run_poll(int timeout = -1);

        bool is_readable(size_t index) const;
        bool is_writable(size_t index) const;
        bool is_error(size_t index) const;

        int get_fd(size_t index) const;
        size_t get_size() const;

    private:
        std::vector<SysPoll::PollFd> _pollfds;
};

#endif /* !POLL_HPP_ */
