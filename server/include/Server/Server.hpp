/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Server.hpp
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <map>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Socket/Socket.hpp"
#include "Parsing/Respond.hpp"
#include "Poll/Poll.hpp"

#include "Client/Client.hpp"
#include "Client/AI.hpp"
#include "Client/GUI.hpp"
#include "shared/ILogger.hpp"

class Server {
    public:
        explicit Server(ILogger& logger);
        ~Server();

        void open_server(int port, int clientsNb, const std::vector<std::string> &teamNames, int freq, int width, int height);
        void remove_client(int client_socket);
        const std::unordered_map<int, std::shared_ptr<Client>>& get_clients() const;
        void add_poll_event(int fd, short events);
        int get_clients_nb() const;
        int get_freq() const;
        const std::vector<std::string> &get_team_names() const;
        const std::unique_ptr<GameEngine>& get_game_engine() const;
        void send_event(const class IEventElement &event);
        ILogger& get_logger() const;

    private:
        ILogger& _logger;
        void detect_client_type(int client_socket);
        void run_poll_loop();
        void process_poll_event(size_t &i);
        void accept_client();
        bool handle_client_data(size_t poll_index);
        bool handle_writable(size_t index);
        void cleanup();

        Socket _master_socket;
        Poll _poll;
        std::unordered_map<int, std::shared_ptr<Client>> _clients;
        std::unique_ptr<GameEngine> _game_engine;

        int _port = 0;
        int _clientsNb = 0;
        std::vector<std::string> _teamNames;
        int _freq = 0;
};

#endif /* !SERVER_HPP_ */