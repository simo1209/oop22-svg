#include <string>

#include "ASTNode.hpp"

#ifndef __SVGPROGRAM
#define __SVGPROGRAM

class SVGProgram
{
private:

    std::string programText;
    ASTNode *root;

    void open();
    void print();
    void parse();
    void save();
    void saveas();
    void close();

public:
    SVGProgram(){}
    ~SVGProgram();

    void run(std::string command);

};

#endif