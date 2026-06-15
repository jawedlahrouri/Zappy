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
class INetworkProvider;
class ILogger;

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

struct Egg {
    int id;
    int ownerId;
    std::string teamName;
    int x;
    int y;
    
    int getId() const { return id; }
    int getOwnerId() const { return ownerId; }
};

struct Tile {
    std::array<int, RESOURCE_COUNT> resources = {0, 0, 0, 0, 0, 0, 0};
    std::vector<int> playerIds;
    std::vector<std::shared_ptr<Egg>> eggs;
    
    const std::vector<std::shared_ptr<Egg>>& getEggs() const { return eggs; }
};

class GameEngine {
    public:
        GameEngine(int width, int height, ILogger& logger);
        ~GameEngine() = default;

        void update(INetworkProvider& network);
        void updateAi(std::shared_ptr<AI> ai, INetworkProvider& network, std::vector<int>& clientsToRemove);

        int getWidth() const { return _width; }
        int getHeight() const { return _height; }
        
        const Tile& getTile(int x, int y) const;
        void addResourceToTile(int x, int y, ResourceType type, int quantity);
        void removeResourceFromTile(int x, int y, ResourceType type, int quantity);
        void removeEgg(int eggId);
        void moveAi(int aiId, int oldX, int oldY, int newX, int newY);

        void generateInitialResources();
        void spawnResourcesCycle();

    private:
        bool handleAiSurvival(int fd, std::shared_ptr<AI> ai, INetworkProvider& network, std::vector<int>& clientsToRemove);
        void handleAiActiveCommand(std::shared_ptr<AI> ai, INetworkProvider& network);
        void handleAiNextCommand(std::shared_ptr<AI> ai, INetworkProvider& network);
        void executeAiAction(std::shared_ptr<AI> ai, INetworkProvider& network);
        int getCommandDuration(const std::string& command);

        ILogger& _logger;
        int _width;
        int _height;

        std::vector<std::vector<Tile>> _map;
        int _resourceSpawnTicks = 0;
        const std::array<double, RESOURCE_COUNT> _densities = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
};

#endif /* !GAME_ENGINE_HPP_ */