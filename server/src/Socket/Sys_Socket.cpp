/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Socket.cpp
*/

#include "Socket/Sys_Socket.hpp"

namespace SysSocket {
    int socket(int domain, int type, int protocol) {
        return ::socket(domain, type, protocol);
    }

    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
        return ::bind(sockfd, addr, addrlen);
    }

    int listen(int sockfd, int backlog) {
        return ::listen(sockfd, backlog);
    }

    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
        return ::accept(sockfd, addr, addrlen);
    }

    ssize_t send(int sockfd, const void *buf, size_t len, int flags) {
        return ::send(sockfd, buf, len, flags);
    }

    ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
        return ::recv(sockfd, buf, len, flags);
    }

    int close(int fd) {
        return ::close(fd);
    }
}
