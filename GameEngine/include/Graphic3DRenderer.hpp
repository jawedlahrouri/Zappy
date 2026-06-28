/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DRenderer
*/

/**
 * @file Graphic3DRenderer.hpp
 * @brief Declares the composition layer for the Raylib 3D renderer.
 */

#ifndef GRAPHIC3DRENDERER_HPP_
    #define GRAPHIC3DRENDERER_HPP_

    #include "GameWorld.hpp"
    #include "Graphic3DInterface.hpp"
    #include "Graphic3DMap.hpp"
    #include "Graphic3DPlayers.hpp"
    #include <raylib.h>
    #include <optional>
    #include <string>
    #include <utility>
    #include <vector>

    /**
     * @brief Coordinates map, player, and overlay rendering for Raylib.
     */
    class Graphic3DRenderer {
        public:
            /**
             * @brief Draws the menu scene and overlay.
             */
            void drawMenu(const Camera3D& camera, const std::string& status,
                const std::vector<std::string>& teams) const;

            /**
             * @brief Draws the complete 3D world frame.
             */
            void drawWorld(const Camera3D& camera, const GameWorld& world,
                const std::optional<std::pair<int, int>>& selectedTile,
                const std::optional<int>& selectedPlayerId) const;

            /**
             * @brief Delegates player picking to the 3D player renderer.
             */
            std::optional<int> pickPlayer(const GameWorld& world,
                const Camera3D& camera, Vector2 mousePosition) const;

            /**
             * @brief Delegates tile picking to the 3D map renderer.
             */
            std::optional<std::pair<int, int>> pickTile(
                const GameWorld& world, const Camera3D& camera,
                Vector2 mousePosition) const;

            /**
             * @brief Checks whether coordinates are blocked by an info panel.
             */
            bool isInsideInfoPanel(
                const std::optional<std::pair<int, int>>& selectedTile,
                const std::optional<int>& selectedPlayerId, int mouseX,
                int mouseY) const;

        private:
            Graphic3DMap _map;
            Graphic3DPlayers _players;
            Graphic3DInterface _interface;
    };

#endif /* !GRAPHIC3DRENDERER_HPP_ */
