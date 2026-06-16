/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Map.cpp
*/

#include "domain/Map/Map.hpp"
#include <algorithm>
#include <stdexcept>
#include <random>


int Egg::getId() const {
    return id;
}


int Egg::getOwnerId() const {
    return ownerId;
}

void Tile::addEgg(std::shared_ptr<Egg> egg) {
    eggs.push_back(egg);
}

void Tile::removeEgg(int eggId) {
    auto it = std::remove_if(eggs.begin(), eggs.end(),
        [eggId](const std::shared_ptr<Egg>& egg) { return egg->getId() == eggId; });
    if (it != eggs.end()) {
        eggs.erase(it, eggs.end());
    }
}

const std::vector<std::shared_ptr<Egg>>& Tile::getEggs() const {
    return eggs;
}

Map::Map(int width, int height) : _width(width), _height(height)
{
    _grid.resize(height);
    for (int i = 0; i < height; ++i) {
        _grid[i].resize(width);
    }
}

int Map::getWidth() const { return _width; }
int Map::getHeight() const { return _height; }

std::pair<int, int> Map::borderWrap(int x, int y) const
{
    int toric_x = (x % _width + _width) % _width;
    int toric_y = (y % _height + _height) % _height;
    return {toric_x, toric_y};
}

Tile& Map::getTile(int x, int y)
{
    auto [toric_x, toric_y] = borderWrap(x, y);
    return _grid[toric_y][toric_x];
}

const Tile& Map::getTile(int x, int y) const
{
    auto [toric_x, toric_y] = borderWrap(x, y);
    return _grid[toric_y][toric_x];
}

void Map::addResourceToTile(int x, int y, ResourceType type, int quantity)
{
    auto [toric_x, toric_y] = borderWrap(x, y);
    _grid[toric_y][toric_x].resources[type] += quantity;
}

void Map::removeResourceFromTile(int x, int y, ResourceType type, int quantity)
{
    auto [toric_x, toric_y] = borderWrap(x, y);
    if (_grid[toric_y][toric_x].resources[type] >= quantity) {
        _grid[toric_y][toric_x].resources[type] -= quantity;
    }
}

void Map::addEgg(std::shared_ptr<Egg> egg)
{
    if (!egg) return;
    auto [toric_x, toric_y] = borderWrap(egg->x, egg->y);
    _grid[toric_y][toric_x].addEgg(egg);
}

void Map::removeEgg(int eggId)
{
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            _grid[y][x].removeEgg(eggId);
        }
    }
}

void Map::addAiToTile(int aiId, int x, int y)
{
    auto [toric_x, toric_y] = borderWrap(x, y);
    _grid[toric_y][toric_x].playerIds.push_back(aiId);
}

void Map::moveAi(int aiId, int oldX, int oldY, int newX, int newY)
{
    auto [toric_old_x, toric_old_y] = borderWrap(oldX, oldY);
    auto [toric_new_x, toric_new_y] = borderWrap(newX, newY);

    auto& oldPlayers = _grid[toric_old_y][toric_old_x].playerIds;
    auto it = std::find(oldPlayers.begin(), oldPlayers.end(), aiId);
    if (it != oldPlayers.end()) {
        oldPlayers.erase(it);
    }
    
    _grid[toric_new_y][toric_new_x].playerIds.push_back(aiId);
}

std::shared_ptr<Egg> Map::getAndRemoveRandomEggForTeam(const std::string& teamName)
{
    std::vector<std::shared_ptr<Egg>> teamEggs;
    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            for (auto &egg : _grid[y][x].getEggs()) {
                if (egg->teamName == teamName) {
                    teamEggs.push_back(egg);
                }
            }
        }
    }
    if (teamEggs.empty()) {
        return nullptr;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, teamEggs.size() - 1);
    int idx = dis(gen);
    
    auto selected = teamEggs[idx];
    removeEgg(selected->getId());
    return selected;
}

void Map::removeAiFromTile(int aiId, int x, int y)
{
    auto [toric_x, toric_y] = borderWrap(x, y);
    auto& players = _grid[toric_y][toric_x].playerIds;
    auto it = std::find(players.begin(), players.end(), aiId);
    if (it != players.end()) {
        players.erase(it);
    }
}
