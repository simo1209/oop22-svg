#include <vector>

#include "ASTNode.hpp"
#include "SVGAttribute.hpp"

#ifndef __SVGELEMENT
#define __SVGELEMENT

class SVGElement : public ASTNode
{
private:
    unsigned programId;

    bool selfClosing;
    std::string tag;
    std::vector<SVGAttribute> attributes;
    unsigned attributesTailingWhitespaces;

    std::vector<ASTNode *> children;

    void parseAttributes(std::string attributesString);
    void printAttributes(std::ostream &os);

public:

    SVGElement(std::string _tag, std::string _attributesString, unsigned _startPos, unsigned _endPos, bool _selfClosing = true) : tag(_tag), ASTNode(_startPos, _endPos), selfClosing(_selfClosing)
    {
        parseAttributes(_attributesString);
    }
    ~SVGElement();

    bool isSelfClosing() const { return selfClosing; }
    std::string getTag() const { return tag; }
    const std::vector<SVGAttribute> &getAttributes() const { return attributes; }

    void print(std::ostream &os);

    friend class SVGProgram;
};

#endif