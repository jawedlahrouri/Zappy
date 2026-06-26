/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DUtils
*/

#ifndef GRAPHIC3DUTILS_HPP_
    #define GRAPHIC3DUTILS_HPP_

    #include "Graphic3DConfig.hpp"
    #include <raylib.h>
    #include <string>

namespace Graphic3DUtils {
    inline Vector3 getTileCenter(int x, int y, int width, int height)
    {
        const float step = Graphic3DConfig::TILE_SIZE +
            Graphic3DConfig::TILE_GAP;
        const float originX = -((static_cast<float>(width) - 1.0f) * step) /
            2.0f;
        const float originZ = -((static_cast<float>(height) - 1.0f) * step) /
            2.0f;

        return {
            originX + static_cast<float>(x) * step,
            Graphic3DConfig::TILE_HEIGHT / 2.0f,
            originZ + static_cast<float>(y) * step
        };
    }

    inline Color getPlayerColor(int teamColor)
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

    inline std::string orientationToString(int orientation)
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
}

#endif /* !GRAPHIC3DUTILS_HPP_ */
