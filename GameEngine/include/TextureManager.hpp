/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** TextureManager
*/

/**
 * @file TextureManager.hpp
 * @brief Declares the shared SFML texture cache.
 */

#ifndef TEXTUREMANAGER_HPP_ 
    #define TEXTUREMANAGER_HPP_

    #include <SFML/Graphics.hpp>
    #include <map>
    #include <string>

    /**
     * @brief Singleton texture cache backed by GameEngine/assets/sprites.
     */
    class TextureManager {
    public:
        /**
         * @brief Gets the process-wide texture manager.
         * @return Mutable singleton instance.
         */
        static TextureManager& getInstance();

        /**
         * @brief Loads and stores a texture under a logical name.
         * @param name Cache key used by renderers.
         * @param filename File path relative to the sprite asset directory.
         * @return true when loading succeeded.
         */
        bool loadTexture(const std::string& name, const std::string& filename);

        /**
         * @brief Retrieves a previously loaded texture.
         * @param name Cache key.
         * @return Stored texture, or a fallback texture when available.
         */
        const sf::Texture& getTexture(const std::string& name) const;

        /**
         * @brief Releases every texture currently kept in the cache.
         */
        void clear();

    private:
        TextureManager() = default;
        ~TextureManager() = default;
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        std::map<std::string, sf::Texture> _textures;

        const std::string _basePath = "GameEngine/assets/sprites/";
    };

#endif /* !TEXTUREMANAGER_HPP_ */
