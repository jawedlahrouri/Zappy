/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

#include "WindowManager.hpp"
#include "InfoPanel.hpp"
#include "Menu.hpp"
#include "Players.hpp"
#include "TextureManager.hpp"
#include "World.hpp"
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <limits>

WindowManager::WindowManager(unsigned int width, unsigned int height, const std::string& title)
    : _viewCenter(static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f),
    _selectedTile(std::nullopt),
    _selectedPlayerId(std::nullopt),
    _pendingTileRequest(std::nullopt),
    _pendingTimeRequest(std::nullopt),
    _openedTeamInventory(std::nullopt),
    _zoomLevel(1.0f),
    _width(width),
    _height(height),
    _title(title),
    _fontLoaded(false),
    _menuMusicLoaded(false),
    _menuIp("127.0.0.1"),
    _menuPort("4242"),
    _menuStatus(""),
    _menuTeamNames(),
    _menuConnectionEdited(false),
    _showGrid(false),
    _showInfoPanel(true),
    _serverFreq(100),
    _resumeServerFreq(100),
    _timePaused(false),
    _activeMenuField(IP_FIELD)
{
    create2DWindow(width, height);
    loadTextures();
    loadFont();
    loadMenuMusic();
}

void WindowManager::loadTextures()
{
    TextureManager& tm = TextureManager::getInstance();
    
    tm.loadTexture("menuBackground", "background_menu.png");
    tm.loadTexture("spursBackground", "spurs_background.png");
    tm.loadTexture("ground", "ground.png");
    tm.loadTexture("food", "food.png");
    tm.loadTexture("linemate",  "brique.png");
    tm.loadTexture("deraumere", "shoe.png");
    tm.loadTexture("sibur", "ballon.png");
    tm.loadTexture("mendiane", "jersey.png");
    tm.loadTexture("phiras", "trophy.png");
    tm.loadTexture("thystame",  "carte.png");
    tm.loadTexture("egg", "oeuf.png");
    tm.loadTexture("timePauseButton", "1boutonpause.png");
    tm.loadTexture("timePlayButton", "boutonplay.png");
    tm.loadTexture("timeSlowerButton", "boutongauche.png");
    tm.loadTexture("timeFasterButton", "boutondroite.png");

    tm.loadTexture("playerFace", "character_face.png");
    tm.loadTexture("playerBack", "character_back.png");
    tm.loadTexture("playerRight", "character_right.png");
    tm.loadTexture("playerLeft", "character_left.png");
    tm.loadTexture("whiteRedFace", "white_red_face.png");
    tm.loadTexture("whiteRedBack", "white_red_back.png");
    tm.loadTexture("whiteRedRight", "white_red_right.png");
    tm.loadTexture("whiteRedLeft", "white_red_left.png");

    tm.loadTexture("sayen01", "sayen01.png");
    tm.loadTexture("sayen02", "sayen02.png");
    tm.loadTexture("sayen03", "sayen03.png");
    tm.loadTexture("sayen04", "sayen04.png");
    tm.loadTexture("sayen05", "sayen05.png");
    tm.loadTexture("sayen06", "sayen06.png");
    tm.loadTexture("sayen07", "sayen07.png");
    tm.loadTexture("sayen08", "sayen08.png");
    tm.loadTexture("sayen09", "sayen09.png");
    tm.loadTexture("sayen10", "sayen10.png");
    tm.loadTexture("sayen11", "sayen11.png");
    tm.loadTexture("sayen12", "sayen12.png");
    tm.loadTexture("sayen13", "sayen13.png");
    tm.loadTexture("sayen14", "sayen14.png");
    tm.loadTexture("sayen15", "sayen15.png");
    tm.loadTexture("sayen16", "sayen16.png");
    tm.loadTexture("sayen17", "sayen17.png");

    tm.loadTexture("bluePlayer", "blue_character.png");
    tm.loadTexture("blueBack", "blue_back.png");
    tm.loadTexture("blueRight", "blue_right.png");
    tm.loadTexture("blueLeft", "blue_left.png");
    tm.loadTexture("whiteBlueFace", "white_blue_face.png");
    tm.loadTexture("whiteBlueBack", "white_blue_back.png");
    tm.loadTexture("whiteBlueRight", "white_blue_right.png");
    tm.loadTexture("whiteBlueLeft", "white_blue_left.png");

    tm.loadTexture("greenPlayer", "green_character.png");
    tm.loadTexture("greenBack", "green_back.png");
    tm.loadTexture("greenRight", "green_right.png");
    tm.loadTexture("greenLeft", "green_left.png");
    tm.loadTexture("whiteGreenFace", "white_green_face.png");
    tm.loadTexture("whiteGreenBack", "white_green_back.png");
    tm.loadTexture("whiteGreenRight", "white_green_right.png");
    tm.loadTexture("whiteGreenLeft", "white_green_left.png");

    tm.loadTexture("pinkPlayer", "pink_character.png");
    tm.loadTexture("pinkBack", "pink_back.png");
    tm.loadTexture("pinkRight", "pink_right.png");
    tm.loadTexture("pinkLeft", "pink_left.png");
    tm.loadTexture("whitePinkFace", "white_pink_face.png");
    tm.loadTexture("whitePinkBack", "white_pink_back.png");
    tm.loadTexture("whitePinkRight", "white_pink_right.png");
    tm.loadTexture("whitePinkLeft", "white_pink_left.png");
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

void WindowManager::loadMenuMusic()
{
    const std::string musicPath = "GameEngine/assets/sprites/Chicago_Bulls_intro.mp3";

    if (!_menuMusic.openFromFile(musicPath)) {
        std::cerr << "[WindowManager] Musique de menu introuvable: "
            << musicPath << std::endl;
        return;
    }
    _menuMusic.setLoop(true);
    _menuMusicLoaded = true;
}

void WindowManager::playMenuMusic()
{
    if (!_menuMusicLoaded)
        return;
    if (_menuMusic.getStatus() != sf::Music::Playing)
        _menuMusic.play();
}

void WindowManager::stopMenuMusic()
{
    if (!_menuMusicLoaded)
        return;
    if (_menuMusic.getStatus() != sf::Music::Stopped)
        _menuMusic.stop();
}

WindowManager::~WindowManager()
{
    stopMenuMusic();
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

bool WindowManager::select2DTile(const GameWorld& world, int mouseX, int mouseY)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty())
        return false;
    if (isInsideSelectionPanel(mouseX, mouseY))
        return false;

    const sf::Vector2u windowSize = _window.getSize();
    const sf::Vector2f worldPos = _window.mapPixelToCoords({mouseX, mouseY}, _worldView);
    const float totalMapWidth = static_cast<float>(mapWidth) * WorldRenderer::TILE_SIZE;
    const float totalMapHeight = static_cast<float>(mapHeight) * WorldRenderer::TILE_SIZE;
    const float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) / 2.0f;
    const float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) / 2.0f;
    const int tileX = static_cast<int>(std::floor((worldPos.x - offsetX) / WorldRenderer::TILE_SIZE));
    const int tileY = static_cast<int>(std::floor((worldPos.y - offsetY) / WorldRenderer::TILE_SIZE));

    if (tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight)
        return false;
    _selectedTile = {tileX, tileY};
    _selectedPlayerId.reset();
    _pendingTileRequest = _selectedTile;
    return true;
}

bool WindowManager::select2DPlayer(const GameWorld& world, int mouseX, int mouseY)
{
    const int mapWidth = world.getWidth();
    const int mapHeight = world.getHeight();

    if (mapWidth <= 0 || mapHeight <= 0 || world.getMap().empty())
        return false;
    if (isInsideSelectionPanel(mouseX, mouseY))
        return false;

    const sf::Vector2u windowSize = _window.getSize();
    const sf::Vector2f worldPos = _window.mapPixelToCoords({mouseX, mouseY}, _worldView);
    const float totalMapWidth = static_cast<float>(mapWidth) * WorldRenderer::TILE_SIZE;
    const float totalMapHeight = static_cast<float>(mapHeight) * WorldRenderer::TILE_SIZE;
    const float offsetX = (static_cast<float>(windowSize.x) - totalMapWidth) / 2.0f;
    const float offsetY = (static_cast<float>(windowSize.y) - totalMapHeight) / 2.0f;
    int selectedPlayerId = 0;

    if (!Players::pickPlayer(world, worldPos, offsetX, offsetY,
            selectedPlayerId))
        return false;
    const auto& players = world.getPlayers();
    const auto it = players.find(selectedPlayerId);

    if (it == players.end())
        return false;
    _selectedPlayerId = selectedPlayerId;
    _selectedTile = {it->second.x, it->second.y};
    _pendingTileRequest.reset();
    return true;
}

bool WindowManager::handleTimeControlMousePressed(int mouseX, int mouseY)
{
    if (!isInsideTimeControlPanel(mouseX, mouseY))
        return false;

    const sf::Vector2u windowSize = _window.getSize();
    const sf::FloatRect timePanel =
        InfoPanelRenderer::getTimePanelRect(windowSize);
    const float panelX = timePanel.left;
    const float panelY = timePanel.top;
    const float buttonsY = panelY + 62.0f;
    const float buttonWidth = InfoPanelRenderer::TIME_BUTTON_WIDTH;
    const float buttonHeight = InfoPanelRenderer::TIME_BUTTON_HEIGHT;
    const float gap = 24.0f;
    const sf::FloatRect slowerButton(panelX + 16.0f, buttonsY, buttonWidth,
        buttonHeight);
    const sf::FloatRect pauseButton(panelX + 16.0f + buttonWidth + gap,
        buttonsY, buttonWidth, buttonHeight);
    const sf::FloatRect fasterButton(panelX + 16.0f +
        (buttonWidth + gap) * 2.0f, buttonsY, buttonWidth, buttonHeight);
    const float mx = static_cast<float>(mouseX);
    const float my = static_cast<float>(mouseY);

    if (pauseButton.contains(mx, my)) {
        if (_timePaused) {
            _serverFreq = std::clamp(_resumeServerFreq, MIN_SERVER_FREQ,
                MAX_SERVER_FREQ);
            _timePaused = false;
        } else {
            _resumeServerFreq = std::clamp(_serverFreq, MIN_SERVER_FREQ,
                MAX_SERVER_FREQ);
            _serverFreq = MIN_SERVER_FREQ;
            _timePaused = true;
        }
        _pendingTimeRequest = _serverFreq;
        return true;
    }
    if (slowerButton.contains(mx, my)) {
        if (_timePaused) {
            _resumeServerFreq = std::clamp(_resumeServerFreq / 2,
                MIN_SERVER_FREQ, MAX_SERVER_FREQ);
        } else {
            _serverFreq = std::clamp(_serverFreq / 2, MIN_SERVER_FREQ,
                MAX_SERVER_FREQ);
            _resumeServerFreq = _serverFreq;
            _pendingTimeRequest = _serverFreq;
        }
        return true;
    }
    if (fasterButton.contains(mx, my)) {
        if (_timePaused) {
            _resumeServerFreq = std::clamp(_resumeServerFreq * 2,
                MIN_SERVER_FREQ, MAX_SERVER_FREQ);
        } else {
            _serverFreq = std::clamp(_serverFreq * 2, MIN_SERVER_FREQ,
                MAX_SERVER_FREQ);
            _resumeServerFreq = _serverFreq;
            _pendingTimeRequest = _serverFreq;
        }
        return true;
    }
    return true;
}

bool WindowManager::handleGlobalInfoMousePressed(const GameWorld& world,
    int mouseX, int mouseY)
{
    if (!isInsideGlobalInfoPanel(mouseX, mouseY))
        return false;

    const std::optional<std::string> team =
        InfoPanelRenderer::getTeamInventoryAt(world, _openedTeamInventory,
            mouseX, mouseY);

    if (team.has_value()) {
        if (_openedTeamInventory.has_value() && *_openedTeamInventory == *team)
            _openedTeamInventory.reset();
        else
            _openedTeamInventory = *team;
    }
    return true;
}

bool WindowManager::isInsideSelectionPanel(int mouseX, int mouseY) const
{
    if (!_selectedPlayerId.has_value() && !_selectedTile.has_value())
        return false;

    const sf::Vector2u windowSize = _window.getSize();
    const float panelX = static_cast<float>(windowSize.x) -
        (_selectedPlayerId.has_value() ?
        InfoPanelRenderer::PLAYER_PANEL_WIDTH :
        InfoPanelRenderer::INFO_PANEL_WIDTH) -
        InfoPanelRenderer::INFO_PANEL_MARGIN;
    const float panelY = InfoPanelRenderer::INFO_PANEL_MARGIN;
    const float panelWidth = _selectedPlayerId.has_value() ?
        InfoPanelRenderer::PLAYER_PANEL_WIDTH :
        InfoPanelRenderer::INFO_PANEL_WIDTH;
    const float panelHeight = _selectedPlayerId.has_value() ?
        InfoPanelRenderer::PLAYER_PANEL_HEIGHT :
        InfoPanelRenderer::INFO_PANEL_HEIGHT;

    return static_cast<float>(mouseX) >= panelX &&
        static_cast<float>(mouseX) <= panelX + panelWidth &&
        static_cast<float>(mouseY) >= panelY &&
        static_cast<float>(mouseY) <= panelY + panelHeight;
}

bool WindowManager::isInsideTimeControlPanel(int mouseX, int mouseY) const
{
    const sf::Vector2u windowSize = _window.getSize();
    const sf::FloatRect panel =
        InfoPanelRenderer::getTimePanelRect(windowSize);

    return panel.contains(static_cast<float>(mouseX),
        static_cast<float>(mouseY));
}

bool WindowManager::isInsideGlobalInfoPanel(int mouseX, int mouseY) const
{
    if (!_showInfoPanel)
        return false;

    const sf::FloatRect panel(InfoPanelRenderer::INFO_PANEL_MARGIN,
        InfoPanelRenderer::INFO_PANEL_MARGIN,
        InfoPanelRenderer::GLOBAL_PANEL_WIDTH,
        InfoPanelRenderer::GLOBAL_PANEL_HEIGHT);

    return panel.contains(static_cast<float>(mouseX),
        static_cast<float>(mouseY));
}

void WindowManager::clearSelection()
{
    _selectedTile.reset();
    _selectedPlayerId.reset();
    _pendingTileRequest.reset();
}

std::optional<std::pair<int, int>> WindowManager::consumeTileRequest()
{
    std::optional<std::pair<int, int>> request = _pendingTileRequest;

    _pendingTileRequest.reset();
    return request;
}

std::optional<int> WindowManager::consumeTimeRequest()
{
    std::optional<int> request = _pendingTimeRequest;

    _pendingTimeRequest.reset();
    return request;
}

void WindowManager::setServerFreq(int freq)
{
    _serverFreq = std::clamp(freq, MIN_SERVER_FREQ, MAX_SERVER_FREQ);
    if (!_timePaused)
        _resumeServerFreq = _serverFreq;
}

int WindowManager::getServerFreq() const
{
    return _serverFreq;
}

bool WindowManager::isTimePaused() const
{
    return _timePaused;
}

const std::string& WindowManager::getMenuIp() const
{
    return _menuIp;
}

const std::string& WindowManager::getMenuPort() const
{
    return _menuPort;
}

void WindowManager::setMenuConnectionInfo(const std::string& ip, const std::string& port)
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
    if (unicode == 8) {
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
        } else if (event.key.code == sf::Keyboard::G) {
            _showGrid = !_showGrid;
        } else if (event.key.code == sf::Keyboard::I) {
            _showInfoPanel = !_showInfoPanel;
        }
    } else if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        if (handleTimeControlMousePressed(event.mouseButton.x, event.mouseButton.y))
            return false;
        if (handleGlobalInfoMousePressed(world, event.mouseButton.x,
            event.mouseButton.y))
            return false;
        if (isInsideSelectionPanel(event.mouseButton.x, event.mouseButton.y))
            return false;
        if (select2DPlayer(world, event.mouseButton.x, event.mouseButton.y))
            return false;
        if (select2DTile(world, event.mouseButton.x, event.mouseButton.y))
            return false;
        clearSelection();
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
    playMenuMusic();
    Menu::draw(_window, _font, _fontLoaded, _menuIp, _menuPort,
        _activeMenuField, _menuStatus, _menuTeamNames);
}

void WindowManager::drawWorld(const GameWorld& world)
{
    stopMenuMusic();
    WorldRenderer::draw(_window, _worldView, world, _selectedTile,
        _selectedPlayerId, _font, _fontLoaded, _showGrid, _serverFreq,
        _timePaused, _showInfoPanel, _openedTeamInventory);
}
