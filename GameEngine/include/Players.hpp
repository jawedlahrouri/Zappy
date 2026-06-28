/**
 * @file Players.hpp
 * @brief Declares 2D player rendering, selection, and animation helpers.
 */

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

/**
 * @brief Stateless renderer for players and player-related effects.
 */
class Players {
public:
    /**
     * @brief Draws all players, their animations, and optional broadcasts.
     */
    static void draw(sf::RenderWindow& target, const GameWorld& world,
        float offsetX, float offsetY, const std::optional<int>& selectedPlayerId,
        bool showBroadcasts);

    /**
     * @brief Selects the player located under a world-space mouse position.
     * @return true when a player was picked.
     */
    static bool pickPlayer(const GameWorld& world, const sf::Vector2f& worldPos,
        float offsetX, float offsetY, int& selectedPlayerId);

    /**
     * @brief Draws the selected-player information panel.
     */
    static void drawPanel(sf::RenderWindow& window, const GameWorld& world,
        int playerId, const sf::Font& font, bool fontLoaded);

private:
    /** Ratio of TILE_SIZE used by the default player sprite. */
    static constexpr float PLAYER_SIZE_RATIO = 0.5f;
    /** Ratio of TILE_SIZE used by incantation sprites. */
    static constexpr float INCANTATION_SIZE_RATIO = 0.7f;
    /** Ratio of TILE_SIZE used by death sprites. */
    static constexpr float DEATH_SIZE_RATIO = 0.75f;
    /** Number of frames in the sayen incantation animation. */
    static constexpr int SAYEN_FRAME_COUNT = 17;
    /** Number of frames in the player death animation. */
    static constexpr int DEATH_FRAME_COUNT = 7;
    /** Sound file played when a player starts a sayen animation. */
    static constexpr const char* SAYEN_SOUND_PATH =
        "GameEngine/assets/sprites/crisayenbest.mp3";
    /** Volume used for sayen transformation sounds. */
    static constexpr float SAYEN_SOUND_VOLUME = 35.0f;

    /** Width of the player information panel in pixels. */
    static constexpr float PANEL_WIDTH = 330.0f;
    /** Height of the player information panel in pixels. */
    static constexpr float PANEL_HEIGHT = 380.0f;
    /** Margin around the player information panel. */
    static constexpr float PANEL_MARGIN = 20.0f;
    /** Size of the portrait drawn inside the player panel. */
    static constexpr float PORTRAIT_SIZE = 96.0f;

    /**
     * @brief Resolves the texture key for a player's current orientation.
     */
    static std::string getPlayerTextureKey(const Player& player);

    /**
     * @brief Resolves the texture key for a player using a forced orientation.
     */
    static std::string getPlayerTextureKey(const Player& player, const int orientation);

    /**
     * @brief Resolves the texture key for a selected player's white variant.
     */
    static std::string getSelectedPlayerTextureKey(const Player& player);

    /**
     * @brief Resolves the current incantation animation frame key.
     */
    static std::string getIncantationTextureKey(const IncantationState& incantation,
        int teamColor);

    /**
     * @brief Resolves the current death animation frame key.
     */
    static std::string getDeathTextureKey(const DeathState& death);

    /**
     * @brief Starts or cleans sayen sounds according to active incantations.
     */
    static void updateSayenSounds(const GameWorld& world);

    /**
     * @brief Plays one sayen sound instance for a player.
     */
    static void playSayenSound(int playerId);

    /**
     * @brief Removes finished sayen sound instances.
     */
    static void cleanupSayenSounds();

    /**
     * @brief Returns the broadcast color associated with a team palette.
     */
    static sf::Color getBroadcastColor(int teamColor, sf::Uint8 alpha);

    /**
     * @brief Returns whether the selected-player blink is currently visible.
     */
    static bool shouldShowSelectedSprite();

    /**
     * @brief Draws active broadcast effects.
     */
    static void drawBroadcasts(sf::RenderWindow& target, const GameWorld& world,
        float offsetX, float offsetY);

    /**
     * @brief Draws a player portrait in a fixed square.
     */
    static void drawPlayerPortrait(sf::RenderTarget& target, const Player& player, float x, float y, float size);

    /**
     * @brief Draws one row in the player information panel.
     */
    static void drawInfoLine(sf::RenderWindow& window, sf::Text& text,
        const std::string& value, int row, unsigned int size, float panelX, float panelY);

    /**
     * @brief Draws player details inside the information panel.
     */
    static void drawTextInPanel(const sf::Font& font, sf::RenderWindow& window,  const float panelX ,  const float panelY,const Player player);

    /**
     * @brief Converts a protocol orientation value to a readable label.
     */
    static std::string intToOrientation(int orientation);

    static std::set<int> _activeSayenSounds;
    static std::vector<std::unique_ptr<sf::Music>> _sayenSounds;
};

#endif /* !PLAYERS_HPP_ */
