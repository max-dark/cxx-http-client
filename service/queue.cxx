//
// Created by max on 09.12.2020.
//

#include "queue.hxx"

void Queue::push(Queue::value_type value)
{
    std::lock_guard lock{guard};

    queue.emplace(std::move(value));
    event.notify_one();
}

bool Queue::empty() const
{
    std::lock_guard lock{guard};
    return queue.empty();
}

bool Queue::try_pop(Queue::value_type& value, Queue::timeout millisec)
{
    std::unique_lock lock{guard};

    auto state = event.wait_for(lock, millisec, [this]{
        return !queue.empty();
    });

    if (state)
    {
        value = std::move(queue.front());
        queue.pop();
    }

    return state;
}
