/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Map.hpp
*/

/**
 * @file Map.hpp
 * @brief In-memory representation of the game world (tiles, resources, eggs).
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>
#include <memory>
#include <string>
#include <array>
#include <unordered_set>

/**
 * @enum ResourceType
 * @brief The seven resource types that can be found on a tile.
 */
enum ResourceType {
    FOOD = 0,       /**< Food, consumed by players to survive. */
    LINEMATE,       /**< Stone required for elevation rituals. */
    DERAUMERE,      /**< Stone required for elevation rituals. */
    SIBUR,          /**< Stone required for elevation rituals. */
    MENDIANE,       /**< Stone required for elevation rituals. */
    PHIRAS,         /**< Stone required for elevation rituals. */
    THYSTAME,       /**< Rarest stone, required at high levels. */
    RESOURCE_COUNT  /**< Sentinel: total number of resource types. */
};

/**
 * @struct Egg
 * @brief An egg laid by a player, hatching into a new AI slot for a team.
 */
struct Egg {
    int id;                 /**< Unique egg identifier. */
    int ownerId;            /**< Id of the player that laid the egg. */
    std::string teamName;   /**< Team the hatched player will belong to. */
    int x;                  /**< X coordinate on the map. */
    int y;                  /**< Y coordinate on the map. */

    /** @brief Returns the egg id. */
    int getId() const;
    /** @brief Returns the id of the player that laid the egg. */
    int getOwnerId() const;
};

/**
 * @struct Tile
 * @brief One map cell: its resources, the players on it and its eggs.
 */
struct Tile {
    std::array<int, RESOURCE_COUNT> resources = {0, 0, 0, 0, 0, 0, 0}; /**< Resource counts. */
    std::vector<int> playerIds;            /**< Ids of players standing here. */
    std::vector<std::shared_ptr<Egg>> eggs; /**< Eggs laid on this tile. */

    /** @brief Adds an egg to this tile. */
    void addEgg(std::shared_ptr<Egg> egg);
    /** @brief Removes the egg with the given id from this tile. */
    void removeEgg(int eggId);
    /** @brief Returns the eggs currently on this tile. */
    const std::vector<std::shared_ptr<Egg>>& getEggs() const;
};

/**
 * @class Map
 * @brief Toroidal grid holding every tile, resource, player and egg.
 *
 * The map wraps around its borders (see @ref borderWrap) and tracks "dirty"
 * tiles so the GUI can be refreshed only for cells that changed.
 */
class Map {
    public:
        /**
         * @brief Builds an empty map of the given dimensions.
         * @param width Map width in tiles.
         * @param height Map height in tiles.
         */
        Map(int width, int height);
        ~Map() = default;

        /** @brief Returns the map width in tiles. */
        int getWidth() const;
        /** @brief Returns the map height in tiles. */
        int getHeight() const;

        /** @brief Returns the mutable tile at @p x,@p y. */
        Tile& getTile(int x, int y);
        /** @brief Returns the const tile at @p x,@p y. */
        const Tile& getTile(int x, int y) const;

        /**
         * @brief Adds a quantity of a resource onto a tile (marks it dirty).
         * @param x,y Tile coordinates.
         * @param type Resource type to add.
         * @param quantity Amount to add.
         */
        void addResourceToTile(int x, int y, ResourceType type, int quantity);
        /**
         * @brief Removes a quantity of a resource from a tile (marks it dirty).
         * @param x,y Tile coordinates.
         * @param type Resource type to remove.
         * @param quantity Amount to remove.
         */
        void removeResourceFromTile(int x, int y, ResourceType type, int quantity);
        /**
         * @brief Counts a resource across the whole map.
         * @param type Resource type to count.
         * @return Total quantity present on the map.
         */
        int countResource(ResourceType type) const;

        /** @brief Returns the set of tile indices changed since the last clear. */
        const std::unordered_set<int>& getDirtyTiles() const;
        /** @brief Clears the dirty-tile set (after the GUI was refreshed). */
        void clearDirtyTiles();

        /** @brief Registers an egg on its tile and on the map. */
        void addEgg(std::shared_ptr<Egg> egg);
        /** @brief Removes the egg with the given id from the map. */
        void removeEgg(int eggId);
        /**
         * @brief Pops a random egg belonging to a team (for hatching).
         * @param teamName Team to draw an egg from.
         * @return The egg, or @c nullptr if the team has none left.
         */
        std::shared_ptr<Egg> getAndRemoveRandomEggForTeam(const std::string& teamName);

        /** @brief Records that a player is standing on a tile. */
        void addAiToTile(int aiId, int x, int y);
        /** @brief Removes a player from a tile. */
        void removeAiFromTile(int aiId, int x, int y);
        /**
         * @brief Moves a player from one tile to another (marks both dirty).
         * @param aiId Player id.
         * @param oldX,oldY Previous coordinates.
         * @param newX,newY New coordinates.
         */
        void moveAi(int aiId, int oldX, int oldY, int newX, int newY);

        /**
         * @brief Wraps coordinates around the map borders (toroidal world).
         * @param x,y Raw coordinates, possibly out of bounds.
         * @return The equivalent in-bounds @c {x, y} coordinates.
         */
        std::pair<int, int> borderWrap(int x, int y) const;

    private:
        int _width;                            /**< Map width in tiles. */
        int _height;                           /**< Map height in tiles. */
        std::vector<std::vector<Tile>> _grid;  /**< Row-major tile grid. */
        std::unordered_set<int> _dirtyTiles;   /**< Indices of changed tiles. */
};

#endif /* !MAP_HPP_ */
