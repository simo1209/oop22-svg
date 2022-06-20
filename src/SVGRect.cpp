#include "SVGRect.hpp"

SVGRect::SVGRect(std::string xString, std::string yString, std::string widthString, std::string heightString, std::string _fill, unsigned _id) : fill(_fill), SVGShape(_id)
{
    x = std::stol(xString);
    y = std::stol(yString);
    width = std::stoul(widthString);
    height = std::stoul(heightString);
}

SVGElement *SVGRect::generateElement(unsigned startPos)
{
    std::string tag = "rect";

    std::string attributesString = " ";
    attributesString += "x=\"" + std::to_string(x) + "\" ";
    attributesString += "y=\"" + std::to_string(y) + "\" ";
    attributesString += "width=\"" + std::to_string(width) + "\" ";
    attributesString += "height=\"" + std::to_string(height) + "\" ";
    attributesString += "fill=\"" + fill + "\" ";

    unsigned endPos = startPos + 3 + tag.length() + attributesString.length();

    SVGElement *element = new SVGElement(tag, attributesString, startPos, endPos, true);
    setElement(element);
    return element;
}
