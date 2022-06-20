#include <regex>
#include <stack>
#include <iostream>
#include <fstream>
#include <tuple>

#include "SVGProgram.hpp"
#include "SVGElement.hpp"
#include "SVGRect.hpp"
#include "SVGCircle.hpp"
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

    currentFile.open(fileLocation, std::ios::in);

    if (!currentFile.is_open())
    {
        throw std::runtime_error("Can't open file.");
    }

    std::cout << "Opened file.\n";
    std::string str(std::istreambuf_iterator<char>{currentFile}, {});
    currentFile.close();

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
                ParseShapeFunctionPointer shapeFunction = std::get<1>(shapeTable.at(match[2]));
                SVGShape *newShape = (this->*shapeFunction)(nextChild);
                shapes.push_back(newShape);
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
    for (auto &shape : shapes)
    {
        shape->print(std::cout);
        std::cout << '\n';
    }
}

void SVGProgram::save()
{
    std::cout << "Saving\n";
    currentFile.open(fileLocation, std::ios::out | std::ios::trunc);
    for (auto &child : root->getChildren())
    {
        child->print(currentFile);
    }
    currentFile.flush();
    currentFile.close();
    std::cout << "Successfully saved file " << fileLocation << '\n';
}

void SVGProgram::saveas()
{
    fileLocation = readArgument();
    std::cout << "Saving as " << fileLocation << '\n';

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
    currentFile.close();

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

SVGAttribute *SVGProgram::findElementAttribute(std::string attributeName, SVGElement *element)
{
    auto attributeIter = std::find_if(element->getAttributes().begin(), element->getAttributes().end(), [attributeName](SVGAttribute *attributePtr)
                                      { return attributeName.compare(attributePtr->getName()) == 0; });

    if (attributeIter == element->getAttributes().end())
    {
        throw std::runtime_error("Shape missing required attribute: " + attributeName);
    }
    return *attributeIter;
}

SVGShape *SVGProgram::createRect()
{
    std::string xString, yString, widthString, heightString, fill;

    xString = readArgument();
    yString = readArgument();
    widthString = readArgument();
    heightString = readArgument();
    fill = readArgument();

    SVGRect *rect = new SVGRect(xString, yString, widthString, heightString, fill, supportedIdSequence++);

    unsigned startPos = svg->getChildren().back()->getEndPosition();
    svg->addChild(rect->generateElement(startPos));

    return rect;
}

SVGShape *SVGProgram::createCircle()
{
    std::string xString, yString, radiusString, fill;

    xString = readArgument();
    yString = readArgument();
    radiusString = readArgument();
    fill = readArgument();

    SVGCircle *circle = new SVGCircle(xString, yString, radiusString, fill, supportedIdSequence++);

    unsigned startPos = svg->getChildren().back()->getEndPosition();
    svg->addChild(circle->generateElement(startPos));

    return circle;
}

SVGShape *SVGProgram::parseRect(SVGElement *element)
{
    std::string xString, yString, widthString, heightString, fill;

    xString = findElementAttribute("x", element)->getValue();
    yString = findElementAttribute("y", element)->getValue();
    widthString = findElementAttribute("width", element)->getValue();
    heightString = findElementAttribute("height", element)->getValue();
    fill = findElementAttribute("fill", element)->getValue();

    SVGRect *rect = new SVGRect(xString, yString, widthString, heightString, fill, supportedIdSequence++);
    rect->setElement(element);

    return rect;
}

SVGShape *SVGProgram::parseCircle(SVGElement *element)
{
    std::string xString, yString, radiusString, fill;

    xString = findElementAttribute("cx", element)->getValue();
    yString = findElementAttribute("cy", element)->getValue();
    radiusString = findElementAttribute("r", element)->getValue();
    fill = findElementAttribute("fill", element)->getValue();

    SVGCircle *circle = new SVGCircle(xString, yString, radiusString, fill, supportedIdSequence++);
    circle->setElement(element);

    return circle;
}

void SVGProgram::create()
{
    std::string figure = readArgument();

    if (!isTagSupported(figure))
    {
        throw std::runtime_error("Figure not supported.");
    }

    CreateShapeFunctionPointer shapeFunction = std::get<0>(shapeTable.at(figure));
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

SVGProgram::SVGProgram(SVGProgram const &other)
{
    setProgramText(other.getProgramText());
    parse();
}

SVGProgram &SVGProgram::operator=(SVGProgram const &other)
{
    if (this != &other)
    {
        setProgramText(other.getProgramText());
        parse();
    }
    return *this;
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
