/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Socket_Win.cpp — Windows network backend (WinSock2)
** Compiled as a shared library: zappy_network_win.dll
**
** Differences vs Linux:
**  - MSG_NOSIGNAL does not exist on Windows → flag 0 is used instead
**  - fcntl does not exist → ioctlsocket(FIONBIO) is used for non-blocking mode
**  - WSAStartup / WSACleanup must bracket all WinSock2 usage
*/

#ifndef _WIN32
    #error "This file must only be compiled on Windows"
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <cstddef>
#include <string>

// #pragma comment(lib, "ws2_32.lib")

/*
** WsaGuard — RAII wrapper that calls WSAStartup on construction
** and WSACleanup on destruction. Instantiated once at DLL load time.
*/
namespace {
    class WsaGuard {
    public:
        WsaGuard()
        {
            WSADATA data;
            WSAStartup(MAKEWORD(2, 2), &data);
        }
        ~WsaGuard()
        {
            WSACleanup();
        }
    };

    static WsaGuard s_wsaGuard;
    static thread_local char s_errorBuf[256];

    static std::string winLastError()
    {
        DWORD code = WSAGetLastError();
        char buf[256] = {};
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0, buf, sizeof(buf), nullptr);
        return std::string(buf);
    }
}

extern "C" {

__declspec(dllexport) int net_createInetSocket()
{
    return static_cast<int>(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
}

__declspec(dllexport) int net_bindInet(int sockfd, int port)
{
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(static_cast<u_short>(port));
    addr.sin_addr.s_addr = INADDR_ANY;
    return ::bind(static_cast<SOCKET>(sockfd),
                  reinterpret_cast<struct sockaddr *>(&addr),
                  sizeof(addr));
}

__declspec(dllexport) int net_acceptInet(int sockfd)
{
    struct sockaddr_in addr;
    int addrLen = sizeof(addr);
    SOCKET result = ::accept(static_cast<SOCKET>(sockfd),
                             reinterpret_cast<struct sockaddr *>(&addr),
                             &addrLen);
    if (result == INVALID_SOCKET) {
        return -1;
    }
    return static_cast<int>(result);
}

__declspec(dllexport) int net_listen(int sockfd, int backlog)
{
    return ::listen(static_cast<SOCKET>(sockfd), backlog);
}

__declspec(dllexport) long net_send(int sockfd, const void *buf, std::size_t len)
{
    /* MSG_NOSIGNAL does not exist on Windows; flag 0 is the equivalent */
    int result = ::send(static_cast<SOCKET>(sockfd),
                        static_cast<const char *>(buf),
                        static_cast<int>(len),
                        0);
    return static_cast<long>(result);
}

__declspec(dllexport) long net_recv(int sockfd, void *buf, std::size_t len)
{
    int result = ::recv(static_cast<SOCKET>(sockfd),
                        static_cast<char *>(buf),
                        static_cast<int>(len),
                        0);
    return static_cast<long>(result);
}

__declspec(dllexport) int net_close(int fd)
{
    return ::closesocket(static_cast<SOCKET>(fd));
}

__declspec(dllexport) int net_setReuseAddr(int sockfd, int reuse)
{
    return ::setsockopt(static_cast<SOCKET>(sockfd),
                        SOL_SOCKET, SO_REUSEADDR,
                        reinterpret_cast<const char *>(&reuse),
                        sizeof(reuse));
}

__declspec(dllexport) int net_setNonBlocking(int sockfd, int nonBlocking)
{
    u_long mode = nonBlocking ? 1UL : 0UL;
    return ::ioctlsocket(static_cast<SOCKET>(sockfd), FIONBIO, &mode);
}

__declspec(dllexport) const char *net_getLastError()
{
    std::string msg = winLastError();
    std::strncpy(s_errorBuf, msg.c_str(), sizeof(s_errorBuf) - 1);
    s_errorBuf[sizeof(s_errorBuf) - 1] = '\0';
    return s_errorBuf;
}

__declspec(dllexport) int net_wouldBlock()
{
    int code = WSAGetLastError();
    return (code == WSAEWOULDBLOCK) ? 1 : 0;
}

} /* extern "C" */
