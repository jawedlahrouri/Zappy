#ifndef IGUICOMMANDELEMENT_HPP_
#define IGUICOMMANDELEMENT_HPP_

class IGuiVisitor;

class IGuiCommandElement {
public:
    virtual ~IGuiCommandElement() = default;
    virtual void Accept(IGuiVisitor *visitor) = 0;
};

#endif /* !IGUICOMMANDELEMENT_HPP_ */
