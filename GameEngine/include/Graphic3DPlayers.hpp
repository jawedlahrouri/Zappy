/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DPlayers
*/

/**
 * @file Graphic3DPlayers.hpp
 * @brief Declares 3D player rendering and picking helpers.
 */

#ifndef GRAPHIC3DPLAYERS_HPP_
    #define GRAPHIC3DPLAYERS_HPP_

    #include "GameWorld.hpp"
    #include <optional>
    #include <raylib.h>

/**
 * @brief Draws and selects players in the Raylib 3D scene.
 */
class Graphic3DPlayers {
    public:
        /**
         * @brief Draws all players in the world.
         */
        void draw(const GameWorld& world,
            const std::optional<int>& selectedPlayerId) const;

        /**
         * @brief Picks the player under a mouse cursor.
         * @return Player identifier when a player is hit.
         */
        std::optional<int> pickPlayer(const GameWorld& world,
            const Camera3D& camera, Vector2 mousePosition) const;

    private:
        /**
         * @brief Converts a protocol orientation to a 3D direction vector.
         */
        Vector3 getDirection(int orientation) const;

        /**
         * @brief Draws one player marker at a tile position.
         */
        void drawPlayer(const Player& player, const Vector3& tile,
            bool selected) const;
};

#endif /* !GRAPHIC3DPLAYERS_HPP_ */
