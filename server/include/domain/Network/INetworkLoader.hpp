/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** INetworkLoader.hpp
*/

/**
 * @file INetworkLoader.hpp
 * @brief Interface for loading the platform network backend and its symbols.
 */

#ifndef INETWORKLOADER_HPP_
#define INETWORKLOADER_HPP_

#include <string>
#include <cstddef>

/**
 * @namespace NetworkBackend
 * @brief Function-pointer types and table exported by each platform backend.
 *
 * Every platform (`.so` on Linux, `.dll` on Windows) exports the same set of C
 * functions (C linkage avoids name mangling). They are resolved at runtime into
 * a @ref NetworkBackend::Symbols table that @ref Socket and @ref Poll call
 * through, instead of calling the OS directly.
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

    /**
     * @struct Symbols
     * @brief Resolved function pointers for one loaded network backend.
     *
     * Populated by @ref INetworkLoader::load; every field is @c nullptr until
     * the matching symbol is resolved from the shared library.
     */
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

/**
 * @interface INetworkLoader
 * @brief Loads/unloads a network backend shared library and exposes its symbols.
 *
 * @par Design Pattern: Adapter
 * Hides the platform's dynamic-loading mechanism (@c dlopen / @c LoadLibrary)
 * behind a uniform interface, turning a `.so`/`.dll` into a ready-to-use
 * @ref NetworkBackend::Symbols table.
 */
class INetworkLoader {
public:
    virtual ~INetworkLoader() = default;

    /**
     * @brief Loads the backend library and resolves its symbols.
     * @param libraryPath Path to the `.so`/`.dll` to load.
     * @throws ServerException If loading or a symbol resolution fails.
     */
    virtual void load(const std::string &libraryPath) = 0;

    /**
     * @brief Unloads the currently loaded backend library, if any.
     */
    virtual void unload() = 0;

    /**
     * @brief Returns the resolved symbol table.
     * @return The backend's @ref NetworkBackend::Symbols.
     */
    virtual const NetworkBackend::Symbols &getSymbols() const = 0;

    /**
     * @brief Tells whether a backend is currently loaded.
     * @return @c true if a library is loaded and its symbols are available.
     */
    virtual bool isLoaded() const = 0;
};

#endif /* !INETWORKLOADER_HPP_ */
