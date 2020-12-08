//
// Created by max on 08.12.2020.
//

#ifndef STREAMED_CLIENT_HXX
#define STREAMED_CLIENT_HXX

#include <string>
#include <cstdint>
#include <chrono>

/// @brief HTTP Client
class Client
{
public:
    using Address = std::string;
    using Interval = std::chrono::milliseconds;
    using Port = uint16_t;

    /**
     * @brief Client constructor
     * @param address host name or ip
     * @param port service port
     * @param interval update interval
     */
    Client(Address address, Port port, Interval interval);
    ~Client();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;


    Client(Client&& other) noexcept;
    Client& operator=(Client&& other) noexcept;

    /// @brief main cycle
    void run();

    /// @brief send stop notify to this client
    void stop();

    [[nodiscard]] bool is_running() const noexcept;
private:

    void wait();

    struct Data;
    /// internal data
    Data* self = nullptr;
};

#endif //STREAMED_CLIENT_HXX
