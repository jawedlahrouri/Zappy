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
#include <cmath>
#include <filesystem>
#include <iostream>

namespace {
    constexpr float TILE_SIZE = 64.0f;
    constexpr float ZOOM_STEP = 1.1f;
    constexpr float MIN_ZOOM = 0.35f;
    constexpr float MAX_ZOOM = 3.0f;
    constexpr float PAN_SPEED = 650.0f;
    constexpr float INFO_PANEL_WIDTH = 270.0f;
    constexpr float INFO_PANEL_HEIGHT = 250.0f;
    constexpr float INFO_PANEL_MARGIN = 20.0f;
}

WindowManager::WindowManager(unsigned int width, unsigned int height, const std::string& title)
    : _viewCenter(static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f),
    _selectedTile(std::nullopt),
    _pendingTileRequest(std::nullopt),
    _zoomLevel(1.0f),
    _width(width),
    _height(height),
    _title(title),
    _fontLoaded(false)
{
    create2DWindow(width, height);
    loadTextures();
    loadFont();
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

    tm.loadTexture("PlayerFace", "character_face.png");
    tm.loadTexture("playerBack", "character_back.png");
    tm.loadTexture("PlayerRight", "character_right.png");
    // tm.loadTexture("PlayerLeft", "character_left.png");
}

void WindowManager::loadFont()
{
    const std::string fonts[] = {
        "GameEngine/assets/fonts/NimbusSans-Regular.otf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf",
        "/usr/share/fonts/opentype/urw-base35/NimbusSans-Regular.otf"
    };

    for (const std::string& path : fonts) {
        if (std::filesystem::exists(path) && _font.loadFromFile(path)) {
            _fontLoaded = true;
            return;
        }
    }
    std::cerr << "[WindowManager] Police introuvable: panneau d'information 2D sans texte." << std::endl;
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

void WindowManager::create2DWindow(unsigned int width, unsigned int height)
{
    if (_window.isOpen())
        _window.close();

    _width = width;
    _height = height;
    _window.create(sf::VideoMode(width, height), _title);
    _window.setFramerateLimit(60);
    _viewCenter = {static_cast<float>(width) / 2.0f,
        static_cast<float>(height) / 2.0f};
    _panClock.restart();
    updateView(width, height);
}

void WindowManager::updateView(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0)
        return;

    sf::Vector2f windowSize(static_cast<float>(width), static_cast<float>(height));

    _worldView.setSize(windowSize.x * _zoomLevel, windowSize.y * _zoomLevel);
    _worldView.setCenter(_viewCenter);
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

void WindowManager::update2DPan()
{
    const float deltaTime = std::min(_panClock.restart().asSeconds(), 0.05f);
    const float panStep = PAN_SPEED * _zoomLevel * deltaTime;
    sf::Vector2f movement(0.0f, 0.0f);

    if (!_window.isOpen())
        return;
    if (!_window.hasFocus())
        return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= panStep;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += panStep;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        movement.x -= panStep;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        movement.x += panStep;
    if (movement.x == 0.0f && movement.y == 0.0f)
        return;
    _viewCenter += movement;
    _worldView.setCenter(_viewCenter);
    _window.setView(_worldView);
}

void WindowManager::select2DTile(const GameWorld& world, int mouseX, int mouseY)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty())
        return;

    const sf::Vector2u windowSize = _window.getSize();
    const float panelLeft = static_cast<float>(windowSize.x) -
        INFO_PANEL_WIDTH - INFO_PANEL_MARGIN;
    if (_selectedTile.has_value() && static_cast<float>(mouseX) >= panelLeft &&
        static_cast<float>(mouseY) <= INFO_PANEL_MARGIN + INFO_PANEL_HEIGHT)
        return;

    const sf::Vector2f worldPos = _window.mapPixelToCoords(
        {mouseX, mouseY}, _worldView);
    const float totalMapWidth = static_cast<float>(mapWidth) * TILE_SIZE;
    const float totalMapHeight = static_cast<float>(mapHeight) * TILE_SIZE;
    const float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) / 2.0f;
    const float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) / 2.0f;
    const int tileX = static_cast<int>(std::floor((worldPos.x - offsetX) / TILE_SIZE));
    const int tileY = static_cast<int>(std::floor((worldPos.y - offsetY) / TILE_SIZE));

    if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight)
        return;
    _selectedTile = {tileX, tileY};
    _pendingTileRequest = _selectedTile;
}

std::optional<std::pair<int, int>> WindowManager::consumeTileRequest()
{
    std::optional<std::pair<int, int>> request = _pendingTileRequest;

    _pendingTileRequest.reset();
    return request;
}

void WindowManager::handleEvents(const GameWorld& world)
{
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
            }
        } else if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            select2DTile(world, event.mouseButton.x, event.mouseButton.y);
        }
    }
    update2DPan();
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
            if (_selectedTile.has_value() && _selectedTile->first == x &&
                _selectedTile->second == y) {
                sf::RectangleShape selection({TILE_SIZE, TILE_SIZE});

                selection.setPosition(posX, posY);
                selection.setFillColor(sf::Color::Transparent);
                selection.setOutlineColor(sf::Color(255, 220, 80));
                selection.setOutlineThickness(3.0f);
                _window.draw(selection);
            }
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
    drawInfoPanel2D(world);
}

void WindowManager::drawInfoPanel2D(const GameWorld& world)
{
    if (!_selectedTile.has_value() || !_fontLoaded)
        return;

    const int x = _selectedTile->first;
    const int y = _selectedTile->second;

    if (x < 0 || y < 0 || x >= world.getWidth() || y >= world.getHeight() ||
        world.getMap().empty())
        return;

    const Resources& resources = world.getMap()[x][y].resources;
    const sf::View previousView = _window.getView();
    const sf::Vector2u windowSize = _window.getSize();
    const float panelX = static_cast<float>(windowSize.x) -
        INFO_PANEL_WIDTH - INFO_PANEL_MARGIN;
    const float panelY = INFO_PANEL_MARGIN;
    sf::RectangleShape panel({INFO_PANEL_WIDTH, INFO_PANEL_HEIGHT});
    sf::Text text;

    _window.setView(_window.getDefaultView());
    panel.setPosition(panelX, panelY);
    panel.setFillColor(sf::Color(18, 24, 30, 225));
    panel.setOutlineColor(sf::Color(255, 220, 80, 230));
    panel.setOutlineThickness(2.0f);
    _window.draw(panel);

    text.setFont(_font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color(235, 240, 245));

    const auto drawLine = [&](const std::string& value, int row, unsigned int size = 16) {
        text.setString(value);
        text.setCharacterSize(size);
        text.setPosition(panelX + 18.0f, panelY + 16.0f + static_cast<float>(row) * 25.0f);
        _window.draw(text);
    };

    drawLine("Case " + std::to_string(x) + " " + std::to_string(y), 0, 20);
    drawLine("food: " + std::to_string(resources.food), 2);
    drawLine("linemate: " + std::to_string(resources.linemate), 3);
    drawLine("deraumere: " + std::to_string(resources.deraumere), 4);
    drawLine("sibur: " + std::to_string(resources.sibur), 5);
    drawLine("mendiane: " + std::to_string(resources.mendiane), 6);
    drawLine("phiras: " + std::to_string(resources.phiras), 7);
    drawLine("thystame: " + std::to_string(resources.thystame), 8);
    _window.setView(previousView);
}
