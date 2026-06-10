/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Socket.hpp
*/

#ifndef SYS_SOCKET_HPP_
#define SYS_SOCKET_HPP_

#include <sys/socket.h>
#include <unistd.h>

namespace SysSocket {
    int socket(int domain, int type, int protocol);
    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    int listen(int sockfd, int backlog);
    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    ssize_t send(int sockfd, const void *buf, size_t len, int flags);
    ssize_t recv(int sockfd, void *buf, size_t len, int flags);
    int close(int fd);
}

#endif /* !SYS_SOCKET_HPP_ */
