/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GraphicLibrary
*/

#ifndef GRAPHICLIBRARY_HPP_
    #define GRAPHICLIBRARY_HPP_

    #include "IGraphic.hpp"
    #include <dlfcn.h>
    #include <iostream>
    #include <stdexcept>
    #include <string>

class GraphicLibrary {
public:
    GraphicLibrary();
    ~GraphicLibrary();

    GraphicLibrary(const GraphicLibrary&) = delete;
    GraphicLibrary& operator=(const GraphicLibrary&) = delete;

    bool load(const std::string& path);
    void unload();
    bool isLoaded() const;
    IGraphic& get();
    const std::string& getPath() const;
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
