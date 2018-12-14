#ifndef __ENGINE_INTERFACE_H__
#define __ENGINE_INTERFACE_H__
#include <string>
#include <util/mylogs.h>

class EngineInterface
{
public:
  virtual void setup(int, const char **) = 0;
  virtual std::string display() = 0;
};

#endif