/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GuiClientHandler.hpp
*/

#ifndef GUICLIENTHANDLER_HPP_
#define GUICLIENTHANDLER_HPP_

#include "application/Server/IClientDataHandler.hpp"

/**
 * @class GuiClientHandler
 * @brief Strategy handling input from a GUI client.
 *
 * Concrete @ref IClientDataHandler that parses each complete GUI command line
 * and executes it immediately (GUI commands have no tick cost).
 */
class GuiClientHandler : public IClientDataHandler {
    public:
        GuiClientHandler() = default;
        ~GuiClientHandler() override = default;

        /** @brief Parses and runs each complete command line from the GUI. */
        void handleData(std::shared_ptr<Client> client, Server& server) override;
};

#endif /* !GUICLIENTHANDLER_HPP_ */
