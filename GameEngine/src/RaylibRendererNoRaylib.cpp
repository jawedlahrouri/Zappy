/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RaylibRenderer fallback without raylib
*/

#include "RaylibRenderer.hpp"
#include <iostream>

struct RaylibRenderer::Impl {
    bool warningPrinted = false;
};

RaylibRenderer::RaylibRenderer() : _impl(std::make_unique<Impl>())
{
}

RaylibRenderer::~RaylibRenderer()
{
    close();
}

bool RaylibRenderer::isAvailable() const
{
    return false;
}

void RaylibRenderer::open(unsigned int width, unsigned int height,
    const std::string& title)
{
    (void)width;
    (void)height;
    (void)title;
    if (!_impl->warningPrinted) {
        std::cerr << "[RaylibRenderer] Impossible de lancer la vue 3D "
            "sans la lib raylib." << std::endl;
        _impl->warningPrinted = true;
    }
}

void RaylibRenderer::close()
{
}

bool RaylibRenderer::isOpen() const
{
    return false;
}

bool RaylibRenderer::shouldToggleTo2D() const
{
    return false;
}

void RaylibRenderer::handleInput(const GameWorld& world)
{
    (void)world;
}

std::optional<std::pair<int, int>> RaylibRenderer::consumeTileRequest()
{
    return std::nullopt;
}

void RaylibRenderer::beginFrame()
{
}

void RaylibRenderer::drawWorld(const GameWorld& world)
{
    (void)world;
}

void RaylibRenderer::endFrame()
{
}
