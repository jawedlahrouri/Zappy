/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GraphicLibrary
*/

/**
 * @file GraphicLibrary.hpp
 * @brief Provides the dynamic loader used to instantiate graphical plugins.
 */

#ifndef GRAPHICLIBRARY_HPP_
    #define GRAPHICLIBRARY_HPP_

    #include "IGraphic.hpp"
    #include <dlfcn.h>
    #include <iostream>
    #include <stdexcept>
    #include <string>

/**
 * @brief Owns the lifetime of a dynamically loaded graphical backend.
 *
 * The loader expects each shared object to export createGraphic() and
 * destroyGraphic() symbols compatible with IGraphic.
 */
class GraphicLibrary {
public:
    /**
     * @brief Creates an empty loader.
     */
    GraphicLibrary();

    /**
     * @brief Unloads the current backend, if one is loaded.
     */
    ~GraphicLibrary();

    GraphicLibrary(const GraphicLibrary&) = delete;
    GraphicLibrary& operator=(const GraphicLibrary&) = delete;

    /**
     * @brief Loads a graphical plugin from a shared-library path.
     * @param path Path passed to dlopen().
     * @return true when the plugin was opened and instantiated.
     */
    bool load(const std::string& path);

    /**
     * @brief Destroys the current graphical instance and closes its library.
     */
    void unload();

    /**
     * @brief Checks whether a graphical backend is currently loaded.
     * @return true when get() can safely be called.
     */
    bool isLoaded() const;

    /**
     * @brief Returns the loaded graphical backend.
     * @return Reference to the active IGraphic implementation.
     * @throws std::runtime_error when no backend is loaded.
     */
    IGraphic& get();

    /**
     * @brief Gets the path used for the last load attempt.
     * @return Shared-library path.
     */
    const std::string& getPath() const;

    /**
     * @brief Gets the last dynamic-loading error.
     * @return Empty string when the last load succeeded.
     */
    const std::string& getLastError() const;

private:
    void* _handle;
    IGraphic* _graphic;
    DestroyGraphicFunction _destroy;
    std::string _path;
    std::string _lastError;
};

inline GraphicLibrary::GraphicLibrary()
    : _handle(nullptr), _graphic(nullptr), _destroy(nullptr)
{
}

inline GraphicLibrary::~GraphicLibrary()
{
    unload();
}

inline bool GraphicLibrary::load(const std::string& path)
{
    CreateGraphicFunction create = nullptr;
    const char* error = nullptr;

    unload();
    _path = path;
    _handle = dlopen(path.c_str(), RTLD_NOW);
    if (_handle == nullptr) {
        error = dlerror();
        _lastError = error == nullptr ? "unknown dlopen error" : error;
        return false;
    }
    dlerror();
    create = reinterpret_cast<CreateGraphicFunction>(
        dlsym(_handle, "createGraphic"));
    error = dlerror();
    if (error != nullptr || create == nullptr) {
        _lastError = error == nullptr ? "missing createGraphic" : error;
        unload();
        return false;
    }
    dlerror();
    _destroy = reinterpret_cast<DestroyGraphicFunction>(
        dlsym(_handle, "destroyGraphic"));
    error = dlerror();
    if (error != nullptr || _destroy == nullptr) {
        _lastError = error == nullptr ? "missing destroyGraphic" : error;
        unload();
        return false;
    }
    _graphic = create();
    if (_graphic == nullptr) {
        _lastError = "createGraphic returned null";
        unload();
        return false;
    }
    _lastError.clear();
    std::cout << "[GraphicLibrary] Loaded " << path << std::endl;
    return true;
}

inline void GraphicLibrary::unload()
{
    if (_graphic != nullptr && _destroy != nullptr)
        _destroy(_graphic);
    _graphic = nullptr;
    _destroy = nullptr;
    if (_handle != nullptr)
        dlclose(_handle);
    _handle = nullptr;
}

inline bool GraphicLibrary::isLoaded() const
{
    return _graphic != nullptr;
}

inline IGraphic& GraphicLibrary::get()
{
    if (_graphic == nullptr)
        throw std::runtime_error("graphic library is not loaded");
    return *_graphic;
}

inline const std::string& GraphicLibrary::getPath() const
{
    return _path;
}

inline const std::string& GraphicLibrary::getLastError() const
{
    return _lastError;
}

#endif /* !GRAPHICLIBRARY_HPP_ */
