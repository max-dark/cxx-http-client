//
// Created by max on 08.12.2020.
//

#include "client.hxx"

#include <atomic>
#include <mutex>
#include <condition_variable>

using namespace std::chrono_literals;

struct Client::Data
{
    /// hostname or ip
    Address address;
    Port port = 80;
    Interval interval = 1000ms;

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

void Client::run()
{
    self->running = true;
    while (is_running())
    {
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
