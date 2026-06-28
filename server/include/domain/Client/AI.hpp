/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AI.hpp
*/

/**
 * @file AI.hpp
 * @brief AI player client: position, inventory, level and command queue.
 */

#ifndef AI_HPP_
#define AI_HPP_

#include "Client.hpp"
#include <map>
#include <queue>
#include <string>
#include <vector>

/**
 * @class AI
 * @brief A playing client driven by an AI, holding its full game state.
 *
 * Concrete @ref Client whose @ref update advances queued commands, consumes food
 * and handles incantation timing each tick.
 */
class AI : public Client {
    public:
        /**
         * @brief Builds an AI player around an accepted socket.
         * @param clientSocket Owned socket for this client.
         * @param playerId Unique player id (socket fd).
         */
        AI(std::unique_ptr<ISocket> clientSocket, int playerId);

        /**
         * @brief Advances this AI's state for one game tick.
         * @param self Shared pointer to this client.
         * @param network Network/game facade for replies and events.
         * @param engine Game engine for the current tick.
         * @param clientsToRemove Out-list of client ids to disconnect (e.g. starved).
         */
        void update(std::shared_ptr<Client> self, INetworkProvider& network, GameEngine& engine, std::vector<int>& clientsToRemove) override;

        /** @brief Returns the player's team name. */
        const std::string& getTeamName() const { return _teamName; }
        /** @brief Sets the player's team name. */
        void setTeamName(const std::string& name) { _teamName = name; }

        /** @brief Returns the X coordinate on the map. */
        int getX() const { return _x; }
        /** @brief Sets the X coordinate on the map. */
        void setX(int x) { _x = x; }

        /** @brief Returns the Y coordinate on the map. */
        int getY() const { return _y; }
        /** @brief Sets the Y coordinate on the map. */
        void setY(int y) { _y = y; }

        /** @brief Returns the elevation level (1-8). */
        int getLevel() const { return _level; }
        /** @brief Sets the elevation level. */
        void setLevel(int level) { _level = level; }

        /** @brief Returns the orientation (1=N, 2=E, 3=S, 4=W). */
        int getOrientation() const { return _orientation; }
        /** @brief Sets the orientation. */
        void setOrientation(int orientation) { _orientation = orientation; }

        /** @brief Returns the mutable inventory (resource name -> quantity). */
        std::map<std::string, int>& getInventory() { return _inventory; }
        /** @brief Returns the const inventory. */
        const std::map<std::string, int>& getInventory() const { return _inventory; }

        /** @brief Returns the mutable pending command queue. */
        std::queue<std::string>& getCommandQueue() { return _commandQueue; }
        /** @brief Returns the const pending command queue. */
        const std::queue<std::string>& getCommandQueue() const { return _commandQueue; }

        /** @brief Returns ticks left before the current action completes (-1 = idle). */
        int getActionTicksLeft() const { return _actionTicksLeft; }
        /** @brief Sets ticks left for the current action. */
        void setActionTicksLeft(int ticks) { _actionTicksLeft = ticks; }

        /** @brief Returns ticks of food left before starvation. */
        int getFoodTicks() const { return _foodTicks; }
        /** @brief Sets ticks of food left. */
        void setFoodTicks(int ticks) { _foodTicks = ticks; }

        /** @brief Tells whether the player is currently in an incantation. */
        bool isInIncantation() const { return _inIncantation; }
        /** @brief Sets the incantation-in-progress flag. */
        void setInIncantation(bool v) { _inIncantation = v; }

    private:
        std::string _teamName;        /**< Team the player belongs to. */
        int _x = 0;                   /**< X coordinate on the map. */
        int _y = 0;                   /**< Y coordinate on the map. */
        int _level;                   /**< Elevation level (1-8). */
        int _orientation;             /**< Facing direction (1=N..4=W). */

        std::map<std::string, int> _inventory; /**< Held resources by name. */
        std::queue<std::string> _commandQueue; /**< Buffered commands (max 10). */
        int _actionTicksLeft = -1;    /**< Ticks until current action ends (-1 idle). */
        int _foodTicks;               /**< Ticks of food remaining. */
        bool _inIncantation = false;  /**< Incantation-in-progress flag. */
};

#endif /* !AI_HPP_ */
