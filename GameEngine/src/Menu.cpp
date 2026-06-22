/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Menu
*/

#include "Menu.hpp"
#include "TextureManager.hpp"

static std::string getMenuTeamTexture(std::size_t index)
{
    if (index == 0)
        return "bluePlayer";
    if (index == 1)
        return "playerFace";
    if (index == 2)
        return "greenPlayer";
    if (index == 3)
        return "pinkPlayer";
    return "playerFace";
}

void Menu::draw(sf::RenderWindow& window, const sf::Font& font, bool fontLoaded, const std::string& ip, const std::string& port, int activeField, const std::string& status, const std::vector<std::string>& teams)
{
    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();

    window.setView(window.getDefaultView());
    drawBackground(window, windowSize);
    const sf::RectangleShape startButton = drawStartButton(window, windowSize);
    if (fontLoaded)
        drawTexts(window, font, windowSize, startButton, ip, port, activeField, status, teams);
    window.setView(previousView);
}

void Menu::drawBackground(sf::RenderWindow& window, const sf::Vector2u& windowSize)
{
    const sf::Texture& backgroundTexture = TextureManager::getInstance().getTexture("menuBackground");
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

    startButton.setOrigin(startButton.getSize().x / 2.0f, startButton.getSize().y / 2.0f);
    startButton.setPosition(static_cast<float>(windowSize.x) / 2.0f, static_cast<float>(windowSize.y) / 2.0f + 105.0f);
    startButton.setFillColor(sf::Color(255, 220, 80));
    window.draw(startButton);
    return startButton;
}

sf::RectangleShape Menu::drawInputBox(sf::RenderWindow& window, const sf::Vector2u& windowSize, float y, bool active)
{
    sf::RectangleShape inputBox({360.0f, 46.0f});

    inputBox.setOrigin(inputBox.getSize().x / 2.0f, inputBox.getSize().y / 2.0f);
    inputBox.setPosition(static_cast<float>(windowSize.x) / 2.0f, y);
    inputBox.setFillColor(sf::Color(245, 248, 250, 220));
    if (active) {
        inputBox.setOutlineThickness(4.0f);
        inputBox.setOutlineColor(sf::Color(255, 220, 80));
    } else {
        inputBox.setOutlineThickness(2.0f);
        inputBox.setOutlineColor(sf::Color(80, 95, 110));
    }
    window.draw(inputBox);
    return inputBox;
}

void Menu::drawTeamCharacter(sf::RenderWindow& window, const sf::Font& font, const std::string& textureName, const std::string& teamName, float x, float y, float wantedHeight)
{
    const sf::Texture& texture = TextureManager::getInstance().getTexture(textureName);
    const sf::Vector2u textureSize = texture.getSize();

    if (textureSize.x == 0 || textureSize.y == 0)
        return;

    const float scale = wantedHeight / static_cast<float>(textureSize.y);
    sf::Sprite character(texture);

    drawText(window, font, teamName, 26, sf::Color(235, 240, 245), x, y - (wantedHeight / 2.0f) - 24.0f);
    character.setOrigin(static_cast<float>(textureSize.x) / 2.0f, static_cast<float>(textureSize.y) / 2.0f);
    character.setScale(scale, scale);
    character.setPosition(x, y);
    window.draw(character);
}

void Menu::drawTeams(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize, const std::vector<std::string>& teams)
{
    if (teams.empty())
        return;

    const float centerX = static_cast<float>(windowSize.x) / 2.0f;
    const float centerY = static_cast<float>(windowSize.y) / 2.0f;
    const float characterOffset = std::min(500.0f,
    static_cast<float>(windowSize.x) * 0.36f);
    const std::size_t rows = (teams.size() + 1) / 2;
    const float rowGap = std::clamp(260.0f / static_cast<float>(rows), 82.0f, 130.0f);
    const float wantedHeight = std::clamp(rowGap * 0.68f, 54.0f, 100.0f);
    const float firstY = centerY - (static_cast<float>(rows - 1) * rowGap / 2.0f) - 25.0f;

    for (std::size_t index = 0; index < teams.size(); ++index) {
        const bool leftColumn = (index % 2 == 0);
        float x = centerX + characterOffset;
        const float y = firstY + static_cast<float>(index / 2) * rowGap;
        std::string textureName = getMenuTeamTexture(index);

        if (leftColumn) {
            x = centerX - characterOffset;
        }
        drawTeamCharacter(window, font, textureName, teams[index], x, y, wantedHeight);
    }
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

void Menu::drawTexts(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2u& windowSize, const sf::RectangleShape& startButton, const std::string& ip, const std::string& port, int activeField, const std::string& status, const std::vector<std::string>& teams)
{
    const float centerX = static_cast<float>(windowSize.x) / 2.0f;
    const float centerY = static_cast<float>(windowSize.y) / 2.0f;

    drawTeams(window, font, windowSize, teams);
    drawText(window, font, "IP", 24, sf::Color(235, 240, 245), centerX, centerY - 140.0f);
    drawInputBox(window, windowSize, centerY - 95.0f, activeField == 0);
    drawText(window, font, ip, 22, sf::Color(18, 24, 30), centerX, centerY - 101.0f);
    drawText(window, font, "Port", 24, sf::Color(235, 240, 245), centerX, centerY - 35.0f);
    drawInputBox(window, windowSize, centerY + 10.0f, activeField == 1);
    drawText(window, font, port, 22, sf::Color(18, 24, 30), centerX, centerY + 4.0f);
    drawText(window, font, "Press Enter", 26, sf::Color(18, 24, 30), startButton.getPosition().x, startButton.getPosition().y - 6.0f);
    if (!status.empty())
        drawText(window, font, status, 18, sf::Color(255, 220, 80), centerX, startButton.getPosition().y + 65.0f);
    drawText(window, font, "Esc to quit", 18, sf::Color(160, 170, 180), centerX, startButton.getPosition().y + 95.0f);
}

void Menu::centerTextOrigin(sf::Text& text)
{
    const sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
}
