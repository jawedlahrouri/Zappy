#include "Players.hpp"
#include "World.hpp"
#include "TextureManager.hpp"
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <limits>
#include <sstream>

std::set<int> Players::_activeSayenSounds;
std::vector<std::unique_ptr<sf::Music>> Players::_sayenSounds;

std::string Players::getPlayerTextureKey(const Player& player)
{
    if (player.teamColor == 1) {
        if (player.orientation == 1)
            return "blueBack";
        if (player.orientation == 2)
            return "blueRight";
        if (player.orientation == 4)
            return "blueLeft";
        return "bluePlayer";
    }
    if (player.teamColor == 3) {
        if (player.orientation == 1)
            return "greenBack";
        if (player.orientation == 2)
            return "greenRight";
        if (player.orientation == 4)
            return "greenLeft";
        return "greenPlayer";
    }
    if (player.teamColor == 4) {
        if (player.orientation == 1)
            return "pinkBack";
        if (player.orientation == 2)
            return "pinkRight";
        if (player.orientation == 4)
            return "pinkLeft";
        return "pinkPlayer";
    }
    if (player.orientation == 1)
        return "playerBack";
    if (player.orientation == 2)
        return "playerRight";
    if (player.orientation == 4)
        return "playerLeft";
    return "playerFace";
}

std::string Players::getPlayerTextureKey(const Player& player, const int orientation)
{
    if (player.teamColor == 1) {
        if (orientation == 1)
            return "blueBack";
        if (orientation == 2)
            return "blueRight";
        if (orientation == 4)
            return "blueLeft";
        return "bluePlayer";
    }
    if (player.teamColor == 3) {
        if (orientation == 1)
            return "greenBack";
        if (orientation == 2)
            return "greenRight";
        if (orientation == 4)
            return "greenLeft";
        return "greenPlayer";
    }
    if (player.teamColor == 4) {
        if (orientation == 1)
            return "pinkBack";
        if (orientation == 2)
            return "pinkRight";
        if (orientation == 4)
            return "pinkLeft";
        return "pinkPlayer";
    }
    if (orientation == 1)
        return "playerBack";
    if (orientation == 2)
        return "playerRight";
    if (orientation == 4)
        return "playerLeft";
    return "playerFace";
}

std::string Players::getSelectedPlayerTextureKey(const Player& player)
{
    if (player.teamColor == 1) {
        if (player.orientation == 1)
            return "whiteBlueBack";
        if (player.orientation == 2)
            return "whiteBlueRight";
        if (player.orientation == 4)
            return "whiteBlueLeft";
        return "whiteBlueFace";
    }
    if (player.teamColor == 3) {
        if (player.orientation == 1)
            return "whiteGreenBack";
        if (player.orientation == 2)
            return "whiteGreenRight";
        if (player.orientation == 4)
            return "whiteGreenLeft";
        return "whiteGreenFace";
    }
    if (player.teamColor == 4) {
        if (player.orientation == 1)
            return "whitePinkBack";
        if (player.orientation == 2)
            return "whitePinkRight";
        if (player.orientation == 4)
            return "whitePinkLeft";
        return "whitePinkFace";
    }
    if (player.orientation == 1)
        return "whiteRedBack";
    if (player.orientation == 2)
        return "whiteRedRight";
    if (player.orientation == 4)
        return "whiteRedLeft";
    return "whiteRedFace";
}

std::string Players::getIncantationTextureKey(const IncantationState& incantation,
    int teamColor)
{
    const auto now = std::chrono::steady_clock::now();
    const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - incantation.startTime).count();
    const int durationMs = std::max(1, incantation.durationMs);
    const int frameDurationMs = std::max(1, durationMs / SAYEN_FRAME_COUNT);
    const int frameIndex = static_cast<int>((elapsedMs / frameDurationMs) % SAYEN_FRAME_COUNT) + 1;
    std::ostringstream key;
    std::string color = "red";

    if (teamColor == 1)
        color = "blue";
    if (teamColor == 3)
        color = "green";
    if (teamColor == 4)
        color = "pink";
    key << color << "Sayen" << std::setw(2) << std::setfill('0') << frameIndex;
    return key.str();
}

std::string Players::getDeathTextureKey(const DeathState& death)
{
    const auto now = std::chrono::steady_clock::now();
    const auto elapsedMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            now - death.startTime).count();
    const int durationMs = std::max(1, death.durationMs);
    const int frameDurationMs = std::max(1, durationMs / DEATH_FRAME_COUNT);
    const int frameIndex = std::clamp(
        static_cast<int>(elapsedMs / frameDurationMs), 0,
        DEATH_FRAME_COUNT - 1);
    std::ostringstream key;

    if (death.teamColor == 1)
        key << "blueDeath";
    else if (death.teamColor == 3)
        key << "greenDeath";
    else if (death.teamColor == 4)
        key << "pinkDeath";
    else
        key << "redDeath";
    key << frameIndex;
    return key.str();
}

void Players::cleanupSayenSounds()
{
    _sayenSounds.erase(std::remove_if(_sayenSounds.begin(),
        _sayenSounds.end(), [](const std::unique_ptr<sf::Music>& sound) {
            return sound->getStatus() == sf::Music::Stopped;
        }), _sayenSounds.end());
}

void Players::clearSounds()
{
    for (const std::unique_ptr<sf::Music>& sound : _sayenSounds) {
        if (sound != nullptr)
            sound->stop();
    }
    _sayenSounds.clear();
    _activeSayenSounds.clear();
}

void Players::playSayenSound(int playerId)
{
    std::unique_ptr<sf::Music> sound = std::make_unique<sf::Music>();

    if (!sound->openFromFile(SAYEN_SOUND_PATH)) {
        _activeSayenSounds.erase(playerId);
        return;
    }
    sound->setVolume(SAYEN_SOUND_VOLUME);
    sound->play();
    _sayenSounds.push_back(std::move(sound));
}

void Players::updateSayenSounds(const GameWorld& world)
{
    std::set<int> activePlayers;

    cleanupSayenSounds();
    for (const auto& pair : world.getPlayers()) {
        const int playerId = pair.first;

        if (!world.getPlayerIncantation(playerId).has_value())
            continue;
        activePlayers.insert(playerId);
        if (_activeSayenSounds.insert(playerId).second)
            playSayenSound(playerId);
    }
    for (auto it = _activeSayenSounds.begin();
        it != _activeSayenSounds.end(); ) {
        if (activePlayers.find(*it) == activePlayers.end())
            it = _activeSayenSounds.erase(it);
        else
            ++it;
    }
}

bool Players::shouldShowSelectedSprite()
{
    static const sf::Clock clock;
    static constexpr int BLINK_PERIOD_MS = 350;

    return ((clock.getElapsedTime().asMilliseconds() / BLINK_PERIOD_MS) % 2) == 0;
}

sf::Color Players::getBroadcastColor(int teamColor, sf::Uint8 alpha)
{
    if (teamColor == 1)
        return sf::Color(75, 165, 255, alpha);
    if (teamColor == 3)
        return sf::Color(75, 235, 145, alpha);
    if (teamColor == 4)
        return sf::Color(255, 105, 195, alpha);
    return sf::Color(255, 95, 95, alpha);
}

void Players::drawBroadcasts(sf::RenderWindow& target, const GameWorld& world,
    float offsetX, float offsetY)
{
    const auto now = std::chrono::steady_clock::now();

    for (const BroadcastState& broadcast : world.getBroadcasts()) {
        const float duration = static_cast<float>(std::max(1,
            broadcast.durationMs));
        const float elapsed = static_cast<float>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - broadcast.startTime).count());
        const float progress = std::clamp(elapsed / duration, 0.0f, 1.0f);
        const float minRadius = WorldRenderer::TILE_SIZE * 0.08f;
        const float maxRadius = WorldRenderer::TILE_SIZE * 1.15f;
        const float radius = minRadius + (maxRadius - minRadius) * progress;
        const sf::Uint8 alpha = static_cast<sf::Uint8>(
            std::clamp(220.0f * (1.0f - progress), 0.0f, 220.0f));
        sf::CircleShape circle(radius);

        circle.setOrigin(radius, radius);
        circle.setPosition(
            static_cast<float>(broadcast.x) * WorldRenderer::TILE_SIZE +
                offsetX + (WorldRenderer::TILE_SIZE / 2.0f),
            static_cast<float>(broadcast.y) * WorldRenderer::TILE_SIZE +
                offsetY + (WorldRenderer::TILE_SIZE / 2.0f));
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(getBroadcastColor(broadcast.teamColor, alpha));
        circle.setOutlineThickness(2.0f + 3.0f * (1.0f - progress));
        target.draw(circle);
    }
}

void Players::draw(sf::RenderWindow& target, const GameWorld& world,
    float offsetX, float offsetY, const std::optional<int>& selectedPlayerId,
    bool showBroadcasts)
{
    const auto& players = world.getPlayers();

    updateSayenSounds(world);
    if (showBroadcasts)
        drawBroadcasts(target, world, offsetX, offsetY);
    for (const auto& pair : players) {
        const Player& player = pair.second;
        const std::optional<DeathState> death =
            world.getPlayerDeath(player.id);
        const std::optional<IncantationState> incantation =
            world.getPlayerIncantation(player.id);
        const bool selected = selectedPlayerId.has_value() &&
            *selectedPlayerId == player.id;
        const float centerX = player.x * WorldRenderer::TILE_SIZE + offsetX +
            (WorldRenderer::TILE_SIZE / 2.0f);
        const float centerY = player.y * WorldRenderer::TILE_SIZE + offsetY +
            (WorldRenderer::TILE_SIZE / 2.0f);
        const TextureManager& tm = TextureManager::getInstance();
        std::string texKey;

        if (death.has_value())
            texKey = getDeathTextureKey(*death);
        else if (incantation.has_value())
            texKey = getIncantationTextureKey(*incantation, player.teamColor);
        else if (selected && shouldShowSelectedSprite())
            texKey = getSelectedPlayerTextureKey(player);
        else
            texKey = getPlayerTextureKey(player);
        sf::Sprite sprite(tm.getTexture(texKey));
        const sf::Vector2u texSize = tm.getTexture(texKey).getSize();

        if (texSize.x > 0 && texSize.y > 0) {
            float desired = WorldRenderer::TILE_SIZE * PLAYER_SIZE_RATIO;

            if (death.has_value())
                desired = WorldRenderer::TILE_SIZE * DEATH_SIZE_RATIO;
            else if (incantation.has_value())
                desired = WorldRenderer::TILE_SIZE * INCANTATION_SIZE_RATIO;
            const float scaleX = desired / static_cast<float>(texSize.x);
            const float scaleY = desired / static_cast<float>(texSize.y);
            sprite.setScale(scaleX, scaleY);
            sprite.setOrigin(static_cast<float>(texSize.x) / 2.0f,
                static_cast<float>(texSize.y) / 2.0f);
        }
        sprite.setPosition(centerX, centerY);
        target.draw(sprite);
    }
}

bool Players::pickPlayer(const GameWorld& world, const sf::Vector2f& worldPos, float offsetX, float offsetY, int& selectedPlayerId)
{
    const auto& players = world.getPlayers();
    const Player* closestPlayer = nullptr;
    float closestDistance = std::numeric_limits<float>::max();
    const float playerHalfSize = WorldRenderer::TILE_SIZE * 0.25f;

    for (const auto& pair : players) {
        const Player& player = pair.second;
        const float centerX = static_cast<float>(player.x) *
            WorldRenderer::TILE_SIZE + offsetX +
            (WorldRenderer::TILE_SIZE / 2.0f);
        const float centerY = static_cast<float>(player.y) *
            WorldRenderer::TILE_SIZE + offsetY +
            (WorldRenderer::TILE_SIZE / 2.0f);

        if (std::abs(worldPos.x - centerX) > playerHalfSize ||
            std::abs(worldPos.y - centerY) > playerHalfSize)
            continue;
        const float dx = worldPos.x - centerX;
        const float dy = worldPos.y - centerY;
        const float distance = dx * dx + dy * dy;

        if (distance < closestDistance) {
            closestDistance = distance;
            closestPlayer = &player;
        }
    }
    if (closestPlayer == nullptr)
        return false;
    selectedPlayerId = closestPlayer->id;
    return true;
}

void Players::drawPlayerPortrait(sf::RenderTarget& target, const Player& player, float x, float y, float size)
{
    const TextureManager& tm = TextureManager::getInstance();
    sf::Sprite portrait(tm.getTexture(getPlayerTextureKey(player, 3)));
    const sf::Vector2u texSize = tm.getTexture(getPlayerTextureKey(player, 3)).getSize();

    if (texSize.x > 0 && texSize.y > 0) {
        const float scaleX = size / static_cast<float>(texSize.x);
        const float scaleY = size / static_cast<float>(texSize.y);

        portrait.setScale(scaleX, scaleY);
    }
    portrait.setPosition(x, y);
    target.draw(portrait);
}

std::string Players::intToOrientation(int orientation)
{
    switch (orientation) {
        case 1: return "North";
        case 2: return "East";
        case 3: return "South";
        case 4: return "West";
        default: return "Unknown";
    }
}

void Players::drawTextInPanel(const sf::Font& font, sf::RenderWindow& window,  const float panelX ,  const float panelY,const Player player)
{
    sf::Text text;

    text.setFont(font);
    text.setFillColor(sf::Color(235, 240, 245));

    drawInfoLine(window, text, "Player #" + std::to_string(player.id), 0, 20, panelX, panelY);
    drawInfoLine(window, text, "team: " + player.teamName, 1, 17, panelX, panelY);
    drawInfoLine(window, text, "level: " + std::to_string(player.level), 2, 17, panelX, panelY);
    drawInfoLine(window, text, "position: (" + std::to_string(player.x) + ", " + std::to_string(player.y) + ")", 3, 17, panelX, panelY);
    drawInfoLine(window, text, "orientation: " + intToOrientation(player.orientation), 4, 17, panelX, panelY);
    drawInfoLine(window, text, "inventory", 6, 18, panelX, panelY);
    drawInfoLine(window, text, "food: " + std::to_string(player.inventory.food), 7, 16, panelX, panelY);
    drawInfoLine(window, text, "brick: " + std::to_string(player.inventory.linemate), 8, 16, panelX, panelY);
    drawInfoLine(window, text, "shoe: " + std::to_string(player.inventory.deraumere), 9, 16, panelX, panelY);
    drawInfoLine(window, text, "balloon: " + std::to_string(player.inventory.sibur), 10, 16, panelX, panelY);
    drawInfoLine(window, text, "jersey: " + std::to_string(player.inventory.mendiane), 11, 16, panelX, panelY);
    drawInfoLine(window, text, "trophy: " + std::to_string(player.inventory.phiras), 12, 16, panelX, panelY);
    drawInfoLine(window, text, "card: " + std::to_string(player.inventory.thystame), 13, 16, panelX, panelY);
}

void Players::drawPanel(sf::RenderWindow& window, const GameWorld& world,
    int playerId, const sf::Font& font, bool fontLoaded)
{
    const auto& players = world.getPlayers();
    const auto it = players.find(playerId);

    if (it == players.end() || !fontLoaded)
        return;

    const Player& player = it->second;
    const sf::View previousView = window.getView();
    const sf::Vector2u windowSize = window.getSize();
    const float panelX = static_cast<float>(windowSize.x) - PANEL_WIDTH - PANEL_MARGIN;
    const float panelY = PANEL_MARGIN;
    sf::RectangleShape panel({PANEL_WIDTH, PANEL_HEIGHT});
    sf::RectangleShape portraitFrame({PORTRAIT_SIZE + 14.0f, PORTRAIT_SIZE + 14.0f});
    sf::RectangleShape portraitBack({PORTRAIT_SIZE, PORTRAIT_SIZE});

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
    drawPlayerPortrait(window, player, panelX + 213.0f, panelY + 23.0f, PORTRAIT_SIZE);

    drawTextInPanel(font, window, panelX, panelY, player);

    window.setView(previousView);
}

void Players::drawInfoLine(sf::RenderWindow& window, sf::Text& text,
    const std::string& value, int row, unsigned int size, float panelX,
    float panelY)
{
    text.setString(value);
    text.setCharacterSize(size);
    text.setPosition(panelX + 18.0f, panelY + 18.0f + static_cast<float>(row) * 23.0f);
    window.draw(text);
}
