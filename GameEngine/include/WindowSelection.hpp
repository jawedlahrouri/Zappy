/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowSelection
*/

/**
 * @file WindowSelection.hpp
 * @brief Declares tile and player selection helpers for the SFML world view.
 */

#ifndef WINDOWSELECTION_HPP_
    #define WINDOWSELECTION_HPP_

#include "GameWorld.hpp"
#include "InfoPanel.hpp"
#include "Players.hpp"
#include "WindowContext.hpp"
#include "World.hpp"
#include <cmath>

/**
 * @brief Converts mouse coordinates into world selections and server requests.
 */
class WindowSelection {
    public:
        WindowSelection() = delete;

        /**
         * @brief Selects a tile under the mouse cursor.
         * @return true when a tile was selected.
         */
        static bool selectTile(WindowContext& context, const GameWorld& world, int mouseX, int mouseY);

        /**
         * @brief Selects a player under the mouse cursor.
         * @return true when a player was selected.
         */
        static bool selectPlayer(WindowContext& context, const GameWorld& world, int mouseX, int mouseY);

        /**
         * @brief Checks whether screen coordinates are inside a blocking UI panel.
         */
        static bool isInsidePanel(const WindowContext& context, int mouseX, int mouseY);

        /**
         * @brief Clears tile and player selections.
         */
        static void clear(WindowContext& context);

        /**
         * @brief Consumes the pending tile refresh request.
         */
        static std::optional<std::pair<int, int>> consumeTileRequest(WindowContext& context);
};

#endif /* !WINDOWSELECTION_HPP_ */
