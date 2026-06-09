/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

#include "WindowManager.hpp"
#include "GraphicalUtils.hpp"
#include "TextureManager.hpp"
#include <algorithm>
#include <iostream>

namespace {
    constexpr float TILE_SIZE = 64.0f;
    constexpr float ZOOM_STEP = 1.1f;
    constexpr float MIN_ZOOM = 0.35f;
    constexpr float MAX_ZOOM = 3.0f;
}

WindowManager::WindowManager(unsigned int width, unsigned int height, const std::string& title)
    : _renderer3D(std::make_unique<RaylibRenderer>()),
    _renderMode(RenderMode::TwoD),
    _zoomLevel(1.0f),
    _width(width),
    _height(height),
    _title(title),
    _raylibWarningPrinted(false)
{
    create2DWindow(width, height);
    loadTextures();
}

void WindowManager::loadTextures()
{
    TextureManager& tm = TextureManager::getInstance();
    
    tm.loadTexture("ground", "ground.png");
    tm.loadTexture("food", "food.png");
    tm.loadTexture("linemate",  "brique.png");
    tm.loadTexture("deraumere", "shoe.png");
    // tm.loadTexture("sibur",     "sibur.png");
    // tm.loadTexture("mendiane",  "mendiane.png");
    // tm.loadTexture("phiras",    "phiras.png");
    // tm.loadTexture("thystame",  "thystame.png");
    tm.loadTexture("egg", "egg.png");
}

WindowManager::~WindowManager()
{
    if (_window.isOpen()) {
        _window.close();
    }
    _renderer3D->close();
}

bool WindowManager::isOpen() const
{
    if (_renderMode == RenderMode::ThreeD)
        return _renderer3D->isOpen();
    return _window.isOpen();
}

void WindowManager::create2DWindow(unsigned int width, unsigned int height)
{
    if (_window.isOpen())
        _window.close();

    _width = width;
    _height = height;
    _window.create(sf::VideoMode(width, height), _title);
    _window.setFramerateLimit(60);
    _renderMode = RenderMode::TwoD;
    updateView(width, height);
}

void WindowManager::updateView(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0)
        return;

    sf::Vector2f windowSize(static_cast<float>(width), static_cast<float>(height));

    _worldView.setSize(windowSize.x * _zoomLevel, windowSize.y * _zoomLevel);
    _worldView.setCenter(windowSize.x / 2.0f, windowSize.y / 2.0f);
    _window.setView(_worldView);
}

void WindowManager::zoomView(float factor)
{
    sf::Vector2u size = _window.getSize();

    _zoomLevel = std::clamp(_zoomLevel * factor, MIN_ZOOM, MAX_ZOOM);
    updateView(size.x, size.y);
}

void WindowManager::resetZoom()
{
    sf::Vector2u size = _window.getSize();

    _zoomLevel = 1.0f;
    updateView(size.x, size.y);
}

void WindowManager::handleEvents()
{
    if (_renderMode == RenderMode::ThreeD) {
        if (_renderer3D->shouldToggleTo2D())
            switchTo2D();
        return;
    }

    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window.close();
        } else if (event.type == sf::Event::Resized) {
            _width = event.size.width;
            _height = event.size.height;
            updateView(event.size.width, event.size.height);
        } else if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0)
                zoomView(1.0f / ZOOM_STEP);
            else if (event.mouseWheelScroll.delta < 0)
                zoomView(ZOOM_STEP);
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal ||
                event.key.code == sf::Keyboard::PageUp) {
                zoomView(1.0f / ZOOM_STEP);
            } else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen ||
                event.key.code == sf::Keyboard::PageDown) {
                zoomView(ZOOM_STEP);
            } else if (event.key.code == sf::Keyboard::R) {
                resetZoom();
            } else if (event.key.code == sf::Keyboard::Tab ||
                event.key.code == sf::Keyboard::V) {
                switchTo3D();
            }
        }
    }
}

void WindowManager::clear()
{
    if (_renderMode == RenderMode::ThreeD) {
        _renderer3D->beginFrame();
        return;
    }
    _window.clear(sf::Color(30, 30, 30));
}

void WindowManager::display()
{
    if (_renderMode == RenderMode::ThreeD) {
        _renderer3D->endFrame();
        return;
    }
    _window.display();
}

void WindowManager::drawWorld(const GameWorld& world)
{
    if (_renderMode == RenderMode::ThreeD) {
        _renderer3D->drawWorld(world);
        return;
    }

    int mapWidth = world.getWidth();
    int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty()) return;

    _window.setView(_worldView);

    sf::Vector2u windowSize = _window.getSize();
    float totalMapWidth = mapWidth * TILE_SIZE;
    float totalMapHeight = mapHeight * TILE_SIZE;
    float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) / 2.0f;
    float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) / 2.0f;

    const TextureManager& tm = TextureManager::getInstance();

    sf::Sprite tileSprite(tm.getTexture("ground"));

    const auto& map = world.getMap();
    
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            float posX = x * TILE_SIZE + offsetX;
            float posY = y * TILE_SIZE + offsetY;

            tileSprite.setPosition(posX, posY);
            _window.draw(tileSprite);
            

            GraphicalUtils::drawResources(_window, map[x][y], TILE_SIZE, offsetX, offsetY);
        }
    }

    const auto& players = world.getPlayers();
    for (const auto& pair : players) {
        GraphicalUtils::drawPlayer(_window, pair.second, TILE_SIZE, offsetX, offsetY);
    }

    const auto& eggs = world.getEggs();
    for (const auto& pair : eggs) {
        GraphicalUtils::drawEgg(_window, pair.second, TILE_SIZE, offsetX, offsetY);
    }
}

void WindowManager::switchTo2D()
{
    _renderer3D->close();
    create2DWindow(_width, _height);
}

void WindowManager::switchTo3D()
{
    if (!_renderer3D->isAvailable()) {
        if (!_raylibWarningPrinted) {
            std::cerr << "[WindowManager] Raylib indisponible: installez raylib pour activer la vue 3D." << std::endl;
            _raylibWarningPrinted = true;
        }
        return;
    }

    if (_window.isOpen()) {
        const sf::Vector2u size = _window.getSize();

        _width = size.x;
        _height = size.y;
        _window.close();
    }
    _renderer3D->open(_width, _height, _title + " - 3D");
    _renderMode = RenderMode::ThreeD;
}
