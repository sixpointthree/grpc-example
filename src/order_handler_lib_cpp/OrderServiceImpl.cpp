#include "OrderServiceImpl.h"
#include "IOrderHandler.h"

OrderServiceImpl::OrderServiceImpl(std::shared_ptr<IOrderHandler> handler) : m_orderHandler(handler) {}

grpc::Status OrderServiceImpl::CreateOrder(grpc::ServerContext* context, const orderservice::CreateOrderRequest* request,
                                           orderservice::CreateOrderResponse* response) {
  auto [success, orderId] = m_orderHandler->createOrder(request->item_id(), request->autostart());
  response->set_success(success);
  response->set_order_id(orderId);
  return grpc::Status::OK;
}
