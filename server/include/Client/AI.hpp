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
        AI(Socket clientSocket, int player_id);
        void update(std::shared_ptr<Client> self, Server& server, GameEngine& engine, std::vector<int>& clients_to_remove) override;
        int id;
        std::string teamName;
        int x = 0;
        int y = 0;
        int level;
        int orientation;
        
        std::map<std::string, int> inventory; 
        std::queue<std::string> commandQueue;
        int action_ticks_left = -1;
        int food_ticks;
};

#endif /* !AI_HPP_ */
