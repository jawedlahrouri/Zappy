/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameCommand.cpp
*/

#include "Command/GameCommand.hpp"

int GameCommand::get_x() const {
    return this->_x;
}
int GameCommand::get_y() const {
    return this->_y;
}
int GameCommand::get_q0() const {
    return _q0;
}
int GameCommand::get_q1() const {
    return _q1;
}
int GameCommand::get_q2() const {
    return _q2;
}
int GameCommand::get_q3() const {
    return _q3;
}
int GameCommand::get_q4() const {
    return _q4;
}
int GameCommand::get_q5() const {
    return _q5;
}
int GameCommand::get_q6() const {
    return _q6;
}

void GameCommand::set_x(int x) {
    this->_x = x;
}
void GameCommand::set_y(int y) {
    this->_y = y;
}
void GameCommand::set_q0(int q0) {
    this->_q0 = q0;
}
void GameCommand::set_q1(int q1) {
    this->_q1 = q1;
}
void GameCommand::set_q2(int q2) {
    this->_q2 = q2;
}
void GameCommand::set_q3(int q3) {
    this->_q3 = q3;
}
void GameCommand::set_q4(int q4) {
    this->_q4 = q4;
}
void GameCommand::set_q5(int q5) {
    this->_q5 = q5;
}
void GameCommand::set_q6(int q6) {
    this->_q6 = q6;
}

void GameCommand::Accept(ICommand *element) const {
    element->visit(this);
}