/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Sys_Poll_Win.cpp — Windows poll backend (WSAPoll via WinSock2)
** Compiled as a shared library: zappy_network_win.dll
*/

#ifndef _WIN32
    #error "This file must only be compiled on Windows"
#endif

#include <winsock2.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

namespace {
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

/*
** Mirrors the Linux backend signature exactly.
** Uses WSAPoll() which is the WinSock2 equivalent of POSIX poll().
*/
__declspec(dllexport)
int net_poll(int *fds, short *events, short *revents, unsigned int count, int timeout)
{
    WSAPOLLFD pfdBuf[count];

    for (unsigned int i = 0; i < count; ++i) {
        pfdBuf[i].fd      = static_cast<SOCKET>(fds[i]);
        pfdBuf[i].events  = events[i];
        pfdBuf[i].revents = 0;
    }

    int ret = ::WSAPoll(pfdBuf, static_cast<ULONG>(count), timeout);

    for (unsigned int i = 0; i < count; ++i) {
        revents[i] = pfdBuf[i].revents;
    }
    return ret;
}

__declspec(dllexport) int net_isInterrupted()
{
    /* WSAEINTR is the WinSock2 equivalent of EINTR */
    return (WSAGetLastError() == WSAEINTR) ? 1 : 0;
}

__declspec(dllexport) const char *net_pollGetLastError()
{
    std::string msg = winLastError();
    std::strncpy(s_errorBuf, msg.c_str(), sizeof(s_errorBuf) - 1);
    s_errorBuf[sizeof(s_errorBuf) - 1] = '\0';
    return s_errorBuf;
}

} /* extern "C" */
