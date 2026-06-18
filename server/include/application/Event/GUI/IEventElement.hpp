/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IEventElement.hpp
*/

#ifndef IEVENTELEMENT_HPP_
#define IEVENTELEMENT_HPP_

#include "IEventVisitor.hpp"

class IEventElement {
public:
    virtual ~IEventElement() = default;
    virtual void Accept(IEventVisitor *visitor) const = 0;
};

#endif /* !IEVENTELEMENT_HPP_ */
