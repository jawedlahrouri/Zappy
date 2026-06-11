/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Menu
*/

#include "Menu.hpp"
#include "TextureManager.hpp"

void Menu::draw(sf::RenderWindow& window, const sf::Font& font, bool fontLoaded)
{
    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();

    window.setView(window.getDefaultView());
    drawBackground(window, windowSize);
    const sf::RectangleShape startButton = drawStartButton(window, windowSize);
    if (fontLoaded)
        drawTexts(window, font, windowSize, startButton);
    window.setView(previousView);
}

void Menu::drawBackground(sf::RenderWindow& window, const sf::Vector2u& windowSize)
{
    const sf::Texture& backgroundTexture =
        TextureManager::getInstance().getTexture("menuBackground");
    const sf::Vector2u textureSize = backgroundTexture.getSize();

    if (textureSize.x == 0 || textureSize.y == 0) {
        sf::RectangleShape background({
            static_cast<float>(windowSize.x),
            static_cast<float>(windowSize.y)
        });

        background.setFillColor(sf::Color(18, 24, 30));
        window.draw(background);
        return;
    }

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x), static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y));
    window.draw(backgroundSprite);
}

sf::RectangleShape Menu::drawStartButton(sf::RenderWindow& window, const sf::Vector2u& windowSize)
{
    sf::RectangleShape startButton({300.0f, 58.0f});

    startButton.setOrigin(startButton.getSize().x / 2.0f,
        startButton.getSize().y / 2.0f);
    startButton.setPosition(static_cast<float>(windowSize.x) / 2.0f,
        static_cast<float>(windowSize.y) / 2.0f + 35.0f);
    startButton.setFillColor(sf::Color(255, 220, 80));
    window.draw(startButton);
    return startButton;
}

void Menu::drawText(sf::RenderWindow& window, const sf::Font& font, const std::string& str, unsigned int size, const sf::Color& color, float x, float y)
{
    sf::Text text;

    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    centerTextOrigin(text);
    text.setPosition(x, y);
    window.draw(text);
}

void Menu::drawTexts(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize, const sf::RectangleShape& startButton)
{
    drawText(window, font, "Port :", 25, sf::Color(235, 240, 245), static_cast<float>(windowSize.x) / 2.0f, static_cast<float>(windowSize.y) / 2.0f - 95.0f);
    drawText(window, font, "Press Enter", 26, sf::Color(18, 24, 30), startButton.getPosition().x, startButton.getPosition().y - 6.0f);
    drawText(window, font, "Esc to quit", 18, sf::Color(160, 170, 180), static_cast<float>(windowSize.x) / 2.0f, startButton.getPosition().y + 75.0f);
}

void Menu::centerTextOrigin(sf::Text& text)
{
    const sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}
