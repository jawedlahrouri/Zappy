/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DRenderer
*/

#ifndef GRAPHIC3DRENDERER_HPP_
    #define GRAPHIC3DRENDERER_HPP_

    #include "GameWorld.hpp"
    #include <raylib.h>
    #include <string>

    class Graphic3DRenderer {
        public:
            void drawMenu(const Camera3D& camera, const std::string& status) const;
            void drawWorld(const Camera3D& camera, const GameWorld& world) const;

        private:
            void drawPreviewWorld() const;
            void drawMap(const GameWorld& world) const;
            void drawTiles(const GameWorld& world) const;
            void drawResources(const Tile& tile, int width, int height) const;
            void drawPlayers(const GameWorld& world) const;
            void drawEggs(const GameWorld& world) const;
            void drawHud(const GameWorld& world) const;
            Vector3 getTileCenter(int x, int y, int width, int height) const;
            Color getTileColor(const Tile& tile) const;
            Color getPlayerColor(int teamColor) const;
            int getResourceTotal(const Resources& resources) const;
    };

#endif /* !GRAPHIC3DRENDERER_HPP_ */
