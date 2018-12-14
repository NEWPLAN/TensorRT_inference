#ifndef __PARSER_INTERFACE_H__
#define __PARSER_INTERFACE_H__
#include <string>

class ParserInterface
{
  public:
    virtual void setup(std::string &nm) = 0;
    virtual std::string display() = 0;
};
#endif