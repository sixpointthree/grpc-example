#include "AuthNServiceAuthMetadataProcessor.h"
#include "AuthNService.h"

grpc::Status AuthNServiceAuthMetadataProcessor::Process(const InputMetadata& auth_metadata, grpc::AuthContext* context, OutputMetadata* consumed_auth_metadata, OutputMetadata* response_metadata)
{
  std::cout << "AuthNService::Process" << std::endl;
  grpc::string_ref access_token_ref = auth_metadata.find("authorization")->second;
  std::string accessToken(access_token_ref.data(), access_token_ref.length());
  if (m_authNService->verify(accessToken)) {
    std::cout << "AuthNService::Process: verified" << std::endl;
    consumed_auth_metadata->insert(std::make_pair("authorization", accessToken));
    return grpc::Status::OK;
  }
  std::cout << "AuthNService::Process: not verified" << std::endl;
  return grpc::Status::CANCELLED;
}
