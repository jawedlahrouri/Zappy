/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** Graphic3D
*/

/**
 * @file Graphic3D.hpp
 * @brief Declares the Raylib-based graphical backend.
 */

#ifndef GRAPHIC3D_HPP_
    #define GRAPHIC3D_HPP_

    #include "Graphic3DCamera.hpp"
    #include "Graphic3DRenderer.hpp"
    #include "IGraphic.hpp"
    #include <optional>
    #include <string>
    #include <utility>
    #include <vector>

    /**
     * @brief Experimental Raylib implementation of the IGraphic interface.
     */
    class Graphic3D : public IGraphic {
        public:
            /**
             * @brief Creates the Raylib window and initializes renderer state.
             */
            Graphic3D();

            /**
             * @brief Closes the Raylib window when it is still open.
             */
            ~Graphic3D() override;

            /**
             * @copydoc IGraphic::isOpen()
             */
            bool isOpen() const override;

            /**
             * @copydoc IGraphic::handleMenuEvents()
             */
            bool handleMenuEvents() override;

            /**
             * @copydoc IGraphic::handleGameEvents()
             */
            bool handleGameEvents(const GameWorld& world) override;

            /**
             * @copydoc IGraphic::consumeTileRequest()
             */
            std::optional<std::pair<int, int>> consumeTileRequest() override;

            /**
             * @copydoc IGraphic::consumeTimeRequest()
             */
            std::optional<int> consumeTimeRequest() override;

            /**
             * @copydoc IGraphic::clear()
             */
            void clear() override;

            /**
             * @copydoc IGraphic::display()
             */
            void display() override;

            /**
             * @copydoc IGraphic::drawMenu()
             */
            void drawMenu() override;

            /**
             * @copydoc IGraphic::drawWorld()
             */
            void drawWorld(const GameWorld& world) override;

            /**
             * @copydoc IGraphic::getMenuIp()
             */
            const std::string& getMenuIp() const override;

            /**
             * @copydoc IGraphic::getMenuPort()
             */
            const std::string& getMenuPort() const override;

            /**
             * @copydoc IGraphic::setMenuConnectionInfo()
             */
            void setMenuConnectionInfo(const std::string& ip,
                const std::string& port) override;

            /**
             * @copydoc IGraphic::setMenuStatus()
             */
            void setMenuStatus(const std::string& status) override;

            /**
             * @copydoc IGraphic::resetMenuTeamNames()
             */
            void resetMenuTeamNames() override;

            /**
             * @copydoc IGraphic::addMenuTeamName()
             */
            void addMenuTeamName(const std::string& teamName) override;

            /**
             * @copydoc IGraphic::consumeMenuConnectionEdited()
             */
            bool consumeMenuConnectionEdited() override;

            /**
             * @copydoc IGraphic::setServerFreq()
             */
            void setServerFreq(int freq) override;

            /**
             * @copydoc IGraphic::getServerFreq()
             */
            int getServerFreq() const override;

            /**
             * @copydoc IGraphic::isTimePaused()
             */
            bool isTimePaused() const override;

        private:
            /**
             * @brief Creates and configures the Raylib window.
             */
            void createWindow();

            /**
             * @brief Updates selected tile or player from a mouse click.
             */
            void handleSelection(const GameWorld& world);

            Graphic3DCamera _camera;
            Graphic3DRenderer _renderer;
            bool _windowOpen;
            std::string _menuIp;
            std::string _menuPort;
            std::string _menuStatus;
            std::vector<std::string> _teamNames;
            bool _menuConnectionEdited;
            int _serverFreq;
            bool _timePaused;
            std::optional<std::pair<int, int>> _selectedTile;
            std::optional<int> _selectedPlayerId;
            std::optional<std::pair<int, int>> _pendingTileRequest;
    };

#endif /* !GRAPHIC3D_HPP_ */
