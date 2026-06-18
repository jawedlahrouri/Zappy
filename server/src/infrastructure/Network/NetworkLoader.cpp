/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** NetworkLoader.cpp
*/

#include "NetworkLoader.hpp"
#include "ServerException.hpp"
#include <stdexcept>

#ifdef _WIN32
    #include <windows.h>
    #define OPEN_LIB(path)      static_cast<void *>(LoadLibraryA(path.c_str()))
    #define CLOSE_LIB(handle)   FreeLibrary(static_cast<HMODULE>(handle))
    #define GET_SYM(h, name)    static_cast<void *>(GetProcAddress(static_cast<HMODULE>(h), name))
    static std::string getPlatformError()
    {
        DWORD code = GetLastError();
        char buf[256] = {};
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, code, 0, buf, sizeof(buf), nullptr);
        return std::string(buf);
    }
#else
    #include <dlfcn.h>
    #define OPEN_LIB(path)      dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL)
    #define CLOSE_LIB(handle)   dlclose(handle)
    #define GET_SYM(h, name)    dlsym(h, name)
    static std::string getPlatformError()
    {
        const char *err = dlerror();
        return err ? std::string(err) : std::string("unknown error");
    }
#endif

NetworkLoader::NetworkLoader()
    : _handle(nullptr), _symbols{}
{
}

NetworkLoader::~NetworkLoader()
{
    unload();
}

std::string NetworkLoader::defaultLibraryPath()
{
#ifdef _WIN32
    return "zappy_network_win.dll";
#else
    return "./zappy_network_linux.so";
#endif
}

void NetworkLoader::load(const std::string &libraryPath)
{
    if (_handle != nullptr) {
        unload();
    }
    _handle = OPEN_LIB(libraryPath);
    if (_handle == nullptr) {
        throw ServerException("NetworkLoader: failed to load '" + libraryPath + "': " + getPlatformError());
    }
    loadAllSymbols();
}

void NetworkLoader::unload()
{
    if (_handle != nullptr) {
        CLOSE_LIB(_handle);
        _handle = nullptr;
        _symbols = {};
    }
}

const NetworkBackend::Symbols &NetworkLoader::getSymbols() const
{
    return _symbols;
}

bool NetworkLoader::isLoaded() const
{
    return _handle != nullptr;
}

void *NetworkLoader::resolveSymbol(const std::string &name) const
{
    void *sym = GET_SYM(_handle, name.c_str());
    if (sym == nullptr) {
        throw ServerException("NetworkLoader: missing symbol '" + name + "': " + getPlatformError());
    }
    return sym;
}

void NetworkLoader::loadAllSymbols()
{
    _symbols.createInetSocket = reinterpret_cast<NetworkBackend::FnCreateInetSocket>(resolveSymbol("net_createInetSocket"));
    _symbols.bindInet         = reinterpret_cast<NetworkBackend::FnBindInet>        (resolveSymbol("net_bindInet"));
    _symbols.acceptInet       = reinterpret_cast<NetworkBackend::FnAcceptInet>      (resolveSymbol("net_acceptInet"));
    _symbols.listen           = reinterpret_cast<NetworkBackend::FnListen>          (resolveSymbol("net_listen"));
    _symbols.send             = reinterpret_cast<NetworkBackend::FnSend>            (resolveSymbol("net_send"));
    _symbols.recv             = reinterpret_cast<NetworkBackend::FnRecv>            (resolveSymbol("net_recv"));
    _symbols.close            = reinterpret_cast<NetworkBackend::FnClose>           (resolveSymbol("net_close"));
    _symbols.setReuseAddr     = reinterpret_cast<NetworkBackend::FnSetReuseAddr>    (resolveSymbol("net_setReuseAddr"));
    _symbols.setNonBlocking   = reinterpret_cast<NetworkBackend::FnSetNonBlocking>  (resolveSymbol("net_setNonBlocking"));
    _symbols.getLastError     = reinterpret_cast<NetworkBackend::FnGetLastError>    (resolveSymbol("net_getLastError"));
    _symbols.wouldBlock       = reinterpret_cast<NetworkBackend::FnWouldBlock>      (resolveSymbol("net_wouldBlock"));

    _symbols.poll             = reinterpret_cast<NetworkBackend::FnPoll>            (resolveSymbol("net_poll"));
    _symbols.isInterrupted    = reinterpret_cast<NetworkBackend::FnIsInterrupted>   (resolveSymbol("net_isInterrupted"));
    _symbols.pollGetLastError = reinterpret_cast<NetworkBackend::FnPollGetLastError>(resolveSymbol("net_pollGetLastError"));
}
