/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowEvents
*/

#include "WindowEvents.hpp"

bool WindowEvents::handleGameEvents(WindowContext& context,
    const GameWorld& world)
{
    sf::Event event;
    bool backToMenu = false;

    while (context.window.pollEvent(event)) {
        if (handleWindowEvent(context, event))
            continue;
        if (handleGameEvent(context, event, world))
            backToMenu = true;
    }
    if (!backToMenu)
        WindowView::updatePan(context);
    return backToMenu;
}

bool WindowEvents::handleWindowEvent(WindowContext& context,
    const sf::Event& event)
{
    if (event.type == sf::Event::Closed) {
        context.window.close();
        return true;
    }
    if (event.type == sf::Event::Resized) {
        context.width = event.size.width;
        context.height = event.size.height;
        WindowView::update(context, event.size.width, event.size.height);
        return true;
    }
    return false;
}

bool WindowEvents::handleGameEvent(WindowContext& context,
    const sf::Event& event, const GameWorld& world)
{
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0)
            WindowView::zoom(context, 1.0f / WindowContext::ZOOM_STEP);
        else if (event.mouseWheelScroll.delta < 0)
            WindowView::zoom(context, WindowContext::ZOOM_STEP);
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape)
            return true;
        if (event.key.code == sf::Keyboard::Add ||
            event.key.code == sf::Keyboard::Equal ||
            event.key.code == sf::Keyboard::PageUp) {
            WindowView::zoom(context, 1.0f / WindowContext::ZOOM_STEP);
        } else if (event.key.code == sf::Keyboard::Subtract ||
            event.key.code == sf::Keyboard::Hyphen ||
            event.key.code == sf::Keyboard::PageDown) {
            WindowView::zoom(context, WindowContext::ZOOM_STEP);
        } else if (event.key.code == sf::Keyboard::R) {
            WindowView::resetZoom(context);
        } else if (event.key.code == sf::Keyboard::G) {
            context.display.showGrid = !context.display.showGrid;
        } else if (event.key.code == sf::Keyboard::B) {
            context.display.showBroadcasts = !context.display.showBroadcasts;
        } else if (event.key.code == sf::Keyboard::I) {
            context.display.showInfoPanel = !context.display.showInfoPanel;
        }
    } else if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        if (handleLegendMousePressed(context, event.mouseButton.x,
            event.mouseButton.y))
            return false;
        if (WindowTime::handleMousePressed(context, event.mouseButton.x,
            event.mouseButton.y))
            return false;
        if (handleGlobalInfoMousePressed(context, world, event.mouseButton.x,
            event.mouseButton.y))
            return false;
        if (WindowSelection::isInsidePanel(context, event.mouseButton.x,
            event.mouseButton.y))
            return false;
        if (WindowSelection::selectPlayer(context, world, event.mouseButton.x,
            event.mouseButton.y))
            return false;
        if (WindowSelection::selectTile(context, world, event.mouseButton.x,
            event.mouseButton.y))
            return false;
        WindowSelection::clear(context);
    }
    return false;
}

bool WindowEvents::handleLegendMousePressed(WindowContext& context,
    int mouseX, int mouseY)
{
    const sf::Vector2u windowSize = context.window.getSize();
    const sf::FloatRect button = InfoPanelRenderer::getInfoButtonRect(windowSize);

    if (button.contains(static_cast<float>(mouseX), static_cast<float>(mouseY))) {
        context.display.showLegend = !context.display.showLegend;
        return true;
    }
    return InfoPanelRenderer::isInsideLegendUi(windowSize, mouseX, mouseY,
        context.display.showLegend);
}

bool WindowEvents::handleGlobalInfoMousePressed(WindowContext& context,
    const GameWorld& world, int mouseX, int mouseY)
{
    if (!isInsideGlobalInfoPanel(context, mouseX, mouseY))
        return false;

    const std::optional<std::string> team =
        InfoPanelRenderer::getTeamInventoryAt(world,
            context.selection.openedTeamInventory, mouseX, mouseY);

    if (team.has_value()) {
        if (context.selection.openedTeamInventory.has_value() &&
            *context.selection.openedTeamInventory == *team)
            context.selection.openedTeamInventory.reset();
        else
            context.selection.openedTeamInventory = *team;
    }
    return true;
}

bool WindowEvents::isInsideGlobalInfoPanel(const WindowContext& context,
    int mouseX, int mouseY)
{
    if (!context.display.showInfoPanel)
        return false;

    const sf::FloatRect panel(InfoPanelRenderer::INFO_PANEL_MARGIN,
        InfoPanelRenderer::INFO_PANEL_MARGIN,
        InfoPanelRenderer::GLOBAL_PANEL_WIDTH,
        InfoPanelRenderer::GLOBAL_PANEL_HEIGHT);

    return panel.contains(static_cast<float>(mouseX),
        static_cast<float>(mouseY));
}
