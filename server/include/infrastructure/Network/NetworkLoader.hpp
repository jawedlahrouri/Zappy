/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** NetworkLoader.hpp
*/

#ifndef NETWORKLOADER_HPP_
#define NETWORKLOADER_HPP_

#include "INetworkLoader.hpp"

/*
** Loads the OS-specific network backend shared library at runtime.
** On Linux : uses dlopen / dlsym / dlclose
** On Windows: uses LoadLibraryA / GetProcAddress / FreeLibrary
**
** The correct library path is resolved automatically based on the current OS.
*/
class NetworkLoader : public INetworkLoader {
public:
    NetworkLoader();
    ~NetworkLoader() override;

    NetworkLoader(const NetworkLoader &)            = delete;
    NetworkLoader &operator=(const NetworkLoader &) = delete;

    void load(const std::string &libraryPath) override;
    void unload() override;

    const NetworkBackend::Symbols &getSymbols() const override;
    bool isLoaded() const override;

    static std::string defaultLibraryPath();

private:
    void *_handle;
    NetworkBackend::Symbols _symbols;

    void *resolveSymbol(const std::string &name) const;
    void loadAllSymbols();
};

#endif /* !NETWORKLOADER_HPP_ */
