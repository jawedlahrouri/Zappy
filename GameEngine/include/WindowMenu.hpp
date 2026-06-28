/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowMenu
*/

/**
 * @file WindowMenu.hpp
 * @brief Declares connection-menu state and input helpers.
 */

#ifndef WINDOWMENU_HPP_
    #define WINDOWMENU_HPP_

#include "Menu.hpp"
#include "WindowAssets.hpp"
#include "WindowContext.hpp"
#include "WindowEvents.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Utf.hpp>

/**
 * @brief Stateless controller for the SFML connection menu.
 */
class WindowMenu {
public:
    WindowMenu() = delete;

    /**
     * @brief Polls menu events and updates menu input state.
     * @return true when the user submits the menu.
     */
    static bool handleEvents(WindowContext& context);

    /**
     * @brief Draws the full connection menu.
     */
    static void draw(WindowContext& context);

    /**
     * @brief Gets the current menu IP field.
     */
    static const std::string& getIp(const WindowContext& context);

    /**
     * @brief Gets the current menu port field.
     */
    static const std::string& getPort(const WindowContext& context);

    /**
     * @brief Updates both connection fields at once.
     */
    static void setConnectionInfo(WindowContext& context,
        const std::string& ip, const std::string& port);

    /**
     * @brief Updates the menu status text.
     */
    static void setStatus(WindowContext& context, const std::string& status);

    /**
     * @brief Clears displayed team names.
     */
    static void resetTeams(WindowContext& context);

    /**
     * @brief Adds a team name if it is not already displayed.
     */
    static void addTeamName(WindowContext& context, const std::string& teamName);

    /**
     * @brief Consumes the connection-edited flag.
     * @return true once after the user edits IP or port.
     */
    static bool consumeConnectionEdited(WindowContext& context);

private:
    /**
     * @brief Returns the string currently targeted by text input.
     */
    static std::string& getActiveInput(WindowContext& context);

    /**
     * @brief Applies one Unicode text-input event to the active field.
     */
    static void handleTextEntered(WindowContext& context, sf::Uint32 unicode);

    /**
     * @brief Handles mouse focus and submit actions in the menu.
     */
    static bool handleMousePressed(WindowContext& context,
        const sf::Event& event);
};

#endif /* !WINDOWMENU_HPP_ */
