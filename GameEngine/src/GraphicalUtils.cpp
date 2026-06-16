/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GraphicalUtils
*/

#include "GraphicalUtils.hpp"
#include "TextureManager.hpp"
#include <cmath>

static std::string getPlayerTextureKey(const Player& player)
{
    if (player.teamColor == 1) {
        if (player.orientation == 1)
            return "blueBack";
        if (player.orientation == 2)
            return "blueRight";
        if (player.orientation == 4)
            return "blueLeft";
        return "bluePlayer";
    }
    if (player.teamColor == 3) {
        if (player.orientation == 1)
            return "greenBack";
        if (player.orientation == 2)
            return "greenRight";
        if (player.orientation == 4)
            return "greenLeft";
        return "greenPlayer";
    }
    if (player.teamColor == 4) {
        if (player.orientation == 1)
            return "pinkBack";
        if (player.orientation == 2)
            return "pinkRight";
        if (player.orientation == 4)
            return "pinkLeft";
        return "pinkPlayer";
    }
    if (player.orientation == 1)
        return "playerBack";
    if (player.orientation == 2)
        return "playerRight";
    if (player.orientation == 4)
        return "playerLeft";
    return "playerFace";
}

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

void GraphicalUtils::drawPlayer(sf::RenderTarget& target, const Player& player, float tileSize, float offsetX, float offsetY)
{
    if (player.level < 1)
        return;
    float centerX = player.x * tileSize + offsetX + (tileSize / 2.0f);
    float centerY = player.y * tileSize + offsetY + (tileSize / 2.0f);

    const TextureManager& tm = TextureManager::getInstance();
    std::string texKey = getPlayerTextureKey(player);
    
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
