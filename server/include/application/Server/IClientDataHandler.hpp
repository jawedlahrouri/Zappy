/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IClientDataHandler.hpp
*/

/**
 * @file IClientDataHandler.hpp
 * @brief Strategy interface for processing a client's incoming data.
 */

#ifndef ICLIENTDATAHANDLER_HPP_
#define ICLIENTDATAHANDLER_HPP_

#include <memory>

class Client;
class Server;

/**
 * @interface IClientDataHandler
 * @brief Per-client-type strategy for consuming buffered input.
 *
 * @par Design Pattern: Strategy
 * Each @ref ClientType has its own concrete handler (PendingClientHandler,
 * AiClientHandler, GuiClientHandler). The @ref Server keeps them in a map and
 * picks the right one at runtime from @c client->getType(), so adding a client
 * kind means adding a strategy rather than branching in the server.
 */
class IClientDataHandler {
    public:
        virtual ~IClientDataHandler() = default;

        /**
         * @brief Consumes complete lines from the client's read buffer.
         * @param client Client whose buffered input is processed.
         * @param server Server facade used to reply, dispatch or disconnect.
         */
        virtual void handleData(std::shared_ptr<Client> client, Server& server) = 0;
};

#endif /* !ICLIENTDATAHANDLER_HPP_ */
