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
#include "Poll/Poll.hpp"

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
        std::string buffer;
        std::string write_buffer;
};

class AI : public Client {
    public:
        AI(Socket clientSocket, int player_id);
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
        bool consume_food();
};

class GUI : public Client {
    public:
        GUI(Socket clientSocket);
};

class ClockFrequency;

class Server {
    public:
        Server() = default;
        ~Server();

        void open_server(int port, int clientsNb, const std::vector<std::string> &teamNames, int freq);
        void send_to_guis(const std::string &message);
        void send_response(int client_socket, const std::string &message);

    private:
        bool get_next_line(std::string &buffer, std::string &line);
        void detect_client_type(int client_socket);
        void run_poll_loop(ClockFrequency &clock);
        void update_game();
        bool handle_ai_survival(int fd, std::shared_ptr<AI> ai, std::vector<int> &clients_to_remove);
        void handle_ai_active_command(std::shared_ptr<AI> ai);
        void handle_ai_next_command(std::shared_ptr<AI> ai);
        void execute_ai_action(std::shared_ptr<AI> client);
        int get_command_duration(const std::string &command);
        void process_poll_event(size_t &i);
        void accept_client();
        void remove_client(int client_socket);
        bool handle_client_data(size_t poll_index);
        bool handle_writable(size_t index);
        void broadcast_event(const std::string &message, int exclude_socket = -1);
        void cleanup();

        Socket _master_socket;
        Poll _poll;
        std::unordered_map<int, std::shared_ptr<Client>> _clients;

        int _port = 0;
        int _clientsNb = 0;
        std::vector<std::string> _teamNames;
};

#endif /* !SERVER_HPP_ */