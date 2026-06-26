/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DConfig
*/

/**
 * @file Graphic3DConfig.hpp
 * @brief Defines global constants used by the Raylib 3D renderer.
 */

#ifndef GRAPHIC3DCONFIG_HPP_
    #define GRAPHIC3DCONFIG_HPP_

/**
 * @brief Shared 3D rendering constants.
 */
namespace Graphic3DConfig {
    /** Default 3D window width in pixels. */
    constexpr int WINDOW_WIDTH = 1600;
    /** Default 3D window height in pixels. */
    constexpr int WINDOW_HEIGHT = 900;
    /** Width and depth of one rendered tile. */
    constexpr float TILE_SIZE = 1.8f;
    /** Visual spacing between rendered tiles. */
    constexpr float TILE_GAP = 0.16f;
    /** Height of the rendered tile block. */
    constexpr float TILE_HEIGHT = 0.16f;
}

#endif /* !GRAPHIC3DCONFIG_HPP_ */
