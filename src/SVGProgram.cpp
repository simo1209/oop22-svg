#include <regex>
#include <stack>
#include <iostream>
#include <fstream>

#include "SVGProgram.hpp"
#include "SVGElement.hpp"

void SVGProgram::open()
{

    if (std::cin.peek() == '\n')
    {
        std::cout << "Argument missing.\n";
        return;
    }

    std::cin >> fileLocation;

    currentFile.open(fileLocation, std::ios::in | std::ios::out);

    if (!currentFile.is_open())
    {
        throw std::runtime_error("Can't open file.");
    }

    std::cout << "Opened file.\n";
    std::string str(std::istreambuf_iterator<char>{currentFile}, {});

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
            if (supportedTags.find(match[2]) != supportedTags.end())
            {
                SVGElement *el = static_cast<SVGElement *>(nextChild);
                el->programId = supportedIdSequence++;
            }

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
    std::cout << "Printing\n";
    for (auto &child : root->getChildren())
    {
        child->print(std::cout);
    }
    std::cout << '\n';
}

void SVGProgram::save()
{
    std::cout << "Saving\n";
    for (auto &child : root->getChildren())
    {
        child->print(std::cerr);
    }
    std::cout << '\n';
}

void SVGProgram::saveas()
{
    std::cout << "Saving as " << fileLocation << '\n';
    if (std::cin.peek() == '\n')
    {
        std::cout << "Argument missing.\n";
        return;
    }

    std::cin >> fileLocation;

    currentFile.close();
    currentFile.open(fileLocation, std::ios::out | std::ios::trunc);

    if (!currentFile.is_open())
    {
        throw std::runtime_error("Can't open file.");
    }

    for (auto &child : root->getChildren())
    {
        child->print(currentFile);
    }

    currentFile.flush();

    std::cout << "Successfully saved file " << fileLocation << '\n';
}

void SVGProgram::close()
{
    std::cout << "Closing\n";
    exit(0);
}

void SVGProgram::help()
{
    std::cout << "Helping\n";
    exit(0);
}

void SVGProgram::run(std::string command)
{
    auto commandFunction = commandTable.find(command);
    if (commandFunction != commandTable.end())
    {
        CommandFunctionPointer cfp = commandFunction->second;
        (this->*cfp)();
    }
}

SVGProgram::~SVGProgram()
{
    currentFile.close();
    delete root;
}
