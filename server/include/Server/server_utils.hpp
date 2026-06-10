/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** server_utils.hpp
*/

#ifndef SERVER_UTILS_HPP_
#define SERVER_UTILS_HPP_

#include <map>
#include <queue>
#include <string>
#include <vector>
#include <memory>
#include "Socket/Socket.hpp"

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
        const std::string& get_write_buffer() const;
        void clear_write_buffer();

        virtual void update(std::shared_ptr<Client>, Server&, GameEngine&, std::vector<int>&) {}

    private:
        std::string _buffer;
        std::string _write_buffer;
};

class AI : public Client {
    public:
        AI(Socket clientSocket, int player_id);
        void update(std::shared_ptr<Client> self, Server& server, GameEngine& engine, std::vector<int>& clients_to_remove) override;
        int id;
        std::string teamName;
        int x = 0;
        int y = 0;
        int level;
        int orientation;
        
        std::map<std::string, int> inventory; 
        std::queue<std::string> commandQueue;
        int action_ticks_left = -1;
        int food_ticks;
};

class GUI : public Client {
    public:
        GUI(Socket clientSocket);
        void update(std::shared_ptr<Client>, Server&, GameEngine&, std::vector<int>&) override {}
};

#endif /* !SERVER_UTILS_HPP_ */
