/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

#include "WindowManager.hpp"
#include "Menu.hpp"
#include "TextureManager.hpp"
#include "World.hpp"
#include <cmath>
#include <filesystem>

WindowManager::WindowManager(unsigned int width, unsigned int height, const std::string& title)
    : _viewCenter(static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f),
    _selectedTile(std::nullopt),
    _pendingTileRequest(std::nullopt),
    _zoomLevel(1.0f),
    _width(width),
    _height(height),
    _title(title),
    _fontLoaded(false),
    _menuIp("127.0.0.1"),
    _menuPort("4242"),
    _menuStatus(""),
    _menuTeamNames(),
    _menuConnectionEdited(false),
    _activeMenuField(IP_FIELD)
{
    create2DWindow(width, height);
    loadTextures();
    loadFont();
}

void WindowManager::loadTextures()
{
    TextureManager& tm = TextureManager::getInstance();
    
    tm.loadTexture("menuBackground", "background_menu.png");
    tm.loadTexture("ground", "ground.png");
    tm.loadTexture("food", "food.png");
    tm.loadTexture("linemate",  "brique.png");
    tm.loadTexture("deraumere", "shoe.png");
    tm.loadTexture("sibur", "ballon.png");
    tm.loadTexture("mendiane", "jersey.png");
    tm.loadTexture("phiras", "trophy.png");
    tm.loadTexture("thystame",  "carte.png");
    tm.loadTexture("egg", "oeuf.png");

    tm.loadTexture("playerFace", "character_face.png");
    tm.loadTexture("bluePlayer", "bleu_character.png");
    tm.loadTexture("playerBack", "character_back.png");
    tm.loadTexture("playerRight", "character_right.png");
    tm.loadTexture("playerLeft", "character_left.png");
}

void WindowManager::loadFont()
{
    const std::string fonts[] = {
        "GameEngine/assets/fonts/arial.ttf",
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
    if (_window.isOpen())
        _window.close();
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
    _window.requestFocus();
    _viewCenter = {static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f};
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        movement.y -= panStep;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        movement.y += panStep;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
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
    const float panelLeft = static_cast<float>(windowSize.x) - WorldRenderer::INFO_PANEL_WIDTH - WorldRenderer::INFO_PANEL_MARGIN;
    if (_selectedTile.has_value() && static_cast<float>(mouseX) >= panelLeft && static_cast<float>(mouseY) <= WorldRenderer::INFO_PANEL_MARGIN + WorldRenderer::INFO_PANEL_HEIGHT)
        return;

    const sf::Vector2f worldPos = _window.mapPixelToCoords({mouseX, mouseY}, _worldView);
    const float totalMapWidth = static_cast<float>(mapWidth) * WorldRenderer::TILE_SIZE;
    const float totalMapHeight = static_cast<float>(mapHeight) * WorldRenderer::TILE_SIZE;
    const float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) / 2.0f;
    const float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) / 2.0f;
    const int tileX = static_cast<int>(std::floor((worldPos.x - offsetX) / WorldRenderer::TILE_SIZE));
    const int tileY = static_cast<int>(std::floor((worldPos.y - offsetY) / WorldRenderer::TILE_SIZE));

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

const std::string& WindowManager::getMenuIp() const
{
    return _menuIp;
}

const std::string& WindowManager::getMenuPort() const
{
    return _menuPort;
}

void WindowManager::setMenuConnectionInfo(const std::string& ip,
    const std::string& port)
{
    _menuIp = ip;
    _menuPort = port;
}

void WindowManager::setMenuStatus(const std::string& status)
{
    _menuStatus = status;
}

void WindowManager::resetMenuTeamNames()
{
    _menuTeamNames.clear();
}

void WindowManager::addMenuTeamName(const std::string& teamName)
{
    if (teamName.empty())
        return;
    if (std::find(_menuTeamNames.begin(), _menuTeamNames.end(), teamName) != _menuTeamNames.end())
        return;
    _menuTeamNames.push_back(teamName);
}

bool WindowManager::consumeMenuConnectionEdited()
{
    const bool wasEdited = _menuConnectionEdited;

    _menuConnectionEdited = false;
    return wasEdited;
}

bool WindowManager::handleWindowEvent(const sf::Event& event)
{
    if (event.type == sf::Event::Closed) {
        _window.close();
        return true;
    }
    if (event.type == sf::Event::Resized) {
        _width = event.size.width;
        _height = event.size.height;
        updateView(event.size.width, event.size.height);
        return true;
    }
    return false;
}

std::string& WindowManager::getActiveMenuInput()
{
    if (_activeMenuField == IP_FIELD)
        return _menuIp;
    return _menuPort;
}

void WindowManager::handleMenuTextEntered(sf::Uint32 unicode)
{
    std::string& input = getActiveMenuInput();

    _menuStatus.clear();
    if (unicode == 8 || unicode == 127) {
        if (!input.empty()) {
            input.pop_back();
            _menuConnectionEdited = true;
        }
        return;
    }
    if (unicode < 32 || unicode > 126)
        return;
    const char character = static_cast<char>(unicode);

    if (_activeMenuField == PORT_FIELD) {
        if (std::isdigit(static_cast<unsigned char>(character)) &&
            input.size() < 5) {
            input += character;
            _menuConnectionEdited = true;
        }
        return;
    }
    if ((std::isalnum(static_cast<unsigned char>(character)) ||
        character == '.' || character == '-' || character == ':') &&
        input.size() < 64) {
        input += character;
        _menuConnectionEdited = true;
    }
}

bool WindowManager::handleMenuMousePressed(const sf::Event& event)
{
    const sf::Vector2u windowSize = _window.getSize();
    const float centerX = static_cast<float>(windowSize.x) / 2.0f;
    const float centerY = static_cast<float>(windowSize.y) / 2.0f;
    const float mouseX = static_cast<float>(event.mouseButton.x);
    const float mouseY = static_cast<float>(event.mouseButton.y);
    const sf::FloatRect ipBox(centerX - 180.0f, centerY - 118.0f, 360.0f, 46.0f);
    const sf::FloatRect portBox(centerX - 180.0f, centerY - 13.0f, 360.0f, 46.0f);
    const sf::FloatRect startButton(centerX - 150.0f, centerY + 76.0f, 300.0f, 58.0f);

    if (ipBox.contains(mouseX, mouseY)) {
        _activeMenuField = IP_FIELD;
        return false;
    }
    if (portBox.contains(mouseX, mouseY)) {
        _activeMenuField = PORT_FIELD;
        return false;
    }
    return startButton.contains(mouseX, mouseY);
}

bool WindowManager::handleGameEvent(const sf::Event& event, const GameWorld& world)
{
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.delta > 0)
            zoomView(1.0f / ZOOM_STEP);
        else if (event.mouseWheelScroll.delta < 0)
            zoomView(ZOOM_STEP);
    } else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape)
            return true;
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
    return false;
}

bool WindowManager::handleMenuEvents()
{
    sf::Event event;
    bool startGame = false;

    while (_window.pollEvent(event)) {
        if (handleWindowEvent(event))
            continue;
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 13 || event.text.unicode == 10) {
                startGame = true;
                continue;
            }
            handleMenuTextEntered(event.text.unicode);
        } else if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            if (handleMenuMousePressed(event))
                startGame = true;
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter)
                startGame = true;
            else if (event.key.code == sf::Keyboard::Tab) {
                if (_activeMenuField == IP_FIELD)
                    _activeMenuField = PORT_FIELD;
                else
                    _activeMenuField = IP_FIELD;
            } else if (event.key.code == sf::Keyboard::Escape)
                _window.close();
        }
    }
    return startGame;
}

bool WindowManager::handleGameEvents(const GameWorld& world)
{
    sf::Event event;
    bool backToMenu = false;

    while (_window.pollEvent(event)) {
        if (handleWindowEvent(event))
            continue;
        if (handleGameEvent(event, world))
            backToMenu = true;
    }
    if (!backToMenu)
        update2DPan();
    return backToMenu;
}

void WindowManager::clear()
{
    _window.clear(sf::Color(30, 30, 30));
}

void WindowManager::display()
{
    _window.display();
}

void WindowManager::drawMenu()
{
    Menu::draw(_window, _font, _fontLoaded, _menuIp, _menuPort,
        _activeMenuField, _menuStatus, _menuTeamNames);
}

void WindowManager::drawWorld(const GameWorld& world)
{
    WorldRenderer::draw(_window, _worldView, world, _selectedTile, _font,
        _fontLoaded);
}
