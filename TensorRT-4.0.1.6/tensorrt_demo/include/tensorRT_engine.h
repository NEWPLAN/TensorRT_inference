#ifndef __TENSORRT_ENGINE_H__
#define __TENSORRT_ENGINE_H__

#include <string>
#include <util/mylogs.h>
#include <engine_inferface.h>

class TensorRTEngine : public EngineInterface
{
public:
  TensorRTEngine(std::string nm);
  virtual ~TensorRTEngine();

  virtual void setup(int, const char **);
  virtual std::string display();

private:
  std::string name_;
};

#endif
