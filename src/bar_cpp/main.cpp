#include <iostream>
#include <RunAuthServer.h>
#include <RunServer.h>
#include <OrderHandler.h>
#include <AuthNServiceFactory.h>
#include <memory>
#include <thread>

int main()
{
    std::shared_ptr<IAuthNService> authNService = AuthNServiceFactory::create();
    std::thread authThread(auth::RunServer, authNService);
    order::RunServer(std::make_shared<OrderHandler>());
    return 0;
}
