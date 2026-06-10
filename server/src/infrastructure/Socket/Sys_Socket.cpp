/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Socket.cpp
*/

#include "Socket/Sys_Socket.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

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

    int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
        return ::setsockopt(sockfd, level, optname, optval, optlen);
    }

    int fcntl(int fd, int cmd, int arg) {
        return ::fcntl(fd, cmd, arg);
    }

    int createInetSocket() {
        return ::socket(AF_INET, SOCK_STREAM, 0);
    }

    int bindInet(int sockfd, int port) {
        struct sockaddr_in addr;
        std::memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        return ::bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    }

    int acceptInet(int sockfd) {
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        return ::accept(sockfd, (struct sockaddr *)&addr, &addr_len);
    }

    int setReuseAddr(int sockfd, bool reuse) {
        int opt = reuse ? 1 : 0;
        return ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }

    int setNonBlocking(int sockfd, bool non_blocking) {
        int flags = ::fcntl(sockfd, F_GETFL, 0);
        if (flags == -1) {
            return -1;
        }
        if (non_blocking) {
            flags |= O_NONBLOCK;
        } else {
            flags &= ~O_NONBLOCK;
        }
        return ::fcntl(sockfd, F_SETFL, flags);
    }
}
