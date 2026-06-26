/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** NetworkManager
*/

/**
 * @file NetworkManager.hpp
 * @brief Declares the TCP client used by the graphical application.
 */

#ifndef NETWORKMANAGER_HPP_
#define NETWORKMANAGER_HPP_

#include <SFML/Network.hpp>
#include <iostream>

/**
 * @brief Lightweight wrapper around the SFML TCP socket used for Zappy.
 */
class NetworkManager {
    public:
        /**
         * @brief Creates a disconnected network manager.
         */
        NetworkManager();

        /**
         * @brief Disconnects from the server if needed.
         */
        ~NetworkManager();

        /**
         * @brief Opens a TCP connection to the server.
         * @param ip Server IP address or hostname.
         * @param port Server port.
         * @return true when the TCP connection succeeded.
         */
        bool connectToServer(const std::string &ip, unsigned short port);

        /**
         * @brief Performs the graphical-client authentication handshake.
         * @return true when the server accepted the GRAPHIC client.
         */
        bool authenticateAsGraphic();

        /**
         * @brief Receives available protocol data from the socket.
         * @return Complete raw data chunk, or an empty string when none is ready.
         */
        std::string receiveData();

        /**
         * @brief Sends one protocol command to the server.
         * @param command Command line to send.
         * @return true when the socket accepted the data.
         */
        bool sendCommand(const std::string& command);

        /**
         * @brief Closes the socket and resets connection state.
         */
        void disconnect();
        
        /**
         * @brief Checks whether the TCP client is connected.
         * @return true when the socket is considered connected.
         */
        bool isConnected() const;

    private:
        sf::TcpSocket _socket;
        bool _isConnected;
        std::string _buffer;
        static constexpr int BUFFER_SIZE = 1024;
};

#endif /* !NETWORKMANAGER_HPP_ */
