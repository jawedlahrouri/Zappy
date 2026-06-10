/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AI.hpp
*/

#ifndef AI_HPP_
#define AI_HPP_

#include "Client/Client.hpp"
#include <map>
#include <queue>
#include <string>
#include <vector>

class AI : public Client {
    public:
        AI(std::unique_ptr<ISocket> clientSocket, int playerId);
        void update(std::shared_ptr<Client> self, INetworkProvider& network, GameEngine& engine, std::vector<int>& clientsToRemove) override;
        
        const std::string& getTeamName() const { return m_teamName; }
        void setTeamName(const std::string& name) { m_teamName = name; }
        
        int getX() const { return m_x; }
        void setX(int x) { m_x = x; }
        
        int getY() const { return m_y; }
        void setY(int y) { m_y = y; }
        
        int getLevel() const { return m_level; }
        void setLevel(int level) { m_level = level; }
        
        int getOrientation() const { return m_orientation; }
        void setOrientation(int orientation) { m_orientation = orientation; }
        
        std::map<std::string, int>& getInventory() { return m_inventory; }
        const std::map<std::string, int>& getInventory() const { return m_inventory; }
        
        std::queue<std::string>& getCommandQueue() { return m_commandQueue; }
        const std::queue<std::string>& getCommandQueue() const { return m_commandQueue; }
        
        int getActionTicksLeft() const { return m_actionTicksLeft; }
        void setActionTicksLeft(int ticks) { m_actionTicksLeft = ticks; }
        
        int getFoodTicks() const { return m_foodTicks; }
        void setFoodTicks(int ticks) { m_foodTicks = ticks; }

    private:
        std::string m_teamName;
        int m_x = 0;
        int m_y = 0;
        int m_level;
        int m_orientation;
        
        std::map<std::string, int> m_inventory; 
        std::queue<std::string> m_commandQueue;
        int m_actionTicksLeft = -1;
        int m_foodTicks;
};

#endif /* !AI_HPP_ */
