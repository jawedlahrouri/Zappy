/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowMenu
*/

#include "WindowMenu.hpp"
#include "Menu.hpp"
#include "WindowAssets.hpp"
#include "WindowEvents.hpp"
#include "WindowContext.hpp"

bool WindowMenu::handleEvents(WindowContext& context)
{
    sf::Event event;
    bool startGame = false;

    while (context.window.pollEvent(event)) {
        if (WindowEvents::handleWindowEvent(context, event))
            continue;
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 13 || event.text.unicode == 10) {
                startGame = true;
                continue;
            }
            handleTextEntered(context, event.text.unicode);
        } else if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            if (handleMousePressed(context, event))
                startGame = true;
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter)
                startGame = true;
            else if (event.key.code == sf::Keyboard::Tab) {
                if (context.menu.activeField == WindowContext::IP_FIELD)
                    context.menu.activeField = WindowContext::PORT_FIELD;
                else
                    context.menu.activeField = WindowContext::IP_FIELD;
            } else if (event.key.code == sf::Keyboard::Escape)
                context.window.close();
        }
    }
    return startGame;
}

void WindowMenu::draw(WindowContext& context)
{
    WindowAssets::playMusic(context);
    Menu::draw(context.window, context.assets.font, context.assets.fontLoaded,
        context.menu.ip, context.menu.port, context.menu.activeField,
        context.menu.status, context.menu.teamNames);
}

const std::string& WindowMenu::getIp(const WindowContext& context)
{
    return context.menu.ip;
}

const std::string& WindowMenu::getPort(const WindowContext& context)
{
    return context.menu.port;
}

void WindowMenu::setConnectionInfo(WindowContext& context, const std::string& ip, const std::string& port)
{
    context.menu.ip = ip;
    context.menu.port = port;
}

void WindowMenu::setStatus(WindowContext& context, const std::string& status)
{
    context.menu.status = status;
}

void WindowMenu::resetTeams(WindowContext& context)
{
    context.menu.teamNames.clear();
}

void WindowMenu::addTeamName(WindowContext& context, const std::string& teamName)
{
    if (teamName.empty())
        return;
    if (std::find(context.menu.teamNames.begin(),
        context.menu.teamNames.end(), teamName) !=
        context.menu.teamNames.end())
        return;
    context.menu.teamNames.push_back(teamName);
}

bool WindowMenu::consumeConnectionEdited(WindowContext& context)
{
    const bool wasEdited = context.menu.connectionEdited;

    context.menu.connectionEdited = false;
    return wasEdited;
}

std::string& WindowMenu::getActiveInput(WindowContext& context)
{
    if (context.menu.activeField == WindowContext::IP_FIELD)
        return context.menu.ip;
    return context.menu.port;
}

void WindowMenu::handleTextEntered(WindowContext& context, sf::Uint32 unicode)
{
    std::string& input = getActiveInput(context);

    context.menu.status.clear();
    if (unicode == 8 || unicode == 127) {
        if (!input.empty()) {
            input.pop_back();
            context.menu.connectionEdited = true;
        }
        return;
    }
    if (unicode < 32 || unicode > 126)
        return;
    const char character = static_cast<char>(unicode);

    if (context.menu.activeField == WindowContext::PORT_FIELD) {
        if (std::isdigit(static_cast<unsigned char>(character)) &&
            input.size() < 5) {
            input += character;
            context.menu.connectionEdited = true;
        }
        return;
    }
    if ((std::isalnum(static_cast<unsigned char>(character)) ||
        character == '.' || character == '-' || character == ':') &&
        input.size() < 64) {
        input += character;
        context.menu.connectionEdited = true;
    }
}

bool WindowMenu::handleMousePressed(WindowContext& context, const sf::Event& event)
{
    const sf::Vector2u windowSize = context.window.getSize();
    const float centerX = static_cast<float>(windowSize.x) / 2.0f;
    const float centerY = static_cast<float>(windowSize.y) / 2.0f;
    const float mouseX = static_cast<float>(event.mouseButton.x);
    const float mouseY = static_cast<float>(event.mouseButton.y);
    const sf::FloatRect ipBox(centerX - 180.0f, centerY - 118.0f, 360.0f, 46.0f);
    const sf::FloatRect portBox(centerX - 180.0f, centerY - 13.0f, 360.0f, 46.0f);
    const sf::FloatRect startButton(centerX - 150.0f, centerY + 76.0f, 300.0f, 58.0f);

    if (ipBox.contains(mouseX, mouseY)) {
        context.menu.activeField = WindowContext::IP_FIELD;
        return false;
    }
    if (portBox.contains(mouseX, mouseY)) {
        context.menu.activeField = WindowContext::PORT_FIELD;
        return false;
    }
    return startButton.contains(mouseX, mouseY);
}
