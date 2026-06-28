/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DMap
*/

#include "Graphic3DMap.hpp"
#include "Graphic3DConfig.hpp"
#include "Graphic3DUtils.hpp"
#include <algorithm>
#include <array>
#include <cmath>

int Graphic3DMap::getResourceTotal(const Resources& resources) const
{
    return resources.food + resources.linemate + resources.deraumere +
        resources.sibur + resources.mendiane + resources.phiras +
        resources.thystame;
}

Color Graphic3DMap::getTileColor(const Tile& tile) const
{
    const int total = getResourceTotal(tile.resources);
    const int green = std::clamp(94 + total * 8, 94, 190);

    return {68, static_cast<unsigned char>(green), 104, 255};
}

void Graphic3DMap::drawResources(const Tile& tile, int width,
    int height) const
{
    struct ResourceMarker {
        int count;
        Vector3 offset;
        Color color;
    };
    const Vector3 center = Graphic3DUtils::getTileCenter(tile.x, tile.y,
        width, height);
    const std::array<ResourceMarker, 7> markers = {{
        {tile.resources.food, {-0.54f, 0.0f, -0.48f}, {239, 198, 92, 255}},
        {tile.resources.linemate, {-0.18f, 0.0f, -0.48f},
            {91, 176, 238, 255}},
        {tile.resources.deraumere, {0.18f, 0.0f, -0.48f},
            {155, 111, 232, 255}},
        {tile.resources.sibur, {0.54f, 0.0f, -0.48f},
            {242, 137, 91, 255}},
        {tile.resources.mendiane, {-0.36f, 0.0f, 0.28f},
            {99, 216, 169, 255}},
        {tile.resources.phiras, {0.0f, 0.0f, 0.28f},
            {231, 104, 137, 255}},
        {tile.resources.thystame, {0.36f, 0.0f, 0.28f},
            {245, 238, 120, 255}}
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

void Graphic3DMap::drawTiles(const GameWorld& world,
    const std::optional<std::pair<int, int>>& selectedTile) const
{
    const int width = world.getWidth();
    const int height = world.getHeight();
    const auto& map = world.getMap();

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            const Tile& tile = map[x][y];
            const Vector3 center = Graphic3DUtils::getTileCenter(x, y,
                width, height);

            DrawCube(center, Graphic3DConfig::TILE_SIZE,
                Graphic3DConfig::TILE_HEIGHT, Graphic3DConfig::TILE_SIZE,
                getTileColor(tile));
            DrawCubeWires(center, Graphic3DConfig::TILE_SIZE,
                Graphic3DConfig::TILE_HEIGHT + 0.01f,
                Graphic3DConfig::TILE_SIZE, {31, 40, 42, 255});
            if (selectedTile.has_value() && selectedTile->first == x &&
                selectedTile->second == y) {
                DrawCubeWires(center, Graphic3DConfig::TILE_SIZE + 0.08f,
                    Graphic3DConfig::TILE_HEIGHT + 0.08f,
                    Graphic3DConfig::TILE_SIZE + 0.08f,
                    {255, 221, 83, 255});
            }
            drawResources(tile, width, height);
        }
    }
}

void Graphic3DMap::drawEggs(const GameWorld& world) const
{
    const int width = std::max(1, world.getWidth());
    const int height = std::max(1, world.getHeight());

    for (const auto& [id, egg] : world.getEggs()) {
        (void)id;
        const Vector3 tile = Graphic3DUtils::getTileCenter(egg.x, egg.y,
            width, height);

        DrawSphere({
            tile.x + 0.52f,
            Graphic3DConfig::TILE_HEIGHT + 0.18f,
            tile.z + 0.52f
        }, 0.22f, {235, 232, 218, 255});
    }
}

void Graphic3DMap::draw(const GameWorld& world,
    const std::optional<std::pair<int, int>>& selectedTile) const
{
    if (world.getWidth() <= 0 || world.getHeight() <= 0 ||
        world.getMap().empty()) {
        drawPreview();
        return;
    }
    drawTiles(world, selectedTile);
    drawEggs(world);
}

void Graphic3DMap::drawPreview() const
{
    constexpr int width = 8;
    constexpr int height = 8;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            const Vector3 center = Graphic3DUtils::getTileCenter(x, y,
                width, height);
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

std::optional<std::pair<int, int>> Graphic3DMap::pickTile(
    const GameWorld& world, const Camera3D& camera, Vector2 mousePosition) const
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty())
        return std::nullopt;

    const Ray ray = GetScreenToWorldRay(mousePosition, camera);

    if (std::fabs(ray.direction.y) < TILE_PICK_EPSILON)
        return std::nullopt;

    const float planeY = Graphic3DConfig::TILE_HEIGHT;
    const float distance = (planeY - ray.position.y) / ray.direction.y;

    if (distance < 0.0f)
        return std::nullopt;

    const Vector3 hit = {
        ray.position.x + ray.direction.x * distance,
        planeY,
        ray.position.z + ray.direction.z * distance
    };
    const float step = Graphic3DConfig::TILE_SIZE + Graphic3DConfig::TILE_GAP;
    const float originX = -((static_cast<float>(mapWidth) - 1.0f) * step) /
        2.0f;
    const float originZ = -((static_cast<float>(mapHeight) - 1.0f) * step) /
        2.0f;
    const int tileX = static_cast<int>(std::round((hit.x - originX) / step));
    const int tileY = static_cast<int>(std::round((hit.z - originZ) / step));

    if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight)
        return std::nullopt;

    const float centerX = originX + static_cast<float>(tileX) * step;
    const float centerZ = originZ + static_cast<float>(tileY) * step;
    const float halfSize = Graphic3DConfig::TILE_SIZE / 2.0f;

    if (std::fabs(hit.x - centerX) > halfSize ||
        std::fabs(hit.z - centerZ) > halfSize)
        return std::nullopt;
    return std::make_pair(tileX, tileY);
}
