/**
 * @file IAiCommandElement.hpp
 * @brief Element interface of the AI command Visitor pattern.
 */

#ifndef IAICOMMANDELEMENT_HPP_
#define IAICOMMANDELEMENT_HPP_

class IAiVisitor;

/**
 * @interface IAiCommandElement
 * @ingroup visitor_ai
 * @brief Base for every AI command DTO; enables double dispatch.
 *
 * Concrete commands implement @ref Accept as @c visitor->visit(this), which
 * resolves to the @c visit() overload matching the element's concrete type.
 */
class IAiCommandElement {
public:
    virtual ~IAiCommandElement() = default;

    /**
     * @brief Dispatches this element to the matching @c visit() of @p visitor.
     * @param visitor Visitor whose overload for this command type is invoked.
     */
    virtual void Accept(IAiVisitor *visitor) = 0;

};

#endif /* !IAICOMMANDELEMENT_HPP_ */
