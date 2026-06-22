#ifndef IAICOMMANDELEMENT_HPP_
#define IAICOMMANDELEMENT_HPP_

class IAiVisitor;

class IAiCommandElement {
public:
    virtual ~IAiCommandElement() = default;
    virtual void Accept(IAiVisitor *visitor) = 0;
    
};

#endif /* !IAICOMMANDELEMENT_HPP_ */
