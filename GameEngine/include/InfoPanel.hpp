/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** InfoPanel
*/

/**
 * @file InfoPanel.hpp
 * @brief Declares HUD and information-panel rendering utilities.
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

    /**
     * @brief Draws informational overlays for the SFML world renderer.
     */
    class InfoPanelRenderer {
        public:
            /** Width of the selected-tile panel in pixels. */
            static constexpr float INFO_PANEL_WIDTH = 270.0f;
            /** Height of the selected-tile panel in pixels. */
            static constexpr float INFO_PANEL_HEIGHT = 250.0f;
            /** Margin used around floating information panels. */
            static constexpr float INFO_PANEL_MARGIN = 20.0f;
            /** Width of the global map/team panel in pixels. */
            static constexpr float GLOBAL_PANEL_WIDTH = 390.0f;
            /** Height of the global map/team panel in pixels. */
            static constexpr float GLOBAL_PANEL_HEIGHT = 720.0f;
            /** Width of the selected-player panel in pixels. */
            static constexpr float PLAYER_PANEL_WIDTH = 330.0f;
            /** Height of the selected-player panel in pixels. */
            static constexpr float PLAYER_PANEL_HEIGHT = 380.0f;
            /** Width of the time-control panel in pixels. */
            static constexpr float TIME_PANEL_WIDTH = 280.0f;
            /** Height of the time-control panel in pixels. */
            static constexpr float TIME_PANEL_HEIGHT = 140.0f;
            /** Margin used by the time-control panel. */
            static constexpr float TIME_PANEL_MARGIN = 20.0f;
            /** Width of each time-control button. */
            static constexpr float TIME_BUTTON_WIDTH = 52.0f;
            /** Height of each time-control button. */
            static constexpr float TIME_BUTTON_HEIGHT = 52.0f;

            /**
             * @brief Draws global map statistics and shared team inventories.
             */
            static void drawGlobalInfoPanel(sf::RenderWindow& window, const GameWorld& world, const sf::Font& font, bool fontLoaded, int serverFreq, bool timePaused, const std::optional<std::string>& openedTeamInventory);

            /**
             * @brief Draws pause, slower, faster, and frequency controls.
             */
            static void drawTimePanel(sf::RenderWindow& window, const sf::Font& font, bool fontLoaded, int serverFreq, bool timePaused);

            /**
             * @brief Draws the selected tile or selected player panel.
             */
            static void drawInfoPanel(sf::RenderWindow& window, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const std::optional<int>& selectedPlayerId, const sf::Font& font, bool fontLoaded);

            /**
             * @brief Draws the map legend button and optional expanded legend.
             */
            static void drawLegend(sf::RenderWindow& window,
                const sf::Font& font, bool fontLoaded, bool showLegend);

            /**
             * @brief Computes the time-control panel hitbox for a window size.
             */
            static sf::FloatRect getTimePanelRect(const sf::Vector2u& windowSize);

            /**
             * @brief Computes the global-info toggle hitbox.
             */
            static sf::FloatRect getInfoButtonRect(
                const sf::Vector2u& windowSize);

            /**
             * @brief Computes the legend panel hitbox.
             */
            static sf::FloatRect getLegendPanelRect(
                const sf::Vector2u& windowSize);

            /**
             * @brief Checks whether coordinates are inside legend UI.
             */
            static bool isInsideLegendUi(const sf::Vector2u& windowSize,
                int mouseX, int mouseY, bool showLegend);

            /**
             * @brief Returns the team inventory row hit by the mouse.
             */
            static std::optional<std::string> getTeamInventoryAt(
                const GameWorld& world,
                const std::optional<std::string>& openedTeamInventory,
                int mouseX, int mouseY);

        private:
            /** Maximum row index available in the global information panel. */
            static constexpr int MAX_GLOBAL_INFO_ROW = 31;
            /** Highest Zappy player level displayed by level grids. */
            static constexpr int MAX_PLAYER_LEVEL = 8;

            /**
             * @brief Adds resource counts into an aggregate.
             */
            static void addResources(Resources& total, const Resources& resources);

            /**
             * @brief Counts all resource items stored in a Resources structure.
             */
            static int countResourceItems(const Resources& resources);

            /**
             * @brief Draws compact shared inventories for each team.
             * @return Next row index after the rendered block.
             */
            static int drawTeamSharedInventories(sf::RenderWindow& window,
                sf::Text& text, const std::map<std::string, int>& teamCounts,
                const std::map<std::string, Resources>& teamResources,
                const std::map<std::string,
                    std::array<int, MAX_PLAYER_LEVEL + 1>>& teamLevelCounts,
                const std::optional<std::string>& openedTeamInventory, int row,
                float panelX, float panelY);

            /**
             * @brief Draws detailed resources for one expanded team inventory.
             */
            static void drawTeamInventoryDetails(sf::RenderWindow& window,
                sf::Text& text, const Resources& resources, int row,
                float panelX, float panelY);

            /**
             * @brief Draws a compact level distribution grid for one team.
             */
            static void drawTeamLevelGrid(sf::RenderWindow& window,
                sf::Text& text,
                const std::array<int, MAX_PLAYER_LEVEL + 1>& levelCounts,
                int row, float panelX, float panelY);

            /**
             * @brief Draws one sprite-backed time-control button.
             */
            static void drawTimeButton(sf::RenderWindow& window,
                sf::Text& text, const std::string& fallbackLabel,
                const std::string& textureName, const sf::FloatRect& rect,
                bool active);

            /**
             * @brief Draws one legend entry with its texture and label.
             */
            static void drawLegendItem(sf::RenderWindow& window,
                sf::Text& text, const std::string& textureName,
                const std::string& label, int row, float panelX, float panelY);

            /**
             * @brief Draws a line of text in a panel at a row-based position.
             */
            static void drawInfoLine(sf::RenderWindow& window, sf::Text& text, const std::string& value, int row, unsigned int size, float panelX, float panelY);
    };

#endif /* !INFO_PANEL_HPP_ */
