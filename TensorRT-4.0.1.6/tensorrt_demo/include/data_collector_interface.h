#ifndef __DATA_COLLECTOR_INTERFACE_H__
#define __DATA_COLLECTOR_INTERFACE_H__
#include <string>
#include <util/mylogs.h>

class DataCollectorInterface
{
  public:
    virtual void setup(std::string &nm) = 0;
    virtual std::string display() = 0;
};
#endif
