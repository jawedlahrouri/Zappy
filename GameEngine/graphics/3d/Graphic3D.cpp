/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3D
*/

#include "Graphic3D.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>

Graphic3D::Graphic3D()
    : _fontLoaded(false),
    _menuIp("127.0.0.1"),
    _menuPort("4242"),
    _menuStatus("Press to enter"),
    _menuConnectionEdited(false),
    _serverFreq(100),
    _timePaused(false)
{
    createWindow(1600, 900);
    loadFont();
}

Graphic3D::~Graphic3D()
{
    if (_window.isOpen())
        _window.close();
}

void Graphic3D::createWindow(unsigned int width, unsigned int height)
{
    _window.create(sf::VideoMode(width, height), "Zappy 3D");
    _window.setFramerateLimit(60);
    _window.requestFocus();
}

void Graphic3D::loadFont()
{
    const std::string fonts[] = {
        "GameEngine/assets/fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
    };

    for (const std::string& path : fonts) {
        if (std::filesystem::exists(path) && _font.loadFromFile(path)) {
            _fontLoaded = true;
            return;
        }
    }
}

bool Graphic3D::isOpen() const
{
    return _window.isOpen();
}

void Graphic3D::handleCloseEvent(const sf::Event& event, bool& exitToMenu)
{
    if (event.type == sf::Event::Closed) {
        _window.close();
        exitToMenu = true;
    }
}

bool Graphic3D::handleMenuEvents()
{
    sf::Event event;
    bool startGame = false;

    while (_window.pollEvent(event)) {
        handleCloseEvent(event, startGame);
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter)
                startGame = true;
            else if (event.key.code == sf::Keyboard::Escape)
                _window.close();
        }
    }
    return startGame;
}

bool Graphic3D::handleGameEvents(const GameWorld& world)
{
    sf::Event event;
    bool backToMenu = false;
    (void)world;

    while (_window.pollEvent(event)) {
        handleCloseEvent(event, backToMenu);
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            backToMenu = true;
        }
    }
    return backToMenu;
}

std::optional<std::pair<int, int>> Graphic3D::consumeTileRequest()
{
    return std::nullopt;
}

std::optional<int> Graphic3D::consumeTimeRequest()
{
    return std::nullopt;
}

void Graphic3D::clear()
{
    _window.clear(sf::Color(12, 18, 28));
}

void Graphic3D::display()
{
    _window.display();
}

void Graphic3D::drawIsoTile(sf::RenderTarget& target, float centerX,
    float centerY, sf::Color fillColor, sf::Color outlineColor)
{
    sf::ConvexShape tile(4);

    tile.setPoint(0, {centerX, centerY - TILE_HEIGHT / 2.0f});
    tile.setPoint(1, {centerX + TILE_WIDTH / 2.0f, centerY});
    tile.setPoint(2, {centerX, centerY + TILE_HEIGHT / 2.0f});
    tile.setPoint(3, {centerX - TILE_WIDTH / 2.0f, centerY});
    tile.setFillColor(fillColor);
    tile.setOutlineColor(outlineColor);
    tile.setOutlineThickness(1.0f);
    target.draw(tile);
}

void Graphic3D::drawSceneBackground(sf::RenderTarget& target)
{
    sf::RectangleShape background;

    background.setSize(sf::Vector2f(
        static_cast<float>(_window.getSize().x),
        static_cast<float>(_window.getSize().y)));
    background.setFillColor(sf::Color(12, 18, 28));
    target.draw(background);
}

sf::Color Graphic3D::tileColorFromResources(const Tile& tile) const
{
    const int total = tile.resources.food + tile.resources.linemate +
        tile.resources.deraumere + tile.resources.sibur +
        tile.resources.mendiane + tile.resources.phiras +
        tile.resources.thystame;
    const int base = std::clamp(55 + total * 12, 55, 210);

    return sf::Color(base, std::clamp(base + 25, 0, 255),
        std::clamp(base + 10, 0, 255), 255);
}

void Graphic3D::drawIsoGrid(sf::RenderTarget& target, int width, int height,
    float originX, float originY, bool previewMode)
{
    const int safeWidth = std::max(1, width);
    const int safeHeight = std::max(1, height);

    for (int y = 0; y < safeHeight; y++) {
        for (int x = 0; x < safeWidth; x++) {
            const float isoX = originX + (static_cast<float>(x - y) * TILE_WIDTH / 2.0f);
            const float isoY = originY + (static_cast<float>(x + y) * TILE_HEIGHT / 2.0f);
            const sf::Color fill = previewMode ?
                sf::Color(60, 100, 90, 220) :
                sf::Color(70, 110, 96, 230);
            const sf::Color outline = previewMode ?
                sf::Color(150, 210, 190, 180) :
                sf::Color(30, 40, 36, 255);

            drawIsoTile(target, isoX, isoY, fill, outline);
        }
    }
}

void Graphic3D::drawMenuLabel(sf::RenderTarget& target)
{
    if (!_fontLoaded)
        return;

    sf::Text title("Zappy 3D", _font, 34);
    sf::Text subtitle("Press to enter", _font, 18);
    sf::Text status(_menuStatus, _font, 16);

    title.setPosition(28.0f, 18.0f);
    subtitle.setPosition(28.0f, 62.0f);
    status.setPosition(28.0f, 88.0f);
    title.setFillColor(sf::Color(240, 240, 240));
    subtitle.setFillColor(sf::Color(190, 210, 210));
    status.setFillColor(sf::Color(140, 180, 180));
    target.draw(title);
    target.draw(subtitle);
    target.draw(status);
}

void Graphic3D::drawMenu()
{
    drawSceneBackground(_window);
    drawIsoGrid(_window, 8, 8, 340.0f, 180.0f, true);
    drawMenuLabel(_window);
}

void Graphic3D::drawWorld(const GameWorld& world)
{
    drawSceneBackground(_window);
    if (world.getWidth() <= 0 || world.getHeight() <= 0 ||
        world.getMap().empty()) {
        drawIsoGrid(_window, 8, 8, 420.0f, 180.0f, false);
        return;
    }

    const int width = world.getWidth();
    const int height = world.getHeight();
    const float originX = static_cast<float>(_window.getSize().x) / 2.0f;
    const float originY = 120.0f;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const Tile& tile = world.getMap()[y][x];
            const float isoX = originX + (static_cast<float>(x - y) * TILE_WIDTH / 2.0f);
            const float isoY = originY + (static_cast<float>(x + y) * TILE_HEIGHT / 2.0f);
            const sf::Color fill = tileColorFromResources(tile);

            drawIsoTile(_window, isoX, isoY, fill, sf::Color(25, 30, 32));
        }
    }
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
}

void Graphic3D::addMenuTeamName(const std::string& teamName)
{
    (void)teamName;
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
