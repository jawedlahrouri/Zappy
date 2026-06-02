/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** NetworkManager
*/

#include "NetworkManager.hpp"

NetworkManager::NetworkManager() : _isConnected(false)
{

}

NetworkManager::~NetworkManager()
{
    if (isConnected() == true)
        _socket.disconnect();
}

bool NetworkManager::isConnected() const {
    return _isConnected;
}

bool NetworkManager::connectToServer(const std::string& ip, unsigned short port) {
    if (_socket.connect(ip, port) == sf::Socket::Done) {
        _isConnected = true;
        std::cout << "Connected to server " << ip << ":" << port << std::endl;
        return true;
    }

    std::cerr << "Failed to connect to server " << ip << ":" << port << std::endl;
    return false;
}

bool NetworkManager::authenticateAsGraphic() {
    if (!_isConnected)
        return false;

    std::vector<char> rawBuffer(1024);
    std::size_t received = 0;

    sf::Socket::Status status = _socket.receive(rawBuffer.data(), rawBuffer.size(), received);

    if (status == sf::Socket::Done) {
        _buffer.append(rawBuffer.data(), received);
    }

    if (_buffer != "WELCOME\n") {
        std::cerr << "error in authenticate as graphic" << std::endl;
        return false;
    }

    std::size_t pos = _buffer.find('\n');

    if (pos != std::string::npos) {
        std::string command = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 1);
    }

    std::string message = "GRAPHIC\n";
    _socket.send(message.c_str(), message.size());

    _socket.setBlocking(false);

    return true;
}

std::string NetworkManager::receiveData() {
    if (!_isConnected)
        return "";

    std::vector<char> rawBuffer(1024);
    std::size_t received = 0;
    
    sf::Socket::Status status = _socket.receive(rawBuffer.data(), rawBuffer.size(), received);

    if (status == sf::Socket::Done) {
        _buffer.append(rawBuffer.data(), received);
    } 
    else if (status == sf::Socket::Disconnected) {
        std::cout << "[Graphic] server is disconnected" << std::endl;
        _isConnected = false;
        return "";
    }

    std::size_t pos = _buffer.find('\n');

    if (pos != std::string::npos) {
        std::string command = _buffer.substr(0, pos);
        _buffer.erase(0, pos + 1);
        return command;
    }

    return "";
}
