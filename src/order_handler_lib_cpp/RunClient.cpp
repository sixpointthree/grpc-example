#include "RunClient.h"
#include <grpcpp/grpcpp.h>
#include "build/orderservice.pb.h"
#include "build/orderservice.grpc.pb.h"
#include <iostream>
#include <chrono>
#include <thread>

void CreateOrder(orderservice::OrderService::Stub* stub) {
  orderservice::CreateOrderRequest request;
  request.set_item_id(1);
  request.set_autostart(true);

  orderservice::CreateOrderResponse response;

  grpc::ClientContext context;
  grpc::Status status = stub->CreateOrder(&context, request, &response);

  if (status.ok() && response.success()) {
    std::cout << "Order created with id: " << response.order_id() << std::endl;
  } else {
    std::cout << "Order creation failed" << std::endl;
  }
}

void RunClient() {
  std::string server_address("localhost:50051");
  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
  std::unique_ptr<orderservice::OrderService::Stub> stub = orderservice::OrderService::NewStub(channel);

  // Flag, um zu überprüfen, ob eine neue Bestellung erstellt werden kann
  bool canCreateOrder = false;
  std::mutex canCreateOrderMutex;

  // Worker-Thread, um den Stream zu empfangen und Bestellungen zu erstellen
  std::thread orderStateChangedRcvThread([&]() {
    std::cout << "Start listening for order state changes" << std::endl;
    grpc::ClientContext context;
    orderservice::OrderStateChangedSubscribeRequest subReq;
    std::unique_ptr<grpc::ClientReader<orderservice::OrderStateChangedUpdate>> clientReader = stub->OrderStateChanged(&context, subReq);

    while (true) {
      orderservice::OrderStateChangedUpdate update;
      if (!clientReader->Read(&update)) {
        // Wenn der Stream geschlossen ist, den Worker-Thread beenden
        break;
      }

      std::cout << "Order state changed: " << update.order_id() << " " << update.state() << std::endl;

      if (update.state() == orderservice::OrderState::Completed) {
        std::cout << "Order completed. Free for a new order request" << std::endl;
        {
          std::lock_guard<std::mutex> lock(canCreateOrderMutex);
          canCreateOrder = true;
        }
      }
    }
    std::cout << "Stop listening for order state changes" << std::endl;
  });

  // Haupt-Thread, um Bestellungen zu erstellen
  CreateOrder(stub.get());

  while (true) {
    std::lock_guard<std::mutex> lock(canCreateOrderMutex);
    if (canCreateOrder) {
      std::cout << "Creating new order" << std::endl;
      CreateOrder(stub.get());  // Erstellen Sie eine neue Bestellung
      canCreateOrder = false;   // Zurücksetzen des Flags
    }
  }

  // Warten, bis der Worker-Thread beendet ist
  orderStateChangedRcvThread.join();
}
