#include <regex>
#include <ostream>
#include <iostream>

#include "SVGElement.hpp"
#include "SVGProgram.hpp"

void SVGElement::parseAttributes(std::string attributesString)
{
    const std::regex attributesRegex("(\\w+)(=\"([\\w]*)\")?");
    auto words_end = std::sregex_iterator();
    auto matches = std::sregex_iterator(attributesString.begin(), attributesString.end(), attributesRegex);

    unsigned lastEndPosition = 0;

    for (std::sregex_iterator regexIter = matches; regexIter != words_end; ++regexIter)
    {
        std::smatch match = *regexIter;
        SVGAttribute *attribute = new SVGAttribute(match[1], match[3], match.position(), match.position() + match.length(), match[2].length() > 0);
        attributes.push_back(attribute);
        lastEndPosition = match.position() + match.length();
    }

    attributesTailingWhitespaces = attributesString.length() - lastEndPosition;
}

SVGElement::SVGElement(SVGElement const &other)
{
    setTag(other.getTag());
    setSelfClosing(other.isSelfClosing());
    copyAttributes(other.getAttributes());
}

SVGElement &SVGElement::operator=(SVGElement const &other)
{
    if (this != &other)
    {
        setTag(other.getTag());
        setSelfClosing(other.isSelfClosing());
        copyAttributes(other.getAttributes());
    }
    return *this;
}

void SVGElement::printAttributes(std::ostream &os)
{
    unsigned lastEndPosition = 0;
    for (auto &attribute : attributes)
    {
        for (unsigned i = lastEndPosition; i < attribute->getStartPosition(); ++i)
        {
            os << ' ';
        }
        os << *attribute;

        lastEndPosition = attribute->getEndPosition();
    }

    for (unsigned i = 0; i < attributesTailingWhitespaces; ++i)
    {
        os << ' ';
    }
}

void SVGElement::print(std::ostream &os)
{
    os << '<' << tag;

    printAttributes(os);

    if (selfClosing)
    {
        os << '/';
    }

    os << '>';

    for (auto &child : getChildren())
    {
        child->print(os);
    }

    if (selfClosing == false)
    {
        os << "</" << tag << '>';
    }
}

SVGElement::~SVGElement()
{
    for (auto &attribute : attributes)
    {
        delete attribute;
    }
}
