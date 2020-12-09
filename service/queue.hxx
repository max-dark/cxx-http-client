//
// Created by max on 09.12.2020.
//

#ifndef STREAMED_QUEUE_HXX
#define STREAMED_QUEUE_HXX

#include <queue>
#include <string>

#include <chrono>

#include <mutex>
#include <condition_variable>

class Queue
{
public:
    using value_type = std::string;
    using timeout = std::chrono::milliseconds;

    void push(value_type value);

    bool empty() const;

    bool try_pop(value_type& value, timeout millisec);
private:
    std::queue<value_type> queue;

    mutable std::mutex guard;
    std::condition_variable event;
};

#endif //STREAMED_QUEUE_HXX
