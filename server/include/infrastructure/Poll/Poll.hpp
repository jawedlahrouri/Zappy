/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Poll.hpp
*/

/**
 * @file Poll.hpp
 * @brief Concrete @ref IPoll backed by the system @c poll() call.
 */

#ifndef POLL_HPP_
#define POLL_HPP_

#include "IPoll.hpp"
#include "INetworkLoader.hpp"
#include <vector>

/**
 * @class Poll
 * @brief @ref IPoll implementation that drives the platform @c poll() syscall.
 *
 * @par Design Pattern: Adapter (over a dynamically loaded backend)
 * @c Poll adapts the raw @c poll() syscall to the @ref IPoll contract. It does
 * not call @c poll() directly: the function is reached through a
 * @c NetworkBackend::Symbols table, whose entries are resolved at runtime from a
 * shared library by the @ref NetworkLoader (e.g. @c libzappy_network_linux.so).
 * This keeps the platform-specific code (@c SysPoll) on the far side of a stable
 * function-pointer boundary.
 *
 * The watch list is held as a contiguous @c std::vector of @ref PollEntry, so a
 * scratch @c pollfd array can be handed to the syscall and the returned
 * @c revents copied straight back by index.
 */
class Poll : public IPoll {
    public:
        /**
         * @brief Builds the multiplexer over a resolved network backend.
         * @param symbols Function-pointer table providing @c poll() and its
         *        error helpers, supplied by @ref NetworkLoader.
         */
        explicit Poll(const NetworkBackend::Symbols &symbols);
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
        /**
         * @struct PollEntry
         * @brief One watched descriptor, mirroring a @c struct pollfd.
         */
        struct PollEntry {
            int fd;        /**< Watched file descriptor. */
            short events;  /**< Requested event mask (POLLIN/POLLOUT). */
            short revents; /**< Event mask returned by the last @ref runPoll. */
        };

        const NetworkBackend::Symbols &_symbols; /**< Resolved network backend. */
        std::vector<PollEntry> _pollfds;         /**< Watch list, index-aligned. */
};

#endif /* !POLL_HPP_ */
