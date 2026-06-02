/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PlayerCommand.hpp
*/

#ifndef PLAYERCOMMAND_HPP
#define PLAYERCOMMAND_HPP

#include "./ICommand.hpp"
#include <string>

class PlayerCommand {
    public:
        virtual ~PlayerCommand() {}
        virtual void Accept(ICommand *element) const;
    
        int get_nb_players() const;
        int get_orientation() const;
        int get_level() const;
        int get_egg() const;
        int get_time() const;
        std::string get_name_team() const;
        bool get_incantation_result() const;
        std::string get_message() const;
        int get_ressource_nb() const;

        void set_nb_players(int nb_players);
        void set_orientation(int orientation);
        void set_level(int level);
        void set_egg(int egg);
        void set_time(int time);
        void set_name_team(std::string name_team);
        void set_incantation_result(bool incantation_result);
        void set_message(std::string message);
        void set_ressource_nb(int ressource_nb);

    private:
        int _nb_players; // player number
        int _orientation; // orientation: 1(N), 2(E), 3(S), 4(W)
        int _level; // level
        int _egg; // ege number
        int _time; // time unit
        std::string _name_team; // name of the team
        bool _incantation_result; // incantation result
        std::string _message; // message
        int _ressource_nb; // number of the resource
        
};

#endif