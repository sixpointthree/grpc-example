#include "RunServer.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/security/auth_metadata_processor.h>
#include <grpcpp/security/tls_credentials_options.h>
#include "IOrderHandler.h"
#include "OrderServiceImpl.h"

class IOrderHandler;

namespace order {
void RunServer(std::shared_ptr<IOrderHandler> handler, std::shared_ptr<grpc::AuthMetadataProcessor> authMetadataProcessor)
{
  std::string server_address("0.0.0.0:50051");
  OrderServiceImpl service(handler);

  grpc::SslServerCredentialsOptions sslOpts;
  // TODO: Fix certificates and paths
  sslOpts.pem_root_certs = "/workspaces/grpc-example/certs/root.crt";
  sslOpts.pem_key_cert_pairs.push_back({ "/workspaces/grpc-example/certs/bar.pem", "" });

  // deal with token authentication
  std::shared_ptr<grpc::ServerCredentials> serverCredentials = grpc::SslServerCredentials(sslOpts);
  //serverCredentials->SetAuthMetadataProcessor(authMetadataProcessor);
  // Not supported in C++ Implementation of gRPC: App will crash with E1105 11:25:13.761753525    9850 insecure_server_credentials.cc:42]    ASSERTION FAILED: 0
  // https://groups.google.com/g/grpc-io/c/ifL63H0kN48 
  // serverCredentials->SetAuthMetadataProcessor(authMetadataProcessor);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, serverCredentials);
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();
}
} // namespace order