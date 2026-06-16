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
        int teamColor = 2; // 1: blue, 2: red, 3: green, 4: pink
    };

    struct Egg {
        int id;
        int x;
        int y;
        int playerParentId;
        bool isHatched; // false = pondu, true = éclos/prêt
    };

    class GameWorld {
    public:
        GameWorld();
        ~GameWorld();

        void setMapSize(int width, int height);
        void addTeamName(const std::string& teamName);
        void setTileResources(int x, int y, const Resources& res);
        Resources resToResources(int q0, int q1, int q2, int q3, int q4, int q5, int q6);
        void addPlayer(const Player& player);
        void movePlayer(int id, int x, int y, int orientation);
        void removePlayer(int id);
        void addEgg(int id, int parentId, int x, int y);
        void hatchEgg(int id);
        void removeEgg(int id);

        int getWidth() const;
        int getHeight() const;
        const std::vector<std::vector<Tile>>& getMap() const;
        const std::map<int, Player>& getPlayers() const;
        const std::map<int, Egg>& getEggs() const;

    private:
        int getTeamColor(const std::string& teamName);
        int _width = 0;
        int _height = 0;
        std::vector<std::string> _teamNames;
        std::vector<std::vector<Tile>> _map;
        std::map<int, Player> _players;
        std::map<int, Egg> _eggs;
    };

#endif /* !GAMEWORLD_HPP_ */
