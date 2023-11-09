#pragma once

#include "IAuthNService.h"
#include "build/auth.pb.h"
#include "build/auth.grpc.pb.h"
#include <mutex>

class AuthNService : public IAuthNService, public auth::AuthService::Service
{
public:
  AuthNService();
  ~AuthNService();

  // Endpoint for gRPC service
  grpc::Status Login(grpc::ServerContext* context, const auth::Credentials* request, auth::AccessToken* response) override;
  bool verify(std::string accessToken) override;

private:
  std::vector<std::string> m_tokens;
  std::mutex m_tokensMutex;
};