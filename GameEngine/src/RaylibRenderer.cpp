/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RaylibRenderer
*/

#include "RaylibRenderer.hpp"
#include <algorithm>
#include <cmath>

#ifndef ZAPPY_HAS_RAYLIB
    #define ZAPPY_HAS_RAYLIB 0
#endif

#if ZAPPY_HAS_RAYLIB
    #include <raylib.h>
#endif

namespace {
    constexpr float TILE_SIZE = 2.0f;
    constexpr float RESOURCE_RADIUS = 0.12f;

#if ZAPPY_HAS_RAYLIB
    int countResources(const Resources& resources)
    {
        return resources.food + resources.linemate + resources.deraumere +
            resources.sibur + resources.mendiane + resources.phiras +
            resources.thystame;
    }

    Vector3 tileCenter(int x, int y, int width, int height)
    {
        const float originX = -static_cast<float>(width - 1) * TILE_SIZE / 2.0f;
        const float originZ = -static_cast<float>(height - 1) * TILE_SIZE / 2.0f;

        return {
            originX + static_cast<float>(x) * TILE_SIZE,
            0.0f,
            originZ + static_cast<float>(y) * TILE_SIZE
        };
    }

    Vector3 orientationVector(int orientation)
    {
        if (orientation == 1)
            return {0.0f, 0.0f, -1.0f};
        if (orientation == 2)
            return {1.0f, 0.0f, 0.0f};
        if (orientation == 3)
            return {0.0f, 0.0f, 1.0f};
        if (orientation == 4)
            return {-1.0f, 0.0f, 0.0f};
        return {0.0f, 0.0f, -1.0f};
    }

    void drawResourceStack(Vector3 center, int quantity, Vector3 offset,
        float radius, Color color)
    {
        const int visibleQuantity = std::min(quantity, 4);

        for (int i = 0; i < visibleQuantity; ++i) {
            DrawSphere({
                center.x + offset.x,
                center.y + 0.18f + offset.y + static_cast<float>(i) * radius,
                center.z + offset.z
            }, radius, color);
        }
    }

    void drawTileResources(const Tile& tile, const Vector3& center)
    {
        drawResourceStack(center, tile.resources.food,
            {-0.58f, 0.0f, -0.58f}, RESOURCE_RADIUS, ORANGE);
        drawResourceStack(center, tile.resources.linemate,
            {0.0f, 0.0f, -0.58f}, RESOURCE_RADIUS, SKYBLUE);
        drawResourceStack(center, tile.resources.deraumere,
            {0.58f, 0.0f, -0.58f}, RESOURCE_RADIUS, LIME);
        drawResourceStack(center, tile.resources.sibur,
            {-0.58f, 0.0f, 0.0f}, RESOURCE_RADIUS, VIOLET);
        drawResourceStack(center, tile.resources.mendiane,
            {0.58f, 0.0f, 0.0f}, RESOURCE_RADIUS, GOLD);
        drawResourceStack(center, tile.resources.phiras,
            {-0.30f, 0.0f, 0.58f}, RESOURCE_RADIUS, PINK);
        drawResourceStack(center, tile.resources.thystame,
            {0.30f, 0.0f, 0.58f}, RESOURCE_RADIUS, RED);
    }
#endif
}

struct RaylibRenderer::Impl {
#if ZAPPY_HAS_RAYLIB
    bool initialized = false;
    bool frameStarted = false;
    int lastMapWidth = 0;
    int lastMapHeight = 0;
    Camera3D camera = {};

    void configureCamera(int mapWidth, int mapHeight)
    {
        const float biggestSide = static_cast<float>(std::max(mapWidth, mapHeight));
        const float distance = std::max(8.0f, biggestSide * 2.0f);

        camera.position = {distance, distance * 0.75f, distance};
        camera.target = {0.0f, 0.0f, 0.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
        lastMapWidth = mapWidth;
        lastMapHeight = mapHeight;
    }
#endif
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
    return ZAPPY_HAS_RAYLIB != 0;
}

void RaylibRenderer::open(unsigned int width, unsigned int height,
    const std::string& title)
{
#if ZAPPY_HAS_RAYLIB
    if (_impl->initialized)
        return;

    InitWindow(static_cast<int>(width), static_cast<int>(height), title.c_str());
    SetTargetFPS(60);
    _impl->initialized = true;
    _impl->configureCamera(10, 10);
#else
    (void)width;
    (void)height;
    (void)title;
#endif
}

void RaylibRenderer::close()
{
#if ZAPPY_HAS_RAYLIB
    if (_impl->initialized) {
        CloseWindow();
        _impl->initialized = false;
        _impl->frameStarted = false;
    }
#endif
}

bool RaylibRenderer::isOpen() const
{
#if ZAPPY_HAS_RAYLIB
    return _impl->initialized && !WindowShouldClose();
#else
    return false;
#endif
}

bool RaylibRenderer::shouldToggleTo2D() const
{
#if ZAPPY_HAS_RAYLIB
    return _impl->initialized &&
        (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_V));
#else
    return false;
#endif
}

void RaylibRenderer::beginFrame()
{
#if ZAPPY_HAS_RAYLIB
    if (!_impl->initialized)
        return;

    BeginDrawing();
    ClearBackground({20, 23, 26, 255});
    _impl->frameStarted = true;
#endif
}

void RaylibRenderer::drawWorld(const GameWorld& world)
{
#if ZAPPY_HAS_RAYLIB
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    if (!_impl->initialized || !_impl->frameStarted)
        return;
    if (mapWidth > 0 && mapHeight > 0 &&
        (_impl->lastMapWidth != mapWidth || _impl->lastMapHeight != mapHeight)) {
        _impl->configureCamera(mapWidth, mapHeight);
    }

    UpdateCamera(&_impl->camera, CAMERA_ORBITAL);
    BeginMode3D(_impl->camera);

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty()) {
        DrawGrid(20, 1.0f);
    } else {
        const auto& map = world.getMap();

        for (int x = 0; x < mapWidth; ++x) {
            for (int y = 0; y < mapHeight; ++y) {
                const Vector3 center = tileCenter(x, y, mapWidth, mapHeight);
                const Color tileColor = ((x + y) % 2 == 0) ?
                    Color{52, 112, 77, 255} : Color{45, 96, 68, 255};

                DrawCube({center.x, -0.06f, center.z},
                    TILE_SIZE * 0.96f, 0.12f, TILE_SIZE * 0.96f, tileColor);
                DrawCubeWires({center.x, 0.01f, center.z},
                    TILE_SIZE * 0.96f, 0.04f, TILE_SIZE * 0.96f,
                    Color{19, 36, 29, 180});
                if (countResources(map[x][y].resources) > 0)
                    drawTileResources(map[x][y], center);
            }
        }
    }

    for (const auto& pair : world.getPlayers()) {
        const Player& player = pair.second;
        const Vector3 center = tileCenter(player.x, player.y, mapWidth, mapHeight);
        const Vector3 direction = orientationVector(player.orientation);
        const Vector3 body = {center.x, 0.5f, center.z};

        DrawCylinder(body, 0.26f, 0.34f, 0.9f, 16, Color{55, 188, 214, 255});
        DrawCylinderWires(body, 0.26f, 0.34f, 0.9f, 16, Color{7, 73, 92, 255});
        DrawCylinderEx(
            {center.x, 0.75f, center.z},
            {center.x + direction.x * 0.75f, 0.75f,
                center.z + direction.z * 0.75f},
            0.10f, 0.0f, 12, Color{250, 250, 250, 255});
    }

    for (const auto& pair : world.getEggs()) {
        const Egg& egg = pair.second;
        const Vector3 center = tileCenter(egg.x, egg.y, mapWidth, mapHeight);
        const Color eggColor = egg.isHatched ?
            Color{250, 210, 48, 255} : Color{240, 236, 216, 255};

        DrawSphere({center.x + 0.45f, 0.28f, center.z + 0.45f},
            0.24f, eggColor);
    }

    EndMode3D();
    DrawText("TAB/V : vue 2D", 20, 20, 20, RAYWHITE);
    DrawText(TextFormat("%dx%d | joueurs: %i", mapWidth, mapHeight,
        static_cast<int>(world.getPlayers().size())), 20, 46, 18,
        Color{210, 220, 225, 255});
#else
    (void)world;
#endif
}

void RaylibRenderer::endFrame()
{
#if ZAPPY_HAS_RAYLIB
    if (!_impl->initialized || !_impl->frameStarted)
        return;

    EndDrawing();
    _impl->frameStarted = false;
#endif
}
