/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** CommandMap.cpp
*/

#include "Command/MapCommand.hpp"

void MapCommand::expulsion_player() const {
    
}

std::string MapCommand::broadcast() const {
    return "";
}

void MapCommand::end_incantation() const {
    
}

void MapCommand::egg_laying() const {
    
}

void MapCommand::drop_ressources() const {
    
}

void MapCommand::collect_ressources() const {
    
}

void MapCommand::death_player() const {
    
}

void MapCommand::egg_laid_by_player() const {
    
}

void MapCommand::player_connection_egg() const {
    
}

void MapCommand::death_egg() const {
    
}

void MapCommand::get_time_unit() const {
    
}

void MapCommand::modify_time_unit() const {
    
}

void MapCommand::end_game() const {
    
}

std::string MapCommand::message_server() const {
    return "";
}

void MapCommand::unknow_command() const {
    
}

void MapCommand::command_parameters() const {
    
}

std::vector<std::vector<int>> MapCommand::get_map() const {
    return _map;
}

int MapCommand::get_map_width() const {
    return _map_width;
}

int MapCommand::get_map_height() const {
    return _map_height;
}

std::vector<std::vector<int>> MapCommand::get_content_of_tile(int x, int y) const {
    (void)x;
    (void)y;
    return _content_of_tile;
}

std::vector<std::vector<int>> MapCommand::get_content_all_map() const {
    return _content_all_map;
}

std::string MapCommand::get_team_name() const {
    return _team_name;
}

int MapCommand::get_connection_player() const {
    return _connection_player;
}

int MapCommand::get_player_position_x() const {
    return _player_position_x;
}

int MapCommand::get_player_level() const {
    return _player_level;
}

std::vector<int> MapCommand::get_player_inventory() const {
    return _player_inventory;
}

void MapCommand::Accept(ICommand *element) {
    element->visit(this);
}

void MapCommand::set_map_width(int width) {
    _map_width = width;
}

void MapCommand::set_map_height(int height) {
    _map_height = height;
}

void MapCommand::set_team_name(const std::string &team_name) {
    _team_name = team_name;
}

void MapCommand::set_connection_player(int connection_player) {
    _connection_player = connection_player;
}
