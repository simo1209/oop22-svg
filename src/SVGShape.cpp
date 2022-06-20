#include <ostream>

#include "SVGShape.hpp"

void SVGShape::print(std::ostream &os)
{
    os << "[" << id << "]";
    element->print(os);
}
