/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Socket.cpp
*/

#ifndef _WIN32

#include "Sys_Socket.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>

namespace SysSocket {
    int listen(int sockfd, int backlog) {
        return ::listen(sockfd, backlog);
    }

    ssize_t send(int sockfd, const void *buf, size_t len) {
        int flags = MSG_NOSIGNAL;
        return ::send(sockfd, buf, len, flags);
    }

    ssize_t recv(int sockfd, void *buf, size_t len) {
        int flags = 0;
        return ::recv(sockfd, buf, len, flags);
    }

    int close(int fd) {
        return ::close(fd);
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

    std::string getLastError() {
        return std::strerror(errno);
    }

    bool wouldBlock() {
        return (errno == EAGAIN || errno == EWOULDBLOCK);
    }
}

#endif
