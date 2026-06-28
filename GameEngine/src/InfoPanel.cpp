/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** InfoPanel
*/

#include "InfoPanel.hpp"

void InfoPanelRenderer::addResources(Resources& total, const Resources& resources)
{
    total.food += resources.food;
    total.linemate += resources.linemate;
    total.deraumere += resources.deraumere;
    total.sibur += resources.sibur;
    total.mendiane += resources.mendiane;
    total.phiras += resources.phiras;
    total.thystame += resources.thystame;
}

int InfoPanelRenderer::countResourceItems(const Resources& resources)
{
    return resources.food + resources.linemate + resources.deraumere + resources.sibur + resources.mendiane + resources.phiras + resources.thystame;
}

void InfoPanelRenderer::drawGlobalInfoPanel(sf::RenderWindow& window, const GameWorld& world, const sf::Font& font, bool fontLoaded, int serverFreq, bool timePaused, const std::optional<std::string>& openedTeamInventory)
{
    if (!fontLoaded)
        return;

    const sf::View previousView = window.getView();
    const float panelX = INFO_PANEL_MARGIN;
    const float panelY = INFO_PANEL_MARGIN;
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();
    const int tileCount = std::max(0, mapWidth * mapHeight);
    const auto& players = world.getPlayers();
    const auto& eggs = world.getEggs();
    Resources mapResources;
    std::map<std::string, int> teamCounts;
    std::map<std::string, Resources> teamResources;
    std::map<std::string, std::array<int, MAX_PLAYER_LEVEL + 1>> teamLevelCounts;
    int maxLevel = 0;

    if (mapWidth > 0 && mapHeight > 0 && !world.getMap().empty()) {
        const auto& map = world.getMap();

        for (int x = 0; x < mapWidth; x++) {
            for (int y = 0; y < mapHeight; y++)
                addResources(mapResources, map[x][y].resources);
        }
    }
    for (const auto& pair : players) {
        const Player& player = pair.second;

        teamCounts[player.teamName]++;
        teamLevelCounts[player.teamName][std::clamp(player.level, 1,
            MAX_PLAYER_LEVEL)]++;
        maxLevel = std::max(maxLevel, player.level);
        addResources(teamResources[player.teamName], player.inventory);
    }

    sf::RectangleShape panel({GLOBAL_PANEL_WIDTH, GLOBAL_PANEL_HEIGHT});
    sf::Text text;

    window.setView(window.getDefaultView());
    panel.setPosition(panelX, panelY);
    panel.setFillColor(sf::Color(18, 24, 30, 225));
    panel.setOutlineColor(sf::Color(255, 220, 80, 230));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);

    text.setFont(font);
    text.setFillColor(sf::Color(235, 240, 245));

    drawInfoLine(window, text, "Map infos", 0, 20, panelX, panelY);
    drawInfoLine(window, text, "size: " + std::to_string(mapWidth) + " x " + std::to_string(mapHeight) + " (" + std::to_string(tileCount) + " tiles)", 2, 15, panelX, panelY);
    drawInfoLine(window, text, "players: " + std::to_string(players.size()) + "  teams: " + std::to_string(teamCounts.size()), 3, 15, panelX, panelY);
    drawInfoLine(window, text, "eggs: " + std::to_string(eggs.size()), 4, 15, panelX, panelY);
    drawInfoLine(window, text, "max level: " + std::to_string(maxLevel) + "  freq: " + std::to_string(serverFreq) + (timePaused ? " paused" : ""), 5, 15, panelX, panelY);
    drawInfoLine(window, text, "map resources: " + std::to_string(countResourceItems(mapResources)), 7, 16, panelX, panelY);
    drawInfoLine(window, text, "food: " + std::to_string(mapResources.food) + "  brick: " + std::to_string(mapResources.linemate), 8, 14, panelX, panelY);
    drawInfoLine(window, text, "shoe: " + std::to_string(mapResources.deraumere) + "  balloon: " + std::to_string(mapResources.sibur), 9, 14, panelX, panelY);
    drawInfoLine(window, text, "jersey: " + std::to_string(mapResources.mendiane) + "  trophy: " + std::to_string(mapResources.phiras), 10, 14, panelX, panelY);
    drawInfoLine(window, text, "card: " + std::to_string(mapResources.thystame), 11, 14, panelX, panelY);
    drawInfoLine(window, text, "teams", 13, 16, panelX, panelY);
    const int row = drawTeamSharedInventories(window, text, teamCounts,
        teamResources, teamLevelCounts, openedTeamInventory, 14, panelX,
        panelY);

    drawInfoLine(window, text, "I: hide/show infos", row + 1, 13, panelX, panelY);
    window.setView(previousView);
}

int InfoPanelRenderer::drawTeamSharedInventories(sf::RenderWindow& window, sf::Text& text, const std::map<std::string, int>& teamCounts, const std::map<std::string, Resources>& teamResources, const std::map<std::string, std::array<int, MAX_PLAYER_LEVEL + 1>>& teamLevelCounts, const std::optional<std::string>& openedTeamInventory, int row, float panelX, float panelY)
{
    int hiddenTeams = 0;

    for (const auto& pair : teamCounts) {
        const auto resourcesIt = teamResources.find(pair.first);
        const auto levelsIt = teamLevelCounts.find(pair.first);
        const Resources resources = resourcesIt == teamResources.end() ? Resources() : resourcesIt->second;
        const std::array<int, MAX_PLAYER_LEVEL + 1> levels =
            levelsIt == teamLevelCounts.end() ?
            std::array<int, MAX_PLAYER_LEVEL + 1>() : levelsIt->second;
        const bool opened = openedTeamInventory.has_value() && *openedTeamInventory == pair.first;
        const int neededRows = opened ? 6 : 1;

        if (row + neededRows > MAX_GLOBAL_INFO_ROW) {
            hiddenTeams++;
            continue;
        }
        drawInfoLine(window, text, std::string(opened ? "- " : "+ ") + pair.first + ": " + std::to_string(pair.second) + " players", row, 14, panelX, panelY);
        row++;
        if (opened) {
            drawTeamLevelGrid(window, text, levels, row, panelX, panelY);
            row += 2;
            drawTeamInventoryDetails(window, text, resources, row, panelX, panelY);
            row += 3;
        }
    }
    if (hiddenTeams > 0 && row <= MAX_GLOBAL_INFO_ROW) {
        drawInfoLine(window, text, "... " + std::to_string(hiddenTeams) + " more teams", row, 13, panelX, panelY);
        row++;
    }
    return row;
}

void InfoPanelRenderer::drawTeamInventoryDetails(sf::RenderWindow& window, sf::Text& text, const Resources& resources, int row, float panelX, float panelY)
{
    drawInfoLine(window, text, "food: " + std::to_string(resources.food) + "  brick: " + std::to_string(resources.linemate) + "  shoe: " + std::to_string(resources.deraumere), row, 13, panelX, panelY);
    drawInfoLine(window, text, "balloon: " + std::to_string(resources.sibur) + "  jersey: " + std::to_string(resources.mendiane), row + 1, 13, panelX, panelY);
    drawInfoLine(window, text, "trophy: " + std::to_string(resources.phiras) + "  card: " + std::to_string(resources.thystame), row + 2, 13, panelX, panelY);
}

void InfoPanelRenderer::drawTeamLevelGrid(sf::RenderWindow& window, sf::Text& text, const std::array<int, MAX_PLAYER_LEVEL + 1>& levelCounts, int row, float panelX, float panelY)
{
    const float startX = panelX + 18.0f;
    const float startY = panelY + 16.0f + static_cast<float>(row) * 22.0f;
    const float gap = 6.0f;
    const float cellWidth = (GLOBAL_PANEL_WIDTH - 36.0f - (gap * 3.0f)) / 4.0f;
    const float cellHeight = 20.0f;

    for (int level = 1; level <= MAX_PLAYER_LEVEL; level++) {
        const int index = level - 1;
        const int column = index % 4;
        const int line = index / 4;
        const float x = startX + static_cast<float>(column) *
            (cellWidth + gap);
        const float y = startY + static_cast<float>(line) * 22.0f;
        sf::RectangleShape cell({cellWidth, cellHeight});

        cell.setPosition(x, y);
        cell.setFillColor(sf::Color(34, 43, 52, 230));
        cell.setOutlineThickness(1.0f);
        cell.setOutlineColor(levelCounts[level] > 0 ?
            sf::Color(255, 220, 80, 190) : sf::Color(80, 95, 110, 160));
        window.draw(cell);

        text.setCharacterSize(11);
        text.setFillColor(sf::Color(165, 178, 190));
        text.setString("L" + std::to_string(level));
        text.setPosition(x + 7.0f, y + 2.0f);
        window.draw(text);

        text.setCharacterSize(13);
        text.setFillColor(levelCounts[level] > 0 ?
            sf::Color(255, 232, 130) : sf::Color(235, 240, 245));
        text.setString(std::to_string(levelCounts[level]));
        const sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(x + cellWidth - bounds.width - 8.0f, y + 1.0f);
        window.draw(text);
    }
    text.setFillColor(sf::Color(235, 240, 245));
}

std::optional<std::string> InfoPanelRenderer::getTeamInventoryAt(const GameWorld& world, const std::optional<std::string>& openedTeamInventory, int mouseX, int mouseY)
{
    const float panelX = INFO_PANEL_MARGIN;
    const float panelY = INFO_PANEL_MARGIN;
    const float mx = static_cast<float>(mouseX);
    const float my = static_cast<float>(mouseY);
    std::map<std::string, int> teamCounts;
    int row = 14;

    if (mx < panelX || mx > panelX + GLOBAL_PANEL_WIDTH || my < panelY || my > panelY + GLOBAL_PANEL_HEIGHT)
        return std::nullopt;
    for (const auto& pair : world.getPlayers())
        teamCounts[pair.second.teamName]++;
    for (const auto& pair : teamCounts) {
        const bool opened = openedTeamInventory.has_value() && *openedTeamInventory == pair.first;
        const int neededRows = opened ? 6 : 1;
        const sf::FloatRect teamLine(panelX + 12.0f, panelY + 16.0f + static_cast<float>(row) * 22.0f, GLOBAL_PANEL_WIDTH - 24.0f, static_cast<float>(neededRows) * 22.0f);

        if (row + neededRows > MAX_GLOBAL_INFO_ROW)
            return std::nullopt;
        if (teamLine.contains(mx, my))
            return pair.first;
        row += neededRows;
    }
    return std::nullopt;
}

sf::FloatRect InfoPanelRenderer::getTimePanelRect(const sf::Vector2u& windowSize)
{
    const float panelX = (static_cast<float>(windowSize.x) - TIME_PANEL_WIDTH) / 2.0f;
    const float panelY = static_cast<float>(windowSize.y) - TIME_PANEL_HEIGHT - TIME_PANEL_MARGIN;

    return {panelX, panelY, TIME_PANEL_WIDTH, TIME_PANEL_HEIGHT};
}

sf::FloatRect InfoPanelRenderer::getInfoButtonRect(const sf::Vector2u& windowSize)
{
    static constexpr float buttonSize = 56.0f;

    return {static_cast<float>(windowSize.x) - buttonSize - INFO_PANEL_MARGIN,
        static_cast<float>(windowSize.y) - buttonSize - INFO_PANEL_MARGIN,
        buttonSize, buttonSize};
}

sf::FloatRect InfoPanelRenderer::getLegendPanelRect(const sf::Vector2u& windowSize)
{
    static constexpr float panelWidth = 310.0f;
    static constexpr float panelHeight = 238.0f;
    const sf::FloatRect button = getInfoButtonRect(windowSize);

    return {static_cast<float>(windowSize.x) - panelWidth - INFO_PANEL_MARGIN,
        button.top - panelHeight - 12.0f, panelWidth, panelHeight};
}

bool InfoPanelRenderer::isInsideLegendUi(const sf::Vector2u& windowSize,
    int mouseX, int mouseY, bool showLegend)
{
    const float mx = static_cast<float>(mouseX);
    const float my = static_cast<float>(mouseY);

    if (getInfoButtonRect(windowSize).contains(mx, my))
        return true;
    if (showLegend && getLegendPanelRect(windowSize).contains(mx, my))
        return true;
    return false;
}

void InfoPanelRenderer::drawLegend(sf::RenderWindow& window,
    const sf::Font& font, bool fontLoaded, bool showLegend)
{
    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();
    const sf::FloatRect buttonRect = getInfoButtonRect(windowSize);
    const sf::Texture& buttonTexture =
        TextureManager::getInstance().getTexture("infoButton");
    const sf::Vector2u buttonTextureSize = buttonTexture.getSize();
    sf::RectangleShape fallback({buttonRect.width, buttonRect.height});

    window.setView(window.getDefaultView());
    if (buttonTextureSize.x > 0 && buttonTextureSize.y > 0) {
        sf::Sprite button(buttonTexture);

        button.setPosition(buttonRect.left, buttonRect.top);
        button.setScale(buttonRect.width / static_cast<float>(buttonTextureSize.x),
            buttonRect.height / static_cast<float>(buttonTextureSize.y));
        if (showLegend)
            button.setColor(sf::Color(255, 232, 130, 255));
        window.draw(button);
    } else {
        fallback.setPosition(buttonRect.left, buttonRect.top);
        fallback.setFillColor(sf::Color(18, 24, 30, 230));
        fallback.setOutlineColor(sf::Color(255, 220, 80, 230));
        fallback.setOutlineThickness(2.0f);
        window.draw(fallback);
    }
    if (!showLegend || !fontLoaded) {
        window.setView(previousView);
        return;
    }

    const sf::FloatRect legendRect = getLegendPanelRect(windowSize);
    sf::RectangleShape panel({legendRect.width, legendRect.height});
    sf::Text text;

    panel.setPosition(legendRect.left, legendRect.top);
    panel.setFillColor(sf::Color(18, 24, 30, 235));
    panel.setOutlineColor(sf::Color(255, 220, 80, 230));
    panel.setOutlineThickness(2.0f);
    window.draw(panel);

    text.setFont(font);
    text.setFillColor(sf::Color(235, 240, 245));
    drawInfoLine(window, text, "Legend", 0, 20, legendRect.left,
        legendRect.top);
    drawLegendItem(window, text, "food", "food", 2, legendRect.left,
        legendRect.top);
    drawLegendItem(window, text, "linemate", "brick = linemate", 3,
        legendRect.left, legendRect.top);
    drawLegendItem(window, text, "deraumere", "shoe = deraumere", 4,
        legendRect.left, legendRect.top);
    drawLegendItem(window, text, "sibur", "balloon = sibur", 5,
        legendRect.left, legendRect.top);
    drawLegendItem(window, text, "mendiane", "jersey = mendiane", 6,
        legendRect.left, legendRect.top);
    drawLegendItem(window, text, "phiras", "trophy = phiras", 7,
        legendRect.left, legendRect.top);
    drawLegendItem(window, text, "thystame", "card = thystame", 8,
        legendRect.left, legendRect.top);
    window.setView(previousView);
}

void InfoPanelRenderer::drawLegendItem(sf::RenderWindow& window,
    sf::Text& text, const std::string& textureName, const std::string& label,
    int row, float panelX, float panelY)
{
    const float iconSize = 20.0f;
    const float itemX = panelX + 18.0f;
    const float itemY = panelY + 16.0f + static_cast<float>(row) * 22.0f;
    const sf::Texture& texture =
        TextureManager::getInstance().getTexture(textureName);
    const sf::Vector2u textureSize = texture.getSize();

    if (textureSize.x > 0 && textureSize.y > 0) {
        sf::Sprite icon(texture);

        icon.setPosition(itemX, itemY - 1.0f);
        icon.setScale(iconSize / static_cast<float>(textureSize.x),
            iconSize / static_cast<float>(textureSize.y));
        window.draw(icon);
    }
    text.setString(label);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color(235, 240, 245));
    text.setPosition(itemX + 32.0f, itemY - 1.0f);
    window.draw(text);
}

void InfoPanelRenderer::drawTimePanel(sf::RenderWindow& window, const sf::Font& font, bool fontLoaded, int serverFreq, bool timePaused)
{
    if (!fontLoaded)
        return;

    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();
    const sf::FloatRect timePanel = getTimePanelRect(windowSize);
    const float panelX = timePanel.left;
    const float panelY = timePanel.top;
    const float buttonY = panelY + 62.0f;
    const sf::FloatRect slowerButton(panelX + 16.0f, buttonY, TIME_BUTTON_WIDTH, TIME_BUTTON_HEIGHT);
    const sf::FloatRect pauseButton(panelX + 16.0f + TIME_BUTTON_WIDTH + 24.0f, buttonY, TIME_BUTTON_WIDTH, TIME_BUTTON_HEIGHT);
    const sf::FloatRect fasterButton(panelX + 16.0f + (TIME_BUTTON_WIDTH + 24.0f) * 2.0f, buttonY, TIME_BUTTON_WIDTH, TIME_BUTTON_HEIGHT);
    sf::RectangleShape panel({TIME_PANEL_WIDTH, TIME_PANEL_HEIGHT});
    sf::Text text;

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
    text.setString("freq: " + std::to_string(serverFreq) + (timePaused ? " (pause)" : ""));
    window.draw(text);

    drawTimeButton(window, text, "-", "timeSlowerButton", slowerButton, false);
    drawTimeButton(window, text, timePaused ? "Resume" : "Pause",
        timePaused ? "timePlayButton" : "timePauseButton", pauseButton,
        timePaused);
    drawTimeButton(window, text, "+", "timeFasterButton", fasterButton, false);

    window.setView(previousView);
}

void InfoPanelRenderer::drawTimeButton(sf::RenderWindow& window, sf::Text& text, const std::string& fallbackLabel, const std::string& textureName, const sf::FloatRect& rect, bool active)
{
    const sf::Texture& texture = TextureManager::getInstance().getTexture(textureName);
    const sf::Vector2u textureSize = texture.getSize();

    if (textureSize.x > 0 && textureSize.y > 0) {
        sf::Sprite button(texture);

        button.setPosition(rect.left, rect.top);
        button.setScale(rect.width / static_cast<float>(textureSize.x),
            rect.height / static_cast<float>(textureSize.y));
        if (active)
            button.setColor(sf::Color(180, 230, 255, 255));
        window.draw(button);
        return;
    }

    sf::RectangleShape fallback({rect.width, rect.height});

    fallback.setPosition(rect.left, rect.top);
    fallback.setFillColor(active ? sf::Color(120, 200, 255, 210)
        : sf::Color(36, 44, 52, 255));
    fallback.setOutlineColor(sf::Color(120, 200, 255, 220));
    fallback.setOutlineThickness(2.0f);
    window.draw(fallback);

    text.setCharacterSize(16);
    text.setString(fallbackLabel);
    const sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(rect.left + (rect.width - bounds.width) / 2.0f - bounds.left, rect.top + (rect.height - bounds.height) / 2.0f - bounds.top - 2.0f);
    window.draw(text);
}

void InfoPanelRenderer::drawInfoPanel(sf::RenderWindow& window, const GameWorld& world, const std::optional<std::pair<int, int>>& selectedTile, const std::optional<int>& selectedPlayerId, const sf::Font& font, bool fontLoaded)
{
    if (selectedPlayerId.has_value()) {
        Players::drawPanel(window, world, *selectedPlayerId, font, fontLoaded);
        return;
    }
    if (!selectedTile.has_value() || !fontLoaded)
        return;

    const int x = selectedTile->first;
    const int y = selectedTile->second;

    if (x < 0 || y < 0 || x >= world.getWidth() || y >= world.getHeight() ||
        world.getMap().empty())
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

    drawInfoLine(window, text, "Case " + std::to_string(x) + " " + std::to_string(y), 0, 20, panelX, panelY);
    drawInfoLine(window, text, "food: " + std::to_string(resources.food), 2, 16, panelX, panelY);
    drawInfoLine(window, text, "brick: " + std::to_string(resources.linemate), 3, 16, panelX, panelY);
    drawInfoLine(window, text, "shoe: " + std::to_string(resources.deraumere), 4, 16, panelX, panelY);
    drawInfoLine(window, text, "balloon: " + std::to_string(resources.sibur), 5, 16, panelX, panelY);
    drawInfoLine(window, text, "jersey: " + std::to_string(resources.mendiane), 6, 16, panelX, panelY);
    drawInfoLine(window, text, "trophy: " + std::to_string(resources.phiras), 7, 16, panelX, panelY);
    drawInfoLine(window, text, "card: " + std::to_string(resources.thystame), 8, 16, panelX, panelY);

    window.setView(previousView);
}

void InfoPanelRenderer::drawInfoLine(sf::RenderWindow& window, sf::Text& text, const std::string& value, int row, unsigned int size, float panelX, float panelY)
{
    text.setString(value);
    text.setCharacterSize(size);
    text.setPosition(panelX + 18.0f, panelY + 16.0f + static_cast<float>(row) * 22.0f);
    window.draw(text);
}
