/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** SegEvent.hpp
*/

#include "IEventElement.hpp"
#include "IEventVisitor.hpp"
#include <string>

class SegEvent : public IEventElement {
public:
    SegEvent(const std::string& teamName);
    ~SegEvent() override = default;
    void Accept(IEventVisitor *visitor) const override;
    const std::string& getTeamName() const;
private:
    std::string _teamName;
};
