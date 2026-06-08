/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** GameCommand.hpp
*/

#ifndef GAMECOMMAND_HPP
#define GAMECOMMAND_HPP

#include "./ICommand.hpp"

class GameCommand {
    public:
        virtual ~GameCommand() {}
        virtual void Accept(ICommand *element);

        int get_x() const;
        int get_y() const;
        int get_q0() const;
        int get_q1() const;
        int get_q2() const;
        int get_q3() const;
        int get_q4() const;
        int get_q5() const;
        int get_q6() const;

        void set_x(int x);
        void set_y(int y);
        void set_q0(int q0);
        void set_q1(int q1);
        void set_q2(int q2);
        void set_q3(int q3);
        void set_q4(int q4);
        void set_q5(int q5);
        void set_q6(int q6);
    private:
        int _x; //width or horizontal position of the map
        int _y; //height or vertical position of the map

        int _q0; // resource 0 (food) quantity
        int _q1; // resource 1 (linemate) quantity
        int _q2; // resource 2 (deraumere) quantity
        int _q3; // resource 3 (sibur) quantity
        int _q4; // resource 4 (mendiane) quantity
        int _q5; // resource 5 (phiras) quantity
        int _q6; // resource 6 (thystame) quantity
};

#endif