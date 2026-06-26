/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu
*/

/**
 * @file Menu.hpp
 * @brief Declares the SFML connection-menu renderer.
 */

#ifndef MENU_HPP_
    #define MENU_HPP_

    #include <SFML/Graphics.hpp>
    #include <string>
    #include <vector>
    #include <algorithm>

    /**
     * @brief Draws the graphical connection menu.
     */
    class Menu {
        public:
            /**
             * @brief Draws the full menu, including inputs, status, and teams.
             */
            static void draw(sf::RenderWindow& window, const sf::Font& font,
                bool fontLoaded, const std::string& ip, const std::string& port,
                int activeField, const std::string& status,
                const std::vector<std::string>& teams);
    
        private:
            /**
             * @brief Draws the menu background.
             */
            static void drawBackground(sf::RenderWindow& window, const sf::Vector2u& windowSize);

            /**
             * @brief Draws and returns the start button hitbox.
             */
            static sf::RectangleShape drawStartButton(sf::RenderWindow& window, const sf::Vector2u& windowSize);

            /**
             * @brief Draws one input field and returns its hitbox.
             */
            static sf::RectangleShape drawInputBox(sf::RenderWindow& window,
                const sf::Vector2u& windowSize, float y, bool active);

            /**
             * @brief Draws a team character preview in the menu.
             */
            static void drawTeamCharacter(sf::RenderWindow& window,
                const sf::Font& font,
                const std::string& textureName, const std::string& teamName,
                float x, float y, float wantedHeight);

            /**
             * @brief Draws all known teams in the menu preview area.
             */
            static void drawTeams(sf::RenderWindow& window, const sf::Font& font,
                const sf::Vector2u& windowSize,
                const std::vector<std::string>& teams);

            /**
             * @brief Draws menu text, labels, inputs, and status.
             */
            static void drawTexts(sf::RenderWindow& window, const sf::Font& font,
                const sf::Vector2u& windowSize, const sf::RectangleShape& startButton,
                const std::string& ip, const std::string& port, int activeField,
                const std::string& status,
                const std::vector<std::string>& teams);

            /**
             * @brief Moves a text origin to its visual center.
             */
            static void centerTextOrigin(sf::Text& text);

            /**
             * @brief Draws one text label at the given screen position.
             */
            static void drawText(sf::RenderWindow& window, const sf::Font& font, const std::string& str, unsigned int size, const sf::Color& color, float x, float y);
    };

#endif
