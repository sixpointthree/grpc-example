#pragma once

#include <memory>
#include "IAuthNService.h"

class AuthNServiceFactory
{
public:
  static std::shared_ptr<IAuthNService> create();
};