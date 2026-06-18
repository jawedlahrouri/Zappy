/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic2D
*/

#include "IGraphic.hpp"
#include "WindowManager.hpp"

extern "C" IGraphic* createGraphic()
{
    return new WindowManager();
}

extern "C" void destroyGraphic(IGraphic* graphic)
{
    delete graphic;
}
