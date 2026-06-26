/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** IEventElement.hpp
*/

/**
 * @file IEventElement.hpp
 * @brief Element interface of the GUI event Visitor pattern.
 */

#ifndef IEVENTELEMENT_HPP_
#define IEVENTELEMENT_HPP_

#include "IEventVisitor.hpp"

/**
 * @interface IEventElement
 * @ingroup visitor_event
 * @brief Base for every GUI event DTO; enables (const) double dispatch.
 */
class IEventElement {
public:
    virtual ~IEventElement() = default;

    /**
     * @brief Dispatches this event to the matching @c visit() of @p visitor.
     * @param visitor Visitor whose overload for this event type is invoked.
     */
    virtual void Accept(IEventVisitor *visitor) const = 0;
};

#endif /* !IEVENTELEMENT_HPP_ */
