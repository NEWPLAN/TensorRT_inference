#ifndef __IMG_CV_H__
#define __IMG_CV_H__
#include <string>
#include <vector>
#include <mutex>

struct pixelMx
{
  int size_in_byte;
  int height;
  int width;
  int channel;
  void *dataptr;
  int label;
};

class CVReader
{
public:
  explicit CVReader(std::string default_path = "/home/newplan/software/tensorrt/TensorRT-4.0.1.6/tensorrt_demo/data/",
                    std::string manifest = "1.jpg");
  ~CVReader() {}

  pixelMx *load_one();

private:
  std::string folder_root;
  std::vector<std::string> manifest;
  std::string testfile;
};

#endif