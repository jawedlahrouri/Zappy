/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowView
*/

#include "WindowView.hpp"

void WindowView::update(WindowContext& context, unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0)
        return;

    sf::Vector2f windowSize(static_cast<float>(width), static_cast<float>(height));

    context.view.world.setSize(windowSize.x * context.view.zoomLevel, windowSize.y * context.view.zoomLevel);
    context.view.world.setCenter(context.view.center);
    context.window.setView(context.view.world);
}

void WindowView::zoom(WindowContext& context, float factor)
{
    sf::Vector2u size = context.window.getSize();

    context.view.zoomLevel = std::clamp(context.view.zoomLevel * factor, WindowContext::MIN_ZOOM, WindowContext::MAX_ZOOM);
    update(context, size.x, size.y);
}

void WindowView::resetZoom(WindowContext& context)
{
    sf::Vector2u size = context.window.getSize();

    context.view.zoomLevel = 1.0f;
    update(context, size.x, size.y);
}

void WindowView::updatePan(WindowContext& context)
{
    const float deltaTime = std::min(context.view.panClock.restart().asSeconds(), 0.05f);
    const float panStep = WindowContext::PAN_SPEED * context.view.zoomLevel * deltaTime;
    sf::Vector2f movement(0.0f, 0.0f);

    if (!context.window.isOpen())
        return;
    if (!context.window.hasFocus())
        return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= panStep;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        movement.y += panStep;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        movement.x -= panStep;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        movement.x += panStep;
    if (movement.x == 0.0f && movement.y == 0.0f)
        return;
    context.view.center += movement;
    context.view.world.setCenter(context.view.center);
    context.window.setView(context.view.world);
}
