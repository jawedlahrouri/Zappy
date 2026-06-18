/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** PicEvent.hpp
*/

#ifndef PICEVENT_HPP_
#define PICEVENT_HPP_

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"
#include <vector>

class PicEvent : public IEventElement {
public:
    PicEvent(int x, int y, int level, const std::vector<int> &playerIds);
    ~PicEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;

    int getX() const;
    int getY() const;
    int getLevel() const;
    const std::vector<int>& getPlayerIds() const;

private:
    int _x;
    int _y;
    int _level;
    std::vector<int> _playerIds;
};

#endif /* !PICEVENT_HPP_ */
