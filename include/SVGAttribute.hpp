#include <string>

#ifndef __SVGATTRIBUTE
#define __SVGATTRIBUTE

class SVGAttribute
{
private:
    bool valuePresent;
    std::string name;
    std::string value;

    unsigned startPosition;
    unsigned endPosition;

public:
    SVGAttribute(std::string _name, std::string _value, unsigned _startPos, unsigned _endPos, bool _valuePresent) : name(_name), value(_value), startPosition(_startPos), endPosition(_endPos), valuePresent(_valuePresent) {}

    bool hasValue() const { return valuePresent; }
    std::string getName() const { return name; }
    std::string getValue() const { return value; }
    unsigned getStartPosition() const { return startPosition; }
    unsigned getEndPosition() const { return endPosition; }

    void setName(std::string _name) { this->name = _name; }
    void setValue(std::string _val) { this->value = _val; }
    void setValuePresent(bool _hasValue) { this->value = _hasValue; }
    void setStartPosition(unsigned _startPos) { this->startPosition = _startPos; }
    void setEndPosition(unsigned _endPos) { this->endPosition = _endPos; }
};

std::ostream &operator<<(std::ostream &os, SVGAttribute const &attribute);

#endif