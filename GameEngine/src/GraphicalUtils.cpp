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
    // sf::Sprite siburSprite(tm.getTexture("sibur"));
    // sf::Sprite mendianeSprite(tm.getTexture("mendiane"));
    // sf::Sprite phirasSprite(tm.getTexture("phiras"));
    // sf::Sprite thystameSprite(tm.getTexture("thystame"));

    foodSprite.setScale(resScale, resScale);
    linemateSprite.setScale(resScale, resScale);
    deraumereSprite.setScale(resScale, resScale);
    // siburSprite.setScale(resScale, resScale);
    // mendianeSprite.setScale(resScale, resScale);
    // phirasSprite.setScale(resScale, resScale);
    // thystameSprite.setScale(resScale, resScale);

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

    // // q3 : Sibur (Au centre exact de la case)
    // if (tile.resources.sibur > 0) {
    //     siburSprite.setPosition(startX + 24.0f, startY + 24.0f);
    //     target.draw(siburSprite);
    // }

    // // q4 : Mendiane (Au milieu à droite)
    // if (tile.resources.mendiane > 0) {
    //     mendianeSprite.setPosition(startX + 44.0f, startY + 24.0f);
    //     target.draw(mendianeSprite);
    // }

    // // q5 : Phiras (En bas à gauche)
    // if (tile.resources.phiras > 0) {
    //     phirasSprite.setPosition(startX + 4.0f, startY + 44.0f);
    //     target.draw(phirasSprite);
    // }

    // // q6 : Thystame (En bas à droite)
    // if (tile.resources.thystame > 0) {
    //     thystameSprite.setPosition(startX + 44.0f, startY + 44.0f);
    //     target.draw(thystameSprite);
    // }
}

void GraphicalUtils::drawPlayer(sf::RenderTarget& target, const Player& player, float tileSize, float offsetX, float offsetY)
{
    // Un cercle à 3 côtés est un triangle en SFML
    sf::CircleShape triangle(12.0f, 3); 
    triangle.setFillColor(sf::Color::Cyan);
    
    // On définit le centre du triangle pour que la rotation se fasse proprement
    triangle.setOrigin(12.0f, 12.0f);

    // Position calculée au centre de la case du joueur
    float centerX = player.x * tileSize + offsetX + (tileSize / 2.0f);
    float centerY = player.y * tileSize + offsetY + (tileSize / 2.0f);
    triangle.setPosition(centerX, centerY);

    // Orientation : 1: N, 2: E, 3: S, 4: W
    // Par défaut, le triangle SFML pointe vers le haut (Nord = 0°).
    if (player.orientation == 1) triangle.setRotation(0.0f);
    else if (player.orientation == 2) triangle.setRotation(90.0f);
    else if (player.orientation == 3) triangle.setRotation(180.0f);
    else if (player.orientation == 4) triangle.setRotation(270.0f);

    target.draw(triangle);
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