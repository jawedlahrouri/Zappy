/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DRenderer
*/

#include "Graphic3DRenderer.hpp"
#include "Graphic3DConfig.hpp"
#include <algorithm>
#include <array>
#include <sstream>

Vector3 Graphic3DRenderer::getTileCenter(int x, int y, int width,
    int height) const
{
    const float step = Graphic3DConfig::TILE_SIZE + Graphic3DConfig::TILE_GAP;
    const float originX = -((static_cast<float>(width) - 1.0f) * step) / 2.0f;
    const float originZ = -((static_cast<float>(height) - 1.0f) * step) / 2.0f;

    return {
        originX + static_cast<float>(x) * step,
        Graphic3DConfig::TILE_HEIGHT / 2.0f,
        originZ + static_cast<float>(y) * step
    };
}

int Graphic3DRenderer::getResourceTotal(const Resources& resources) const
{
    return resources.food + resources.linemate + resources.deraumere +
        resources.sibur + resources.mendiane + resources.phiras +
        resources.thystame;
}

Color Graphic3DRenderer::getTileColor(const Tile& tile) const
{
    const int total = getResourceTotal(tile.resources);
    const int green = std::clamp(94 + total * 8, 94, 190);

    return {68, static_cast<unsigned char>(green), 104, 255};
}

Color Graphic3DRenderer::getPlayerColor(int teamColor) const
{
    switch (teamColor) {
        case 1:
            return {78, 154, 242, 255};
        case 3:
            return {74, 201, 120, 255};
        case 4:
            return {235, 94, 184, 255};
        case 2:
        default:
            return {231, 91, 84, 255};
    }
}

void Graphic3DRenderer::drawResources(const Tile& tile, int width,
    int height) const
{
    struct ResourceMarker {
        int count;
        Vector3 offset;
        Color color;
    };
    const Vector3 center = getTileCenter(tile.x, tile.y, width, height);
    const std::array<ResourceMarker, 7> markers = {{
        {tile.resources.food, {-0.54f, 0.0f, -0.48f}, {239, 198, 92, 255}},
        {tile.resources.linemate, {-0.18f, 0.0f, -0.48f}, {91, 176, 238, 255}},
        {tile.resources.deraumere, {0.18f, 0.0f, -0.48f}, {155, 111, 232, 255}},
        {tile.resources.sibur, {0.54f, 0.0f, -0.48f}, {242, 137, 91, 255}},
        {tile.resources.mendiane, {-0.36f, 0.0f, 0.28f}, {99, 216, 169, 255}},
        {tile.resources.phiras, {0.0f, 0.0f, 0.28f}, {231, 104, 137, 255}},
        {tile.resources.thystame, {0.36f, 0.0f, 0.28f}, {245, 238, 120, 255}}
    }};

    for (const ResourceMarker& marker : markers) {
        if (marker.count <= 0)
            continue;
        const float heightValue = 0.12f + std::min(marker.count, 8) * 0.06f;
        const Vector3 position = {
            center.x + marker.offset.x,
            Graphic3DConfig::TILE_HEIGHT + heightValue / 2.0f,
            center.z + marker.offset.z
        };

        DrawCube(position, 0.18f, heightValue, 0.18f, marker.color);
    }
}

void Graphic3DRenderer::drawTiles(const GameWorld& world) const
{
    const int width = world.getWidth();
    const int height = world.getHeight();
    const auto& map = world.getMap();

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            const Tile& tile = map[x][y];
            const Vector3 center = getTileCenter(x, y, width, height);

            DrawCube(center, Graphic3DConfig::TILE_SIZE,
                Graphic3DConfig::TILE_HEIGHT, Graphic3DConfig::TILE_SIZE,
                getTileColor(tile));
            DrawCubeWires(center, Graphic3DConfig::TILE_SIZE,
                Graphic3DConfig::TILE_HEIGHT + 0.01f,
                Graphic3DConfig::TILE_SIZE, {31, 40, 42, 255});
            drawResources(tile, width, height);
        }
    }
}

void Graphic3DRenderer::drawPlayers(const GameWorld& world) const
{
    const int width = std::max(1, world.getWidth());
    const int height = std::max(1, world.getHeight());

    for (const auto& [id, player] : world.getPlayers()) {
        (void)id;
        const Vector3 tile = getTileCenter(player.x, player.y, width, height);
        const Color color = getPlayerColor(player.teamColor);
        const Vector3 body = {
            tile.x,
            Graphic3DConfig::TILE_HEIGHT + 0.36f,
            tile.z
        };
        Vector3 direction = {0.0f, 0.0f, -1.0f};

        if (player.orientation == 2)
            direction = {1.0f, 0.0f, 0.0f};
        if (player.orientation == 3)
            direction = {0.0f, 0.0f, 1.0f};
        if (player.orientation == 4)
            direction = {-1.0f, 0.0f, 0.0f};
        DrawCylinder(body, 0.24f, 0.28f, 0.64f, 12, color);
        DrawSphere({
            tile.x,
            Graphic3DConfig::TILE_HEIGHT + 0.82f,
            tile.z
        }, 0.22f, RAYWHITE);
        DrawCube({
            tile.x + direction.x * 0.34f,
            Graphic3DConfig::TILE_HEIGHT + 0.48f,
            tile.z + direction.z * 0.34f
        }, 0.18f, 0.18f, 0.18f, BLACK);
    }
}

void Graphic3DRenderer::drawEggs(const GameWorld& world) const
{
    const int width = std::max(1, world.getWidth());
    const int height = std::max(1, world.getHeight());

    for (const auto& [id, egg] : world.getEggs()) {
        (void)id;
        const Vector3 tile = getTileCenter(egg.x, egg.y, width, height);
        const Color color = egg.isHatched ?
            Color{247, 213, 88, 255} : Color{235, 232, 218, 255};

        DrawSphere({
            tile.x + 0.52f,
            Graphic3DConfig::TILE_HEIGHT + 0.18f,
            tile.z + 0.52f
        }, 0.22f, color);
    }
}

void Graphic3DRenderer::drawMap(const GameWorld& world) const
{
    if (world.getWidth() <= 0 || world.getHeight() <= 0 ||
        world.getMap().empty()) {
        drawPreviewWorld();
        return;
    }
    drawTiles(world);
    drawEggs(world);
    drawPlayers(world);
}

void Graphic3DRenderer::drawPreviewWorld() const
{
    constexpr int width = 8;
    constexpr int height = 8;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            const Vector3 center = getTileCenter(x, y, width, height);
            const Color color = ((x + y) % 2 == 0) ?
                Color{58, 112, 102, 255} : Color{52, 96, 92, 255};

            DrawCube(center, Graphic3DConfig::TILE_SIZE,
                Graphic3DConfig::TILE_HEIGHT, Graphic3DConfig::TILE_SIZE,
                color);
            DrawCubeWires(center, Graphic3DConfig::TILE_SIZE,
                Graphic3DConfig::TILE_HEIGHT + 0.01f,
                Graphic3DConfig::TILE_SIZE, {38, 48, 50, 255});
        }
    }
}

void Graphic3DRenderer::drawHud(const GameWorld& world) const
{
    std::ostringstream status;

    status << "Zappy 3D  |  map " << world.getWidth() << "x"
        << world.getHeight() << "  |  players " << world.getPlayers().size();
    DrawRectangle(0, 0, GetScreenWidth(), 54, {9, 12, 17, 210});
    DrawText(status.str().c_str(), 18, 14, 20, RAYWHITE);
    DrawText("Mouse wheel: zoom  |  ZQSD/right drag: rotate  |  arrows: move  |  R: reset",
        18, 34, 16, {190, 204, 210, 255});
}

void Graphic3DRenderer::drawMenu(const Camera3D& camera,
    const std::string& status) const
{
    BeginMode3D(camera);
    drawPreviewWorld();
    EndMode3D();
    DrawRectangle(0, 0, GetScreenWidth(), 90, {9, 12, 17, 218});
    DrawText("Zappy 3D", 24, 18, 34, RAYWHITE);
    DrawText(status.c_str(), 26, 58, 18, {183, 205, 205, 255});
}

void Graphic3DRenderer::drawWorld(const Camera3D& camera,
    const GameWorld& world) const
{
    BeginMode3D(camera);
    drawMap(world);
    EndMode3D();
    drawHud(world);
}
