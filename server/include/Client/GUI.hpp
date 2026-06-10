/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GUI.hpp
*/

#ifndef GUI_HPP_
#define GUI_HPP_

#include "Client/Client.hpp"

class GUI : public Client {
    public:
        GUI(Socket clientSocket);
        void update(std::shared_ptr<Client>, Server&, GameEngine&, std::vector<int>&) override {}
};

#endif /* !GUI_HPP_ */
