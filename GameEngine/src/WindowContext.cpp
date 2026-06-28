/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowContext
*/

#include "WindowContext.hpp"

WindowContext::WindowContext(unsigned int width, unsigned int height, const std::string& title)
    : width(width),
    height(height),
    title(title)
{
    view.center = {
        static_cast<float>(width) / 2.0f,
        static_cast<float>(height) / 2.0f
    };
}
