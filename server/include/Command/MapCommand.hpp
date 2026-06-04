/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** MapCommand.hpp
*/

#ifndef MAPCOMMAND_HPP
#define MAPCOMMAND_HPP

#include "ICommand.hpp"
#include <vector>
#include <string>

class MapCommand {
public:
    virtual ~MapCommand() {}
    virtual void Accept(ICommand *element) const;

    std::vector<std::vector<int>> get_map() const;
    int get_map_width() const;
    int get_map_height() const;

    std::vector<std::vector<int>> get_content_of_tile(int x, int y) const;
    std::vector<std::vector<int>> get_content_all_map() const;

    std::string get_team_name() const;
    int get_connection_player() const;

    int get_player_position_x() const;
    int get_player_level() const;
    std::vector<int> get_player_inventory() const;

    void expulsion_player() const;
    std::string broadcast() const;

    void end_incantation() const;
    void egg_laying() const;
    
    void drop_ressources() const;
    void collect_ressources() const;

    void death_player() const;
    void egg_laid_by_player() const;
    
    void player_connection_egg() const;
    void death_egg() const;

    void get_time_unit() const;
    void modify_time_unit() const;

    void end_game() const;
    std::string message_server() const;

    void unknow_command() const;
    void command_parameters() const;

    private:
    std::vector<std::vector<int>> _map;
    int _map_width;
    int _map_height;

    std::vector<std::vector<int>> _content_of_tile;
    std::vector<std::vector<int>> _content_all_map;

    std::string _team_name;
    int _connection_player;

    int _player_position_x;
    int _player_level;
    std::vector<int> _player_inventory;
};

#endif