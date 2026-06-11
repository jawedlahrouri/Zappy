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

    class Menu {
        public:
            static void draw(sf::RenderWindow& window, const sf::Font& font, bool fontLoaded);
    
        private:
            static void drawBackground(sf::RenderWindow& window, const sf::Vector2u& windowSize);
            static sf::RectangleShape drawStartButton(sf::RenderWindow& window, const sf::Vector2u& windowSize);
            static void drawTexts(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize, const sf::RectangleShape& startButton);
            static void centerTextOrigin(sf::Text& text);
            static void drawText(sf::RenderWindow& window, const sf::Font& font, const std::string& str, unsigned int size, const sf::Color& color, float x, float y);
    };

#endif
