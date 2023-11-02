#include "OrderServiceImpl.h"
#include "IOrderHandler.h"
#include <iostream>

OrderServiceImpl::OrderServiceImpl(std::shared_ptr<IOrderHandler> handler) : m_orderHandler(handler) {}

grpc::Status OrderServiceImpl::CreateOrder(grpc::ServerContext* context, const orderservice::CreateOrderRequest* request,
                                           orderservice::CreateOrderResponse* response) {         
  std::cout << std::string("CreateOrder: ") + std::to_string(request->item_id()) + std::string(" ") + std::to_string(request->autostart()) << std::endl; // (1")                                   
  auto [success, orderId] = m_orderHandler->createOrder(request->item_id(), request->autostart());
  response->set_success(success);
  response->set_order_id(orderId);
  return grpc::Status::OK;
}

grpc::Status OrderServiceImpl::OrderStateChanged(grpc::ServerContext* context, const orderservice::OrderStateChangedSubscribeRequest* request,
                                 grpc::ServerWriter<orderservice::OrderStateChangedUpdate>* writer) {
  std::cout << "OrderStateChangedSubscribe called" << std::endl;

  while (!context->IsCancelled()) {
    auto [orderId, orderState] = m_orderHandler->waitForOrderStateChange();

    orderservice::OrderStateChangedUpdate update;
    update.set_order_id(orderId);
    update.set_state(static_cast<orderservice::OrderState>(orderState));

    // Schreiben Sie die Aktualisierung an den Client
    if (!writer->Write(update)) {
      break; // Wenn das Schreiben fehlschlägt, brechen Sie die Schleife ab
    }
  }
  std::cout << "OrderStateChangedSubscribe ended" << std::endl;
  return grpc::Status::OK;
}