/// entry point

#include <service/client.hxx>
#include <service/queue.hxx>
#include <service/system.hxx>

#include <iostream>
#include <thread>
#include <memory>

#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
    System app{argc, argv};

    if (app.startUp())
    {
        std::clog << "Start Up" << std::endl;

        // TODO: parse command line args
        // TODO: extract page from address
        Client client{"www.yandex.ru", 80, 1000ms};

        auto queue = std::make_shared<Queue>();

        if (client.try_resolve())
        {
            std::clog << "IP resolved / start worker" << std::endl;

            auto worker = std::thread{&Client::run, &client, queue};

            while (!client.is_running())
            {
                // wait for start up
                std::this_thread::yield();
            }

            auto run = true;

            while (run)
            {
                if (app.hasKey())
                {
                    if (27 == app.getKey())
                        run = false;
                }

                {
                    Queue::value_type data;

                    if (queue->try_pop(data, 100ms))
                    {
                        std::cout << data;
                    }
                }

                std::this_thread::yield();
            }

            client.stop();
            worker.join();
        }
        else
        {
            std::cerr << "Cant resolve ip address" << std::endl;
        }
    }
    else
    {
        std::cerr << "Start Up failure" << std::endl;
    }

    return app.shutdown();
}
