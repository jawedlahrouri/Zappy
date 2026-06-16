#ifndef GRAPHICALUTILS_HPP_
#define GRAPHICALUTILS_HPP_

#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"

class GraphicalUtils {
public:
    static void drawResources(sf::RenderTarget& target, const Tile& tile, float tileSize, float offsetX, float offsetY);
    static void drawPlayer(sf::RenderTarget& target, const Player& player, float tileSize, float offsetX, float offsetY);
    static void drawPlayerPortrait(sf::RenderTarget& target, const Player& player, float x, float y, float size);
    static void drawEgg(sf::RenderTarget& target, const Egg& egg, float tileSize, float offsetX, float offsetY);

private:
    static std::string getPlayerTextureKey(const Player& player);
    static std::string getPlayerTextureKey(const Player& player, int orientation);
};

#endif /* !GRAPHICALUTILS_HPP_ */
