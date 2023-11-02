#include <iostream>
#include <RunServer.h>
#include <OrderHandler.h>
#include <memory>
#include <thread>

int main()
{
    RunServer(std::make_shared<OrderHandler>());
    return 0;
}
