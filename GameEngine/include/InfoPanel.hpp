/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** InfoPanel
*/

#ifndef INFO_PANEL_HPP_
    #define INFO_PANEL_HPP_

#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"
#include "Players.hpp"
#include "TextureManager.hpp"
#include <algorithm>
#include <array>
#include <map>
#include <optional>
#include <string>
#include <utility>

    class InfoPanelRenderer {
        public:
            static constexpr float INFO_PANEL_WIDTH = 270.0f;
            static constexpr float INFO_PANEL_HEIGHT = 250.0f;
            static constexpr float INFO_PANEL_MARGIN = 20.0f;
            static constexpr float GLOBAL_PANEL_WIDTH = 390.0f;
            static constexpr float GLOBAL_PANEL_HEIGHT = 720.0f;
            static constexpr float PLAYER_PANEL_WIDTH = 330.0f;
            static constexpr float PLAYER_PANEL_HEIGHT = 380.0f;
            static constexpr float TIME_PANEL_WIDTH = 280.0f;
            static constexpr float TIME_PANEL_HEIGHT = 140.0f;
            static constexpr float TIME_PANEL_MARGIN = 20.0f;
            static constexpr float TIME_BUTTON_WIDTH = 52.0f;
            static constexpr float TIME_BUTTON_HEIGHT = 52.0f;

            static void drawGlobalInfoPanel(sf::RenderWindow& window, const GameWorld& world, const sf::Font& font, bool fontLoaded, int serverFreq, bool timePaused, const std::optional<std::string>& openedTeamInventory);
            static void drawTimePanel(sf::RenderWindow& window, const sf::Font& font, bool fontLoaded, int serverFreq, bool timePaused);
            static void drawInfoPanel(sf::RenderWindow& window, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const std::optional<int>& selectedPlayerId, const sf::Font& font, bool fontLoaded);
            static void drawLegend(sf::RenderWindow& window,
                const sf::Font& font, bool fontLoaded, bool showLegend);
            static sf::FloatRect getTimePanelRect(const sf::Vector2u& windowSize);
            static sf::FloatRect getInfoButtonRect(
                const sf::Vector2u& windowSize);
            static sf::FloatRect getLegendPanelRect(
                const sf::Vector2u& windowSize);
            static bool isInsideLegendUi(const sf::Vector2u& windowSize,
                int mouseX, int mouseY, bool showLegend);
            static std::optional<std::string> getTeamInventoryAt(
                const GameWorld& world,
                const std::optional<std::string>& openedTeamInventory,
                int mouseX, int mouseY);

        private:
            static constexpr int MAX_GLOBAL_INFO_ROW = 31;
            static constexpr int MAX_PLAYER_LEVEL = 8;

            static void addResources(Resources& total, const Resources& resources);
            static int countResourceItems(const Resources& resources);
            static int countHatchedEggs(const GameWorld& world);
            static int drawTeamSharedInventories(sf::RenderWindow& window,
                sf::Text& text, const std::map<std::string, int>& teamCounts,
                const std::map<std::string, Resources>& teamResources,
                const std::map<std::string,
                    std::array<int, MAX_PLAYER_LEVEL + 1>>& teamLevelCounts,
                const std::optional<std::string>& openedTeamInventory, int row,
                float panelX, float panelY);
            static void drawTeamInventoryDetails(sf::RenderWindow& window,
                sf::Text& text, const Resources& resources, int row,
                float panelX, float panelY);
            static void drawTeamLevelGrid(sf::RenderWindow& window,
                sf::Text& text,
                const std::array<int, MAX_PLAYER_LEVEL + 1>& levelCounts,
                int row, float panelX, float panelY);
            static void drawTimeButton(sf::RenderWindow& window,
                sf::Text& text, const std::string& fallbackLabel,
                const std::string& textureName, const sf::FloatRect& rect,
                bool active);
            static void drawLegendItem(sf::RenderWindow& window,
                sf::Text& text, const std::string& textureName,
                const std::string& label, int row, float panelX, float panelY);
            static void drawInfoLine(sf::RenderWindow& window, sf::Text& text, const std::string& value, int row, unsigned int size, float panelX, float panelY);
    };

#endif /* !INFO_PANEL_HPP_ */
