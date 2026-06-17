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
#include "Players.hpp"
#include <optional>
#include <string>
#include <utility>

    class WorldRenderer {
        public:
            static constexpr float TILE_SIZE = 64.0f;
            static constexpr float INFO_PANEL_WIDTH = 270.0f;
            static constexpr float INFO_PANEL_HEIGHT = 250.0f;
            static constexpr float INFO_PANEL_MARGIN = 20.0f;
            static constexpr float PLAYER_PANEL_WIDTH = 330.0f;
            static constexpr float PLAYER_PANEL_HEIGHT = 380.0f;
            static constexpr float PORTRAIT_SIZE = 96.0f;
            static constexpr float TIME_PANEL_WIDTH = 280.0f;
            static constexpr float TIME_PANEL_HEIGHT = 140.0f;
            static constexpr float TIME_PANEL_MARGIN = 20.0f;
            static constexpr float TIME_BUTTON_WIDTH = 74.0f;
            static constexpr float TIME_BUTTON_HEIGHT = 30.0f;
        
            static void draw(sf::RenderWindow& window, const sf::View& worldView, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const std::optional<int>& selectedPlayerId, const sf::Font& font, bool fontLoaded, bool showGrid, int serverFreq, bool timePaused);
        private:
            static void drawBackground(sf::RenderWindow& window);
            static void drawTiles(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawGrid(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawSelectedTile(sf::RenderWindow& window, int x, int y, float posX, float posY, const std::optional<std::pair<int, int>>& selectedTile);
            static void drawEggs(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY);
            static void drawTimePanel(sf::RenderWindow& window, const sf::Font& font, bool fontLoaded, int serverFreq, bool timePaused);
            static void drawTimeButton(sf::RenderWindow& window, sf::Text& text, const std::string& label, const sf::FloatRect& rect, bool active);
            static void drawInfoPanel(sf::RenderWindow& window, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const std::optional<int>& selectedPlayerId, const sf::Font& font, bool fontLoaded);
            static void drawInfoLine(sf::RenderWindow& window, sf::Text& text, const std::string& value, int row, unsigned int size, float panelX, float panelY);
        };

#endif /* !WORLD_HPP_ */
