/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowContext
*/

#include "WindowContext.hpp"

WindowContext::WindowContext(unsigned int width, unsigned int height, const std::string& title)
    : assets{sf::Music(), sf::Font(), false, false, sf::Texture(), sf::Texture()},
    view{sf::View(), {static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f}, sf::Clock(), 1.0f},
    selection{std::nullopt, std::nullopt, std::nullopt, std::nullopt},
    menu{"", "", "", {}, false, IP_FIELD},
    time{std::nullopt, NORMAL_SERVER_FREQ, NORMAL_SERVER_FREQ, false},
    display{false, true, true},
    width(width),
    height(height),
    title(title)
{
}
