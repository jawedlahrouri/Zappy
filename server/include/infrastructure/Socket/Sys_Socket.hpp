/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Socket.hpp
*/

#ifndef SYS_SOCKET_HPP_
#define SYS_SOCKET_HPP_

#include <string>
#include <cstddef>
#include <sys/types.h>

namespace SysSocket {
    int createInetSocket();
    int bindInet(int sockfd, int port);
    int acceptInet(int sockfd);
    int listen(int sockfd, int backlog);
    ssize_t send(int sockfd, const void *buf, size_t len);
    ssize_t recv(int sockfd, void *buf, size_t len);
    int close(int fd);
    
    int setReuseAddr(int sockfd, bool reuse);
    int setNonBlocking(int sockfd, bool non_blocking);
    int setTcpNoDelay(int sockfd, bool no_delay);

    std::string getLastError();
    bool wouldBlock();
}

#endif /* !SYS_SOCKET_HPP_ */
