/**
 * @file GraphicalUtils.hpp
 * @brief Declares shared low-level drawing helpers for the SFML renderer.
 */

#ifndef GRAPHICALUTILS_HPP_
#define GRAPHICALUTILS_HPP_

#include <SFML/Graphics.hpp>
#include "GameWorld.hpp"

/**
 * @brief Stateless utility class for drawing tile resources and eggs.
 */
class GraphicalUtils {
public:
    /**
     * @brief Draws every resource sprite contained by a tile.
     */
    static void drawResources(sf::RenderTarget& target, const Tile& tile, float tileSize, float offsetX, float offsetY);

    /**
     * @brief Draws one egg at its map position.
     */
    static void drawEgg(sf::RenderTarget& target, const Egg& egg, float tileSize, float offsetX, float offsetY);
};

#endif /* !GRAPHICALUTILS_HPP_ */
