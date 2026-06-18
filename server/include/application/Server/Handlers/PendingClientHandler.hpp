/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PendingClientHandler.hpp
*/

#ifndef PENDINGCLIENTHANDLER_HPP_
#define PENDINGCLIENTHANDLER_HPP_

#include "application/Server/IClientDataHandler.hpp"

class PendingClientHandler : public IClientDataHandler {
    public:
        PendingClientHandler() = default;
        ~PendingClientHandler() override = default;

        void handleData(std::shared_ptr<Client> client, Server& server) override;
};

#endif /* !PENDINGCLIENTHANDLER_HPP_ */
