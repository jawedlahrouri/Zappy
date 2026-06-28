/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll.hpp
*/

/**
 * @file Sys_Poll.hpp
 * @brief Thin POSIX wrapper around the @c poll() syscall (network backend side).
 */

#ifndef SYS_POLL_HPP_
#define SYS_POLL_HPP_

#include <vector>
#include <string>

/**
 * @namespace SysPoll
 * @brief Platform-specific @c poll() primitives loaded as a network backend.
 *
 * This layer lives in the dynamically loaded network library and is reached by
 * @ref Poll only through the @c NetworkBackend::Symbols function table, never by
 * direct call. It exists so that all @c <poll.h> usage is confined to one
 * compilation unit guarded by @c \#ifndef _WIN32.
 */
namespace SysPoll {
    /**
     * @enum Event
     * @brief POSIX @c poll() event bits mirrored for the backend ABI.
     */
    enum Event : short {
        EventIn = 1,   /**< POLLIN. */
        EventOut = 4,  /**< POLLOUT. */
        EventErr = 8,  /**< POLLERR. */
        EventHup = 16, /**< POLLHUP. */
        EventNval = 32,/**< POLLNVAL. */
    };

    /**
     * @struct PollFd
     * @brief ABI-compatible mirror of @c struct pollfd for the backend boundary.
     */
    struct PollFd {
        int fd;        /**< Watched file descriptor. */
        short events;  /**< Requested event mask. */
        short revents; /**< Returned event mask. */
    };

    /**
     * @brief Calls the system @c poll() over @p fds.
     * @param fds Watch list, reinterpreted as a @c struct pollfd array.
     * @param timeout Milliseconds to wait (@c -1 blocks indefinitely).
     * @return Number of ready fds, @c 0 on timeout, @c -1 on error.
     */
    int poll(std::vector<PollFd> &fds, int timeout);

    /**
     * @brief Tells whether the last syscall failed because of @c EINTR.
     * @return @c true if interrupted by a signal (caller should retry).
     */
    bool isInterrupted();

    /**
     * @brief Returns a human-readable description of the last @c errno.
     * @return The @c strerror(errno) message.
     */
    std::string getLastError();

}

#endif /* !SYS_POLL_HPP_ */
