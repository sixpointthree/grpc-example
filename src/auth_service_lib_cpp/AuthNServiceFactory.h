#pragma once

#include <memory>

class IAuthNService;

class AuthNServiceFactory
{
public:
  static std::shared_ptr<IAuthNService> create();
};