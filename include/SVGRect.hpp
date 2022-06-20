#include "SVGShape.hpp"

#ifndef __SVGRECT
#define __SVGRECT

class SVGRect : public SVGShape
{
private:
    long x, y;
    unsigned width, height;
    std::string fill;

public:
    SVGRect(long _x, long _y, unsigned _width, unsigned _height, std::string _fill, unsigned _id) : x(_x), y(_y), width(_width), height(_height), fill(_fill), SVGShape(_id) {}
    SVGRect(long _x, long _y, unsigned _width, unsigned _height, std::string _fill, unsigned _id, SVGElement *el) : x(_x), y(_y), width(_width), height(_height), fill(_fill), SVGShape(_id, el) {}

    SVGRect(std::string xString, std::string yString, std::string widthString, std::string heightString, std::string _fill, unsigned _id);

    SVGElement *generateElement(unsigned startPos);

    ~SVGRect() {}
};

#endif
