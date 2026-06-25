/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowRender
*/

#ifndef WINDOWRENDER_HPP_
    #define WINDOWRENDER_HPP_

#include "GameWorld.hpp"
#include "WindowAssets.hpp"
#include "WindowContext.hpp"
#include "World.hpp"

class WindowRender {
    public:
        WindowRender() = delete;
    
        static void clear(WindowContext& context);
        static void display(WindowContext& context);
        static void drawWorld(WindowContext& context, const GameWorld& world);
};

#endif /* !WINDOWRENDER_HPP_ */
