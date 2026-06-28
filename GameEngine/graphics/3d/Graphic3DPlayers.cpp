/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DPlayers
*/

#include "Graphic3DPlayers.hpp"
#include "Graphic3DConfig.hpp"
#include "Graphic3DUtils.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

Vector3 Graphic3DPlayers::getDirection(int orientation) const
{
    if (orientation == 2)
        return {1.0f, 0.0f, 0.0f};
    if (orientation == 3)
        return {0.0f, 0.0f, 1.0f};
    if (orientation == 4)
        return {-1.0f, 0.0f, 0.0f};
    return {0.0f, 0.0f, -1.0f};
}

void Graphic3DPlayers::drawPlayer(const Player& player, const Vector3& tile,
    bool selected) const
{
    const Color color = Graphic3DUtils::getPlayerColor(player.teamColor);
    const Vector3 body = {
        tile.x,
        Graphic3DConfig::TILE_HEIGHT + 0.36f,
        tile.z
    };
    const Vector3 direction = getDirection(player.orientation);

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

    if (!selected)
        return;
    DrawCylinderWires(body, 0.32f, 0.36f, 0.78f, 16,
        {255, 221, 83, 255});
    DrawCircle3D({
        tile.x,
        Graphic3DConfig::TILE_HEIGHT + 0.02f,
        tile.z
    }, 0.52f, {1.0f, 0.0f, 0.0f}, 90.0f, {255, 221, 83, 210});
}

void Graphic3DPlayers::draw(const GameWorld& world,
    const std::optional<int>& selectedPlayerId) const
{
    const int width = std::max(1, world.getWidth());
    const int height = std::max(1, world.getHeight());

    for (const auto& [id, player] : world.getPlayers()) {
        const Vector3 tile = Graphic3DUtils::getTileCenter(player.x, player.y,
            width, height);
        const bool selected = selectedPlayerId.has_value() &&
            *selectedPlayerId == player.id;

        (void)id;
        drawPlayer(player, tile, selected);
    }
}

std::optional<int> Graphic3DPlayers::pickPlayer(const GameWorld& world,
    const Camera3D& camera, Vector2 mousePosition) const
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();
    int selectedPlayerId = 0;
    float closestDistance = std::numeric_limits<float>::max();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getPlayers().empty())
        return std::nullopt;

    for (const auto& [id, player] : world.getPlayers()) {
        const Vector3 tile = Graphic3DUtils::getTileCenter(player.x, player.y,
            mapWidth, mapHeight);
        const Vector3 bodyCenter = {
            tile.x,
            Graphic3DConfig::TILE_HEIGHT + 0.52f,
            tile.z
        };
        const Vector3 bodyTop = {
            tile.x,
            Graphic3DConfig::TILE_HEIGHT + 1.06f,
            tile.z
        };
        const Vector2 screenCenter = GetWorldToScreen(bodyCenter, camera);
        const Vector2 screenTop = GetWorldToScreen(bodyTop, camera);
        const float dx = mousePosition.x - screenCenter.x;
        const float dy = mousePosition.y - screenCenter.y;
        const float distance = dx * dx + dy * dy;
        const float pickRadius = std::clamp(
            std::fabs(screenCenter.y - screenTop.y) + 12.0f, 18.0f, 56.0f);

        (void)id;
        if (std::fabs(dx) > pickRadius || std::fabs(dy) > pickRadius)
            continue;
        if (distance >= closestDistance)
            continue;
        closestDistance = distance;
        selectedPlayerId = player.id;
    }
    if (closestDistance == std::numeric_limits<float>::max())
        return std::nullopt;
    return selectedPlayerId;
}
