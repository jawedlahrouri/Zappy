/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DInterface
*/

#include "Graphic3DInterface.hpp"
#include "Graphic3DUtils.hpp"
#include <raylib.h>
#include <sstream>

void Graphic3DInterface::drawHud(const GameWorld& world) const
{
    std::ostringstream status;

    status << "Zappy 3D  |  map " << world.getWidth() << "x"
        << world.getHeight() << "  |  players " << world.getPlayers().size();
    DrawRectangle(0, 0, GetScreenWidth(), 54, {9, 12, 17, 210});
    DrawText(status.str().c_str(), 18, 14, 20, RAYWHITE);
    DrawText("Left click: inspect player/tile  |  Mouse wheel: zoom  |  ZQSD/right drag: rotate  |  arrows: move  |  R: reset",
        18, 34, 16, {190, 204, 210, 255});
}

void Graphic3DInterface::drawTextPanelLine(const std::string& value, int row,
    int size, int panelX, int panelY) const
{
    DrawText(value.c_str(), panelX + 18, panelY + 18 + row * 23, size,
        {226, 234, 238, 255});
}

void Graphic3DInterface::drawPanelLine(const std::string& label, int value,
    int row, int panelX, int panelY) const
{
    std::ostringstream line;

    line << label << ": " << value;
    DrawText(line.str().c_str(), panelX + 18, panelY + 74 + row * 22, 16,
        {226, 234, 238, 255});
}

bool Graphic3DInterface::isInsideInfoPanel(
    const std::optional<std::pair<int, int>>& selectedTile,
    const std::optional<int>& selectedPlayerId, int mouseX, int mouseY) const
{
    if (!selectedTile.has_value() && !selectedPlayerId.has_value())
        return false;

    const int panelWidth = selectedPlayerId.has_value() ?
        PLAYER_PANEL_WIDTH : TILE_PANEL_WIDTH;
    const int panelHeight = selectedPlayerId.has_value() ?
        PLAYER_PANEL_HEIGHT : TILE_PANEL_HEIGHT;
    const int panelX = GetScreenWidth() - panelWidth - PANEL_MARGIN;

    return mouseX >= panelX && mouseX <= panelX + panelWidth &&
        mouseY >= PANEL_Y && mouseY <= PANEL_Y + panelHeight;
}

void Graphic3DInterface::drawTileInfoPanel(const GameWorld& world,
    const std::optional<std::pair<int, int>>& selectedTile) const
{
    if (!selectedTile.has_value() || world.getMap().empty())
        return;

    const int x = selectedTile->first;
    const int y = selectedTile->second;

    if (x < 0 || y < 0 || x >= world.getWidth() || y >= world.getHeight())
        return;

    const Resources& resources = world.getMap()[x][y].resources;
    const int panelX = GetScreenWidth() - TILE_PANEL_WIDTH - PANEL_MARGIN;
    std::ostringstream title;
    std::ostringstream coords;

    title << "Case " << x << " " << y;
    coords << "coords: x=" << x << "  y=" << y;
    DrawRectangle(panelX, PANEL_Y, TILE_PANEL_WIDTH, TILE_PANEL_HEIGHT,
        {18, 24, 30, 228});
    DrawRectangleLines(panelX, PANEL_Y, TILE_PANEL_WIDTH, TILE_PANEL_HEIGHT,
        {255, 221, 83, 255});
    DrawText(title.str().c_str(), panelX + 18, PANEL_Y + 16, 22, RAYWHITE);
    DrawText(coords.str().c_str(), panelX + 18, PANEL_Y + 44, 16,
        {190, 204, 210, 255});
    drawPanelLine("food", resources.food, 0, panelX, PANEL_Y);
    drawPanelLine("brick", resources.linemate, 1, panelX, PANEL_Y);
    drawPanelLine("shoe", resources.deraumere, 2, panelX, PANEL_Y);
    drawPanelLine("balloon", resources.sibur, 3, panelX, PANEL_Y);
    drawPanelLine("jersey", resources.mendiane, 4, panelX, PANEL_Y);
    drawPanelLine("trophy", resources.phiras, 5, panelX, PANEL_Y);
    drawPanelLine("card", resources.thystame, 6, panelX, PANEL_Y);
}

void Graphic3DInterface::drawPlayerInfoPanel(const GameWorld& world,
    const std::optional<int>& selectedPlayerId) const
{
    if (!selectedPlayerId.has_value())
        return;

    const auto& players = world.getPlayers();
    const auto playerIt = players.find(*selectedPlayerId);

    if (playerIt == players.end())
        return;

    const Player& player = playerIt->second;
    const int panelX = GetScreenWidth() - PLAYER_PANEL_WIDTH - PANEL_MARGIN;
    std::ostringstream title;
    std::ostringstream team;
    std::ostringstream level;
    std::ostringstream position;
    std::ostringstream orientation;

    title << "Player #" << player.id;
    team << "team: " << player.teamName;
    level << "level: " << player.level;
    position << "position: (" << player.x << ", " << player.y << ")";
    orientation << "orientation: " <<
        Graphic3DUtils::orientationToString(player.orientation);
    DrawRectangle(panelX, PANEL_Y, PLAYER_PANEL_WIDTH, PLAYER_PANEL_HEIGHT,
        {18, 24, 30, 228});
    DrawRectangleLines(panelX, PANEL_Y, PLAYER_PANEL_WIDTH,
        PLAYER_PANEL_HEIGHT, {255, 221, 83, 255});
    DrawCircle(panelX + PLAYER_PANEL_WIDTH - 62, PANEL_Y + 66, 34,
        Graphic3DUtils::getPlayerColor(player.teamColor));
    DrawCircle(panelX + PLAYER_PANEL_WIDTH - 62, PANEL_Y + 31, 19, RAYWHITE);
    drawTextPanelLine(title.str(), 0, 22, panelX, PANEL_Y);
    drawTextPanelLine(team.str(), 1, 17, panelX, PANEL_Y);
    drawTextPanelLine(level.str(), 2, 17, panelX, PANEL_Y);
    drawTextPanelLine(position.str(), 3, 17, panelX, PANEL_Y);
    drawTextPanelLine(orientation.str(), 4, 17, panelX, PANEL_Y);
    drawTextPanelLine("inventory", 6, 18, panelX, PANEL_Y);
    drawPanelLine("food", player.inventory.food, 7, panelX, PANEL_Y - 12);
    drawPanelLine("brick", player.inventory.linemate, 8, panelX, PANEL_Y - 12);
    drawPanelLine("shoe", player.inventory.deraumere, 9, panelX, PANEL_Y - 12);
    drawPanelLine("balloon", player.inventory.sibur, 10, panelX,
        PANEL_Y - 12);
    drawPanelLine("jersey", player.inventory.mendiane, 11, panelX,
        PANEL_Y - 12);
    drawPanelLine("trophy", player.inventory.phiras, 12, panelX,
        PANEL_Y - 12);
    drawPanelLine("card", player.inventory.thystame, 13, panelX,
        PANEL_Y - 12);
}

void Graphic3DInterface::drawInfoPanel(const GameWorld& world,
    const std::optional<std::pair<int, int>>& selectedTile,
    const std::optional<int>& selectedPlayerId) const
{
    if (selectedPlayerId.has_value()) {
        drawPlayerInfoPanel(world, selectedPlayerId);
        return;
    }
    drawTileInfoPanel(world, selectedTile);
}

void Graphic3DInterface::drawMenuTeam(const std::string& teamName,
    std::size_t index, float x, float y) const
{
    const int teamColor = static_cast<int>(index % 4) + 1;
    const Color color = Graphic3DUtils::getPlayerColor(teamColor);
    const int textWidth = MeasureText(teamName.c_str(), 22);

    DrawCircle(static_cast<int>(x), static_cast<int>(y - 4.0f), 22, color);
    DrawCircle(static_cast<int>(x), static_cast<int>(y - 28.0f), 13, RAYWHITE);
    DrawRectangle(static_cast<int>(x - 18.0f), static_cast<int>(y + 9.0f),
        36, 18, color);
    DrawRectangle(static_cast<int>(x + 16.0f), static_cast<int>(y - 6.0f),
        16, 8, BLACK);
    DrawText(teamName.c_str(), static_cast<int>(x - textWidth / 2.0f),
        static_cast<int>(y - 68.0f), 22, RAYWHITE);
}

void Graphic3DInterface::drawMenuTeams(
    const std::vector<std::string>& teams) const
{
    if (teams.empty()) {
        const char* waiting = "Waiting for teams...";
        const int textWidth = MeasureText(waiting, 22);

        DrawText(waiting, GetScreenWidth() / 2 - textWidth / 2,
            GetScreenHeight() - 122, 22, {183, 205, 205, 255});
        return;
    }

    const int columns = teams.size() <= 4 ? static_cast<int>(teams.size()) : 4;
    const int rows = static_cast<int>((teams.size() + 3) / 4);
    const float spacingX = 170.0f;
    const float spacingY = 118.0f;
    const float startX = static_cast<float>(GetScreenWidth()) / 2.0f -
        static_cast<float>(columns - 1) * spacingX / 2.0f;
    const float startY = static_cast<float>(GetScreenHeight()) - 170.0f -
        static_cast<float>(rows - 1) * spacingY;

    for (std::size_t index = 0; index < teams.size(); index++) {
        const float x = startX + static_cast<float>(index % 4) * spacingX;
        const float y = startY + static_cast<float>(index / 4) * spacingY;

        drawMenuTeam(teams[index], index, x, y);
    }
}

void Graphic3DInterface::drawMenu(const std::string& status,
    const std::vector<std::string>& teams) const
{
    DrawRectangle(0, 0, GetScreenWidth(), 90, {9, 12, 17, 218});
    DrawText("Zappy 3D", 24, 18, 34, RAYWHITE);
    DrawText(status.c_str(), 26, 58, 18, {183, 205, 205, 255});
    drawMenuTeams(teams);
}
