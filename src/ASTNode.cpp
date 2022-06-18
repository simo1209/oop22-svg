#include "ASTNode.hpp"

ASTNode::~ASTNode()
{
    for (auto childIt = children.begin(); childIt != children.end(); ++childIt)
    {
        delete *childIt;
    }
}

void ASTNode::print(std::ostream &os)
{
    os << text;
}
