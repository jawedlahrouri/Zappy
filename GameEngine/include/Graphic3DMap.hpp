/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DMap
*/

#ifndef GRAPHIC3DMAP_HPP_
    #define GRAPHIC3DMAP_HPP_

    #include "GameWorld.hpp"
    #include <optional>
    #include <raylib.h>
    #include <utility>

class Graphic3DMap {
    public:
        void draw(const GameWorld& world,
            const std::optional<std::pair<int, int>>& selectedTile) const;
        void drawPreview() const;
        std::optional<std::pair<int, int>> pickTile(const GameWorld& world,
            const Camera3D& camera, Vector2 mousePosition) const;

    private:
        static constexpr float TILE_PICK_EPSILON = 0.0001f;

        void drawTiles(const GameWorld& world,
            const std::optional<std::pair<int, int>>& selectedTile) const;
        void drawResources(const Tile& tile, int width, int height) const;
        void drawEggs(const GameWorld& world) const;
        Color getTileColor(const Tile& tile) const;
        int getResourceTotal(const Resources& resources) const;
};

#endif /* !GRAPHIC3DMAP_HPP_ */
