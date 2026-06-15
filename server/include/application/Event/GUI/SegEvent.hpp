/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SegEvent.hpp
*/

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"

class SegEvent : public IEventElement {
public:
    SegEvent(int teamId);
    ~SegEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;
    int getTeamId() const;
private:
    int _teamId;
};
