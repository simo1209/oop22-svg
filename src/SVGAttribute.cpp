#include <ostream>

#include "SVGAttribute.hpp"

std::ostream &operator<<(std::ostream &os, SVGAttribute const &attribute)
{
    os << attribute.getName();

    if (attribute.hasValue())
    {
        os << '=' << '"' << attribute.getValue() << '"';
    }

    return os;
}
