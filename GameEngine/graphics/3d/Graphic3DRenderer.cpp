/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DRenderer
*/

#include "Graphic3DRenderer.hpp"

void Graphic3DRenderer::drawMenu(const Camera3D& camera,
    const std::string& status, const std::vector<std::string>& teams) const
{
    BeginMode3D(camera);
    _map.drawPreview();
    EndMode3D();
    _interface.drawMenu(status, teams);
}

void Graphic3DRenderer::drawWorld(const Camera3D& camera,
    const GameWorld& world,
    const std::optional<std::pair<int, int>>& selectedTile,
    const std::optional<int>& selectedPlayerId) const
{
    BeginMode3D(camera);
    _map.draw(world, selectedTile);
    _players.draw(world, selectedPlayerId);
    EndMode3D();
    _interface.drawHud(world);
    _interface.drawInfoPanel(world, selectedTile, selectedPlayerId);
}

std::optional<int> Graphic3DRenderer::pickPlayer(const GameWorld& world,
    const Camera3D& camera, Vector2 mousePosition) const
{
    return _players.pickPlayer(world, camera, mousePosition);
}

std::optional<std::pair<int, int>> Graphic3DRenderer::pickTile(
    const GameWorld& world, const Camera3D& camera, Vector2 mousePosition) const
{
    return _map.pickTile(world, camera, mousePosition);
}

bool Graphic3DRenderer::isInsideInfoPanel(
    const std::optional<std::pair<int, int>>& selectedTile,
    const std::optional<int>& selectedPlayerId, int mouseX, int mouseY) const
{
    return _interface.isInsideInfoPanel(selectedTile, selectedPlayerId,
        mouseX, mouseY);
}
