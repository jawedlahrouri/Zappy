/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowManager
*/

#ifndef WINDOWMANAGER_HPP_
#define WINDOWMANAGER_HPP_

#include "GameWorld.hpp"
#include "IGraphic.hpp"
#include "WindowContext.hpp"

class WindowManager : public IGraphic {
    public:
        WindowManager(unsigned int width = 1920, unsigned int height = 1080,
            const std::string& title = "Zappy GUI");
        ~WindowManager() override;
        
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
        void setMenuConnectionInfo(const std::string& ip, const std::string& port) override;
        void setMenuStatus(const std::string& status) override;
        void resetMenuTeamNames() override;
        void addMenuTeamName(const std::string& teamName) override;
        bool consumeMenuConnectionEdited() override;
        void setServerFreq(int freq) override;
        int getServerFreq() const override;
        bool isTimePaused() const override;

    private:
        WindowContext _context;
};

#endif /* !WINDOWMANAGER_HPP_ */
