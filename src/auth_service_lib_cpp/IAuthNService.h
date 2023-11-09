#pragma once

#include <stdint.h>
#include <string>

class IAuthNService {
public:
  virtual ~IAuthNService() = default;
  virtual bool verify(std::string accessToken) = 0;
};