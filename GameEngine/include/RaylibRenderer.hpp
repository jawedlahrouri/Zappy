/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** RaylibRenderer
*/

#ifndef RAYLIBRENDERER_HPP_
    #define RAYLIBRENDERER_HPP_

    #include "GameWorld.hpp"
    #include <memory>
    #include <optional>
    #include <string>
    #include <utility>

    class RaylibRenderer {
    public:
        RaylibRenderer();
        ~RaylibRenderer();

        RaylibRenderer(const RaylibRenderer&) = delete;
        RaylibRenderer& operator=(const RaylibRenderer&) = delete;

        bool isAvailable() const;
        void open(unsigned int width, unsigned int height, const std::string& title);
        void close();

        bool isOpen() const;
        bool shouldToggleTo2D() const;
        void handleInput(const GameWorld& world);
        std::optional<std::pair<int, int>> consumeTileRequest();

        void beginFrame();
        void drawWorld(const GameWorld& world);
        void endFrame();

    private:
        struct Impl;

        std::unique_ptr<Impl> _impl;
    };

#endif /* !RAYLIBRENDERER_HPP_ */
