/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowView
*/

#ifndef WINDOWVIEW_HPP_
    #define WINDOWVIEW_HPP_

#include "WindowContext.hpp"
#include <algorithm>

class WindowView {
    public:
        WindowView() = delete;

        static void update(WindowContext& context, unsigned int width, unsigned int height);
        static void zoom(WindowContext& context, float factor);
        static void resetZoom(WindowContext& context);
        static void updatePan(WindowContext& context);
};

#endif /* !WINDOWVIEW_HPP_ */
