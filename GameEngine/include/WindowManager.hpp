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
#include "RaylibRenderer.hpp"
#include <memory>
#include <string>

class WindowManager {
public:
    WindowManager(unsigned int width = 1920, unsigned int height = 1080, const std::string& title = "Zappy GUI");
    ~WindowManager();

    bool isOpen() const;

    void handleEvents();

    void clear();
    void display();

    void drawWorld(const GameWorld& world);

    private:
        enum class RenderMode {
            TwoD,
            ThreeD
        };

        void create2DWindow(unsigned int width, unsigned int height);
        void loadTextures();
        void updateView(unsigned int width, unsigned int height);
        void zoomView(float factor);
        void resetZoom();
        void switchTo2D();
        void switchTo3D();

        sf::RenderWindow _window;
        sf::View _worldView;
        std::unique_ptr<RaylibRenderer> _renderer3D;
        RenderMode _renderMode;
        float _zoomLevel;
        unsigned int _width;
        unsigned int _height;
        std::string _title;
        bool _raylibWarningPrinted;
        sf::Texture _tileTexture;
        sf::Texture _linemateTexture;
    };

#endif /* !WINDOWMANAGER_HPP_ */
