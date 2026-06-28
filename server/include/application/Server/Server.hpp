/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Server.hpp
*/

/**
 * @file Server.hpp
 * @brief The Zappy server: owns the loop, clients, game engine and I/O.
 */

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <atomic>
#include "ISocket.hpp"
#include "shared/ILogger.hpp"
#include "IPoll.hpp"

#include "INetworkProvider.hpp"
#include "application/Server/ClientManager.hpp"
#include "application/Server/ConnectionHandler.hpp"
#include "application/Server/IClientDataHandler.hpp"
#include <unordered_map>

class GameEngine;

/**
 * @class Server
 * @brief Central object running the event loop and orchestrating the game.
 *
 * @par Design Pattern: Facade + Dependency Injection + Strategy
 * - @b Facade: implements @ref INetworkProvider, the single API the command and
 *   event layers use to talk back to clients and the game.
 * - @b Dependency Injection: the logger, master @ref ISocket and @ref IPoll are
 *   injected at construction, so the server is testable with fakes.
 * - @b Strategy: per-client-type data handlers are stored in @ref _dataHandlers
 *   and chosen at runtime from the client's @ref ClientType.
 *
 * The main loop (@ref runPollLoop) multiplexes I/O via @ref IPoll and advances
 * the @ref GameEngine at a fixed frequency. See @ref IPoll for the poll details.
 */
class Server : public INetworkProvider {
    public:
        /**
         * @brief Builds the server with its injected dependencies.
         * @param logger Logger for server output.
         * @param masterSocket Listening socket (ownership taken).
         * @param poll I/O multiplexer (ownership taken).
         */
        Server(ILogger& logger, std::unique_ptr<ISocket> masterSocket, std::unique_ptr<IPoll> poll);
        ~Server() override;

        /**
         * @brief Opens the listening socket and runs the main loop (blocking).
         * @param port TCP port to listen on.
         * @param clientsNb Starting slots (eggs) per team.
         * @param teamNames Participating team names.
         * @param freq Game frequency in ticks per second.
         * @param width Map width.
         * @param height Map height.
         */
        void openServer(int port, int clientsNb, const std::vector<std::string>& teamNames, int freq, int width, int height);

        int getClientsNb() const override;
        void setClientsNb(int nb) override;
        int getFreq() const override;
        const std::vector<std::string>& getTeamNames() const override;
        const std::unique_ptr<GameEngine>& getGameEngine() const override;
        ILogger& getLogger() const override;

        /** @brief Returns the client manager owning all connected clients. */
        ClientManager& getClientManager();
        /** @brief Sets the game frequency (ticks per second). */
        void setFreq(int freq);

        // INetworkProvider Implementation
        void sendResponse(int clientId, const std::string& message) override;
        void sendToGuis(const std::string& message) override;
        void broadcastEvent(const std::string& message, int excludeClientId = -1) override;
        void sendEvent(const class IEventElement& event) override;
        const std::unordered_map<int, std::shared_ptr<class Client>>& getClients() const override;
        void addPollEvent(int fd, NetworkEvent event) override;
        void executeCommand(std::shared_ptr<class AI> ai, const std::string& command) override;
        void notifyVictory(const std::string& teamName) override;
        int getCommandDuration(const std::string& command) const override;

    private:
        /** @brief Installs the SIGINT/SIGTERM handlers requesting a clean shutdown. */
        void installShutdownHandlers();
        /** @brief Async-signal-safe handler: flags that a shutdown was requested. */
        static void requestShutdown(int sig);
        /** @brief Set by @ref requestShutdown, polled by @ref runPollLoop to exit. */
        inline static std::atomic_flag _shutdownRequested{};

        /** @brief Main loop: multiplex I/O then advance the game (see @ref IPoll). */
        void runPollLoop();
        /** @brief Dispatches the poll result for the entry at @p i. */
        void processPollEvent(size_t i);
        /** @brief Reads and routes data from the client at @p pollIndex. */
        bool handleClientData(size_t pollIndex);
        /** @brief Flushes pending output for the client at @p index. */
        bool handleWritable(size_t index);
        /** @brief Closes the listening socket and releases resources. */
        void cleanup();
        void removeClient(int clientSocket) override;

        /** @brief Computes the poll timeout keeping I/O in step with ticks. */
        int calculateTimeout(const std::chrono::steady_clock::time_point& next_tick_time) const;
        /** @brief Polls once and dispatches every ready descriptor. */
        void handlePollEvents(int timeout_ms);
        /** @brief Resets the tick clock when the population changes. */
        void synchronizeTickTime(bool& was_empty, std::chrono::steady_clock::time_point& next_tick_time, const std::chrono::microseconds& tick_duration) const;
        /** @brief Runs all game ticks that are due. */
        void runGameTick(std::chrono::steady_clock::time_point& next_tick_time, const std::chrono::microseconds& tick_duration);

        ILogger& _logger;                          /**< Server logger. */
        std::unique_ptr<ISocket> _masterSocket;    /**< Listening socket. */
        std::unique_ptr<IPoll> _poll;              /**< I/O multiplexer. */
        ClientManager _clientManager;              /**< Connected clients. */
        std::unique_ptr<GameEngine> _gameEngine;   /**< Game simulation. */

        int _port = 0;                       /**< Listening port. */
        int _clientsNb = 0;                  /**< Starting slots per team. */
        std::vector<std::string> _teamNames; /**< Team names. */
        int _freq = 0;                       /**< Ticks per second. */

        /** @brief Per-type data-handling strategies (PENDING/AI/GUI). */
        std::unordered_map<ClientType, std::unique_ptr<IClientDataHandler>> _dataHandlers;
};

#endif /* !SERVER_HPP_ */
