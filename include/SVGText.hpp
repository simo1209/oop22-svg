#include "ASTNode.hpp"

#ifndef __ASTTEXT
#define __ASTTEXT

class SVGText : public ASTNode
{
private:
    std::string text;

public:
    SVGText(std::string _text, unsigned _startPosition, unsigned _endPosition) : text(_text), ASTNode(_startPosition, _endPosition) {}

    void print(std::ostream &os);

};

#endif
