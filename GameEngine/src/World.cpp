/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** World
*/

#include "World.hpp"
#include "GraphicalUtils.hpp"
#include "InfoPanel.hpp"
#include "Players.hpp"
#include "TextureManager.hpp"

void WorldRenderer::draw(sf::RenderWindow& window, const sf::View& worldView, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const std::optional<int>& selectedPlayerId, const sf::Font& font, bool fontLoaded, bool showGrid, bool showBroadcasts, int serverFreq, bool timePaused, bool showInfoPanel, const std::optional<std::string>& openedTeamInventory)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    drawBackground(window);
    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty()) {
        if (showInfoPanel)
            InfoPanelRenderer::drawGlobalInfoPanel(window, world, font,
                fontLoaded, serverFreq, timePaused, openedTeamInventory);
        return;
    }

    window.setView(worldView);

    const sf::Vector2u windowSize = window.getSize();
    const float totalMapWidth = static_cast<float>(mapWidth) * TILE_SIZE;
    const float totalMapHeight = static_cast<float>(mapHeight) * TILE_SIZE;
    const float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) /
        2.0f;
    const float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) /
        2.0f;

    drawTiles(window, world, offsetX, offsetY);
    if (showGrid)
        drawGrid(window, world, offsetX, offsetY);
    if (selectedTile.has_value()) {
        const int x = selectedTile->first;
        const int y = selectedTile->second;
        const float posX = static_cast<float>(x) * TILE_SIZE + offsetX;
        const float posY = static_cast<float>(y) * TILE_SIZE + offsetY;

        drawSelectedTile(window, x, y, posX, posY, selectedTile);
    }
    Players::draw(window, world, offsetX, offsetY, selectedPlayerId, showBroadcasts);
    drawEggs(window, world, offsetX, offsetY);
    if (showInfoPanel)
        InfoPanelRenderer::drawGlobalInfoPanel(window, world, font, fontLoaded,
            serverFreq, timePaused, openedTeamInventory);
    InfoPanelRenderer::drawInfoPanel(window, world, selectedTile,
        selectedPlayerId, font, fontLoaded);
    InfoPanelRenderer::drawTimePanel(window, font, fontLoaded, serverFreq,
        timePaused);
    if (world.getWinner().has_value())
        drawWinnerOverlay(window, *world.getWinner(), font, fontLoaded);
}

void WorldRenderer::drawWinnerOverlay(sf::RenderWindow& window, const std::string& teamName, const sf::Font& font, bool fontLoaded)
{
    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();

    window.setView(window.getDefaultView());

    sf::RectangleShape overlay({static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    if (!fontLoaded) {
        window.setView(previousView);
        return;
    }

    sf::Text title;
    title.setFont(font);
    title.setString("GAME OVER");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    const sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(
        (static_cast<float>(windowSize.x) - titleBounds.width) / 2.f,
        static_cast<float>(windowSize.y) / 2.f - 90.f
    );
    window.draw(title);

    sf::Text winner;
    winner.setFont(font);
    winner.setString(teamName + " wins!");
    winner.setCharacterSize(40);
    winner.setFillColor(sf::Color::Yellow);
    const sf::FloatRect winnerBounds = winner.getLocalBounds();
    winner.setPosition(
        (static_cast<float>(windowSize.x) - winnerBounds.width) / 2.f,
        static_cast<float>(windowSize.y) / 2.f
    );
    window.draw(winner);

    window.setView(previousView);
}

void WorldRenderer::drawBackground(sf::RenderWindow& window)
{
    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();
    const sf::Texture& texture = TextureManager::getInstance().getTexture("spursBackground");
    const sf::Vector2u textureSize = texture.getSize();

    window.setView(window.getDefaultView());
    if (textureSize.x == 0 || textureSize.y == 0) {
        sf::RectangleShape background({static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)});

        background.setFillColor(sf::Color(18, 24, 30));
        window.draw(background);
        window.setView(previousView);
        return;
    }

    sf::Sprite background(texture);

    background.setScale(static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x), static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y));
    window.draw(background);
    window.setView(previousView);
}

void WorldRenderer::drawTiles(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY)
{
    const TextureManager& tm = TextureManager::getInstance();
    sf::Sprite tileSprite(tm.getTexture("ground"));
    const auto& map = world.getMap();

    for (int x = 0; x < world.getWidth(); x++) {
        for (int y = 0; y < world.getHeight(); y++) {
            const float posX = static_cast<float>(x) * TILE_SIZE + offsetX;
            const float posY = static_cast<float>(y) * TILE_SIZE + offsetY;

            tileSprite.setPosition(posX, posY);
            window.draw(tileSprite);
            GraphicalUtils::drawResources(window, map[x][y], TILE_SIZE, offsetX, offsetY);
        }
    }
}

void WorldRenderer::drawSelectedTile(sf::RenderWindow& window, int x, int y, float posX, float posY, const std::optional<std::pair<int, int>>& selectedTile)
{
    if (!selectedTile.has_value() || selectedTile->first != x ||
        selectedTile->second != y)
        return;

    sf::RectangleShape selection({TILE_SIZE, TILE_SIZE});

    selection.setPosition(posX, posY);
    selection.setFillColor(sf::Color::Transparent);
    selection.setOutlineColor(sf::Color(255, 220, 80));
    selection.setOutlineThickness(-3.0f);
    window.draw(selection);
}

void WorldRenderer::drawGrid(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();
    const sf::Color gridColor(255, 255, 255, 95);
    sf::VertexArray lines(sf::Lines);

    for (int x = 0; x <= mapWidth; x++) {
        const float posX = offsetX + static_cast<float>(x) * TILE_SIZE;

        lines.append(sf::Vertex(sf::Vector2f(posX, offsetY), gridColor));
        lines.append(sf::Vertex(sf::Vector2f(posX, offsetY + static_cast<float>(mapHeight) * TILE_SIZE), gridColor));
    }
    for (int y = 0; y <= mapHeight; y++) {
        const float posY = offsetY + static_cast<float>(y) * TILE_SIZE;

        lines.append(sf::Vertex(sf::Vector2f(offsetX, posY), gridColor));
        lines.append(sf::Vertex(sf::Vector2f(offsetX + static_cast<float>(mapWidth) * TILE_SIZE, posY), gridColor));
    }
    window.draw(lines);
}

void WorldRenderer::drawEggs(sf::RenderWindow& window, const GameWorld& world, float offsetX, float offsetY)
{
    const auto& eggs = world.getEggs();

    for (const auto& pair : eggs) {
        GraphicalUtils::drawEgg(window, pair.second, TILE_SIZE, offsetX,
            offsetY);
    }
}
