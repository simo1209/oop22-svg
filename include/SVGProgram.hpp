#include <string>
#include <fstream>
#include <map>

#include "ASTNode.hpp"

#ifndef __SVGPROGRAM
#define __SVGPROGRAM

const std::map<std::string, bool> supportedTags = {
    {"rect", true},
    {"circle", true}};

class SVGProgram
{
private:
    unsigned supportedIdSequence = 1;

    typedef void (SVGProgram::*CommandFunctionPointer)();
    const std::map<std::string, CommandFunctionPointer> commandTable = {
        {"open", &SVGProgram::open},
        {"print", &SVGProgram::print},
        {"save", &SVGProgram::save},
        {"saveas", &SVGProgram::saveas},
        {"close", &SVGProgram::close},
        {"help", &SVGProgram::help}
    };

    std::string fileLocation;
    std::fstream currentFile;

    std::string programText;
    ASTNode *root;

    void parse();

    void open();
    void print();
    void save();
    void saveas();
    void close();
    void help();

public:
    SVGProgram() {}
    ~SVGProgram();

    void run(std::string command);
};

#endif