/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PlayerCommand.cpp
*/

#include "Command/PlayerCommand.hpp"

int PlayerCommand::get_nb_players() const {
    return _nb_players;
}
int PlayerCommand::get_orientation() const {
    return _orientation;
}
int PlayerCommand::get_level() const {
    return _level;
}
int PlayerCommand::get_egg() const {
    return _egg;
}
int PlayerCommand::get_time() const {
    return _time;
}
std::string PlayerCommand::get_name_team() const {
    return _name_team;
}
bool PlayerCommand::get_incantation_result() const {
    return _incantation_result;
}
std::string PlayerCommand::get_message() const {
    return _message;
}
int PlayerCommand::get_ressource_nb() const {
    return _ressource_nb;
}

void PlayerCommand::set_nb_players(int nb_players) {
    this->_nb_players = nb_players;
}
void PlayerCommand::set_orientation(int orientation) {
    this->_orientation = orientation;
}
void PlayerCommand::set_level(int level) {
    this->_level = level;
}
void PlayerCommand::set_egg(int egg) {
    this->_egg = egg;
}
void PlayerCommand::set_time(int time) {
    this->_time = time;
}
void PlayerCommand::set_name_team(std::string name_team) {
    this->_name_team = name_team;
}
void PlayerCommand::set_incantation_result(bool incantation_result) {
    this->_incantation_result = incantation_result;
}
void PlayerCommand::set_message(std::string message) {
    this->_message = message;
}
void PlayerCommand::set_ressource_nb(int ressource_nb) {
    this->_ressource_nb = ressource_nb;
}

void PlayerCommand::Accept(ICommand *element) const {
    element->visit(this);
}