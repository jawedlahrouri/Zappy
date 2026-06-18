/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AiClientHandler.hpp
*/

#ifndef AICLIENTHANDLER_HPP_
#define AICLIENTHANDLER_HPP_

#include "application/Server/IClientDataHandler.hpp"

class AiClientHandler : public IClientDataHandler {
    public:
        AiClientHandler() = default;
        ~AiClientHandler() override = default;

        void handleData(std::shared_ptr<Client> client, Server& server) override;
};

#endif /* !AICLIENTHANDLER_HPP_ */
