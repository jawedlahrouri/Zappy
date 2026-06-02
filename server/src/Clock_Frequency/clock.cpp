/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** clock.cpp
*/


#include "Clock_frequency/clock.hpp"

ClockFrequency::ClockFrequency(int frequency)
    : _clockFrequency(frequency) {}

ClockFrequency::~ClockFrequency() {}

void ClockFrequency::setClockFrequency(int frequency) {
    _clockFrequency = frequency;
}

int ClockFrequency::getClockFrequency() {
    return _clockFrequency;
}
