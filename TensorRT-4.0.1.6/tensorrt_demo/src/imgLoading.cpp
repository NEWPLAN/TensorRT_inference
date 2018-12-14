#include "util/imgCV.h"
#include <iostream>
#include <cstdio>

CVReader::CVReader(std::string default_path, std::string manifest)
{
    this->folder_root = default_path;
    this->testfile = default_path + manifest;
}

pixelMx *CVReader::load_one()
{
    static pixelMx a;

    FILE *fp = fopen(testfile.c_str(), "rb");
    if (a.dataptr == nullptr)
    {
        std::cout << "malloc data...." << std::endl;
        a.dataptr = new char[1024 * 1024 * 100]; //default 100M
    }
    if (fp == nullptr || a.dataptr == nullptr)
    {
        std::cout << "error in open file or malloc error...";
    }
    size_t nums = fread(a.dataptr, sizeof(char), 1024 * 1024 * 100, fp);
    if (nums <= 0)
    {
        std::cerr << "error in read file..." << std::endl;
    }
    std::cout << "File size: " << nums << " Bytes" << std::endl;
    fclose(fp);
    a.size_in_byte = nums;
    return &a;
}