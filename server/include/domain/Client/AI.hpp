/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** AI.hpp
*/

#ifndef AI_HPP_
#define AI_HPP_

#include "Client.hpp"
#include <map>
#include <queue>
#include <string>
#include <vector>

class AI : public Client {
    public:
        AI(std::unique_ptr<ISocket> clientSocket, int playerId);
        void update(std::shared_ptr<Client> self, INetworkProvider& network, GameEngine& engine, std::vector<int>& clientsToRemove) override;
        
        const std::string& getTeamName() const { return _teamName; }
        void setTeamName(const std::string& name) { _teamName = name; }
        
        int getX() const { return _x; }
        void setX(int x) { _x = x; }
        
        int getY() const { return _y; }
        void setY(int y) { _y = y; }
        
        int getLevel() const { return _level; }
        void setLevel(int level) { _level = level; }
        
        int getOrientation() const { return _orientation; }
        void setOrientation(int orientation) { _orientation = orientation; }
        
        std::map<std::string, int>& getInventory() { return _inventory; }
        const std::map<std::string, int>& getInventory() const { return _inventory; }
        
        std::queue<std::string>& getCommandQueue() { return _commandQueue; }
        const std::queue<std::string>& getCommandQueue() const { return _commandQueue; }
        
        int getActionTicksLeft() const { return _actionTicksLeft; }
        void setActionTicksLeft(int ticks) { _actionTicksLeft = ticks; }
        
        int getFoodTicks() const { return _foodTicks; }
        void setFoodTicks(int ticks) { _foodTicks = ticks; }

        bool isInIncantation() const { return _inIncantation; }
        void setInIncantation(bool v) { _inIncantation = v; }

    private:
        std::string _teamName;
        int _x = 0;
        int _y = 0;
        int _level;
        int _orientation;
        
        std::map<std::string, int> _inventory; 
        std::queue<std::string> _commandQueue;
        int _actionTicksLeft = -1;
        int _foodTicks;
        bool _inIncantation = false;
};

#endif /* !AI_HPP_ */
