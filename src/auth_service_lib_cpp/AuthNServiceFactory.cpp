#include "AuthNServiceFactory.h"
#include "AuthNService.h"

std::shared_ptr<IAuthNService> AuthNServiceFactory::create()
{
  return std::make_shared<AuthNService>();
}