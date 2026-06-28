/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DMap
*/

/**
 * @file Graphic3DMap.hpp
 * @brief Declares 3D map drawing and tile picking helpers.
 */

#ifndef GRAPHIC3DMAP_HPP_
    #define GRAPHIC3DMAP_HPP_

    #include "GameWorld.hpp"
    #include <optional>
    #include <raylib.h>
    #include <utility>

/**
 * @brief Draws the 3D tile grid, resources, and eggs.
 */
class Graphic3DMap {
    public:
        /**
         * @brief Draws the full 3D map.
         */
        void draw(const GameWorld& world,
            const std::optional<std::pair<int, int>>& selectedTile) const;

        /**
         * @brief Draws an ambient preview used by the menu.
         */
        void drawPreview() const;

        /**
         * @brief Picks the tile under a mouse cursor.
         * @return Tile coordinates when the cursor intersects the map.
         */
        std::optional<std::pair<int, int>> pickTile(const GameWorld& world,
            const Camera3D& camera, Vector2 mousePosition) const;

    private:
        /** Epsilon used to avoid division artifacts during tile picking. */
        static constexpr float TILE_PICK_EPSILON = 0.0001f;

        /**
         * @brief Draws map tiles.
         */
        void drawTiles(const GameWorld& world,
            const std::optional<std::pair<int, int>>& selectedTile) const;

        /**
         * @brief Draws resources standing on a tile.
         */
        void drawResources(const Tile& tile, int width, int height) const;

        /**
         * @brief Draws eggs for the current world.
         */
        void drawEggs(const GameWorld& world) const;

        /**
         * @brief Computes a tile color from its resources.
         */
        Color getTileColor(const Tile& tile) const;

        /**
         * @brief Counts resources stored on a tile.
         */
        int getResourceTotal(const Resources& resources) const;
};

#endif /* !GRAPHIC3DMAP_HPP_ */
