#pragma once

#include <memory>

class IAuthNService;

namespace auth
{
  void RunServer(std::shared_ptr<IAuthNService> authNService);
} // namespace auth
