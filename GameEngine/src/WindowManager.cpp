/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

#include "WindowManager.hpp"
#include "WindowAssets.hpp"
#include "WindowEvents.hpp"
#include "WindowMenu.hpp"
#include "WindowRender.hpp"
#include "WindowSelection.hpp"
#include "WindowTime.hpp"

WindowManager::WindowManager(unsigned int width, unsigned int height,
    const std::string& title)
    : _context(width, height, title)
{
    WindowAssets::createWindow(_context, width, height);
    WindowAssets::loadTextures(_context);
    WindowAssets::loadFont(_context);
    WindowAssets::loadMusic(_context);
}

WindowManager::~WindowManager()
{
    WindowAssets::stopMusic(_context);
    if (_context.window.isOpen())
        _context.window.close();
}

bool WindowManager::isOpen() const
{
    return _context.window.isOpen();
}

bool WindowManager::handleMenuEvents()
{
    return WindowMenu::handleEvents(_context);
}

bool WindowManager::handleGameEvents(const GameWorld& world)
{
    return WindowEvents::handleGameEvents(_context, world);
}

std::optional<std::pair<int, int>> WindowManager::consumeTileRequest()
{
    return WindowSelection::consumeTileRequest(_context);
}

std::optional<int> WindowManager::consumeTimeRequest()
{
    return WindowTime::consumeRequest(_context);
}

void WindowManager::clear()
{
    WindowRender::clear(_context);
}

void WindowManager::display()
{
    WindowRender::display(_context);
}

void WindowManager::drawMenu()
{
    WindowMenu::draw(_context);
}

void WindowManager::drawWorld(const GameWorld& world)
{
    WindowRender::drawWorld(_context, world);
}

const std::string& WindowManager::getMenuIp() const
{
    return WindowMenu::getIp(_context);
}

const std::string& WindowManager::getMenuPort() const
{
    return WindowMenu::getPort(_context);
}

void WindowManager::setMenuConnectionInfo(const std::string& ip,
    const std::string& port)
{
    WindowMenu::setConnectionInfo(_context, ip, port);
}

void WindowManager::setMenuStatus(const std::string& status)
{
    WindowMenu::setStatus(_context, status);
}

void WindowManager::resetMenuTeamNames()
{
    WindowMenu::resetTeams(_context);
}

void WindowManager::addMenuTeamName(const std::string& teamName)
{
    WindowMenu::addTeamName(_context, teamName);
}

bool WindowManager::consumeMenuConnectionEdited()
{
    return WindowMenu::consumeConnectionEdited(_context);
}

void WindowManager::setServerFreq(int freq)
{
    WindowTime::setFrequency(_context, freq);
}

int WindowManager::getServerFreq() const
{
    return WindowTime::getFrequency(_context);
}

bool WindowManager::isTimePaused() const
{
    return WindowTime::isPaused(_context);
}
