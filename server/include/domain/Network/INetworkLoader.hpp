/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** INetworkLoader.hpp
*/

#ifndef INETWORKLOADER_HPP_
#define INETWORKLOADER_HPP_

#include <string>
#include <cstddef>

/*
** Function pointer types exported by each platform backend (.so / .dll).
** All symbols use C linkage (extern "C") to avoid name mangling.
*/
namespace NetworkBackend {
    using FnCreateInetSocket  = int (*)();
    using FnBindInet          = int (*)(int sockfd, int port);
    using FnAcceptInet        = int (*)(int sockfd);
    using FnListen            = int (*)(int sockfd, int backlog);
    using FnSend              = long (*)(int sockfd, const void *buf, std::size_t len);
    using FnRecv              = long (*)(int sockfd, void *buf, std::size_t len);
    using FnClose             = int (*)(int fd);
    using FnSetReuseAddr      = int (*)(int sockfd, int reuse);
    using FnSetNonBlocking    = int (*)(int sockfd, int non_blocking);
    using FnGetLastError      = const char *(*)();
    using FnWouldBlock        = int (*)();

    using FnPoll              = int (*)(int *fds, short *events, short *revents, unsigned int count, int timeout);
    using FnIsInterrupted     = int (*)();
    using FnPollGetLastError  = const char *(*)();

    struct Symbols {
        FnCreateInetSocket  createInetSocket  = nullptr;
        FnBindInet          bindInet          = nullptr;
        FnAcceptInet        acceptInet        = nullptr;
        FnListen            listen            = nullptr;
        FnSend              send              = nullptr;
        FnRecv              recv              = nullptr;
        FnClose             close             = nullptr;
        FnSetReuseAddr      setReuseAddr      = nullptr;
        FnSetNonBlocking    setNonBlocking    = nullptr;
        FnGetLastError      getLastError      = nullptr;
        FnWouldBlock        wouldBlock        = nullptr;

        FnPoll              poll              = nullptr;
        FnIsInterrupted     isInterrupted     = nullptr;
        FnPollGetLastError  pollGetLastError  = nullptr;
    };
}

class INetworkLoader {
public:
    virtual ~INetworkLoader() = default;

    virtual void load(const std::string &libraryPath) = 0;
    virtual void unload() = 0;

    virtual const NetworkBackend::Symbols &getSymbols() const = 0;
    virtual bool isLoaded() const = 0;
};

#endif /* !INETWORKLOADER_HPP_ */
