/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GraphicalUtils
*/

#include "GraphicalUtils.hpp"
#include "TextureManager.hpp"
#include <cmath>

void GraphicalUtils::drawResources(sf::RenderTarget& target, const Tile& tile, float tileSize, float offsetX, float offsetY)
{
    float startX = tile.x * tileSize + offsetX;
    float startY = tile.y * tileSize + offsetY;

    const TextureManager& tm = TextureManager::getInstance();

    float resScale = 0.25f;

    sf::Sprite foodSprite(tm.getTexture("food"));
    sf::Sprite linemateSprite(tm.getTexture("linemate"));
    sf::Sprite deraumereSprite(tm.getTexture("deraumere"));
    sf::Sprite siburSprite(tm.getTexture("sibur"));
    sf::Sprite mendianeSprite(tm.getTexture("mendiane"));
    sf::Sprite phirasSprite(tm.getTexture("phiras"));
    sf::Sprite thystameSprite(tm.getTexture("thystame"));

    foodSprite.setScale(resScale, resScale);
    linemateSprite.setScale(resScale, resScale);
    deraumereSprite.setScale(resScale, resScale);
    siburSprite.setScale(resScale, resScale);
    mendianeSprite.setScale(resScale, resScale);
    phirasSprite.setScale(resScale, resScale);
    thystameSprite.setScale(resScale / 4, resScale / 4);


    for (int i = 0; i < tile.resources.food; i++) {
        foodSprite.setPosition(startX + 4.0f + (static_cast<float>(i) * 2.5f), startY + 4.0f);
        target.draw(foodSprite);
    }

    for (int i = 0; i < tile.resources.linemate; i++) {
        linemateSprite.setPosition(startX + 44.0f + (static_cast<float>(i) * 2.5f), startY + 4.0f);
        target.draw(linemateSprite);
    }

    for (int i = 0; i < tile.resources.deraumere; i++) {
        deraumereSprite.setPosition(startX + 4.0f + (static_cast<float>(i) * 2.5f), startY + 24.0f);
        target.draw(deraumereSprite);
    }

    for (int i = 0; i < tile.resources.sibur; i++) {
        siburSprite.setPosition(startX + 4.0f + (static_cast<float>(i) * 2.5f), startY + 44.0f);
        target.draw(siburSprite);
    }

    for (int i = 0; i < tile.resources.mendiane; i++) {
        mendianeSprite.setPosition(startX + 44.0f + (static_cast<float>(i) * 2.5f), startY + 24.0f);
        target.draw(mendianeSprite);
    }

    for (int i = 0; i < tile.resources.phiras; i++) {
        phirasSprite.setPosition(startX + 44.0f + (static_cast<float>(i) * 2.5f), startY + 44.0f);
        target.draw(phirasSprite);
    }

    for (int i = 0; i < tile.resources.thystame; i++) {
        thystameSprite.setPosition(startX + 25.5f + (static_cast<float>(i) * 2.5f), startY + 4.0f);
        target.draw(thystameSprite);
    }
}

void GraphicalUtils::drawEgg(sf::RenderTarget& target, const Egg& egg, float tileSize, float offsetX, float offsetY)
{
    float startX = egg.x * tileSize + offsetX;
    float startY = egg.y * tileSize + offsetY;

    const TextureManager& tm = TextureManager::getInstance();
    sf::Sprite eggSprite(tm.getTexture("egg"));

    float eggScale = 0.3f;
    eggSprite.setScale(eggScale, eggScale);
    eggSprite.setColor(sf::Color(255, 255, 255, 255));

    float posX = startX + (tileSize / 2.0f) - (19.0f / 2.0f);
    float posY = startY + tileSize - 24.0f;

    eggSprite.setPosition(posX, posY);

    target.draw(eggSprite);
}
