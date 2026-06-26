/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DPlayers
*/

#ifndef GRAPHIC3DPLAYERS_HPP_
    #define GRAPHIC3DPLAYERS_HPP_

    #include "GameWorld.hpp"
    #include <optional>
    #include <raylib.h>

class Graphic3DPlayers {
    public:
        void draw(const GameWorld& world,
            const std::optional<int>& selectedPlayerId) const;
        std::optional<int> pickPlayer(const GameWorld& world,
            const Camera3D& camera, Vector2 mousePosition) const;

    private:
        Vector3 getDirection(int orientation) const;
        void drawPlayer(const Player& player, const Vector3& tile,
            bool selected) const;
};

#endif /* !GRAPHIC3DPLAYERS_HPP_ */
