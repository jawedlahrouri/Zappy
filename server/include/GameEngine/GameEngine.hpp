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
#include <array>

class AI;
class Server;

enum ResourceType {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    RESOURCE_COUNT
};

struct Tile {
    std::array<int, RESOURCE_COUNT> resources = {0, 0, 0, 0, 0, 0, 0};
    std::vector<int> playerIds;
};

class GameEngine {
    public:
        GameEngine(int width, int height);
        ~GameEngine() = default;

        void update(Server &server);

        int get_width() const { return _width; }
        int get_height() const { return _height; }
        
        const Tile& get_tile(int x, int y) const;
        void add_resource_to_tile(int x, int y, ResourceType type, int quantity);
        void remove_resource_from_tile(int x, int y, ResourceType type, int quantity);

        void generate_initial_resources();
        void spawn_resources_cycle();

    private:
        bool handle_ai_survival(int fd, std::shared_ptr<AI> ai, Server &server, std::vector<int> &clients_to_remove);
        void handle_ai_active_command(std::shared_ptr<AI> ai, Server &server);
        void handle_ai_next_command(std::shared_ptr<AI> ai, Server &server);
        void execute_ai_action(std::shared_ptr<AI> ai, Server &server);
        int get_command_duration(const std::string &command);

        int _width;
        int _height;

        std::vector<std::vector<Tile>> _map;
        int _resource_spawn_ticks = 0;
        const std::array<double, RESOURCE_COUNT> _densities = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
};

#endif /* !GAME_ENGINE_HPP_ */