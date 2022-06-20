#include "SVGElement.hpp"

#ifndef __SVGSHAPE
#define __SVGSHAPE

class SVGShape
{
private:
    unsigned id;
    SVGElement *element;

public:
    SVGShape(unsigned _id) : id(_id) {}
    SVGShape(unsigned _id, SVGElement *el) : id(_id), element(el) {}
    virtual ~SVGShape() {}

    unsigned getId() const { return id; }
    SVGElement *getElement() const { return element; }

    void setElement(SVGElement *el) { element = el; }
    void print(std::ostream &os);

    virtual SVGElement *generateElement(unsigned startPos) = 0;
};

#endif
