#ifndef GRAPHICALUTILS_HPP_
#define GRAPHICALUTILS_HPP_

#include <SFML/Graphics.hpp>
    #include "GameWorld.hpp"

    class GraphicalUtils {
    public:
        static void drawResources(sf::RenderTarget& target, const Tile& tile, float tileSize, float offsetX, float offsetY);
        static void drawPlayer(sf::RenderTarget& target, const Player& player, float tileSize, float offsetX, float offsetY);
        static void drawEgg(sf::RenderTarget& target, const Egg& egg, float tileSize, float offsetX, float offsetY);
    };

#endif /* !GRAPHICALUTILS_HPP_ */