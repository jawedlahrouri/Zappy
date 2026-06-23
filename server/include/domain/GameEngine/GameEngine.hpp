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
#include <unordered_map>

class AI;
class INetworkProvider;
class ILogger;

#include "domain/Map/Map.hpp"

class GameEngine {
    public:
        GameEngine(int width, int height, ILogger& logger);
        ~GameEngine() = default;

        void update(INetworkProvider& network);
        void updateAi(std::shared_ptr<AI> ai, INetworkProvider& network, std::vector<int>& clientsToRemove);
        bool checkIncantationStart(std::shared_ptr<AI> ai, INetworkProvider& network);

        int getWidth() const;
        int getHeight() const;
        
        Map& getMap();
        const Map& getMap() const;

        int generateEggId() { return _nextEggId++; }

        bool checkIncantationRequirements(int x, int y, int level) const;
        int getRequiredPlayersForIncantation(int level) const;
        std::vector<std::shared_ptr<AI>> getPlayersOnTileForIncantation(int x, int y, int level, const INetworkProvider& network) const;
        void consumeIncantationResources(int x, int y, int level);
        void refundIncantationResources(int x, int y, int level);
        void checkVictory(const std::string& teamName, INetworkProvider& network) const;

        void generateInitialResources();
        void generateInitialEggs(const std::vector<std::string>& teamNames, int initialSlots);
        void spawnResourcesCycle();

    private:
        bool handleAiSurvival(int fd, std::shared_ptr<AI> ai, INetworkProvider& network, std::vector<int>& clientsToRemove);
        void handleAiActiveCommand(std::shared_ptr<AI> ai, INetworkProvider& network);
        void handleAiNextCommand(std::shared_ptr<AI> ai, INetworkProvider& network);
        void executeAiAction(std::shared_ptr<AI> ai, INetworkProvider& network);

        struct IncantationRequirement {
            int players;
            std::array<int, RESOURCE_COUNT> resources;
        };
        static const std::unordered_map<int, IncantationRequirement> _incantationReqs;

        ILogger& _logger;
        int _width;
        int _height;

        Map _map;
        int _resourceSpawnTicks = 0;
        int _nextEggId = 0;
        const std::array<double, RESOURCE_COUNT> _densities = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
};

#endif /* !GAME_ENGINE_HPP_ */