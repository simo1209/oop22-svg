#include <string>
#include <fstream>
#include <map>
#include <tuple>

#include "ASTNode.hpp"
#include "SVGShape.hpp"

#ifndef __SVGPROGRAM
#define __SVGPROGRAM

const std::map<std::string, bool> supportedTags = {
    {"rect", true},
    {"circle", true}};

class SVGProgram
{
private:
    bool running = true;
    unsigned supportedIdSequence = 1;

    typedef void (SVGProgram::*CommandFunctionPointer)();
    const std::map<std::string, CommandFunctionPointer> commandTable = {
        {"open", &SVGProgram::open},
        {"print", &SVGProgram::print},
        {"save", &SVGProgram::save},
        {"saveas", &SVGProgram::saveas},
        {"close", &SVGProgram::close},
        {"help", &SVGProgram::help},
        {"create", &SVGProgram::create},
        {"erase", &SVGProgram::erase}};

    void open();
    void print();
    void save();
    void saveas();
    void close();
    void help();
    void create();
    void erase();

    typedef SVGShape *(SVGProgram::*CreateShapeFunctionPointer)();
    typedef SVGShape *(SVGProgram::*ParseShapeFunctionPointer)(SVGElement *);
    const std::map<std::string, std::tuple<CreateShapeFunctionPointer, ParseShapeFunctionPointer>> shapeTable = {
        {"rect", std::make_tuple(&SVGProgram::createRect, &SVGProgram::parseRect)},
        {"circle", std::make_tuple(&SVGProgram::createCircle, &SVGProgram::parseCircle)}};

    SVGShape *createRect();
    SVGShape *createCircle();
    SVGShape *parseRect(SVGElement *element);
    SVGShape *parseCircle(SVGElement *element);

    SVGAttribute *findElementAttribute(std::string attributeName, SVGElement *element);

    std::string fileLocation;
    std::fstream currentFile;

    std::string programText;

    ASTNode *root;
    SVGElement *svg = nullptr;

    std::vector<SVGShape *> shapes;

    void parse();
    bool isCommandSupported(std::string command);
    bool isTagSupported(std::string tag);

    std::string readArgument();

public:
    SVGProgram() {}
    SVGProgram(SVGProgram const &other);
    SVGProgram &operator=(SVGProgram const &other);
    ~SVGProgram();

    std::string getProgramText() const { return programText; }
    void setProgramText(std::string _programText) { programText = _programText; }

    void run();
};

#endif