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

    if (tile.resources.food > 0) {
        foodSprite.setPosition(startX + 4.0f, startY + 4.0f);
        target.draw(foodSprite);
    }

    if (tile.resources.linemate > 0) {
        linemateSprite.setPosition(startX + 44.0f, startY + 4.0f);
        target.draw(linemateSprite);
    }

    if (tile.resources.deraumere > 0) {
        deraumereSprite.setPosition(startX + 4.0f, startY + 24.0f);
        target.draw(deraumereSprite);
    }

    if (tile.resources.sibur > 0) {
        siburSprite.setPosition(startX + 4.0f, startY + 44.0f);
        target.draw(siburSprite);
    }

    if (tile.resources.mendiane > 0) {
        mendianeSprite.setPosition(startX + 44.0f, startY + 24.0f);
        target.draw(mendianeSprite);
    }

    if (tile.resources.phiras > 0) {
        phirasSprite.setPosition(startX + 44.0f, startY + 44.0f);
        target.draw(phirasSprite);
    }

    if (tile.resources.thystame > 0) {
        thystameSprite.setPosition(startX + 25.5f, startY + 44.0f);
        target.draw(thystameSprite);
    }
}

void GraphicalUtils::drawPlayer(sf::RenderTarget& target, const Player& player, float tileSize, float offsetX, float offsetY)
{
    if (player.level < 1)
        return;
    float centerX = player.x * tileSize + offsetX + (tileSize / 2.0f);
    float centerY = player.y * tileSize + offsetY + (tileSize / 2.0f);

    const TextureManager& tm = TextureManager::getInstance();
    std::string texKey = "playerFace";
    // Orientation : 1: N, 2: E, 3: S, 4: W
    if (player.orientation == 1)
        texKey = "playerBack";
    else if (player.orientation == 2)
        texKey = "playerRight";
    else if (player.orientation == 3)
        texKey = "playerFace";
    else if (player.orientation == 4)
        texKey = "playerLeft";
    
    sf::Sprite playerSprite(tm.getTexture(texKey));
    sf::Vector2u texSize = tm.getTexture(texKey).getSize();
    if (texSize.x > 0 && texSize.y > 0) {
        float desired = tileSize * 0.5f;
        float scaleX = desired / static_cast<float>(texSize.x);
        float scaleY = desired / static_cast<float>(texSize.y);
        playerSprite.setScale(scaleX, scaleY);
        playerSprite.setOrigin(static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f);
    }
    playerSprite.setPosition(centerX, centerY);
    target.draw(playerSprite);
}

void GraphicalUtils::drawEgg(sf::RenderTarget& target, const Egg& egg, float tileSize, float offsetX, float offsetY)
{
    float startX = egg.x * tileSize + offsetX;
    float startY = egg.y * tileSize + offsetY;

    const TextureManager& tm = TextureManager::getInstance();
    sf::Sprite eggSprite(tm.getTexture("egg"));

    float eggScale = 0.3f;
    eggSprite.setScale(eggScale, eggScale);

    if (egg.isHatched) {
        eggSprite.setColor(sf::Color(255, 215, 0, 200));
    } else {
        eggSprite.setColor(sf::Color(255, 255, 255, 255));
    }

    float posX = startX + (tileSize / 2.0f) - (19.0f / 2.0f);
    float posY = startY + tileSize - 24.0f;

    eggSprite.setPosition(posX, posY);

    target.draw(eggSprite);
}
