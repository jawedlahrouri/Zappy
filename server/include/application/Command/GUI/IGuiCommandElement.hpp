/**
 * @file IGuiCommandElement.hpp
 * @brief Element interface of the GUI command Visitor pattern.
 */

#ifndef IGUICOMMANDELEMENT_HPP_
#define IGUICOMMANDELEMENT_HPP_

class IGuiVisitor;

/**
 * @interface IGuiCommandElement
 * @ingroup visitor_gui
 * @brief Base for every GUI command DTO; enables double dispatch.
 */
class IGuiCommandElement {
public:
    virtual ~IGuiCommandElement() = default;

    /**
     * @brief Dispatches this element to the matching @c visit() of @p visitor.
     * @param visitor Visitor whose overload for this command type is invoked.
     */
    virtual void Accept(IGuiVisitor *visitor) = 0;
};

#endif /* !IGUICOMMANDELEMENT_HPP_ */
