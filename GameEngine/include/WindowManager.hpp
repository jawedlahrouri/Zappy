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
        void updateView(unsigned int width, unsigned int height);
        void zoomView(float factor);
        void resetZoom();

        sf::RenderWindow _window;
        sf::View _worldView;
        float _zoomLevel;
        sf::Texture _tileTexture;
        sf::Texture _linemateTexture;
    };

#endif /* !WINDOWMANAGER_HPP_ */
