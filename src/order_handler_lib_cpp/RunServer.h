#pragma once

#include <memory>

class IOrderHandler;
namespace grpc {
  class AuthMetadataProcessor;
}

namespace order {
  void RunServer(std::shared_ptr<IOrderHandler> handler, std::shared_ptr<grpc::AuthMetadataProcessor> authMetadataProcessor);
} // namespace order
