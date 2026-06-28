/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DUtils
*/

/**
 * @file Graphic3DUtils.hpp
 * @brief Defines inline utility functions shared by the Raylib renderer.
 */

#ifndef GRAPHIC3DUTILS_HPP_
    #define GRAPHIC3DUTILS_HPP_

    #include "Graphic3DConfig.hpp"
    #include <raylib.h>
    #include <string>

/**
 * @brief Utility class for 3D coordinate and color conversions.
 */
class Graphic3DUtils {
    public:
        /**
         * @brief Computes the world-space center of a tile.
         * @param x Tile X coordinate.
         * @param y Tile Y coordinate.
         * @param width Map width in tiles.
         * @param height Map height in tiles.
         * @return 3D position at the center of the tile top.
         */
        static Vector3 getTileCenter(int x, int y, int width, int height)
        {
            const float step = Graphic3DConfig::TILE_SIZE +
                Graphic3DConfig::TILE_GAP;
            const float originX =
                -((static_cast<float>(width) - 1.0f) * step) / 2.0f;
            const float originZ =
                -((static_cast<float>(height) - 1.0f) * step) / 2.0f;

            return {
                originX + static_cast<float>(x) * step,
                Graphic3DConfig::TILE_HEIGHT / 2.0f,
                originZ + static_cast<float>(y) * step
            };
        }

        /**
         * @brief Converts a team palette identifier to a Raylib color.
         * @param teamColor Team palette index.
         * @return Color used for player and effect rendering.
         */
        static Color getPlayerColor(int teamColor)
        {
            switch (teamColor) {
                case 1:
                    return {78, 154, 242, 255};
                case 3:
                    return {74, 201, 120, 255};
                case 4:
                    return {235, 94, 184, 255};
                case 2:
                default:
                    return {231, 91, 84, 255};
            }
        }

        /**
         * @brief Converts a protocol orientation to a readable string.
         * @param orientation Direction encoded as 1: north, 2: east, 3: south, 4: west.
         * @return Human-readable orientation label.
         */
        static std::string orientationToString(int orientation)
        {
            switch (orientation) {
                case 1:
                    return "North";
                case 2:
                    return "East";
                case 3:
                    return "South";
                case 4:
                    return "West";
                default:
                    return "Unknown";
            }
        }

    private:
        Graphic3DUtils() = delete;
};

#endif /* !GRAPHIC3DUTILS_HPP_ */
