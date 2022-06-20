#include "SVGCircle.hpp"

SVGCircle::SVGCircle(std::string xString, std::string yString, std::string radiusString, std::string _fill, unsigned _id) : fill(_fill), SVGShape(_id)
{
    x = std::stol(xString);
    y = std::stol(yString);
    radius = std::stoul(radiusString);
}

SVGElement *SVGCircle::generateElement(unsigned startPos)
{
    std::string tag = "circle";

    std::string attributesString = " ";
    attributesString += "cx=\"" + std::to_string(x) + "\" ";
    attributesString += "cy=\"" + std::to_string(y) + "\" ";
    attributesString += "r=\"" + std::to_string(radius) + "\" ";
    attributesString += "fill=\"" + fill + "\" ";

    unsigned endPos = startPos + 3 + tag.length() + attributesString.length();

    SVGElement *element = new SVGElement(tag, attributesString, startPos, endPos, true);
    setElement(element);
    return element;
}