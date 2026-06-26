/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3DRenderer
*/

#ifndef GRAPHIC3DRENDERER_HPP_
    #define GRAPHIC3DRENDERER_HPP_

    #include "GameWorld.hpp"
    #include "Graphic3DInterface.hpp"
    #include "Graphic3DMap.hpp"
    #include "Graphic3DPlayers.hpp"
    #include <raylib.h>
    #include <optional>
    #include <string>
    #include <utility>
    #include <vector>

    class Graphic3DRenderer {
        public:
            void drawMenu(const Camera3D& camera, const std::string& status,
                const std::vector<std::string>& teams) const;
            void drawWorld(const Camera3D& camera, const GameWorld& world,
                const std::optional<std::pair<int, int>>& selectedTile,
                const std::optional<int>& selectedPlayerId) const;
            std::optional<int> pickPlayer(const GameWorld& world,
                const Camera3D& camera, Vector2 mousePosition) const;
            std::optional<std::pair<int, int>> pickTile(
                const GameWorld& world, const Camera3D& camera,
                Vector2 mousePosition) const;
            bool isInsideInfoPanel(
                const std::optional<std::pair<int, int>>& selectedTile,
                const std::optional<int>& selectedPlayerId, int mouseX,
                int mouseY) const;

        private:
            Graphic3DMap _map;
            Graphic3DPlayers _players;
            Graphic3DInterface _interface;
    };

#endif /* !GRAPHIC3DRENDERER_HPP_ */
