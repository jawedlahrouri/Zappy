/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowEvents
*/

/**
 * @file WindowEvents.hpp
 * @brief Declares event dispatch helpers for the SFML game view.
 */

#ifndef WINDOWEVENTS_HPP_
    #define WINDOWEVENTS_HPP_

#include "GameWorld.hpp"
#include "InfoPanel.hpp"
#include "WindowContext.hpp"
#include "WindowSelection.hpp"
#include "WindowTime.hpp"
#include "WindowView.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Stateless dispatcher for SFML window and gameplay events.
 */
class WindowEvents {
    public:
        WindowEvents() = delete;
    
        /**
         * @brief Handles a generic window event such as closing or resizing.
         * @return false when the window should stop running.
         */
        static bool handleWindowEvent(WindowContext& context, const sf::Event& event);

        /**
         * @brief Polls and handles every pending event in the game view.
         * @param context Shared SFML context.
         * @param world Current world snapshot used by selection logic.
         * @return true when the user asks to return to the menu.
         */
        static bool handleGameEvents(WindowContext& context, const GameWorld& world);
    
    private:
        /**
         * @brief Handles one game-view event.
         */
        static bool handleGameEvent(WindowContext& context, const sf::Event& event, const GameWorld& world);

        /**
         * @brief Handles clicks inside the global information panel.
         */
        static bool handleGlobalInfoMousePressed(WindowContext& context, const GameWorld& world, int mouseX, int mouseY);

        /**
         * @brief Checks whether coordinates are inside the global information panel.
         */
        static bool isInsideGlobalInfoPanel(const WindowContext& context, int mouseX, int mouseY);

        /**
         * @brief Handles clicks on the legend button and expanded legend panel.
         */
        static bool handleLegendMousePressed(WindowContext& context,
            int mouseX, int mouseY);
};

#endif /* !WINDOWEVENTS_HPP_ */
