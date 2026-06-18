/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IGraphic
*/

#ifndef IGRAPHIC_HPP_
    #define IGRAPHIC_HPP_

    #include "GameWorld.hpp"
    #include <optional>
    #include <string>
    #include <utility>

class IGraphic {
public:
    virtual ~IGraphic() = default;

    virtual bool isOpen() const = 0;
    virtual bool handleMenuEvents() = 0;
    virtual bool handleGameEvents(const GameWorld& world) = 0;
    virtual std::optional<std::pair<int, int>> consumeTileRequest() = 0;
    virtual std::optional<int> consumeTimeRequest() = 0;
    virtual void clear() = 0;
    virtual void display() = 0;
    virtual void drawMenu() = 0;
    virtual void drawWorld(const GameWorld& world) = 0;
    virtual const std::string& getMenuIp() const = 0;
    virtual const std::string& getMenuPort() const = 0;
    virtual void setMenuConnectionInfo(const std::string& ip, const std::string& port) = 0;
    virtual void setMenuStatus(const std::string& status) = 0;
    virtual void resetMenuTeamNames() = 0;
    virtual void addMenuTeamName(const std::string& teamName) = 0;
    virtual bool consumeMenuConnectionEdited() = 0;
    virtual void setServerFreq(int freq) = 0;
    virtual int getServerFreq() const = 0;
    virtual bool isTimePaused() const = 0;
};

extern "C" {
    typedef IGraphic* (*CreateGraphicFunction)();
    typedef void (*DestroyGraphicFunction)(IGraphic*);
}

#endif /* !IGRAPHIC_HPP_ */
