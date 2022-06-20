#include "ASTNode.hpp"

ASTNode::~ASTNode()
{
    for (auto &child : children)
    {
        delete child;
    }
}

void ASTNode::print(std::ostream &os)
{
    for (auto &child : children)
    {
        child->print(os);
    }
}