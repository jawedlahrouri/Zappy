/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

#include "WindowManager.hpp"
#include "GraphicalUtils.hpp"
#include "TextureManager.hpp"
#include <iostream>

WindowManager::WindowManager(unsigned int width, unsigned int height, const std::string& title)
{
    _window.create(sf::VideoMode(width, height), title);
    _window.setFramerateLimit(60);

    TextureManager& tm = TextureManager::getInstance();
    
    tm.loadTexture("ground", "ground.png");
    tm.loadTexture("food", "food.png");
    tm.loadTexture("linemate",  "brique.png");
    // tm.loadTexture("deraumere", "deraumere.png");
    // tm.loadTexture("sibur",     "sibur.png");
    // tm.loadTexture("mendiane",  "mendiane.png");
    // tm.loadTexture("phiras",    "phiras.png");
    // tm.loadTexture("thystame",  "thystame.png");
}

WindowManager::~WindowManager()
{
    if (_window.isOpen()) {
        _window.close();
    }
}

bool WindowManager::isOpen() const
{
    return _window.isOpen();
}

void WindowManager::handleEvents()
{
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _window.close();
        }
    }
}

void WindowManager::clear()
{
    _window.clear(sf::Color(30, 30, 30));
}

void WindowManager::display()
{
    _window.display();
}

void WindowManager::drawWorld(const GameWorld& world)
{
    int mapWidth = world.getWidth();
    int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty()) return;

    float tileSize = 64.0f; 

    float totalMapWidth = mapWidth * tileSize;
    float totalMapHeight = mapHeight * tileSize;
    float offsetX = (1920.0f - totalMapWidth) / 2.0f;
    float offsetY = (1080.0f - totalMapHeight) / 2.0f;

    const TextureManager& tm = TextureManager::getInstance();

    sf::Sprite tileSprite(tm.getTexture("ground"));

    const auto& map = world.getMap();
    
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            float posX = x * tileSize + offsetX;
            float posY = y * tileSize + offsetY;

            tileSprite.setPosition(posX, posY);
            _window.draw(tileSprite);
            
            GraphicalUtils::drawResources(_window, map[x][y], tileSize, offsetX, offsetY);
        }
    }

    const auto& players = world.getPlayers();
    for (const auto& pair : players) {
        GraphicalUtils::drawPlayer(_window, pair.second, tileSize, offsetX, offsetY);
    }
}