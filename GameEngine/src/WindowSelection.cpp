/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowSelection
*/

#include "WindowSelection.hpp"

bool WindowSelection::selectTile(WindowContext& context, const GameWorld& world,
    int mouseX, int mouseY)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty())
        return false;
    if (isInsidePanel(context, mouseX, mouseY))
        return false;

    const sf::Vector2u windowSize = context.window.getSize();
    const sf::Vector2f worldPos = context.window.mapPixelToCoords({mouseX, mouseY}, context.view.world);
    const float totalMapWidth = static_cast<float>(mapWidth) * WorldRenderer::TILE_SIZE;
    const float totalMapHeight = static_cast<float>(mapHeight) * WorldRenderer::TILE_SIZE;
    const float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) / 2.0f;
    const float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) / 2.0f;
    const int tileX = static_cast<int>(std::floor((worldPos.x - offsetX) / WorldRenderer::TILE_SIZE));
    const int tileY = static_cast<int>(std::floor((worldPos.y - offsetY) / WorldRenderer::TILE_SIZE));

    if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight)
        return false;
    context.selection.tile = {tileX, tileY};
    context.selection.playerId.reset();
    context.selection.pendingTileRequest = context.selection.tile;
    return true;
}

bool WindowSelection::selectPlayer(WindowContext& context,
    const GameWorld& world, int mouseX, int mouseY)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty())
        return false;
    if (isInsidePanel(context, mouseX, mouseY))
        return false;

    const sf::Vector2u windowSize = context.window.getSize();
    const sf::Vector2f worldPos = context.window.mapPixelToCoords({mouseX, mouseY}, context.view.world);
    const float totalMapWidth = static_cast<float>(mapWidth) * WorldRenderer::TILE_SIZE;
    const float totalMapHeight = static_cast<float>(mapHeight) * WorldRenderer::TILE_SIZE;
    const float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) / 2.0f;
    const float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) / 2.0f;
    int selectedPlayerId = 0;

    if (!Players::pickPlayer(world, worldPos, offsetX, offsetY, selectedPlayerId))
        return false;
    const auto& players = world.getPlayers();
    const auto it = players.find(selectedPlayerId);

    if (it == players.end())
        return false;
    context.selection.playerId = selectedPlayerId;
    context.selection.tile = {it->second.x, it->second.y};
    context.selection.pendingTileRequest.reset();
    return true;
}

bool WindowSelection::isInsidePanel(const WindowContext& context, int mouseX, int mouseY)
{
    if (!context.selection.playerId.has_value() && !context.selection.tile.has_value())
        return false;

    const sf::Vector2u windowSize = context.window.getSize();
    const float panelX = static_cast<float>(windowSize.x) -
        (context.selection.playerId.has_value() ?
        InfoPanelRenderer::PLAYER_PANEL_WIDTH :
        InfoPanelRenderer::INFO_PANEL_WIDTH) -
        InfoPanelRenderer::INFO_PANEL_MARGIN;
    const float panelY = InfoPanelRenderer::INFO_PANEL_MARGIN;
    const float panelWidth = context.selection.playerId.has_value() ?
        InfoPanelRenderer::PLAYER_PANEL_WIDTH :
        InfoPanelRenderer::INFO_PANEL_WIDTH;
    const float panelHeight = context.selection.playerId.has_value() ?
        InfoPanelRenderer::PLAYER_PANEL_HEIGHT :
        InfoPanelRenderer::INFO_PANEL_HEIGHT;

    return static_cast<float>(mouseX) >= panelX &&
        static_cast<float>(mouseX) <= panelX + panelWidth &&
        static_cast<float>(mouseY) >= panelY &&
        static_cast<float>(mouseY) <= panelY + panelHeight;
}

void WindowSelection::clear(WindowContext& context)
{
    context.selection.tile.reset();
    context.selection.playerId.reset();
    context.selection.pendingTileRequest.reset();
}

std::optional<std::pair<int, int>> WindowSelection::consumeTileRequest(WindowContext& context)
{
    std::optional<std::pair<int, int>> request = context.selection.pendingTileRequest;

    context.selection.pendingTileRequest.reset();
    return request;
}
