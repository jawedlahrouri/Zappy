/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DInterface
*/

#ifndef GRAPHIC3DINTERFACE_HPP_
    #define GRAPHIC3DINTERFACE_HPP_

    #include "GameWorld.hpp"
    #include <cstddef>
    #include <optional>
    #include <string>
    #include <utility>
    #include <vector>

class Graphic3DInterface {
    public:
        void drawHud(const GameWorld& world) const;
        void drawMenu(const std::string& status,
            const std::vector<std::string>& teams) const;
        void drawInfoPanel(const GameWorld& world,
            const std::optional<std::pair<int, int>>& selectedTile,
            const std::optional<int>& selectedPlayerId) const;
        bool isInsideInfoPanel(
            const std::optional<std::pair<int, int>>& selectedTile,
            const std::optional<int>& selectedPlayerId, int mouseX,
            int mouseY) const;

    private:
        static constexpr int TILE_PANEL_WIDTH = 280;
        static constexpr int TILE_PANEL_HEIGHT = 268;
        static constexpr int PLAYER_PANEL_WIDTH = 330;
        static constexpr int PLAYER_PANEL_HEIGHT = 380;
        static constexpr int PANEL_MARGIN = 20;
        static constexpr int PANEL_Y = 74;

        void drawTileInfoPanel(const GameWorld& world,
            const std::optional<std::pair<int, int>>& selectedTile) const;
        void drawPlayerInfoPanel(const GameWorld& world,
            const std::optional<int>& selectedPlayerId) const;
        void drawMenuTeams(const std::vector<std::string>& teams) const;
        void drawMenuTeam(const std::string& teamName, std::size_t index,
            float x, float y) const;
        void drawPanelLine(const std::string& label, int value, int row,
            int panelX, int panelY) const;
        void drawTextPanelLine(const std::string& value, int row, int size,
            int panelX, int panelY) const;
};

#endif /* !GRAPHIC3DINTERFACE_HPP_ */
