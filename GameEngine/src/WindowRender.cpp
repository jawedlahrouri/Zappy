/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowRender
*/

#include "WindowRender.hpp"

void WindowRender::clear(WindowContext& context)
{
    context.window.clear(sf::Color(30, 30, 30));
}

void WindowRender::display(WindowContext& context)
{
    context.window.display();
}

void WindowRender::drawWorld(WindowContext& context, const GameWorld& world)
{
    WindowAssets::stopMusic(context);
    WorldRenderer::draw(context.window, context.view.world, world,
        context.selection.tile, context.selection.playerId, context.assets.font,
        context.assets.fontLoaded, context.display.showGrid, context.display.showBroadcasts,
        context.time.serverFreq, context.time.paused, context.display.showInfoPanel,
        context.selection.openedTeamInventory);
}
