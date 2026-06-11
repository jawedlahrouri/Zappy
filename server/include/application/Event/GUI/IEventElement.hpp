/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IEventElement.hpp
*/

#ifndef IEVENTELEMENT_HPP_
#define IEVENTELEMENT_HPP_

class IEventVisitor;

class IEventElement {
public:
    virtual ~IEventElement() = default;
    virtual void Accept(IEventVisitor *visitor) const = 0;
};

#endif /* !IEVENTELEMENT_HPP_ */
