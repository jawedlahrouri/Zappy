/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic2D
*/

/**
 * @file Graphic2D.cpp
 * @brief Exports the SFML graphical plugin entry points.
 */

#include "IGraphic.hpp"
#include "WindowManager.hpp"

/**
 * @brief Creates the default SFML graphical backend.
 * @return Newly allocated graphical interface instance.
 */
extern "C" IGraphic* createGraphic()
{
    return new WindowManager();
}

/**
 * @brief Destroys a graphical backend created by createGraphic().
 * @param graphic Instance to destroy.
 */
extern "C" void destroyGraphic(IGraphic* graphic)
{
    delete graphic;
}
