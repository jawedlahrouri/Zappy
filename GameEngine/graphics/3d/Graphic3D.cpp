/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3D
*/

#include "Graphic3D.hpp"
#include "Graphic3DConfig.hpp"
#include <algorithm>
#include <raylib.h>

Graphic3D::Graphic3D()
    : _camera(),
    _renderer(),
    _windowOpen(false),
    _menuIp(""),
    _menuPort(""),
    _menuStatus("Press Enter to enter"),
    _teamNames(),
    _menuConnectionEdited(false),
    _serverFreq(100),
    _timePaused(false),
    _selectedTile(),
    _selectedPlayerId(),
    _pendingTileRequest()
{
    createWindow();
}

Graphic3D::~Graphic3D()
{
    if (IsWindowReady())
        CloseWindow();
}

void Graphic3D::createWindow()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(Graphic3DConfig::WINDOW_WIDTH, Graphic3DConfig::WINDOW_HEIGHT,
        "Zappy 3D");
    SetTargetFPS(60);
    _windowOpen = true;
}

bool Graphic3D::isOpen() const
{
    return _windowOpen && !WindowShouldClose();
}

bool Graphic3D::handleMenuEvents()
{
    if (WindowShouldClose()) {
        _windowOpen = false;
        return false;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        _windowOpen = false;
        return false;
    }
    _camera.update(GameWorld());
    return IsKeyPressed(KEY_ENTER);
}

bool Graphic3D::handleGameEvents(const GameWorld& world)
{
    if (WindowShouldClose()) {
        _windowOpen = false;
        return true;
    }
    if (IsKeyPressed(KEY_ESCAPE))
        return true;
    _camera.update(world);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        handleSelection(world);
    return false;
}

std::optional<std::pair<int, int>> Graphic3D::consumeTileRequest()
{
    std::optional<std::pair<int, int>> request = _pendingTileRequest;

    _pendingTileRequest.reset();
    return request;
}

std::optional<int> Graphic3D::consumeTimeRequest()
{
    return std::nullopt;
}

void Graphic3D::clear()
{
    if (!_windowOpen)
        return;
    BeginDrawing();
    ClearBackground({12, 16, 22, 255});
}

void Graphic3D::display()
{
    if (!_windowOpen)
        return;
    EndDrawing();
}

void Graphic3D::drawMenu()
{
    if (!_windowOpen)
        return;
    _renderer.drawMenu(_camera.getCamera(), _menuStatus, _teamNames);
}

void Graphic3D::drawWorld(const GameWorld& world)
{
    if (!_windowOpen)
        return;
    _renderer.drawWorld(_camera.getCamera(), world, _selectedTile,
        _selectedPlayerId);
}

void Graphic3D::handleSelection(const GameWorld& world)
{
    const Vector2 mousePosition = GetMousePosition();
    const int mouseX = static_cast<int>(mousePosition.x);
    const int mouseY = static_cast<int>(mousePosition.y);

    if (mousePosition.y <= 54.0f || _renderer.isInsideInfoPanel(
        _selectedTile, _selectedPlayerId, mouseX, mouseY))
        return;

    const std::optional<int> playerId = _renderer.pickPlayer(world,
        _camera.getCamera(), mousePosition);

    if (playerId.has_value()) {
        const auto playerIt = world.getPlayers().find(*playerId);

        if (playerIt == world.getPlayers().end())
            return;
        _selectedPlayerId = *playerId;
        _selectedTile = {playerIt->second.x, playerIt->second.y};
        _pendingTileRequest.reset();
        return;
    }

    const std::optional<std::pair<int, int>> tile = _renderer.pickTile(world,
        _camera.getCamera(), mousePosition);

    if (!tile.has_value())
        return;
    _selectedTile = tile;
    _selectedPlayerId.reset();
    _pendingTileRequest = _selectedTile;
}

const std::string& Graphic3D::getMenuIp() const
{
    return _menuIp;
}

const std::string& Graphic3D::getMenuPort() const
{
    return _menuPort;
}

void Graphic3D::setMenuConnectionInfo(const std::string& ip,
    const std::string& port)
{
    _menuIp = ip;
    _menuPort = port;
}

void Graphic3D::setMenuStatus(const std::string& status)
{
    _menuStatus = status;
}

void Graphic3D::resetMenuTeamNames()
{
    _teamNames.clear();
}

void Graphic3D::addMenuTeamName(const std::string& teamName)
{
    if (teamName.empty())
        return;
    if (std::find(_teamNames.begin(), _teamNames.end(), teamName) !=
        _teamNames.end())
        return;
    _teamNames.push_back(teamName);
}

bool Graphic3D::consumeMenuConnectionEdited()
{
    const bool edited = _menuConnectionEdited;

    _menuConnectionEdited = false;
    return edited;
}

void Graphic3D::setServerFreq(int freq)
{
    _serverFreq = freq;
}

int Graphic3D::getServerFreq() const
{
    return _serverFreq;
}

bool Graphic3D::isTimePaused() const
{
    return _timePaused;
}

extern "C" IGraphic* createGraphic()
{
    return new Graphic3D();
}

extern "C" void destroyGraphic(IGraphic* graphic)
{
    delete graphic;
}
