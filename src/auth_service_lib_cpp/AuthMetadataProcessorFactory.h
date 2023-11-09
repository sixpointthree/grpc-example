#pragma once

#include <memory>

class IAuthNService;
namespace grpc {
  class AuthMetadataProcessor;
}

class AuthMetadataProcessorFactory
{
public:
  static std::shared_ptr<grpc::AuthMetadataProcessor> create(std::shared_ptr<IAuthNService> authNService);
};