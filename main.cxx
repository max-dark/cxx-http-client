/// entry point

#include <service/client.hxx>

#include <service/system.hxx>

#include <iostream>

int main(int argc, char** argv)
{
    System app{argc, argv};

    if (app.startUp())
    {
        std::clog << "Start Up" << std::endl;
    }
    else
    {
        std::cerr << "Start Up failure" << std::endl;
    }

    return app.shutdown();
}

