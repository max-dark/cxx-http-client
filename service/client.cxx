//
// Created by max on 08.12.2020.
//

#include <service/client.hxx>
#include <service/queue.hxx>

#include <atomic>
#include <mutex>
#include <condition_variable>

#include <vector>
#include <string>

#include <iostream>
#include <sstream>

#include <winsock2.h>

#include <sys/types.h>
#include <unistd.h>

using namespace std::chrono_literals;

struct Client::Data
{
    /// hostname or ip
    Address address;
    Port port = 80;
    Interval interval = 1000ms;

    std::vector<struct in_addr> ip_list{};
    std::size_t current_ip = 0;

    std::atomic_bool running = false;

    std::mutex event;
    std::condition_variable wait_event;
};


Client::Client(Client::Address address, Client::Port port, Client::Interval interval)
    : self{new Data}
{
    self->address = std::move(address);
    self->port = port;
    self->interval = interval;
}

Client::~Client()
{
    delete self;
}


Client::Client(Client&& other)noexcept
{
    std::swap(self, other.self);
}

Client& Client::operator=(Client&& other) noexcept
{
    std::swap(self, other.self);
    return *this;
}

void Client::run(std::shared_ptr<Queue> queue)
{
    std::ostringstream request, response;

    auto eol = "\r\n";

    request << "GET / HTTP/1.1" << eol
            << "Host: " << self->address << eol
            << eol;

    auto header = request.str();


    self->running = true;
    while (is_running())
    {
        response.str("");

        auto handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if(handle != INVALID_SOCKET)
        {
            sockaddr_in info{};
            memset(&info, 0, sizeof(info));

            info.sin_family = AF_INET;
            info.sin_port = htons(self->port);
            info.sin_addr = self->ip_list[self->current_ip];

            auto status = connect(handle, (sockaddr*)&info, sizeof(info));

            if (status != SOCKET_ERROR)
            {
                // std::clog << "connect" << std::endl;

                auto send_status = send(handle, header.c_str(), header.size(), 0);

                constexpr auto SZ = 1023;
                char buff[SZ + 1];
                bool have_data, more_data;
                do
                {
                    auto recv_status = recv(handle, buff, SZ, 0);
                    have_data = (recv_status > 0) && (recv_status != SOCKET_ERROR);
                    more_data = recv_status == SZ;
                    if (have_data)
                    {
                        buff[recv_status] = 0;
                        response << buff;
                    }
                }
                while (more_data);

                queue->push(response.str());
            }

            // select next ip
            self->current_ip += 1;
            self->current_ip %= self->ip_list.size();

            closesocket(handle);
        }
        wait();
    }
}

void Client::stop()
{
    self->running = false;
    self->wait_event.notify_all();
}

void Client::wait()
{
    std::unique_lock lock(self->event);

    self->wait_event.wait_for(lock, self->interval);
}

bool Client::is_running() const noexcept
{
    return self->running;
}

bool Client::try_resolve()
{
    auto host = gethostbyname(self->address.c_str());

    if (nullptr == host) return false;

    self->ip_list = {};

    for (auto ip = host->h_addr_list; *ip; ++ip)
    {
        auto addr = reinterpret_cast<struct in_addr*>(*ip);
        // std::clog << inet_ntoa(*addr) << std::endl;
        self->ip_list.push_back(*addr);
    }

    return true;
}
