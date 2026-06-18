/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IClientDataHandler.hpp
*/

#ifndef ICLIENTDATAHANDLER_HPP_
#define ICLIENTDATAHANDLER_HPP_

#include <memory>

class Client;
class Server;

class IClientDataHandler {
    public:
        virtual ~IClientDataHandler() = default;
        virtual void handleData(std::shared_ptr<Client> client, Server& server) = 0;
};

#endif /* !ICLIENTDATAHANDLER_HPP_ */
