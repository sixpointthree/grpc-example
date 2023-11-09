#pragma once

#include <memory>
#include <grpcpp/security/auth_metadata_processor.h>

class IAuthNService;

class AuthNServiceAuthMetadataProcessor : public grpc::AuthMetadataProcessor
{
public:
  AuthNServiceAuthMetadataProcessor(std::shared_ptr<IAuthNService> authNService)
    : m_authNService(authNService) {}
  ~AuthNServiceAuthMetadataProcessor() = default;

  // Implementation of AuthMetadataProcessor
  grpc::Status Process(const InputMetadata& auth_metadata, grpc::AuthContext* context, OutputMetadata* consumed_auth_metadata, OutputMetadata* response_metadata) override;

private:
  std::shared_ptr<IAuthNService> m_authNService;
};
