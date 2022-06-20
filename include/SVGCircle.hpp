#include "SVGShape.hpp"

#ifndef __SVGCIRCLE
#define __SVGCIRCLE

class SVGCircle : public SVGShape
{
private:
    long x, y;
    unsigned radius;
    std::string fill;

public:

    SVGCircle(long _x, long _y, unsigned _radius, std::string _fill, unsigned _id) : x(_x), y(_y), radius(_radius), fill(_fill), SVGShape(_id) {}
    SVGCircle(long _x, long _y, unsigned _radius, std::string _fill, unsigned _id, SVGElement *el) : x(_x), y(_y), radius(_radius), fill(_fill), SVGShape(_id, el) {}

    SVGCircle(std::string xString, std::string yString, std::string radiusString, std::string _fill, unsigned _id);

    SVGElement *generateElement(unsigned startPos);

    ~SVGCircle() {}
};

#endif
