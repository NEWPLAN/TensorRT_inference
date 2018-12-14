#include <openCV_parser.h>
#include <util/blockingQueue.h>
#include <thread>
#include <iostream>
#include <chrono>
#include "util/imgCV.h"
#include <opencv2/opencv.hpp>
#include <util/mylogs.h>
#include <fstream>

#include <sys/time.h>
static uint64_t current_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

class DecoderHandler
{
  public:
    DecoderHandler(size_t device_id, size_t batch_size)
    {
        this->device_id_ = device_id;
        this->batch_size_ = batch_size;
        LOG(INFO)
            << "create device: " << device_id_;
        tp_ = new std::thread(&DecoderHandler::main_loop, this);
    }
    ~DecoderHandler()
    {
        if (tp_ != nullptr)
            tp_->join();
        std::cout << "GPU device exit..." << std::endl;
    }

    void main_loop()
    {
        BlockingQueue<int> *bq = new BlockingQueue<int>();
        std::vector<BlockingQueue<int> *> b_vec;
        for (int index = 0; index < 10; index++)
        {
            slaved_decoder.push_back(new std::thread(&DecoderHandler::decoder, this, index, bq));
        }
        long long ii = 0;
        long long cc = 0;
        uint64_t before = current_time();
        uint64_t after = 0;
        while (1)
        {
            //std::this_thread::sleep_for(std::chrono::seconds(2));
            std::vector<int> retrive;
            bq->pop_all(retrive);

            cc += retrive.size();
            if (cc >= 1000 * 5)
            {
                after = current_time();
                LOG(INFO) << "Parser serve GPU " << device_id_ << " from " << bq->pop("Get from reader...") << std::endl
                          << "Parser: Rate: " << cc / ((after - before) / 1000.0 / 1000.0) << " images/s" << std::endl
                          << "time: " << (after - before) << " num: " << cc;
                cc = 0;
                before = after;
            }

            if (ii++ % (1000 * 1000 * 50) == 0)
            {
                LOG(INFO) << "Parser serve GPU " << device_id_ << " from " << bq->pop("Get from reader...") << std::endl
                          << "Get size: " << bq->size();
            }
        }
    }

    void decoder(int index, BlockingQueue<int> *bq_)
    {
        char tmp[1024 * 300] = {0};
        size_t file_size = 0;
        {
            std::ifstream file;
            file.open("/home/newplan/software/tensorrt/TensorRT-4.0.1.6/tensorrt_demo/data/runhuzhang.jpg", std::ios::binary);
            if (!file)
            {
                std::cout << "open file failed..." << std::endl;
            }
            file.seekg(0, std::ios::end);
            file_size = file.tellg();
            std::cout << "File size: " << file_size << " Bytes." << std::endl;
            file.seekg(0, std::ios::beg);
            file.read(tmp, sizeof(tmp));
            file.close();
            if (file_size >= 1024 * 300)
            {
                LOG(INFO) << "error in file malloc...." << std::endl;
            }
        }

        int inddd = 0;
        uint64_t before = current_time();
        while (1)
        {
            inddd++;
            /*
            DLOG(INFO) << "decoder " << index << "... to serve GPU " << device_id_;
            */
            //std::this_thread::sleep_for(std::chrono::seconds(20));

            if (1)
            {

                std::vector<char> data(tmp, tmp + file_size);
                cv::Mat src = cv::imdecode(cv::Mat(data), CV_LOAD_IMAGE_COLOR);
                cv::Mat dst;
                cv::resize(src, dst, cv::Size(227, 227), 0, 0, cv::INTER_LINEAR);

                if (inddd % 10000 == 0)
                {
                    uint64_t after = current_time();
                    LOG(INFO) << "each resize processing time is: " << (after - before) / 1000.0
                              << " ms" << std::endl
                              << "Rate: " << inddd * 1.0 / ((after - before) / 1000.0 / 1000) << " image/s/CPU";
                    before = after;
                    inddd = 0;
                }
                bq_->push(index);
                // DLOG(INFO) << "Image info: " << std::endl
                //            << "Image depth: " << src.depth() << std::endl
                //            << "src image: " << src.rows << " x " << src.cols << std::endl
                //            << "dst iamge: " << dst.rows << " x " << dst.cols << std::endl;

                // switch (src.depth())
                // {
                // case CV_8U:
                // {
                //     //for (size_t i = 0UL; i < img_size; ++i)
                //     //{
                //     //  buf[i] = static_cast<Dtype>(cv_img.ptr<float>(0)[i]);
                //     //}
                //     DLOG(INFO) << "Image depth is CV_8U";
                //     break;
                // }
                // case CV_32F:
                // {
                //     DLOG(INFO) << "Image depth is CV_32F";
                //     break;
                // }
                // case CV_64F:
                // {
                //     DLOG(INFO) << "Image depth is CV_64F";
                //     break;
                // }
                // default:
                // {
                //     DLOG(FATAL) << "Image depth is not supported";
                //     break;
                // }
                // }
            }
        }
        cv::waitKey(0);
    }

  private:
    std::thread *tp_;
    size_t device_id_;
    size_t batch_size_;
    BlockingQueue<int> *channel_toGPU;
    BlockingQueue<int> *channel_fromCollector;
    BlockingQueue<int> *subscripter;
    std::vector<std::thread *> slaved_decoder;
};

OpenCVParser::OpenCVParser(std::string nm)
{
    this->name_ = nm;
    LOG(INFO) << " Create OpenCVParser " << nm;
}

OpenCVParser::~OpenCVParser()
{
    LOG(INFO) << name_ << " OpenCVParser release";
}

void OpenCVParser::setup(std::string &nm)
{
    this->name_ = nm;
    LOG(INFO) << name_ << " OpenCVParser setup";

    for (size_t index = 0; index < 2; index++)
    {
        auto *par = new DecoderHandler(index, 32);
    }
}

std::string OpenCVParser::display()
{
    LOG(INFO) << "This OpenCVParser brand is: " << name_;
    return name_;
}