/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** World
*/

#include "World.hpp"
#include "GraphicalUtils.hpp"
#include "TextureManager.hpp"

void WorldRenderer::draw(sf::RenderWindow& window, const sf::View& worldView,
    const GameWorld& world,
    const std::optional<std::pair<int, int>>& selectedTile,
    const std::optional<int>& selectedPlayerId,
    const sf::Font& font, bool fontLoaded, bool showGrid, int serverFreq,
    bool timePaused)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty())
        return;

    window.setView(worldView);

    const sf::Vector2u windowSize = window.getSize();
    const float totalMapWidth = static_cast<float>(mapWidth) * TILE_SIZE;
    const float totalMapHeight = static_cast<float>(mapHeight) * TILE_SIZE;
    const float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) / 2.0f;
    const float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) / 2.0f;

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
    drawPlayers(window, world, offsetX, offsetY);
    if (selectedPlayerId.has_value()) {
        const auto& players = world.getPlayers();
        const auto it = players.find(*selectedPlayerId);

        if (it != players.end())
            drawSelectedPlayer(window, it->second, offsetX, offsetY);
    }
    drawEggs(window, world, offsetX, offsetY);
    drawInfoPanel(window, world, selectedTile, selectedPlayerId, font, fontLoaded);
    drawTimePanel(window, font, fontLoaded, serverFreq, timePaused);
}

void GraphicalUtils::drawPlayer(sf::RenderTarget& target, const Player& player, float tileSize, float offsetX, float offsetY)
{
    if (player.level < 1)
        return;
    float centerX = player.x * tileSize + offsetX + (tileSize / 2.0f);
    float centerY = player.y * tileSize + offsetY + (tileSize / 2.0f);

    const TextureManager& tm = TextureManager::getInstance();
    std::string texKey = getPlayerTextureKey(player);
    
    sf::Sprite playerSprite(tm.getTexture(texKey));
    sf::Vector2u texSize = tm.getTexture(texKey).getSize();
    if (texSize.x > 0 && texSize.y > 0) {
        float desired = tileSize * 0.5f;
        float scaleX = desired / static_cast<float>(texSize.x);
        float scaleY = desired / static_cast<float>(texSize.y);
        playerSprite.setScale(scaleX, scaleY);
        playerSprite.setOrigin(static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f);
    }
    playerSprite.setPosition(centerX, centerY);
    target.draw(playerSprite);
}

void WorldRenderer::drawTiles(sf::RenderWindow& window, const GameWorld& world,
    float offsetX, float offsetY)
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
            GraphicalUtils::drawResources(window, map[x][y], TILE_SIZE,
                offsetX, offsetY);
        }
    }
}

void WorldRenderer::drawSelectedTile(sf::RenderWindow& window, int x, int y,
    float posX, float posY,
    const std::optional<std::pair<int, int>>& selectedTile)
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

void WorldRenderer::drawSelectedPlayer([[maybe_unused]] sf::RenderWindow& window, const Player& player, float offsetX, float offsetY)
{
    const float centerX = player.x * TILE_SIZE + offsetX + (TILE_SIZE / 2.0f);
    const float centerY = player.y * TILE_SIZE + offsetY + (TILE_SIZE / 2.0f);
    const float radius = TILE_SIZE * 0.32f;
    sf::CircleShape halo(radius);

    halo.setOrigin(radius, radius);
    halo.setPosition(centerX, centerY);
    halo.setFillColor(sf::Color::Transparent);
    halo.setOutlineColor(sf::Color(255, 220, 80));
    halo.setOutlineThickness(1.5f);
    window.draw(halo);
}

void WorldRenderer::drawGrid(sf::RenderWindow& window, const GameWorld& world,
    float offsetX, float offsetY)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();
    const sf::Color gridColor(255, 255, 255, 95);
    sf::VertexArray lines(sf::Lines);

    for (int x = 0; x <= mapWidth; x++) {
        const float posX = offsetX + static_cast<float>(x) * TILE_SIZE;

        lines.append(sf::Vertex(sf::Vector2f(posX, offsetY), gridColor));
        lines.append(sf::Vertex(sf::Vector2f(posX,
            offsetY + static_cast<float>(mapHeight) * TILE_SIZE), gridColor));
    }
    for (int y = 0; y <= mapHeight; y++) {
        const float posY = offsetY + static_cast<float>(y) * TILE_SIZE;

        lines.append(sf::Vertex(sf::Vector2f(offsetX, posY), gridColor));
        lines.append(sf::Vertex(sf::Vector2f(
            offsetX + static_cast<float>(mapWidth) * TILE_SIZE, posY),
            gridColor));
    }
    window.draw(lines);
}

void WorldRenderer::drawPlayers(sf::RenderWindow& window, const GameWorld& world,
    float offsetX, float offsetY)
{
    const auto& players = world.getPlayers();

    for (const auto& pair : players)
        GraphicalUtils::drawPlayer(window, pair.second, TILE_SIZE, offsetX, offsetY);
}

void WorldRenderer::drawEggs(sf::RenderWindow& window, const GameWorld& world,
    float offsetX, float offsetY)
{
    const auto& eggs = world.getEggs();

    for (const auto& pair : eggs) {
        GraphicalUtils::drawEgg(window, pair.second, TILE_SIZE, offsetX, offsetY);
    }
}

void WorldRenderer::drawTimePanel(sf::RenderWindow& window, const sf::Font& font,
    bool fontLoaded, int serverFreq, bool timePaused)
{
    if (!fontLoaded)
        return;

    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();
    const float panelX = TIME_PANEL_MARGIN;
    const float panelY = TIME_PANEL_MARGIN;
    const float buttonY = panelY + 64.0f;
    const sf::FloatRect pauseButton(panelX + 16.0f, buttonY, TIME_BUTTON_WIDTH,
        TIME_BUTTON_HEIGHT);
    const sf::FloatRect slowerButton(panelX + 16.0f + TIME_BUTTON_WIDTH + 10.0f,
        buttonY, TIME_BUTTON_WIDTH, TIME_BUTTON_HEIGHT);
    const sf::FloatRect fasterButton(panelX + 16.0f +
        (TIME_BUTTON_WIDTH + 10.0f) * 2.0f, buttonY, TIME_BUTTON_WIDTH,
        TIME_BUTTON_HEIGHT);
    sf::RectangleShape panel({TIME_PANEL_WIDTH, TIME_PANEL_HEIGHT});
    sf::Text text;

    (void)windowSize;
    window.setView(window.getDefaultView());
    panel.setPosition(panelX, panelY);
    panel.setFillColor(sf::Color(18, 24, 30, 225));
    panel.setOutlineColor(sf::Color(120, 200, 255, 220));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);

    text.setFont(font);
    text.setFillColor(sf::Color(235, 240, 245));
    text.setCharacterSize(18);
    text.setPosition(panelX + 16.0f, panelY + 16.0f);
    text.setString("Time");
    window.draw(text);

    text.setCharacterSize(15);
    text.setPosition(panelX + 16.0f, panelY + 38.0f);
    text.setString("freq: " + std::to_string(serverFreq) +
        (timePaused ? " (pause)" : ""));
    window.draw(text);

    drawTimeButton(window, text, timePaused ? "Resume" : "Pause", pauseButton,
        timePaused);
    drawTimeButton(window, text, "-", slowerButton, false);
    drawTimeButton(window, text, "+", fasterButton, false);

    window.setView(previousView);
}

void WorldRenderer::drawTimeButton(sf::RenderWindow& window, sf::Text& text,
    const std::string& label, const sf::FloatRect& rect, bool active)
{
    sf::RectangleShape button({rect.width, rect.height});

    button.setPosition(rect.left, rect.top);
    button.setFillColor(active ? sf::Color(120, 200, 255, 210) : sf::Color(36, 44, 52, 255));
    button.setOutlineColor(sf::Color(120, 200, 255, 220));
    button.setOutlineThickness(2.0f);
    window.draw(button);

    text.setCharacterSize(16);
    text.setString(label);
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(rect.left + (rect.width - bounds.width) / 2.0f - bounds.left,
        rect.top + (rect.height - bounds.height) / 2.0f - bounds.top - 2.0f);
    window.draw(text);
}

void WorldRenderer::drawInfoPanel(sf::RenderWindow& window, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile,
    const std::optional<int>& selectedPlayerId,
    const sf::Font& font, bool fontLoaded)
{
    if (selectedPlayerId.has_value()) {
        drawPlayerPanel(window, world, *selectedPlayerId, font, fontLoaded);
        return;
    }
    if (!selectedTile.has_value() || !fontLoaded)
        return;

    const int x = selectedTile->first;
    const int y = selectedTile->second;

    if (x < 0 || y < 0 || x >= world.getWidth() ||
        y >= world.getHeight() || world.getMap().empty())
        return;

    const Resources& resources = world.getMap()[x][y].resources;
    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();
    const float panelX = static_cast<float>(windowSize.x) - INFO_PANEL_WIDTH - INFO_PANEL_MARGIN;
    const float panelY = INFO_PANEL_MARGIN;
    sf::RectangleShape panel({INFO_PANEL_WIDTH, INFO_PANEL_HEIGHT});
    sf::Text text;

    window.setView(window.getDefaultView());
    panel.setPosition(panelX, panelY);
    panel.setFillColor(sf::Color(18, 24, 30, 225));
    panel.setOutlineColor(sf::Color(255, 220, 80, 230));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);

    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color(235, 240, 245));

    drawInfoLine(window, text, "Case " + std::to_string(x) + " " +
        std::to_string(y), 0, 20, panelX, panelY);
    drawInfoLine(window, text, "food: " + std::to_string(resources.food), 2,
        16, panelX, panelY);
    drawInfoLine(window, text, "brick: " +
        std::to_string(resources.linemate), 3, 16, panelX, panelY);
    drawInfoLine(window, text, "shoe: " +
        std::to_string(resources.deraumere), 4, 16, panelX, panelY);
    drawInfoLine(window, text, "balloon: " + std::to_string(resources.sibur), 5,
        16, panelX, panelY);
    drawInfoLine(window, text, "jersey: " +
        std::to_string(resources.mendiane), 6, 16, panelX, panelY);
    drawInfoLine(window, text, "trophy: " + std::to_string(resources.phiras),
        7, 16, panelX, panelY);
    drawInfoLine(window, text, "card: " +
        std::to_string(resources.thystame), 8, 16, panelX, panelY);

    window.setView(previousView);
}

void WorldRenderer::drawPlayerPanel(sf::RenderWindow& window,
    const GameWorld& world, int playerId, const sf::Font& font, bool fontLoaded)
{
    const auto& players = world.getPlayers();
    const auto it = players.find(playerId);

    if (it == players.end() || !fontLoaded)
        return;

    const Player& player = it->second;
    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();
    const float panelX = static_cast<float>(windowSize.x) - PLAYER_PANEL_WIDTH - INFO_PANEL_MARGIN;
    const float panelY = INFO_PANEL_MARGIN;
    sf::RectangleShape panel({PLAYER_PANEL_WIDTH, PLAYER_PANEL_HEIGHT});
    sf::RectangleShape portraitFrame({PORTRAIT_SIZE + 14.0f, PORTRAIT_SIZE + 14.0f});
    sf::RectangleShape portraitBack({PORTRAIT_SIZE, PORTRAIT_SIZE});
    sf::Text text;

    window.setView(window.getDefaultView());
    panel.setPosition(panelX, panelY);
    panel.setFillColor(sf::Color(18, 24, 30, 225));
    panel.setOutlineColor(sf::Color(255, 220, 80, 230));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);

    portraitFrame.setPosition(panelX + 206.0f, panelY + 16.0f);
    portraitFrame.setFillColor(sf::Color(36, 44, 52, 255));
    portraitFrame.setOutlineColor(sf::Color(255, 220, 80, 220));
    portraitFrame.setOutlineThickness(2.0f);
    window.draw(portraitFrame);

    portraitBack.setPosition(panelX + 213.0f, panelY + 23.0f);
    portraitBack.setFillColor(sf::Color(14, 18, 22, 255));
    window.draw(portraitBack);
    GraphicalUtils::drawPlayerPortrait(window, player, panelX + 213.0f, panelY + 23.0f, PORTRAIT_SIZE);

    text.setFont(font);
    text.setFillColor(sf::Color(235, 240, 245));

    drawPlayerInfoLine(window, text, "Player #" + std::to_string(player.id), 0, 20, panelX, panelY);
    drawPlayerInfoLine(window, text, "team: " + player.teamName, 1, 17, panelX, panelY);
    drawPlayerInfoLine(window, text, "level: " + std::to_string(player.level), 2, 17, panelX, panelY);
    drawPlayerInfoLine(window, text, "position: (" + std::to_string(player.x) + ", " + std::to_string(player.y) + ")", 3, 17, panelX, panelY);
    drawPlayerInfoLine(window, text, "orientation: " + std::to_string(player.orientation), 4, 17, panelX, panelY);
    drawPlayerInfoLine(window, text, "inventory", 6, 18, panelX, panelY);
    drawPlayerInfoLine(window, text, "food: " + std::to_string(player.inventory.food), 7, 16, panelX, panelY);
    drawPlayerInfoLine(window, text, "linemate: " + std::to_string(player.inventory.linemate), 8, 16, panelX, panelY);
    drawPlayerInfoLine(window, text, "deraumere: " + std::to_string(player.inventory.deraumere), 9, 16, panelX, panelY);
    drawPlayerInfoLine(window, text, "sibur: " + std::to_string(player.inventory.sibur), 10, 16, panelX, panelY);
    drawPlayerInfoLine(window, text, "mendiane: " + std::to_string(player.inventory.mendiane), 11, 16, panelX, panelY);
    drawPlayerInfoLine(window, text, "phiras: " + std::to_string(player.inventory.phiras), 12, 16, panelX, panelY);
    drawPlayerInfoLine(window, text, "thystame: " + std::to_string(player.inventory.thystame), 13, 16, panelX, panelY);

    window.setView(previousView);
}

void WorldRenderer::drawPlayerInfoLine(sf::RenderWindow& window, sf::Text& text,
    const std::string& value, int row, unsigned int size, float panelX,
    float panelY)
{
    text.setString(value);
    text.setCharacterSize(size);
    text.setPosition(panelX + 18.0f,
        panelY + 18.0f + static_cast<float>(row) * 23.0f);
    window.draw(text);
}

void WorldRenderer::drawInfoLine(sf::RenderWindow& window, sf::Text& text,
    const std::string& value, int row, unsigned int size,
    float panelX, float panelY)
{
    text.setString(value);
    text.setCharacterSize(size);
    text.setPosition(panelX + 18.0f,
        panelY + 16.0f + static_cast<float>(row) * 25.0f);
    window.draw(text);
}
