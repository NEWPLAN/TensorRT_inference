#ifndef __DATA_COLLECTOR_FROM_TCP_H__
#define __DATA_COLLECTOR_FROM_TCP_H__
#include <string>
#include <util/mylogs.h>
#include <data_collector_interface.h>
#include <vector>
#include <thread>

class TCPChannel : public DataCollectorInterface
{
  public:
    TCPChannel(std::string nm);
    ~TCPChannel();

    virtual void setup(std::string &nm);
    virtual std::string display();

    void client_handler(int new_fd);

  private:
    std::string name_;
    int listen_fd;
    std::string ip_addr;
    int port;
    std::vector<std::thread *> _handler;
};
#endif
