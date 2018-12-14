#include <util/blockingQueue.h>
#include <thread>
#include <util/mylogs.h>
#include <chrono>
class AA
{
  public:
    explicit AA(BlockingQueue<int> *bq_)
    {
        LOG(INFO) << "test log" << std::endl;
        this->bq = bq_;
        for (int index = 0; index < 10; index++)
        {
            tv.push_back(std::move(std::thread(&AA::run_entry, this, index)));
        }
    }
    ~AA()
    {
        LOG(INFO) << "Destroy A" << std::endl;
        for (auto &i : tv)
        {
            i.join();
        }
    }

    void run_entry(int index)
    {
        while (1)
        {
            if (index % 2 == 0)
            {
                bq->push(index);
                //std::this_thread::sleep_for(std::chrono::nanoseconds(10));
            }
            else
            {
                int ind = bq->pop();
                LOG_EVERY_N(INFO, 100000) << "ind: " << ind << std::endl;
            }
        }
    }

  private:
    BlockingQueue<int> *bq;
    std::vector<std::thread> tv;
};

//BlockingQueue<int> bq;
//AA a(&bq);

#include <util/thread_pool.hpp>
class BB
{
  public:
    BB()
    {
        newplan::ThreadPool tp(10);
        for (int index = 0; index < 1000; index++)
        {
            tp.runTask([] {
                LOG(INFO) << "Hello, World!" << std::endl;
            });
        }
        tp.waitWorkComplete();
    }
};

//BB b;