/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameEngine.hpp
*/

/**
 * @file GameEngine.hpp
 * @brief Core game simulation: map, resources, survival and incantations.
 */

#ifndef GAME_ENGINE_HPP_
#define GAME_ENGINE_HPP_

#include <vector>
#include <memory>
#include <string>
#include <array>
#include <unordered_map>

class AI;
class INetworkProvider;
class ILogger;

#include "domain/Map/Map.hpp"

/**
 * @class GameEngine
 * @brief Owns the world and advances the Zappy simulation one tick at a time.
 *
 * Holds the @ref Map and drives all rule-based behaviour: per-player command
 * execution, food consumption/starvation, resource spawning and the
 * multi-player elevation (incantation) logic, including victory detection.
 */
class GameEngine {
    public:
        /**
         * @brief Builds the engine and its empty map.
         * @param width Map width in tiles.
         * @param height Map height in tiles.
         * @param logger Logger for game events.
         */
        GameEngine(int width, int height, ILogger& logger);
        ~GameEngine() = default;

        /**
         * @brief Advances global world state for one tick (resource spawning).
         * @param network Network/game facade.
         */
        void update(INetworkProvider& network);
        /**
         * @brief Advances one AI player for one tick (survival + commands).
         * @param ai Player to update.
         * @param network Network/game facade.
         * @param clientsToRemove Out-list of ids to disconnect (e.g. starved).
         */
        void updateAi(std::shared_ptr<AI> ai, INetworkProvider& network, std::vector<int>& clientsToRemove);
        /**
         * @brief Validates and starts an incantation on the player's tile.
         * @param ai Player initiating the incantation.
         * @param network Network/game facade.
         * @return @c true if the incantation was successfully started.
         */
        bool checkIncantationStart(std::shared_ptr<AI> ai, INetworkProvider& network);

        /** @brief Returns the map width in tiles. */
        int getWidth() const;
        /** @brief Returns the map height in tiles. */
        int getHeight() const;

        /** @brief Returns the mutable game map. */
        Map& getMap();
        /** @brief Returns the const game map. */
        const Map& getMap() const;

        /** @brief Allocates and returns the next unique egg id. */
        int generateEggId() { return _nextEggId++; }

        /**
         * @brief Checks that a tile holds the players and stones for elevation.
         * @param x,y Tile coordinates.
         * @param level Current level being elevated from.
         * @return @c true if the incantation requirements are met.
         */
        bool checkIncantationRequirements(int x, int y, int level) const;
        /**
         * @brief Returns how many players are required to elevate from @p level.
         * @param level Current level.
         * @return Required player count.
         */
        int getRequiredPlayersForIncantation(int level) const;
        /**
         * @brief Lists the players on a tile eligible for an incantation.
         * @param x,y Tile coordinates.
         * @param level Level being elevated from.
         * @param network Network/game facade for client lookup.
         * @return Eligible players on the tile.
         */
        std::vector<std::shared_ptr<AI>> getPlayersOnTileForIncantation(int x, int y, int level, const INetworkProvider& network) const;
        /**
         * @brief Removes the stones consumed by an incantation from a tile.
         * @param x,y Tile coordinates.
         * @param level Level being elevated from.
         */
        void consumeIncantationResources(int x, int y, int level);
        /**
         * @brief Puts back the stones of a failed/aborted incantation.
         * @param x,y Tile coordinates.
         * @param level Level being elevated from.
         */
        void refundIncantationResources(int x, int y, int level);
        /**
         * @brief Notifies victory if a team reached the winning condition.
         * @param teamName Team to check.
         * @param network Network/game facade for client inspection.
         */
        void checkVictory(const std::string& teamName, INetworkProvider& network) const;

        /** @brief Scatters the initial resources across the map. */
        void generateInitialResources();
        /**
         * @brief Spawns the starting eggs for each team.
         * @param teamNames Participating teams.
         * @param initialSlots Number of starting slots (eggs) per team.
         */
        void generateInitialEggs(const std::vector<std::string>& teamNames, int initialSlots);
        /** @brief Periodically replenishes resources to their target densities. */
        void spawnResourcesCycle();

    private:
        /** @brief Decrements food, disconnecting the player on starvation. */
        bool handleAiSurvival(int fd, std::shared_ptr<AI> ai, INetworkProvider& network, std::vector<int>& clientsToRemove);
        /** @brief Ticks the action currently in progress and fires it when done. */
        void handleAiActiveCommand(std::shared_ptr<AI> ai, INetworkProvider& network);
        /** @brief Pulls and schedules the next queued command, if any. */
        void handleAiNextCommand(std::shared_ptr<AI> ai, INetworkProvider& network);
        /** @brief Executes the player's due command via the network facade. */
        void executeAiAction(std::shared_ptr<AI> ai, INetworkProvider& network);

        /**
         * @struct IncantationRequirement
         * @brief Players and stones needed to elevate from a given level.
         */
        struct IncantationRequirement {
            int players;                                /**< Required player count. */
            std::array<int, RESOURCE_COUNT> resources;  /**< Required stones by type. */
        };
        /** @brief Requirement table keyed by current level (1..7). */
        static const std::unordered_map<int, IncantationRequirement> _incantationReqs;

        ILogger& _logger;            /**< Game-event logger. */
        int _width;                  /**< Map width in tiles. */
        int _height;                 /**< Map height in tiles. */

        Map _map;                    /**< The game world. */
        int _resourceSpawnTicks = 0; /**< Ticks elapsed since last resource spawn. */
        int _nextEggId = 0;          /**< Next egg id to allocate. */
        /** @brief Target density per resource type used when spawning. */
        const std::array<double, RESOURCE_COUNT> _densities = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
};

#endif /* !GAME_ENGINE_HPP_ */