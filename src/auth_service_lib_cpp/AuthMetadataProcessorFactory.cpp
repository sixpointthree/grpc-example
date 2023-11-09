#include "AuthMetadataProcessorFactory.h"
#include "AuthNServiceAuthMetadataProcessor.h"
#include "IAuthNService.h"

std::shared_ptr<grpc::AuthMetadataProcessor> AuthMetadataProcessorFactory::create(std::shared_ptr<IAuthNService> authNService)
{
  return std::make_shared<AuthNServiceAuthMetadataProcessor>(authNService);
}