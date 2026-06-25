/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3D
*/

#ifndef GRAPHIC3D_HPP_
    #define GRAPHIC3D_HPP_

    #include "Graphic3DCamera.hpp"
    #include "Graphic3DRenderer.hpp"
    #include "IGraphic.hpp"
    #include <optional>
    #include <string>
    #include <utility>

    class Graphic3D : public IGraphic {
        public:
            Graphic3D();
            ~Graphic3D() override;

            bool isOpen() const override;
            bool handleMenuEvents() override;
            bool handleGameEvents(const GameWorld& world) override;
            std::optional<std::pair<int, int>> consumeTileRequest() override;
            std::optional<int> consumeTimeRequest() override;
            void clear() override;
            void display() override;
            void drawMenu() override;
            void drawWorld(const GameWorld& world) override;
            const std::string& getMenuIp() const override;
            const std::string& getMenuPort() const override;
            void setMenuConnectionInfo(const std::string& ip,
                const std::string& port) override;
            void setMenuStatus(const std::string& status) override;
            void resetMenuTeamNames() override;
            void addMenuTeamName(const std::string& teamName) override;
            bool consumeMenuConnectionEdited() override;
            void setServerFreq(int freq) override;
            int getServerFreq() const override;
            bool isTimePaused() const override;

        private:
            void createWindow();

            Graphic3DCamera _camera;
            Graphic3DRenderer _renderer;
            bool _windowOpen;
            std::string _menuIp;
            std::string _menuPort;
            std::string _menuStatus;
            bool _menuConnectionEdited;
            int _serverFreq;
            bool _timePaused;
    };

#endif /* !GRAPHIC3D_HPP_ */
