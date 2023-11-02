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
  
  // OrderStateChanged muss die Aktualisierungen an den Client senden, bis der Client die Verbindung trennt oder die Anforderung abbricht.
  // Es ist nicht möglich den writer nach Beendigung dieser Methode zu verwenden, also kann der writer auch keinem lambda übergeben werden.
  
  // Prüfen, ob der Client die Verbindung getrennt hat
  while (!context->IsCancelled()) {
    // Warten Sie auf eine Änderung des Bestellstatus
    auto [orderId, orderState] = m_orderHandler->waitForOrderStateChange();

    orderservice::OrderStateChangedUpdate update;
    update.set_order_id(orderId);
    update.set_state(static_cast<orderservice::OrderState>(orderState));

    // Schreiben Sie die Aktualisierung an den Client
    if (!writer->Write(update)) {
      break; // Wenn das Schreiben fehlschlägt, brechen Sie die Schleife ab
    }
  }
  return grpc::Status::OK;  // Wenn die Schleife beendet wird, geben Sie OK zurück.
}