//
// Created by max on 09.12.2020.
//

#include "system.hxx"

#include <string>
#include <vector>

#include <winsock2.h>
#include <windows.h>

#include <conio.h>

struct System::Data
{
    std::string app_name;
    std::vector<std::string> args;

    bool net_ok = false;
};

System::System(int argc, char** argv)
    : self{new Data}
{
    self->app_name = argv[0];

    self->args.reserve(argc);

    for(auto idx = 1; // skip app name
        idx < argc;
        ++idx)
    {
        self->args.emplace_back(argv[idx]);
    }
}

System::~System()
{
    delete self;
}

bool System::startUp()
{
    WSADATA data{};

    auto error_code = WSAStartup(MAKEWORD(2, 2), &data);

    self->net_ok = 0 == error_code;

    return self->net_ok;
}

int System::shutdown()
{
    return WSACleanup();
}

bool System::hasKey()
{
    return 0 != _kbhit();
}

int System::getKey()
{
    return _getch();
}
