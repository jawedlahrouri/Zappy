/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Map.hpp
*/

#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>
#include <memory>
#include <string>
#include <array>
#include <unordered_set>

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
    
    int getId() const;
    int getOwnerId() const;
};

struct Tile {
    std::array<int, RESOURCE_COUNT> resources = {0, 0, 0, 0, 0, 0, 0};
    std::vector<int> playerIds;
    std::vector<std::shared_ptr<Egg>> eggs;
    
    void addEgg(std::shared_ptr<Egg> egg);
    void removeEgg(int eggId);
    const std::vector<std::shared_ptr<Egg>>& getEggs() const;
};

class Map {
    public:
        Map(int width, int height);
        ~Map() = default;

        int getWidth() const;
        int getHeight() const;

        Tile& getTile(int x, int y);
        const Tile& getTile(int x, int y) const;

        void addResourceToTile(int x, int y, ResourceType type, int quantity);
        void removeResourceFromTile(int x, int y, ResourceType type, int quantity);
        
        const std::unordered_set<int>& getDirtyTiles() const;
        void clearDirtyTiles();

        void addEgg(std::shared_ptr<Egg> egg);
        void removeEgg(int eggId);
        std::shared_ptr<Egg> getAndRemoveRandomEggForTeam(const std::string& teamName);

        void addAiToTile(int aiId, int x, int y);
        void removeAiFromTile(int aiId, int x, int y);
        void moveAi(int aiId, int oldX, int oldY, int newX, int newY);
        
        std::pair<int, int> borderWrap(int x, int y) const;

    private:
        int _width;
        int _height;
        std::vector<std::vector<Tile>> _grid;
        std::unordered_set<int> _dirtyTiles;
};

#endif /* !MAP_HPP_ */
