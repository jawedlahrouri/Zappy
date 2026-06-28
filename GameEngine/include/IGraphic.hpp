/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IGraphic
*/

/**
 * @file IGraphic.hpp
 * @brief Declares the common runtime interface implemented by graphical backends.
 */

#ifndef IGRAPHIC_HPP_
    #define IGRAPHIC_HPP_

    #include "GameWorld.hpp"
    #include <optional>
    #include <string>
    #include <utility>

/**
 * @brief Abstract contract used by the core to drive any graphical backend.
 *
 * Implementations are loaded dynamically and must expose the C factory symbols
 * declared at the bottom of this file.
 */
class IGraphic {
public:
    /**
     * @brief Releases backend-specific graphical resources.
     */
    virtual ~IGraphic() = default;

    /**
     * @brief Checks whether the graphical window is still available.
     * @return true while the backend can keep rendering; false otherwise.
     */
    virtual bool isOpen() const = 0;

    /**
     * @brief Polls and processes menu events.
     * @return true when the user asks to enter the game view.
     */
    virtual bool handleMenuEvents() = 0;

    /**
     * @brief Polls and processes game-view events.
     * @param world Current world snapshot used for hit testing and selection.
     * @return true when the user asks to leave the game view.
     */
    virtual bool handleGameEvents(const GameWorld& world) = 0;

    /**
     * @brief Retrieves and clears the latest tile refresh request.
     * @return Tile coordinates to request from the server, if any.
     */
    virtual std::optional<std::pair<int, int>> consumeTileRequest() = 0;

    /**
     * @brief Retrieves and clears the latest server-frequency request.
     * @return Requested frequency value, if the user changed it.
     */
    virtual std::optional<int> consumeTimeRequest() = 0;

    /**
     * @brief Clears the current frame before drawing.
     */
    virtual void clear() = 0;

    /**
     * @brief Presents the rendered frame to the screen.
     */
    virtual void display() = 0;

    /**
     * @brief Draws the connection menu.
     */
    virtual void drawMenu() = 0;

    /**
     * @brief Draws the live world state.
     * @param world Current authoritative world snapshot.
     */
    virtual void drawWorld(const GameWorld& world) = 0;

    /**
     * @brief Gets the IP address currently entered in the menu.
     * @return Menu IP string.
     */
    virtual const std::string& getMenuIp() const = 0;

    /**
     * @brief Gets the port currently entered in the menu.
     * @return Menu port string.
     */
    virtual const std::string& getMenuPort() const = 0;

    /**
     * @brief Initializes or replaces the menu connection fields.
     * @param ip Default IP address.
     * @param port Default port.
     */
    virtual void setMenuConnectionInfo(const std::string& ip, const std::string& port) = 0;

    /**
     * @brief Updates the status message displayed by the menu.
     * @param status Human-readable connection status.
     */
    virtual void setMenuStatus(const std::string& status) = 0;

    /**
     * @brief Clears the team names currently shown in the menu.
     */
    virtual void resetMenuTeamNames() = 0;

    /**
     * @brief Adds a team name to the menu preview.
     * @param teamName Name received from the server.
     */
    virtual void addMenuTeamName(const std::string& teamName) = 0;

    /**
     * @brief Checks whether the user edited IP or port since the last read.
     * @return true once per edit sequence.
     */
    virtual bool consumeMenuConnectionEdited() = 0;

    /**
     * @brief Stores the latest server frequency known by the GUI.
     * @param freq Frequency value in Zappy time units.
     */
    virtual void setServerFreq(int freq) = 0;

    /**
     * @brief Gets the latest server frequency known by the GUI.
     * @return Current server frequency.
     */
    virtual int getServerFreq() const = 0;

    /**
     * @brief Checks whether the GUI time controls are currently paused.
     * @return true when the pause control is active.
     */
    virtual bool isTimePaused() const = 0;
};

extern "C" {
    /**
     * @brief Factory function signature exported by graphical plugins.
     */
    typedef IGraphic* (*CreateGraphicFunction)();

    /**
     * @brief Destruction function signature exported by graphical plugins.
     */
    typedef void (*DestroyGraphicFunction)(IGraphic*);
}

#endif /* !IGRAPHIC_HPP_ */
