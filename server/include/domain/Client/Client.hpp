/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Client.hpp
*/

/**
 * @file Client.hpp
 * @brief Base connected client (socket + read/write buffers + tick hook).
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <vector>
#include <memory>
#include "ISocket.hpp"
#include "shared/CircularBuffer.hpp"
#include "INetworkProvider.hpp"

class GameEngine;

/**
 * @enum ClientType
 * @brief Role of a connected client, resolved during the handshake.
 */
enum class ClientType {
    PENDING, /**< Just connected; type not yet determined. */
    AI,      /**< An AI player. */
    GUI      /**< A graphical observer. */
};

/**
 * @class Client
 * @brief A connected peer: owns its socket and its buffered I/O.
 *
 * @par Design Pattern: Template Method
 * @ref update is a virtual hook called once per game tick. The base provides an
 * empty default; @ref AI overrides it to advance the player's command/queue,
 * while @ref GUI keeps it a no-op. The server's tick loop calls @c update
 * uniformly without knowing the concrete client type.
 */
class Client {
    public:
        /**
         * @brief Builds a pending client around an accepted socket.
         * @param clientSocket Owned socket for this client.
         * @param id Unique client id (its socket fd).
         */
        Client(std::unique_ptr<ISocket> clientSocket, int id);
        virtual ~Client() = default;

        /** @brief Returns a mutable reference to the client socket. */
        ISocket& getSocket() { return *_socket; }
        /** @brief Returns a const reference to the client socket. */
        const ISocket& getSocket() const { return *_socket; }
        /** @brief Releases ownership of the socket (e.g. when upgrading type). */
        std::unique_ptr<ISocket> releaseSocket() { return std::move(_socket); }

        /** @brief Returns the unique client id (socket fd). */
        int getId() const { return _id; }

        /** @brief Returns the resolved client role. */
        ClientType getType() const { return _type; }
        /** @brief Sets the client role (e.g. after the handshake). */
        void setType(ClientType newType) { _type = newType; }

        /**
         * @brief Appends received bytes to the read buffer.
         * @param data Raw bytes from the socket.
         * @throws std::overflow_error If the read buffer would overflow.
         */
        void appendToBuffer(const std::string& data);
        /**
         * @brief Extracts one complete '\n'-terminated line from the read buffer.
         * @param[out] line Receives the extracted line (without newline).
         * @return @c true if a full line was available.
         */
        bool extractLine(std::string& line);
        /** @brief Appends data to the pending write buffer. */
        void appendToWriteBuffer(const std::string& data);
        /** @brief Returns a copy of the current write buffer. */
        std::string getWriteBuffer() const;
        /** @brief Tells whether there is buffered data still to send. */
        bool hasWriteData() const;
        /** @brief Empties the write buffer. */
        void clearWriteBuffer();
        /**
         * @brief Drops the first @p len bytes already sent from the write buffer.
         * @param len Number of bytes successfully written.
         */
        void removeFromWriteBuffer(size_t len);

        /**
         * @brief Per-tick update hook (overridden by concrete client types).
         * @param self Shared pointer to this client.
         * @param network Network/game facade.
         * @param engine Game engine for the current tick.
         * @param clientsToRemove Out-list of client ids to disconnect this tick.
         */
        virtual void update(std::shared_ptr<Client> self, INetworkProvider& network, GameEngine& engine, std::vector<int>& clientsToRemove) {
            (void)self; (void)network; (void)engine; (void)clientsToRemove;
        }

    protected:
        std::unique_ptr<ISocket> _socket; /**< Owned client socket. */
        int _id;                          /**< Unique id (socket fd). */
        ClientType _type;                 /**< Resolved client role. */

    private:
        CircularBuffer _readBuffer; /**< Inbound byte buffer (line framing). */
        std::string _writeBuffer;   /**< Outbound pending bytes. */
};

#endif /* !CLIENT_HPP_ */
