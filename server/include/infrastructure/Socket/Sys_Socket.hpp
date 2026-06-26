/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Socket.hpp
*/

/**
 * @file Sys_Socket.hpp
 * @brief Thin POSIX wrappers around the BSD socket calls (network backend side).
 */

#ifndef SYS_SOCKET_HPP_
#define SYS_SOCKET_HPP_

#include <string>
#include <cstddef>
#include <sys/types.h>

/**
 * @namespace SysSocket
 * @brief Platform socket primitives loaded as part of the network backend.
 *
 * Confines all @c <sys/socket.h> usage to one place; @ref Socket reaches these
 * only through the @ref NetworkBackend::Symbols table, never directly.
 */
namespace SysSocket {
    /** @brief Creates a TCP/IPv4 socket. @return The socket fd, or -1 on error. */
    int createInetSocket();
    /** @brief Binds @p sockfd to @p port on all interfaces. @return 0 on success. */
    int bindInet(int sockfd, int port);
    /** @brief Accepts a connection on @p sockfd. @return Client fd, or -1. */
    int acceptInet(int sockfd);
    /** @brief Marks @p sockfd listening with the given @p backlog. */
    int listen(int sockfd, int backlog);
    /** @brief Sends @p len bytes from @p buf. @return Bytes sent, or -1. */
    ssize_t send(int sockfd, const void *buf, size_t len);
    /** @brief Receives up to @p len bytes into @p buf. @return Bytes read, or -1. */
    ssize_t recv(int sockfd, void *buf, size_t len);
    /** @brief Closes @p fd. @return 0 on success. */
    int close(int fd);

    /** @brief Toggles SO_REUSEADDR on @p sockfd. */
    int setReuseAddr(int sockfd, bool reuse);
    /** @brief Toggles non-blocking mode (O_NONBLOCK) on @p sockfd. */
    int setNonBlocking(int sockfd, bool non_blocking);
    /** @brief Toggles TCP_NODELAY (Nagle off) on @p sockfd. */
    int setTcpNoDelay(int sockfd, bool no_delay);

    /** @brief Returns @c strerror(errno) for the last failure. */
    std::string getLastError();
    /** @brief Tells whether the last error was EWOULDBLOCK/EAGAIN. */
    bool wouldBlock();
}

#endif /* !SYS_SOCKET_HPP_ */
