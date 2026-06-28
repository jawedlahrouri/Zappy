/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DInterface
*/

/**
 * @file Graphic3DInterface.hpp
 * @brief Declares 2D overlay rendering for the Raylib 3D backend.
 */

#ifndef GRAPHIC3DINTERFACE_HPP_
    #define GRAPHIC3DINTERFACE_HPP_

    #include "GameWorld.hpp"
    #include <cstddef>
    #include <optional>
    #include <string>
    #include <utility>
    #include <vector>

/**
 * @brief Draws HUD, menu, and information panels over the 3D scene.
 */
class Graphic3DInterface {
    public:
        /**
         * @brief Draws the compact world HUD.
         */
        void drawHud(const GameWorld& world) const;

        /**
         * @brief Draws the Raylib menu overlay.
         */
        void drawMenu(const std::string& status,
            const std::vector<std::string>& teams) const;

        /**
         * @brief Draws the selected tile or player information panel.
         */
        void drawInfoPanel(const GameWorld& world,
            const std::optional<std::pair<int, int>>& selectedTile,
            const std::optional<int>& selectedPlayerId) const;

        /**
         * @brief Checks whether screen coordinates are inside an open info panel.
         */
        bool isInsideInfoPanel(
            const std::optional<std::pair<int, int>>& selectedTile,
            const std::optional<int>& selectedPlayerId, int mouseX,
            int mouseY) const;

    private:
        /** Width of the tile information panel. */
        static constexpr int TILE_PANEL_WIDTH = 280;
        /** Height of the tile information panel. */
        static constexpr int TILE_PANEL_HEIGHT = 268;
        /** Width of the player information panel. */
        static constexpr int PLAYER_PANEL_WIDTH = 330;
        /** Height of the player information panel. */
        static constexpr int PLAYER_PANEL_HEIGHT = 380;
        /** Screen margin used by 3D overlay panels. */
        static constexpr int PANEL_MARGIN = 20;
        /** Top coordinate used by 3D overlay panels. */
        static constexpr int PANEL_Y = 74;

        /**
         * @brief Draws information for the selected tile.
         */
        void drawTileInfoPanel(const GameWorld& world,
            const std::optional<std::pair<int, int>>& selectedTile) const;

        /**
         * @brief Draws information for the selected player.
         */
        void drawPlayerInfoPanel(const GameWorld& world,
            const std::optional<int>& selectedPlayerId) const;

        /**
         * @brief Draws all team names in the Raylib menu.
         */
        void drawMenuTeams(const std::vector<std::string>& teams) const;

        /**
         * @brief Draws a single team row in the Raylib menu.
         */
        void drawMenuTeam(const std::string& teamName, std::size_t index,
            float x, float y) const;

        /**
         * @brief Draws a numeric row inside a panel.
         */
        void drawPanelLine(const std::string& label, int value, int row,
            int panelX, int panelY) const;

        /**
         * @brief Draws a text row inside a panel.
         */
        void drawTextPanelLine(const std::string& value, int row, int size,
            int panelX, int panelY) const;
};

#endif /* !GRAPHIC3DINTERFACE_HPP_ */
