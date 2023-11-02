#pragma once

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "build/orderservice.pb.h"
#include "build/orderservice.grpc.pb.h"
#include <memory>

class IOrderHandler;

class OrderServiceImpl final : public orderservice::OrderService::Service {
public:
  OrderServiceImpl(std::shared_ptr<IOrderHandler> handler);

  grpc::Status CreateOrder(grpc::ServerContext* context, const orderservice::CreateOrderRequest* request,
                           orderservice::CreateOrderResponse* response) override;

private:
  std::shared_ptr<IOrderHandler> m_orderHandler;
};
