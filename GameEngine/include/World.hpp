/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** World
*/

/**
 * @file World.hpp
 * @brief Declares the 2D world renderer used by the SFML backend.
 */

#ifndef WORLD_HPP_
    #define WORLD_HPP_

#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"
#include <optional>
#include <string>
#include <utility>

    /**
     * @brief Draws the 2D map, entities, and HUD overlays.
     */
    class WorldRenderer {
        public:
            /** Pixel size of a logical Zappy tile. */
            static constexpr float TILE_SIZE = 64.0f;
        
            /**
             * @brief Draws one complete world frame.
             */
            static void draw(sf::RenderWindow& window, const sf::View& worldView, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const std::optional<int>& selectedPlayerId, const sf::Font& font, bool fontLoaded, bool showGrid, bool showBroadcasts, int serverFreq, bool timePaused, bool showInfoPanel, const std::optional<std::string>& openedTeamInventory, bool showLegend);
        private:
            /**
             * @brief Draws the world background.
             */
            static void drawBackground(sf::RenderWindow& window);

            /**
             * @brief Draws all tiles and tile resources.
             */
            static void drawTiles(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);

            /**
             * @brief Draws the optional tile grid.
             */
            static void drawGrid(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);

            /**
             * @brief Draws the highlighted selected tile, when applicable.
             */
            static void drawSelectedTile(sf::RenderWindow& window, int x, int y, float posX, float posY, const std::optional<std::pair<int, int>>& selectedTile);

            /**
             * @brief Draws eggs over the tile layer.
             */
            static void drawEggs(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawWinnerOverlay(sf::RenderWindow& window, const std::string& teamName, const sf::Font& font, bool fontLoaded);
        };

#endif /* !WORLD_HPP_ */
