#include <iostream>
#include <RunAuthServer.h>
#include <RunServer.h>
#include <OrderHandler.h>
#include <AuthMetadataProcessorFactory.h>
#include <AuthNServiceFactory.h>
#include <memory>
#include <thread>

int main()
{
    std::shared_ptr<IAuthNService> auth = AuthNServiceFactory::create();
    std::shared_ptr<grpc::AuthMetadataProcessor> authMetadataProcessor = AuthMetadataProcessorFactory::create(auth);
    std::thread authThread(auth::RunServer, std::static_pointer_cast<IAuthNService>(auth));
    order::RunServer(std::make_shared<OrderHandler>(), authMetadataProcessor);
    return 0;
}
