/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TextureManager
*/

#ifndef TEXTUREMANAGER_HPP_ 
    #define TEXTUREMANAGER_HPP_

    #include <SFML/Graphics.hpp>
    #include <map>
    #include <string>

    class TextureManager {
    public:
        static TextureManager& getInstance();
        bool loadTexture(const std::string& name, const std::string& filename);
        const sf::Texture& getTexture(const std::string& name) const;

    private:
        TextureManager() = default;
        ~TextureManager() = default;
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        std::map<std::string, sf::Texture> _textures;

        const std::string _basePath = "GameEngine/assets/sprites/";
    };

#endif /* !TEXTUREMANAGER_HPP_ */