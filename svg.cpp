#include <fstream>
#include <iterator>
#include <iostream>
#include <regex>
#include <vector>
#include <map>
#include <stack>
#include <assert.h>

enum AbstractSyntaxNodeType
{
    Root,
    TextNode,
    ElementNode
};

class InvalidSyntaxException : std::exception
{
    const char *what() const throw()
    {
        return "Invalid syntax occured.";
    }
};

class AbstractSyntaxTreeNode
{
public:
    unsigned startPosition;
    unsigned endPosition;
    AbstractSyntaxTreeNode *parent;
    AbstractSyntaxNodeType type;

    std::vector<AbstractSyntaxTreeNode *> children;

    AbstractSyntaxTreeNode(AbstractSyntaxNodeType type, AbstractSyntaxTreeNode *parent, unsigned startPos, unsigned endPos) : type(type), parent(parent), startPosition(startPos), endPosition(endPos) {}
};

class ASTTextNode : public AbstractSyntaxTreeNode
{
public:
    std::string value;

    ASTTextNode(std::string val, AbstractSyntaxTreeNode *parent, unsigned startPos, unsigned endPos) : value(val), AbstractSyntaxTreeNode(TextNode, parent, startPos, endPos) {}
};

class ASTElementNode : public AbstractSyntaxTreeNode
{
public:
    std::string tag;
    // std::map<std::string, std::string> attributes;
    std::string attributesString;

    ASTElementNode(std::string tag, std::string attributesString, AbstractSyntaxTreeNode *parent, unsigned startPos, unsigned endPos) : tag(tag), attributesString(attributesString), AbstractSyntaxTreeNode(ElementNode, parent, startPos, endPos) {}
};

void traverseAST(AbstractSyntaxTreeNode *node)
{
    // std::cout << "Type: " << node->type << '\n';

    if (node->type == TextNode)
    {
        ASTTextNode *textNode = static_cast<ASTTextNode *>(node);
        // std::cout << textNode->value;
    }
    if (node->type == ElementNode)
    {
        ASTElementNode *elementNode = static_cast<ASTElementNode *>(node);
        std::cout << "<" << elementNode->tag << elementNode->attributesString;

        if (elementNode->children.size() == 0)
        {
            std::cout << "/>";
        }
        else
        {
            std::cout << ">";
        }
    }

    for (auto it = node->children.begin(); it != node->children.end(); ++it)
    {
        traverseAST(*it);
    }

    if (node->type == ElementNode && node->children.size() > 0)
    {
        ASTElementNode *elementNode = static_cast<ASTElementNode *>(node);
        std::cout << "</" << elementNode->tag << ">";
    }
}

void parseAttributes(std::string attributesString)
{
    // (\w+)(="([\w]*)")?
    const std::regex attributesRegex("(\\w+)(=\"([\\w]*)\")?");
    auto words_end = std::sregex_iterator();
    auto matches = std::sregex_iterator(attributesString.begin(), attributesString.end(), attributesRegex);

    unsigned lastMatchEnd = 0;

    for (std::sregex_iterator regexIter = matches; regexIter != words_end; ++regexIter)
    {
        std::smatch match = *regexIter;

        for (unsigned i = lastMatchEnd; i < match.position(); i++)
        {
            std::cout << ' ';
        }

        std::cout << match[1];

        if (match[2] != "")
        {
            std::cout << '=' << '"' << match[3] << '"';
        }

        lastMatchEnd = match.position() + match.length();
    }

    for (unsigned i = lastMatchEnd; i < attributesString.size(); i++)
    {
        std::cout << ' ';
    }
}

int main()
{
    std::fstream file;
    file.open("example.svg", std::ios::in);

    if (!file.is_open())
    {
        std::cerr << "Can't open file.\n";
        return 1;
    }

    std::string str(std::istreambuf_iterator<char>{file}, {});

    const std::regex tagRegex("<(/)?(\\w+)(.*?)(/)?>");
    auto words_end = std::sregex_iterator();
    auto matches = std::sregex_iterator(str.begin(), str.end(), tagRegex);

    unsigned lastMatchEnd = 0;
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode(Root, nullptr, 0, str.length());

    AbstractSyntaxTreeNode *currentNode = root;
    std::stack<std::string> tagStack;

    for (std::sregex_iterator regexIter = matches; regexIter != words_end; ++regexIter)
    {
        std::smatch match = *regexIter;

        std::cout << match[2] << ':';
        // std::cout << "Attributes string \"" << match[3] << "\"\n";
        parseAttributes(match[3]);
        std::cout << "|\n";

        /*
        if (lastMatchEnd != match.position())
        {
            currentNode->children.push_back(new ASTTextNode(str.substr(lastMatchEnd, match.position() - lastMatchEnd), currentNode, lastMatchEnd, match.position()));
            lastMatchEnd = match.position() + match.length();
        }

        if (match[1] == '/') // closing tag, pop tag out of stack and see if matches the last added
        {
            if (tagStack.empty() || tagStack.top().compare(match[2]) != 0)
            {
                throw InvalidSyntaxException();
            }
            tagStack.pop();
            currentNode = currentNode->parent;
        }
        else // opening tag
        {
            AbstractSyntaxTreeNode *nextChild = new ASTElementNode(match[2], match[3], currentNode, match.position(), match.position() + match.length());
            currentNode->children.push_back(nextChild);
            if (match[4] != '/') // move cursor to new child and push tag to stack
            {
                tagStack.push(match[2]);
                currentNode = nextChild;
            }
        }
        */
    }

    traverseAST(root);

    std::cout << '\n';

    file.close();
    return 0;
}
