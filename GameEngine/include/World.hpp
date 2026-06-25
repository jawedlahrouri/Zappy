/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** World
*/

#ifndef WORLD_HPP_
    #define WORLD_HPP_

#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"
#include <optional>
#include <string>
#include <utility>

    class WorldRenderer {
        public:
            static constexpr float TILE_SIZE = 64.0f;
        
            static void draw(sf::RenderWindow& window, const sf::View& worldView, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const std::optional<int>& selectedPlayerId, const sf::Font& font, bool fontLoaded, bool showGrid, bool showBroadcasts, int serverFreq, bool timePaused, bool showInfoPanel, const std::optional<std::string>& openedTeamInventory, bool showLegend);
        private:
            static void drawBackground(sf::RenderWindow& window);
            static void drawTiles(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawGrid(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawSelectedTile(sf::RenderWindow& window, int x, int y, float posX, float posY, const std::optional<std::pair<int, int>>& selectedTile);
            static void drawEggs(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
        };

#endif /* !WORLD_HPP_ */
