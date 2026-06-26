/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowAssets
*/

/**
 * @file WindowAssets.hpp
 * @brief Declares asset loading helpers for the SFML backend.
 */

#ifndef WINDOWASSETS_HPP_
    #define WINDOWASSETS_HPP_

#include "TextureManager.hpp"
#include "WindowContext.hpp"
#include "WindowView.hpp"
#include <filesystem>
#include <iostream>

/**
 * @brief Stateless helper responsible for window creation and asset lifetime.
 */
class WindowAssets {
    public:
        WindowAssets() = delete;

        /**
         * @brief Creates the SFML render window stored in the context.
         */
        static void createWindow(WindowContext& context, unsigned int width, unsigned int height);

        /**
         * @brief Loads every texture required by the 2D renderer.
         */
        static void loadTextures(WindowContext& context);

        /**
         * @brief Loads the font used by the menu and HUD.
         */
        static void loadFont(WindowContext& context);

        /**
         * @brief Loads menu music and updates the context flags.
         */
        static void loadMusic(WindowContext& context);

        /**
         * @brief Starts menu music playback when the asset is available.
         */
        static void playMusic(WindowContext& context);

        /**
         * @brief Stops menu music playback.
         */
        static void stopMusic(WindowContext& context);
    };

#endif /* !WINDOWASSETS_HPP_ */
