/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameWorld
*/

#include "GameWorld.hpp"

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}

void GameWorld::setMapSize(int width, int height) 
{
    _width = width;
    _height = height;

    _map.resize(_width);
    for (int x = 0; x < _width; x++) {
        _map[x].resize(_height);
        for (int y = 0; y < _height; y++) {
            _map[x][y].x = x;
            _map[x][y].y = y;
        }
    }
}

void GameWorld::setTileResources(int x, int y, const Resources& res)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        _map[x][y].resources = res;
    }
}

Resources GameWorld::resToResources(int q0, int q1, int q2, int q3, int q4, int q5, int q6)
{
    Resources res;
    res.food = q0;
    res.linemate = q1;
    res.deraumere = q2;
    res.sibur = q3;
    res.mendiane = q4;
    res.phiras = q5;
    res.thystame = q6;
    return res;
}


void GameWorld::addPlayer(const Player& player) {
    _players[player.id] = player;
    std::cout << "[Graphic] Joueur #" << player.id << " ajouté à la position (" << player.x << ", " << player.y << ")" << std::endl;
}

void GameWorld::movePlayer(int id, int x, int y, int orientation)
{
    auto player = _players.find(id);

    if (player == _players.end())
        return;

    Player newPlayer = player->second;
    newPlayer.x = x;
    newPlayer.y = y;
    newPlayer.orientation = orientation;
    _players[id] = newPlayer;
}

void GameWorld::removePlayer(int id)
{
    _players.erase(id);
}

int GameWorld::getWidth() const
{
    return _width;
}
int GameWorld::getHeight() const
{
    return _height;
}
const std::vector<std::vector<Tile>>& GameWorld::getMap() const
{
    return _map;
}
const std::map<int, Player>& GameWorld::getPlayers() const
{
    return _players;
}
