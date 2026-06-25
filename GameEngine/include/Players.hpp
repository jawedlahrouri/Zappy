#ifndef PLAYERS_HPP_
#define PLAYERS_HPP_

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

class Players {
public:
    static void draw(sf::RenderWindow& target, const GameWorld& world,
        float offsetX, float offsetY, const std::optional<int>& selectedPlayerId,
        bool showBroadcasts);
    static bool pickPlayer(const GameWorld& world, const sf::Vector2f& worldPos,
        float offsetX, float offsetY, int& selectedPlayerId);
    static void drawPanel(sf::RenderWindow& window, const GameWorld& world,
        int playerId, const sf::Font& font, bool fontLoaded);

private:
    static constexpr float PLAYER_SIZE_RATIO = 0.5f;
    static constexpr float INCANTATION_SIZE_RATIO = 0.7f;
    static constexpr float DEATH_SIZE_RATIO = 0.75f;
    static constexpr int SAYEN_FRAME_COUNT = 17;
    static constexpr int DEATH_FRAME_COUNT = 7;
    static constexpr const char* SAYEN_SOUND_PATH =
        "GameEngine/assets/sprites/crisayenbest.mp3";
    static constexpr float SAYEN_SOUND_VOLUME = 35.0f;

    static constexpr float PANEL_WIDTH = 330.0f;
    static constexpr float PANEL_HEIGHT = 380.0f;
    static constexpr float PANEL_MARGIN = 20.0f;
    static constexpr float PORTRAIT_SIZE = 96.0f;

    static std::string getPlayerTextureKey(const Player& player);
    static std::string getPlayerTextureKey(const Player& player, const int orientation);
    static std::string getSelectedPlayerTextureKey(const Player& player);
    static std::string getIncantationTextureKey(const IncantationState& incantation,
        int teamColor);
    static std::string getDeathTextureKey(const DeathState& death);
    static void updateSayenSounds(const GameWorld& world);
    static void playSayenSound(int playerId);
    static void cleanupSayenSounds();
    static sf::Color getBroadcastColor(int teamColor, sf::Uint8 alpha);
    static void drawBroadcasts(sf::RenderWindow& target, const GameWorld& world,
        float offsetX, float offsetY);
    static void drawPlayerPortrait(sf::RenderTarget& target, const Player& player, float x, float y, float size);
    static void drawInfoLine(sf::RenderWindow& window, sf::Text& text,
        const std::string& value, int row, unsigned int size, float panelX, float panelY);
    static void drawTextInPanel(const sf::Font& font, sf::RenderWindow& window,  const float panelX ,  const float panelY,const Player player);
    static std::string intToOrientation(int orientation);

    static std::set<int> _activeSayenSounds;
    static std::vector<std::unique_ptr<sf::Music>> _sayenSounds;
};

#endif /* !PLAYERS_HPP_ */
