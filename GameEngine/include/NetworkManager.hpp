/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** NetworkManager
*/

#ifndef NETWORKMANAGER_HPP_
#define NETWORKMANAGER_HPP_

#include <SFML/Network.hpp>
#include <iostream>

class NetworkManager {
    public:
        NetworkManager();
        ~NetworkManager();
        bool connectToServer(const std::string &ip, unsigned short port);
        bool authenticateAsGraphic();
        std::string receiveData();
        
        bool isConnected() const;

    private:
        sf::TcpSocket _socket;
        bool _isConnected;
        std::string _buffer;
};

#endif /* !NETWORKMANAGER_HPP_ */
