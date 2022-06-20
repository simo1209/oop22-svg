#include <vector>
#include <ostream>

#ifndef __ASTNODE
#define __ASTNODE

class ASTNode
{
private:
    unsigned startPosition;
    unsigned endPosition;

    std::vector<ASTNode *> children;

public:
    ASTNode(unsigned _startPosition, unsigned _endPosition) : startPosition(_startPosition), endPosition(_endPosition) {}
    virtual ~ASTNode();

    void addChild(ASTNode *child) { children.push_back(child); };

    unsigned getStartPosition() const { return startPosition; }
    unsigned getEndPosition() const { return endPosition; }
    const std::vector<ASTNode *> &getChildren() const { return children; }

    virtual void print(std::ostream &os);
};

#endif
