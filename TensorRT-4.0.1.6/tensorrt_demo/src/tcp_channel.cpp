#include <tcp_channel.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

TCPChannel::TCPChannel(std::string nm)
{
    this->name_ = nm;
    LOG(INFO) << " Create TCPChannel " << name_;
}

TCPChannel::~TCPChannel()
{
    LOG(INFO) << name_ << " TCP release";
}

void TCPChannel::setup(std::string &nm)
{
    this->name_ = nm;
    this->ip_addr = "0.0.0.0";
    this->port = 12345;
    LOG(INFO) << name_ << " Data collector setup";
    {
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in local;
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = inet_addr(ip_addr.c_str());

        if (bind(listen_fd, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            LOG(FATAL) << "error in bind sock to IP and port...";
        }
        if (listen(listen_fd, 10) == -1)
        {
            LOG(FATAL) << "Listen error ...";
        }
    }
    {
        while (1)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int new_fd = accept(listen_fd, (struct sockaddr *)&client, &len);
            CHECK((new_fd) >= 0) << "error in accept new fd";
            printf("Get client from: %s, %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            std::thread *tmp = new std::thread(&TCPChannel::client_handler, this, new_fd);
            _handler.push_back(tmp);
            tmp->detach();
        }
    }
}
std::string TCPChannel::display()
{
    LOG(INFO) << "This data collector brand is: " << name_;
    return name_;
}

void TCPChannel::client_handler(int new_fd)
{
    LOG(INFO) << "process request from: " << new_fd;
    while (1)
    {
        char buffer[1024];
        int recv_c = read(new_fd, buffer, sizeof(buffer));
        LOG_IF(FATAL, recv_c == -1) << "read error...";
        LOG(INFO) << "received from client " << new_fd << ": " << buffer;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        write(new_fd, buffer, sizeof(buffer));
    }
    close(new_fd);

    //detach()
}
