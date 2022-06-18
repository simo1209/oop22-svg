#include <vector>

#include "ASTNode.hpp"
#include "SVGAttribute.hpp"

#ifndef __SVGELEMENT
#define __SVGELEMENT

class SVGElement : public ASTNode
{
private:

    bool selfClosing;
    std::string tag;
    std::vector<SVGAttribute> attributes;
    unsigned attributesTailingWhitespaces;
    // std::map<std::string, std::string> attributes;

    void parseAttributes(std::string attributesString);

public:
    SVGElement(std::string _tag, std::string _attributesString, std::string _text, unsigned _startPos, unsigned _endPos, bool _selfClosing = true) : tag(_tag), ASTNode(_text, _startPos, _endPos), selfClosing(_selfClosing)
    {
        parseAttributes(_attributesString);
    }

    bool isSelfClosing() const { return selfClosing; }
    std::string getTag() const { return tag; }
    const std::vector<SVGAttribute> &getAttributes() const { return attributes; }

    void print(std::ostream &os);

};


#endif