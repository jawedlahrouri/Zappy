#include <memory>
class GameEngine;
/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** INetworkProvider.hpp
*/

#ifndef INETWORK_PROVIDER_HPP_
#define INETWORK_PROVIDER_HPP_

#include <string>
#include <vector>
#include <unordered_map>

#include "IPoll.hpp"

/**
 * @interface INetworkProvider
 * @brief Unified network/game API exposed to command and event handlers.
 *
 * @par Design Pattern: Facade (+ Dependency Inversion)
 * Gives command parsers, visitors and event visitors a single, simple entry
 * point to everything they need — sending replies, broadcasting GUI events,
 * reading the client list, the game engine and configuration — without exposing
 * the @c Server internals. @c Server is the concrete implementation; handlers
 * receive it only as this interface, which keeps them decoupled and testable.
 */
class INetworkProvider {
    public:
        virtual ~INetworkProvider() = default;

        /** @brief Returns the configured maximum number of AI clients per team. */
        virtual int getClientsNb() const = 0;
        /** @brief Sets the maximum number of AI clients per team. */
        virtual void setClientsNb(int nb) = 0;
        /** @brief Returns the game frequency (ticks per second). */
        virtual int getFreq() const = 0;
        /** @brief Returns the list of team names. */
        virtual const std::vector<std::string>& getTeamNames() const = 0;
        /** @brief Returns the game engine owning the map and simulation state. */
        virtual const std::unique_ptr<GameEngine>& getGameEngine() const = 0;
        /**
         * @brief Serialises a GUI event and dispatches it to the GUI client(s).
         * @param event Event element to visit and send.
         */
        virtual void sendEvent(const class IEventElement& event) = 0;
        /** @brief Returns all connected clients keyed by socket fd. */
        virtual const std::unordered_map<int, std::shared_ptr<class Client>>& getClients() const = 0;
        /** @brief Returns the server logger. */
        virtual class ILogger& getLogger() const = 0;
        /**
         * @brief Arms an additional poll event on a client fd (e.g. POLLOUT).
         * @param fd Client file descriptor.
         * @param event Event to start watching.
         */
        virtual void addPollEvent(int fd, NetworkEvent event) = 0;


        /**
         * @brief Queues a textual response to a single client.
         * @param clientId Target client socket fd.
         * @param message Message to send (newline appended if missing).
         */
        virtual void sendResponse(int clientId, const std::string& message) = 0;
        /**
         * @brief Sends a message to every connected GUI client.
         * @param message Pre-formatted GUI message.
         */
        virtual void sendToGuis(const std::string& message) = 0;
        /**
         * @brief Broadcasts a message to all clients except one.
         * @param message Message to broadcast.
         * @param excludeClientId Client to skip (@c -1 to exclude none).
         */
        virtual void broadcastEvent(const std::string& message, int excludeClientId = -1) = 0;
        /**
         * @brief Parses and executes an AI command for @p ai.
         * @param ai AI client issuing the command.
         * @param command Raw command line.
         */
        virtual void executeCommand(std::shared_ptr<class AI> ai, const std::string& command) = 0;
        /**
         * @brief Notifies all GUIs that a team has won.
         * @param teamName Winning team.
         */
        virtual void notifyVictory(const std::string& teamName) = 0;
        /**
         * @brief Returns the tick cost of a given AI command.
         * @param command Command name.
         * @return Duration in time units, before division by the frequency.
         */
        virtual int getCommandDuration(const std::string& command) const = 0;
        /**
         * @brief Disconnects a client and releases its resources.
         * @param clientId Client socket fd to remove.
         */
        virtual void removeClient(int clientId) = 0;
};

#endif /* !INETWORK_PROVIDER_HPP_ */
