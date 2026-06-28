/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowRender
*/

/**
 * @file WindowRender.hpp
 * @brief Declares high-level frame rendering helpers.
 */

#ifndef WINDOWRENDER_HPP_
    #define WINDOWRENDER_HPP_

#include "GameWorld.hpp"
#include "WindowAssets.hpp"
#include "WindowContext.hpp"
#include "World.hpp"

/**
 * @brief Stateless wrapper around SFML frame lifecycle and world drawing.
 */
class WindowRender {
    public:
        WindowRender() = delete;
    
        /**
         * @brief Clears the window before drawing a new frame.
         */
        static void clear(WindowContext& context);

        /**
         * @brief Displays the rendered frame.
         */
        static void display(WindowContext& context);

        /**
         * @brief Draws the live world and all enabled HUD overlays.
         */
        static void drawWorld(WindowContext& context, const GameWorld& world);
};

#endif /* !WINDOWRENDER_HPP_ */
