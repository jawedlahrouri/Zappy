/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GUI.hpp
*/

/**
 * @file GUI.hpp
 * @brief Graphical observer client.
 */

#ifndef GUI_HPP_
#define GUI_HPP_

#include "Client.hpp"

/**
 * @class GUI
 * @brief A graphical observer; receives world events but runs no game logic.
 *
 * Concrete @ref Client whose @ref update is an intentional no-op: a GUI never
 * advances the simulation, it only displays the events the server pushes to it.
 */
class GUI : public Client {
    public:
        /**
         * @brief Builds a GUI client around an accepted socket.
         * @param clientSocket Owned socket for this client.
         * @param id Unique client id (socket fd).
         */
        GUI(std::unique_ptr<ISocket> clientSocket, int id);

        /** @brief No-op tick hook: a GUI has no per-tick state to advance. */
        void update(std::shared_ptr<Client>, INetworkProvider&, GameEngine&, std::vector<int>&) override {}
};

#endif /* !GUI_HPP_ */
