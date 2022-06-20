#include "SVGElement.hpp"

#ifndef __SVGSHAPE
#define __SVGSHAPE

class SVGShape
{
private:
    unsigned id;
    SVGElement *element;

public:
    SVGShape(unsigned _id, SVGElement *el) : id(_id), element(el) {}
    virtual ~SVGShape() {}

    unsigned getId() const { return id; }
    SVGElement *getElement() const { return element; }
};

#endif
