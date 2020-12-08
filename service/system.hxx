//
// Created by max on 09.12.2020.
//

#ifndef STREAMED_SYSTEM_HXX
#define STREAMED_SYSTEM_HXX

class System
{
public:
    System(int argc, char** argv);

    bool startUp();

    int shutdown();

    bool hasKey();

    int getKey();

    ~System();
private:
    struct Data;
    Data* self;
};

#endif //STREAMED_SYSTEM_HXX
