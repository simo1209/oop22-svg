#include <regex>
#include <stack>
#include <iostream>
#include <fstream>

#include "SVGProgram.hpp"
#include "SVGElement.hpp"
#include "SVGRect.hpp"
#include "SVGText.hpp"

std::string SVGProgram::readArgument()
{
    if (std::cin.peek() == '\n')
    {
        throw std::runtime_error("Argument missing.");
    }
    std::string argument;
    std::cin >> argument;
    return argument;
}

void SVGProgram::open()
{

    fileLocation = readArgument();

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
    root = new ASTNode(0, programText.length());

    ASTNode *cursor = root;
    std::stack<std::string> tagStack;
    std::stack<ASTNode *> cursorStack;

    for (std::sregex_iterator regexIter = matches; regexIter != words_end; ++regexIter)
    {
        std::smatch match = *regexIter;

        if (lastMatchEnd != match.position())
        {
            cursor->addChild(new SVGText(programText.substr(lastMatchEnd, match.position() - lastMatchEnd), lastMatchEnd, match.position()));
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
            SVGElement *nextChild = new SVGElement(match[2], match[3], match.position(), match.position() + match.length(), match[4].length() > 0);

            if (match[2].compare("svg") == 0)
            {
                if (svg != nullptr)
                {
                    throw std::runtime_error("Invalid number of svg tags.");
                }

                svg = nextChild;
            }

            if (isTagSupported(match[2]) && svg == cursor) // svg child
            {
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

    if (svg == nullptr)
    {
        throw std::runtime_error("Invalid number of svg tags.");
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
    fileLocation = readArgument();
    std::cout << "Saving as " << fileLocation << '\n';

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
    running = false;
}

void SVGProgram::help()
{
    std::cout << "Helping\n";
}

SVGShape *SVGProgram::createRect()
{
    std::string xString, yString, widthString, heightString, color;

    xString = readArgument();
    yString = readArgument();
    widthString = readArgument();
    heightString = readArgument();
    color = readArgument();

    long x = std::stol(xString);
    long y = std::stol(yString);
    unsigned width = std::stoul(widthString);
    unsigned height = std::stoul(heightString);

    std::string attributesString = " ";
    attributesString += "x=\"" + xString + "\" ";
    attributesString += "y=\"" + yString + "\" ";
    attributesString += "width=\"" + widthString + "\" ";
    attributesString += "height=\"" + heightString + "\" ";
    attributesString += "color=\"" + color + "\" ";

    std::string tag = "rect";

    unsigned startPos = svg->getChildren().back()->getEndPosition();
    unsigned endPos = startPos + 3 + tag.length() + attributesString.length();

    SVGElement *element = new SVGElement(tag, attributesString, startPos, endPos, true);
    std::cout << "Rect selfclosing : (" << element->isSelfClosing() << ")\n";
    svg->addChild(element);

    return new SVGRect(x, y, width, height, color, supportedIdSequence++, element);
}

SVGShape *SVGProgram::createCircle()
{
}

void SVGProgram::create()
{
    std::string figure = readArgument();

    if (!isTagSupported(figure))
    {
        throw std::runtime_error("Figure not supported.");
    }

    ShapeFunctionPointer shapeFunction = shapeTable.at(figure);
    SVGShape *newShape = (this->*shapeFunction)();
    shapes.push_back(newShape);
    std::cout << "Created " << figure << "(" << newShape->getId() << ")\n";
}

void SVGProgram::run()
{
    while (running)
    {
        std::string command;

        try
        {
            std::cout << "Enter command: ";
            std::cin >> command;
            if (!isCommandSupported(command))
            {
                throw std::runtime_error("Command not supported.");
            }
            CommandFunctionPointer cfp = commandTable.at(command);
            (this->*cfp)();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

SVGProgram::~SVGProgram()
{
    currentFile.close();
    delete root;
    for (auto &shape : shapes)
    {
        delete shape;
    }
}

bool SVGProgram::isTagSupported(std::string tag)
{
    return shapeTable.find(tag) != shapeTable.end();
}

bool SVGProgram::isCommandSupported(std::string command)
{
    return commandTable.find(command) != commandTable.end();
}
