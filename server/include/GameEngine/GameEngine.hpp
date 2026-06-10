/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameEngine.hpp
*/

#ifndef GAME_ENGINE_HPP_
#define GAME_ENGINE_HPP_

#include <vector>
#include <memory>
#include <string>

class AI;
class Server;

class GameEngine {
    public:
        GameEngine(int width, int height);
        ~GameEngine() = default;

        void update(Server &server);

        int get_width() const { return _width; }
        int get_height() const { return _height; }

    private:
        bool handle_ai_survival(int fd, std::shared_ptr<AI> ai, Server &server, std::vector<int> &clients_to_remove);
        void handle_ai_active_command(std::shared_ptr<AI> ai, Server &server);
        void handle_ai_next_command(std::shared_ptr<AI> ai, Server &server);
        void execute_ai_action(std::shared_ptr<AI> ai, Server &server);
        int get_command_duration(const std::string &command);

        int _width;
        int _height;
};

#endif /* !GAME_ENGINE_HPP_ */
