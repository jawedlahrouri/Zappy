/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowView
*/

/**
 * @file WindowView.hpp
 * @brief Declares camera manipulation helpers for the SFML world view.
 */

#ifndef WINDOWVIEW_HPP_
    #define WINDOWVIEW_HPP_

#include "WindowContext.hpp"
#include <algorithm>

/**
 * @brief Stateless helper for resizing, zooming, and panning the world view.
 */
class WindowView {
    public:
        WindowView() = delete;

        /**
         * @brief Updates the world view after a window resize.
         */
        static void update(WindowContext& context, unsigned int width, unsigned int height);

        /**
         * @brief Applies a zoom factor to the current view.
         */
        static void zoom(WindowContext& context, float factor);

        /**
         * @brief Restores the default zoom level.
         */
        static void resetZoom(WindowContext& context);

        /**
         * @brief Pans the world view according to the current keyboard state.
         */
        static void updatePan(WindowContext& context);
};

#endif /* !WINDOWVIEW_HPP_ */
