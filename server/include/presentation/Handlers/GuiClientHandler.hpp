/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GuiClientHandler.hpp
*/

#ifndef GUICLIENTHANDLER_HPP_
#define GUICLIENTHANDLER_HPP_

#include "application/Server/IClientDataHandler.hpp"

class GuiClientHandler : public IClientDataHandler {
    public:
        GuiClientHandler() = default;
        ~GuiClientHandler() override = default;

        void handleData(std::shared_ptr<Client> client, Server& server) override;
};

#endif /* !GUICLIENTHANDLER_HPP_ */
