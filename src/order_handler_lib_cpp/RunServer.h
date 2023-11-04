#pragma once

#include <memory>

class IOrderHandler;

namespace order {
  void RunServer(std::shared_ptr<IOrderHandler> handler);
} // namespace order
