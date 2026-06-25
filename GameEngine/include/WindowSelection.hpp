/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowSelection
*/

#ifndef WINDOWSELECTION_HPP_
    #define WINDOWSELECTION_HPP_

#include "GameWorld.hpp"
#include "InfoPanel.hpp"
#include "Players.hpp"
#include "WindowContext.hpp"
#include "World.hpp"
#include <cmath>

class WindowSelection {
    public:
        WindowSelection() = delete;

        static bool selectTile(WindowContext& context, const GameWorld& world, int mouseX, int mouseY);
        static bool selectPlayer(WindowContext& context, const GameWorld& world, int mouseX, int mouseY);
        static bool isInsidePanel(const WindowContext& context, int mouseX, int mouseY);
        static void clear(WindowContext& context);
        static std::optional<std::pair<int, int>> consumeTileRequest(WindowContext& context);
};

#endif /* !WINDOWSELECTION_HPP_ */
