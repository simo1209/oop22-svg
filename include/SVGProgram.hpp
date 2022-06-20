#include <string>
#include <fstream>
#include <map>

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
        {"create", &SVGProgram::create}};

    void open();
    void print();
    void save();
    void saveas();
    void close();
    void help();
    void create();

    typedef SVGShape *(SVGProgram::*ShapeFunctionPointer)();
    const std::map<std::string, ShapeFunctionPointer> shapeTable = {
        {"rect", &SVGProgram::createRect},
        {"circle", &SVGProgram::createCircle}};

    SVGShape *createRect();
    SVGShape *createCircle();

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
    ~SVGProgram();

    void run();
};

#endif