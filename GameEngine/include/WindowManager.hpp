/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

#ifndef WINDOWMANAGER_HPP_
#define WINDOWMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"
#include <optional>
#include <string>
#include <utility>

class WindowManager {
public:
    WindowManager(unsigned int width = 1920, unsigned int height = 1080, const std::string& title = "Zappy GUI");
    ~WindowManager();

    bool isOpen() const;

    void handleEvents(const GameWorld& world);
    std::optional<std::pair<int, int>> consumeTileRequest();

    void clear();
    void display();

    void drawWorld(const GameWorld& world);

    private:
        void create2DWindow(unsigned int width, unsigned int height);
        void loadTextures();
        void loadFont();
        void updateView(unsigned int width, unsigned int height);
        void zoomView(float factor);
        void resetZoom();
        void update2DPan();
        void select2DTile(const GameWorld& world, int mouseX, int mouseY);
        void drawInfoPanel2D(const GameWorld& world);

        sf::RenderWindow _window;
        sf::View _worldView;
        sf::Font _font;
        sf::Vector2f _viewCenter;
        sf::Clock _panClock;
        std::optional<std::pair<int, int>> _selectedTile;
        std::optional<std::pair<int, int>> _pendingTileRequest;
        float _zoomLevel;
        unsigned int _width;
        unsigned int _height;
        std::string _title;
        bool _fontLoaded;
        sf::Texture _tileTexture;
        sf::Texture _linemateTexture;
    };

#endif /* !WINDOWMANAGER_HPP_ */
