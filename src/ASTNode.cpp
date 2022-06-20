#include "ASTNode.hpp"

ASTNode::~ASTNode()
{
    for (auto &child : children)
    {
        delete child;
    }
}

ASTNode::ASTNode(ASTNode const &other)
{
    for (auto &child : other.getChildren())
    {
        ASTNode *childCopy = new ASTNode(other.getStartPosition(), other.getEndPosition());
        for (auto &grandChild : child->getChildren()){
            childCopy->addChild(new ASTNode(*grandChild));
        }
        addChild(childCopy);
    }
}

void ASTNode::print(std::ostream &os)
{
    for (auto &child : children)
    {
        child->print(os);
    }
}