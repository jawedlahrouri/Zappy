/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowMenu
*/

#ifndef WINDOWMENU_HPP_
    #define WINDOWMENU_HPP_

#include "Menu.hpp"
#include "WindowAssets.hpp"
#include "WindowContext.hpp"
#include "WindowEvents.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Utf.hpp>

class WindowMenu {
public:
    WindowMenu() = delete;

    static bool handleEvents(WindowContext& context);
    static void draw(WindowContext& context);
    static const std::string& getIp(const WindowContext& context);
    static const std::string& getPort(const WindowContext& context);
    static void setConnectionInfo(WindowContext& context,
        const std::string& ip, const std::string& port);
    static void setStatus(WindowContext& context, const std::string& status);
    static void resetTeams(WindowContext& context);
    static void addTeamName(WindowContext& context, const std::string& teamName);
    static bool consumeConnectionEdited(WindowContext& context);

private:
    static std::string& getActiveInput(WindowContext& context);
    static void handleTextEntered(WindowContext& context, sf::Uint32 unicode);
    static bool handleMousePressed(WindowContext& context,
        const sf::Event& event);
};

#endif /* !WINDOWMENU_HPP_ */
