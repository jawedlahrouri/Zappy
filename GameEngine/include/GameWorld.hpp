/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameWorld
*/

#ifndef GAMEWORLD_HPP_
    #define GAMEWORLD_HPP_

    #include <vector>
    #include <string>
    #include <map>
    #include <iostream>

    struct Resources {
        int food = 0;
        int linemate = 0;
        int deraumere = 0;
        int sibur = 0;
        int mendiane = 0;
        int phiras = 0;
        int thystame = 0;
    };

    struct Tile {
        int x = 0;
        int y = 0;
        Resources resources;
    };

    struct Player {
        int id = 0;
        int x = 0;
        int y = 0;
        int orientation = 1; // 1: N, 2: E, 3: S, 4: W
        int level = 1;
        std::string teamName;
        Resources inventory;
    };

    class GameWorld {
    public:
        GameWorld();
        ~GameWorld();

        void setMapSize(int width, int height);
        void setTileResources(int x, int y, const Resources& res);
        void addPlayer(const Player& player);
        void movePlayer(int id, int x, int y, int orientation);
        void removePlayer(int id);

        int getWidth() const;
        int getHeight() const;
        const std::vector<std::vector<Tile>>& getMap() const;
        const std::map<int, Player>& getPlayers() const;

    private:
        int _width;
        int _height;
        std::vector<std::vector<Tile>> _map;
        std::map<int, Player> _players;
    };

#endif /* !GAMEWORLD_HPP_ */
