/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TextureManager
*/

#include "TextureManager.hpp"
#include <iostream>

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

bool TextureManager::loadTexture(const std::string& name, const std::string& filename) {
    if (_textures.find(name) != _textures.end()) {
        return true;
    }

    std::string fullPath = _basePath + filename;

    sf::Texture texture;
    if (!texture.loadFromFile(fullPath)) {
        std::cerr << "[TextureManager] Erreur : impossible de trouver " << fullPath << std::endl;
        return false;
    }

    _textures[name] = texture;
    return true;
}

const sf::Texture& TextureManager::getTexture(const std::string& name) const {
    auto it = _textures.find(name);
    if (it == _textures.end()) {
        std::cerr << "[TextureManager] Error: Texture '" << name << "' introuvable !" << std::endl;
        static sf::Texture dummy;
        return dummy;
    }
    return it->second;
}