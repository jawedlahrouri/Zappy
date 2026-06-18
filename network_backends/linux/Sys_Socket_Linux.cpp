/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Socket_Linux.cpp — Linux network backend (POSIX sockets)
** Compiled as a shared library: zappy_network_linux.so
*/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <cstddef>

namespace {
    static thread_local char s_errorBuf[256];
}

extern "C" {

int net_createInetSocket()
{
    return ::socket(AF_INET, SOCK_STREAM, 0);
}

int net_bindInet(int sockfd, int port)
{
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(static_cast<uint16_t>(port));
    addr.sin_addr.s_addr = INADDR_ANY;
    return ::bind(sockfd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
}

int net_acceptInet(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    return ::accept(sockfd, reinterpret_cast<struct sockaddr *>(&addr), &addrLen);
}

int net_listen(int sockfd, int backlog)
{
    return ::listen(sockfd, backlog);
}

long net_send(int sockfd, const void *buf, std::size_t len)
{
    return static_cast<long>(::send(sockfd, buf, len, MSG_NOSIGNAL));
}

long net_recv(int sockfd, void *buf, std::size_t len)
{
    return static_cast<long>(::recv(sockfd, buf, len, 0));
}

int net_close(int fd)
{
    return ::close(fd);
}

int net_setReuseAddr(int sockfd, int reuse)
{
    return ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}

int net_setNonBlocking(int sockfd, int nonBlocking)
{
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    if (nonBlocking) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    return ::fcntl(sockfd, F_SETFL, flags);
}

const char *net_getLastError()
{
    std::strncpy(s_errorBuf, std::strerror(errno), sizeof(s_errorBuf) - 1);
    s_errorBuf[sizeof(s_errorBuf) - 1] = '\0';
    return s_errorBuf;
}

int net_wouldBlock()
{
    return (errno == EAGAIN || errno == EWOULDBLOCK) ? 1 : 0;
}

} /* extern "C" */
