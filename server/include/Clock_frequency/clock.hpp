/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** clock.hpp
*/

#ifndef CLOCK_FREQUENCY_CLOCK_HPP
#define CLOCK_FREQUENCY_CLOCK_HPP

class ClockFrequency {
public:
    ClockFrequency(int frequency);
    ~ClockFrequency();

    void setClockFrequency(int frequency); 
    int getClockFrequency();
    void tick();

private:
    int _clockFrequency;
};

#endif