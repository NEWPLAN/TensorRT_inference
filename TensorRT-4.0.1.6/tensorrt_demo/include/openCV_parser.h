#ifndef __OPENCV_PARSER_H__
#define __OPENCV_PARSER_H__
#include <parser_inferface.h>
#include <util/mylogs.h>

class OpenCVParser : public ParserInterface
{
  public:
    OpenCVParser(std::string nm);
    virtual ~OpenCVParser();

    virtual void setup(std::string &nm);
    virtual std::string display();

  private:
    std::string name_;
};
#endif