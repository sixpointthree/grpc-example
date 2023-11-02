#include "RunServer.h"
#include <grpcpp/grpcpp.h>
#include "IOrderHandler.h"
#include "OrderServiceImpl.h"

class IOrderHandler;

void RunServer(std::shared_ptr<IOrderHandler> handler)
{
  std::string server_address("0.0.0.0:50051");
  OrderServiceImpl service(handler);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();
}