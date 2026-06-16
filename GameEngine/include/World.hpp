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
            static constexpr float INFO_PANEL_WIDTH = 270.0f;
            static constexpr float INFO_PANEL_HEIGHT = 250.0f;
            static constexpr float INFO_PANEL_MARGIN = 20.0f;
        
            static void draw(sf::RenderWindow& window, const sf::View& worldView, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const sf::Font& font, bool fontLoaded, bool showGrid);
        private:
            static void drawTiles(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawGrid(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawSelectedTile(sf::RenderWindow& window, int x, int y, float posX, float posY, const std::optional<std::pair<int, int>>& selectedTile);
            static void drawPlayers(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawEggs(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawInfoPanel(sf::RenderWindow& window, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const sf::Font& font, bool fontLoaded);
            static void drawInfoLine(sf::RenderWindow& window, sf::Text& text, const std::string& value, int row, unsigned int size, float panelX, float panelY);
        };

#endif /* !WORLD_HPP_ */
