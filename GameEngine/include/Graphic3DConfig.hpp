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
class Graphic3DConfig {
    public:
        /** Default 3D window width in pixels. */
        static constexpr int WINDOW_WIDTH = 1920;
        /** Default 3D window height in pixels. */
        static constexpr int WINDOW_HEIGHT = 1080;
        /** Width and depth of one rendered tile. */
        static constexpr float TILE_SIZE = 1.8f;
        /** Visual spacing between rendered tiles. */
        static constexpr float TILE_GAP = 0.16f;
        /** Height of the rendered tile block. */
        static constexpr float TILE_HEIGHT = 0.16f;

    private:
        Graphic3DConfig() = delete;
};

#endif /* !GRAPHIC3DCONFIG_HPP_ */
