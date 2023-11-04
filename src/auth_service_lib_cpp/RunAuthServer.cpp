#include "RunAuthServer.h"
#include <grpcpp/grpcpp.h>
#include "IAuthNService.h"
#include "build/auth.pb.h"
#include "build/auth.grpc.pb.h"

namespace auth {
void RunServer(std::shared_ptr<IAuthNService> authNService)
{
  std::string server_address("0.0.0.0:50052");

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  auth::AuthService::Service* service = dynamic_cast<auth::AuthService::Service*>(authNService.get());
  builder.RegisterService(service);
  //builder.RegisterService(authNService.get());

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();
}
} // namespace auth