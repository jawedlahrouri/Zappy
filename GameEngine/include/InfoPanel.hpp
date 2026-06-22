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
#include <map>
#include <optional>
#include <string>
#include <utility>

    class InfoPanelRenderer {
        public:
            static constexpr float INFO_PANEL_WIDTH = 270.0f;
            static constexpr float INFO_PANEL_HEIGHT = 250.0f;
            static constexpr float INFO_PANEL_MARGIN = 20.0f;
            static constexpr float GLOBAL_PANEL_WIDTH = 320.0f;
            static constexpr float GLOBAL_PANEL_HEIGHT = 520.0f;
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
            static sf::FloatRect getTimePanelRect(const sf::Vector2u& windowSize);
            static std::optional<std::string> getTeamInventoryAt(
                const GameWorld& world,
                const std::optional<std::string>& openedTeamInventory,
                int mouseX, int mouseY);

        private:
            static constexpr int MAX_GLOBAL_INFO_ROW = 22;

            static void addResources(Resources& total, const Resources& resources);
            static int countResourceItems(const Resources& resources);
            static int countHatchedEggs(const GameWorld& world);
            static int drawTeamSharedInventories(sf::RenderWindow& window,
                sf::Text& text, const std::map<std::string, int>& teamCounts,
                const std::map<std::string, Resources>& teamResources,
                const std::optional<std::string>& openedTeamInventory, int row,
                float panelX, float panelY);
            static void drawTeamInventoryDetails(sf::RenderWindow& window,
                sf::Text& text, const Resources& resources, int row,
                float panelX, float panelY);
            static void drawTimeButton(sf::RenderWindow& window,
                sf::Text& text, const std::string& fallbackLabel,
                const std::string& textureName, const sf::FloatRect& rect,
                bool active);
            static void drawInfoLine(sf::RenderWindow& window, sf::Text& text, const std::string& value, int row, unsigned int size, float panelX, float panelY);
    };

#endif /* !INFO_PANEL_HPP_ */
