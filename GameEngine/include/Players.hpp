#ifndef PLAYERS_HPP_
#define PLAYERS_HPP_

#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"
#include <optional>
#include <string>

class Players {
public:
    static void draw(sf::RenderWindow& target, const GameWorld& world,
        float offsetX, float offsetY, const std::optional<int>& selectedPlayerId);
    static bool pickPlayer(const GameWorld& world, const sf::Vector2f& worldPos,
        float offsetX, float offsetY, int& selectedPlayerId);
    static void drawPanel(sf::RenderWindow& window, const GameWorld& world,
        int playerId, const sf::Font& font, bool fontLoaded);

private:
    static constexpr float PLAYER_SIZE_RATIO = 0.5f;
    static constexpr float INCANTATION_SIZE_RATIO = 0.7f;
    static constexpr int SAYEN_FRAME_COUNT = 17;

    static constexpr float PANEL_WIDTH = 330.0f;
    static constexpr float PANEL_HEIGHT = 380.0f;
    static constexpr float PANEL_MARGIN = 20.0f;
    static constexpr float PORTRAIT_SIZE = 96.0f;

    static std::string getPlayerTextureKey(const Player& player);
    static std::string getPlayerTextureKey(const Player& player, const int orientation);
    static std::string getSelectedPlayerTextureKey(const Player& player);
    static std::string getIncantationTextureKey(const IncantationState& incantation);
    static sf::Color getBroadcastColor(int teamColor, sf::Uint8 alpha);
    static void drawBroadcasts(sf::RenderWindow& target, const GameWorld& world,
        float offsetX, float offsetY);
    static void drawPlayerPortrait(sf::RenderTarget& target, const Player& player, float x, float y, float size);
    static void drawInfoLine(sf::RenderWindow& window, sf::Text& text,
        const std::string& value, int row, unsigned int size, float panelX, float panelY);
    static void drawTextInPanel(const sf::Font& font, sf::RenderWindow& window,  const float panelX ,  const float panelY,const Player player);
    static std::string intToOrientation(int orientation);
};

#endif /* !PLAYERS_HPP_ */
