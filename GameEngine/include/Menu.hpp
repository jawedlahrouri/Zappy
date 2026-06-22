/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Menu
*/

#ifndef MENU_HPP_
    #define MENU_HPP_

    #include <SFML/Graphics.hpp>
    #include <string>
    #include <vector>
    #include <algorithm>

    class Menu {
        public:
            static void draw(sf::RenderWindow& window, const sf::Font& font,
                bool fontLoaded, const std::string& ip, const std::string& port,
                int activeField, const std::string& status,
                const std::vector<std::string>& teams);
    
        private:
            static void drawBackground(sf::RenderWindow& window, const sf::Vector2u& windowSize);
            static sf::RectangleShape drawStartButton(sf::RenderWindow& window, const sf::Vector2u& windowSize);
            static sf::RectangleShape drawInputBox(sf::RenderWindow& window,
                const sf::Vector2u& windowSize, float y, bool active);
            static void drawTeamCharacter(sf::RenderWindow& window,
                const sf::Font& font,
                const std::string& textureName, const std::string& teamName,
                float x, float y, float wantedHeight);
            static void drawTeams(sf::RenderWindow& window, const sf::Font& font,
                const sf::Vector2u& windowSize,
                const std::vector<std::string>& teams);
            static void drawTexts(sf::RenderWindow& window, const sf::Font& font,
                const sf::Vector2u& windowSize, const sf::RectangleShape& startButton,
                const std::string& ip, const std::string& port, int activeField,
                const std::string& status,
                const std::vector<std::string>& teams);
            static void centerTextOrigin(sf::Text& text);
            static void drawText(sf::RenderWindow& window, const sf::Font& font, const std::string& str, unsigned int size, const sf::Color& color, float x, float y);
    };

#endif
