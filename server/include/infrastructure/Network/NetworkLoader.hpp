/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** NetworkLoader.hpp
*/

/**
 * @file NetworkLoader.hpp
 * @brief Runtime loader for the OS-specific network backend library.
 */

#ifndef NETWORKLOADER_HPP_
#define NETWORKLOADER_HPP_

#include "INetworkLoader.hpp"

/**
 * @class NetworkLoader
 * @brief Loads the platform network backend and resolves its symbols.
 *
 * @par Design Pattern: Adapter
 * Wraps the OS dynamic-loading API behind the @ref INetworkLoader contract.
 * - Linux : @c dlopen / @c dlsym / @c dlclose
 * - Windows: @c LoadLibraryA / @c GetProcAddress / @c FreeLibrary
 *
 * The library path is resolved automatically for the current OS
 * (@ref defaultLibraryPath). Non-copyable: it owns a single library handle.
 */
class NetworkLoader : public INetworkLoader {
public:
    /** @brief Builds an empty loader (no library loaded yet). */
    NetworkLoader();
    /** @brief Unloads the library if still loaded. */
    ~NetworkLoader() override;

    NetworkLoader(const NetworkLoader &)            = delete;
    NetworkLoader &operator=(const NetworkLoader &) = delete;

    void load(const std::string &libraryPath) override;
    void unload() override;

    const NetworkBackend::Symbols &getSymbols() const override;
    bool isLoaded() const override;

    /**
     * @brief Returns the default backend library path for the current OS.
     * @return e.g. @c "./libzappy_network_linux.so" on Linux.
     */
    static std::string defaultLibraryPath();

private:
    void *_handle;                   /**< OS library handle (dlopen/LoadLibrary). */
    NetworkBackend::Symbols _symbols; /**< Resolved backend symbols. */

    /**
     * @brief Resolves one symbol by name from the loaded library.
     * @param name Exported symbol name.
     * @return Function pointer, or throws if missing.
     */
    void *resolveSymbol(const std::string &name) const;
    /** @brief Resolves every backend symbol into @ref _symbols. */
    void loadAllSymbols();
};

#endif /* !NETWORKLOADER_HPP_ */
