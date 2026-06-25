/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowEvents
*/

#ifndef WINDOWEVENTS_HPP_
    #define WINDOWEVENTS_HPP_

#include "GameWorld.hpp"
#include <SFML/Graphics.hpp>

class WindowContext;

class WindowEvents {
    public:
        WindowEvents() = delete;
    
        static bool handleWindowEvent(WindowContext& context, const sf::Event& event);
        static bool handleGameEvents(WindowContext& context, const GameWorld& world);
    
    private:
        static bool handleGameEvent(WindowContext& context, const sf::Event& event, const GameWorld& world);
        static bool handleGlobalInfoMousePressed(WindowContext& context, const GameWorld& world, int mouseX, int mouseY);
        static bool isInsideGlobalInfoPanel(const WindowContext& context, int mouseX, int mouseY);
};

#endif /* !WINDOWEVENTS_HPP_ */
