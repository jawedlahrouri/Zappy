/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AiClientHandler.hpp
*/

#ifndef AICLIENTHANDLER_HPP_
#define AICLIENTHANDLER_HPP_

#include "application/Server/IClientDataHandler.hpp"

/**
 * @class AiClientHandler
 * @brief Strategy handling input from an AI client.
 *
 * Concrete @ref IClientDataHandler that extracts complete command lines and
 * enqueues them on the AI's command queue (bounded), enforcing the protocol's
 * pending-command limit.
 */
class AiClientHandler : public IClientDataHandler {
    public:
        AiClientHandler() = default;
        ~AiClientHandler() override = default;

        /** @brief Queues each complete command line read from the AI client. */
        void handleData(std::shared_ptr<Client> client, Server& server) override;
};

#endif /* !AICLIENTHANDLER_HPP_ */
