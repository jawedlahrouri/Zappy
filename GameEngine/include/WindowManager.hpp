/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

/**
 * @file WindowManager.hpp
 * @brief Declares the SFML implementation of the graphical interface.
 */

#ifndef WINDOWMANAGER_HPP_
#define WINDOWMANAGER_HPP_

#include "GameWorld.hpp"
#include "IGraphic.hpp"
#include "WindowAssets.hpp"
#include "WindowContext.hpp"
#include "WindowEvents.hpp"
#include "WindowMenu.hpp"
#include "WindowRender.hpp"
#include "WindowSelection.hpp"
#include "WindowTime.hpp"

/**
 * @brief SFML frontend that implements the IGraphic plugin contract.
 *
 * WindowManager delegates most responsibilities to small Window* helper
 * classes while owning the shared WindowContext.
 */
class WindowManager : public IGraphic {
    public:
        /**
         * @brief Creates the SFML window and loads graphical assets.
         * @param width Initial window width in pixels.
         * @param height Initial window height in pixels.
         * @param title Window title.
         */
        WindowManager(unsigned int width = 1920, unsigned int height = 1080,
            const std::string& title = "Zappy GUI");

        /**
         * @brief Stops runtime resources owned by the window context.
         */
        ~WindowManager() override;
        
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
        void setMenuConnectionInfo(const std::string& ip, const std::string& port) override;

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
        WindowContext _context;
};

#endif /* !WINDOWMANAGER_HPP_ */
