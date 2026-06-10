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

class Server {
    public:
        Server();
        ~Server();

        void open_server(int port, int clientsNb, const std::vector<std::string> &teamNames, int freq, int width, int height);
        void remove_client(int client_socket);
        const std::unordered_map<int, std::shared_ptr<Client>>& get_clients() const { return _clients; }
        void add_poll_event(int fd, short events) { _poll.add_events(fd, events); }
        int get_clients_nb() const { return _clientsNb; }
        int get_freq() const { return _freq; }
        const std::vector<std::string> &get_team_names() const { return _teamNames; }
        const std::unique_ptr<GameEngine>& get_game_engine() const { return _game_engine; }
        void send_event(const class IEventElement &event);

    private:
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