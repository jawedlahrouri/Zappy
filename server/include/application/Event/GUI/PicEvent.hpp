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

/**
 * @class PicEvent
 * @ingroup visitor_event
 * @brief Event: an incantation started on a tile.
 * @par Protocol (server -> GUI): "pic X Y L #n #n ...\n" (pos, level, players).
 */
class PicEvent : public IEventElement {
public:
    /** @brief Builds the event from the tile, level and participating players. */
    PicEvent(int x, int y, int level, const std::vector<int> &playerIds);
    ~PicEvent() override = default;
    /** @brief Dispatches to @c IEventVisitor::visit(PicEvent*). */
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
