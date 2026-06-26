/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameWorld
*/

/**
 * @file GameWorld.hpp
 * @brief Defines the in-memory model rendered by the graphical client.
 */

#ifndef GAMEWORLD_HPP_
    #define GAMEWORLD_HPP_

    #include <vector>
    #include <string>
    #include <map>
    #include <chrono>
    #include <optional>
    #include <iostream>

    /**
     * @brief Inventory or tile resources in Zappy protocol order.
     */
    struct Resources {
        /** Number of food units. */
        int food = 0;
        /** Number of linemate stones. */
        int linemate = 0;
        /** Number of deraumere stones. */
        int deraumere = 0;
        /** Number of sibur stones. */
        int sibur = 0;
        /** Number of mendiane stones. */
        int mendiane = 0;
        /** Number of phiras stones. */
        int phiras = 0;
        /** Number of thystame stones. */
        int thystame = 0;
    };

    /**
     * @brief Content of one map cell.
     */
    struct Tile {
        /** Tile X coordinate. */
        int x = 0;
        /** Tile Y coordinate. */
        int y = 0;
        /** Resources currently present on the tile. */
        Resources resources;
    };

    /**
     * @brief Player state mirrored from graphical protocol messages.
     */
    struct Player {
        /** Unique player identifier. */
        int id = 0;
        /** Current tile X coordinate. */
        int x = 0;
        /** Current tile Y coordinate. */
        int y = 0;
        /** Direction encoded as 1: north, 2: east, 3: south, 4: west. */
        int orientation = 1;
        /** Current incantation level. */
        int level = 1;
        /** Team name received from the server. */
        std::string teamName;
        /** Player inventory. */
        Resources inventory;
        /** Palette index encoded as 1: blue, 2: red, 3: green, 4: pink. */
        int teamColor = 2;
    };

    /**
     * @brief Egg state displayed on the map.
     */
    struct Egg {
        /** Unique egg identifier. */
        int id;
        /** Tile X coordinate. */
        int x;
        /** Tile Y coordinate. */
        int y;
        /** Identifier of the player that laid the egg. */
        int playerParentId;
        /** true when the egg has hatched and is ready. */
        bool isHatched;
    };

    /**
     * @brief Timed visual state for an active incantation.
     */
    struct IncantationState {
        /** Incantation tile X coordinate. */
        int x = 0;
        /** Incantation tile Y coordinate. */
        int y = 0;
        /** Animation duration in milliseconds. */
        int durationMs = 0;
        /** Monotonic timestamp marking the beginning of the animation. */
        std::chrono::steady_clock::time_point startTime;
    };

    /**
     * @brief Timed visual state for a broadcast effect.
     */
    struct BroadcastState {
        /** Identifier of the broadcasting player. */
        int playerId = 0;
        /** Origin tile X coordinate. */
        int x = 0;
        /** Origin tile Y coordinate. */
        int y = 0;
        /** Team palette used to color the effect. */
        int teamColor = 2;
        /** Effect duration in milliseconds. */
        int durationMs = 0;
        /** Monotonic timestamp marking the beginning of the effect. */
        std::chrono::steady_clock::time_point startTime;
    };

    /**
     * @brief Timed visual state for a player death animation.
     */
    struct DeathState {
        /** Identifier of the dying player. */
        int playerId = 0;
        /** Team palette used to choose death sprites. */
        int teamColor = 2;
        /** Animation duration in milliseconds. */
        int durationMs = 0;
        /** Monotonic timestamp marking the beginning of the animation. */
        std::chrono::steady_clock::time_point startTime;
    };

    /**
     * @brief Mutable world snapshot consumed by graphical renderers.
     */
    class GameWorld {
    public:
        /**
         * @brief Creates an empty world.
         */
        GameWorld();

        /**
         * @brief Releases world resources.
         */
        ~GameWorld();

        /**
         * @brief Resizes the map and resets its tile storage.
         * @param width Map width in tiles.
         * @param height Map height in tiles.
         */
        void setMapSize(int width, int height);

        /**
         * @brief Registers a team name and assigns it a stable display color.
         * @param teamName Team name received from the server.
         */
        void addTeamName(const std::string& teamName);

        /**
         * @brief Replaces the resources stored on one tile.
         * @param x Tile X coordinate.
         * @param y Tile Y coordinate.
         * @param res New resource values.
         */
        void setTileResources(int x, int y, const Resources& res);

        /**
         * @brief Builds a Resources structure from protocol quantities.
         * @return Resources in the same order as the bct/pin protocol fields.
         */
        Resources resToResources(int q0, int q1, int q2, int q3, int q4, int q5, int q6);

        /**
         * @brief Adds or replaces a player in the world.
         * @param player Player snapshot to store.
         */
        void addPlayer(const Player& player);

        /**
         * @brief Updates a player's position and orientation.
         * @param id Player identifier.
         * @param x New tile X coordinate.
         * @param y New tile Y coordinate.
         * @param orientation New protocol orientation.
         */
        void movePlayer(int id, int x, int y, int orientation);

        /**
         * @brief Updates a player's level.
         * @param id Player identifier.
         * @param level New incantation level.
         */
        void setPlayerLevel(int id, int level);

        /**
         * @brief Updates a player's inventory and last known position.
         */
        void setPlayerInventory(int id, int x, int y, const Resources& inventory);

        /**
         * @brief Starts incantation effects for the listed players.
         */
        void startIncantation(int x, int y, const std::vector<int>& playerIds, int durationMs);

        /**
         * @brief Stops every incantation effect on the given tile.
         */
        void stopIncantation(int x, int y);

        /**
         * @brief Starts a broadcast visual effect for a player.
         */
        void triggerBroadcast(int playerId, int durationMs);

        /**
         * @brief Starts a death animation for a player.
         */
        void triggerPlayerDeath(int playerId, int durationMs);

        /**
         * @brief Removes expired timed animations from the world state.
         */
        void updateAnimations();

        /**
         * @brief Removes a player and any associated timed state.
         */
        void removePlayer(int id);

        /**
         * @brief Adds or replaces an egg on the map.
         */
        void addEgg(int id, int parentId, int x, int y);

        /**
         * @brief Marks an egg as hatched.
         */
        void hatchEgg(int id);

        /**
         * @brief Removes an egg from the map.
         */
        void removeEgg(int id);

        /**
         * @brief Applies a signed delta to one resource on one tile.
         */
        void adjustTileResource(int x, int y, int resourceIndex, int delta);

        /**
         * @brief Gets the map width.
         * @return Width in tiles.
         */
        int getWidth() const;

        /**
         * @brief Gets the map height.
         * @return Height in tiles.
         */
        int getHeight() const;

        /**
         * @brief Gets the full tile grid.
         * @return Two-dimensional tile storage indexed by y then x.
         */
        const std::vector<std::vector<Tile>>& getMap() const;

        /**
         * @brief Gets all known players indexed by identifier.
         * @return Player map.
         */
        const std::map<int, Player>& getPlayers() const;

        /**
         * @brief Gets all known eggs indexed by identifier.
         * @return Egg map.
         */
        const std::map<int, Egg>& getEggs() const;

        /**
         * @brief Gets the active incantation effect for a player.
         * @return Incantation state when the player is currently incanting.
         */
        std::optional<IncantationState> getPlayerIncantation(int id) const;

        /**
         * @brief Gets the active death effect for a player.
         * @return Death state when the player is currently dying.
         */
        std::optional<DeathState> getPlayerDeath(int id) const;

        /**
         * @brief Gets active broadcast effects.
         * @return Broadcast list after pruning expired effects.
         */
        const std::vector<BroadcastState>& getBroadcasts() const;

    private:
        /**
         * @brief Resolves the display color assigned to a team.
         */
        int getTeamColor(const std::string& teamName);

        /**
         * @brief Removes expired broadcast effects.
         */
        void pruneBroadcasts() const;
        int _width = 0;
        int _height = 0;
        std::vector<std::string> _teamNames;
        std::vector<std::vector<Tile>> _map;
        std::map<int, Player> _players;
        std::map<int, Egg> _eggs;
        std::map<int, IncantationState> _incantingPlayers;
        std::map<std::pair<int, int>, std::vector<int>> _incantationTiles;
        std::map<int, DeathState> _dyingPlayers;
        mutable std::vector<BroadcastState> _broadcasts;
    };

#endif /* !GAMEWORLD_HPP_ */
