#include "SVGShape.hpp"

#ifndef __SVGRECT
#define __SVGRECT

class SVGRect : public SVGShape
{
private:
    long x, y;
    unsigned width, height;
    std::string color;

public:
    SVGRect(long _x, long _y, unsigned _width, unsigned _height, std::string _color, unsigned _id, SVGElement *el) : x(_x), y(_y), width(_width), height(_height), color(_color), SVGShape(_id, el) {}
    ~SVGRect() {}
};

#endif
