#include <regex>
#include <stack>
#include <iostream>
#include <fstream>

#include "SVGProgram.hpp"
#include "SVGElement.hpp"

void SVGProgram::open()
{
    std::string fileLocation;
    std::cin >> fileLocation;

    std::fstream file;
    file.open(fileLocation, std::ios::in);

    if (!file.is_open())
    {
        throw std::runtime_error("Can't open file.");
    }

    std::cout << "Opened file.\n";
    std::string str(std::istreambuf_iterator<char>{file}, {});

    programText = str;

    parse();
}

void SVGProgram::parse()
{

    const std::regex tagRegex("<(/)?(\\w+)(.*?)(/)?>"); // <(\/)?(\w+)(.*?)(\/)?>
    auto words_end = std::sregex_iterator();
    auto matches = std::sregex_iterator(programText.begin(), programText.end(), tagRegex);

    unsigned lastMatchEnd = 0;
    root = new ASTNode(programText, 0, programText.length());

    ASTNode *cursor = root;
    std::stack<std::string> tagStack;
    std::stack<ASTNode *> cursorStack;

    for (std::sregex_iterator regexIter = matches; regexIter != words_end; ++regexIter)
    {
        std::smatch match = *regexIter;

        if (lastMatchEnd != match.position())
        {
            cursor->addChild(new ASTNode(programText.substr(lastMatchEnd, match.position() - lastMatchEnd), lastMatchEnd, match.position()));
            lastMatchEnd = match.position() + match.length();
        }

        if (match[1] == '/') // closing tag, pop tag out of stack and see if matches the last added
        {
            if (tagStack.empty() || tagStack.top().compare(match[2]) != 0)
            {
                throw std::runtime_error("Wrong closing tag.");
            }
            tagStack.pop();
            cursor = cursorStack.top();
            cursorStack.pop();
        }
        else // opening tag
        {
            ASTNode *nextChild = new SVGElement(match[2], match[3], match[0], match.position(), match.position() + match.length(), match[4].length() > 0);
            cursor->addChild(nextChild);
            if (match[4] != '/') // move cursor to new child and push tag to stack
            {
                tagStack.push(match[2]);
                cursorStack.push(cursor);
                cursor = nextChild;
            }
        }
    }
}

void SVGProgram::print()
{
    for (auto &child : root->getChildren())
    {
        child->print(std::cout);
    }
    std::cout << '\n';
}

void SVGProgram::save()
{
    std::cout << "Saving\n";
}

void SVGProgram::saveas()
{
    std::cout << "Saving as\n";
}

void SVGProgram::close()
{
    std::cout << "Closing\n";
    exit(0);
}

void SVGProgram::run(std::string command)
{
    if (command.compare("open") == 0)
    {
        open();
    }

    if (command.compare("print") == 0)
    {
        print();
    }

    if (command.compare("save") == 0)
    {
        save();
    }

    if (command.compare("saveas") == 0)
    {
        saveas();
    }

    if (command.compare("close") == 0)
    {
        close();
    }
}

SVGProgram::~SVGProgram()
{
    delete root;
}
