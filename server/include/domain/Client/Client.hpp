/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Client.hpp
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>
#include <vector>
#include <memory>
#include "Socket/Socket.hpp"
#include "shared/CircularBuffer.hpp"

class GameEngine;
class Server;

enum class ClientType {
    PENDING,
    AI,
    GUI
};

class Client {
    public:
        Client(Socket clientSocket);
        virtual ~Client() = default;
        Socket socket;
        ClientType type;

        void append_to_buffer(const std::string& data);
        bool extract_line(std::string& line);
        void append_to_write_buffer(const std::string& data);
        std::string get_write_buffer() const;
        bool has_write_data() const;
        void clear_write_buffer();
        void remove_from_write_buffer(size_t len);

        virtual void update(std::shared_ptr<Client>, Server&, GameEngine&, std::vector<int>&) {}

    private:
        CircularBuffer _read_buffer;
        CircularBuffer _write_buffer;
};

#endif /* !CLIENT_HPP_ */
