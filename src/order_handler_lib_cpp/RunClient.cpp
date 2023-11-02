#include "RunClient.h"
#include <grpcpp/grpcpp.h>
#include "build/orderservice.pb.h"
#include "build/orderservice.grpc.pb.h"
#include <iostream>
#include <thread>

void RunClient()
{
  std::string server_address("localhost:50051");

  // connect to grpc server
  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());

  // create stub
  std::unique_ptr<orderservice::OrderService::Stub> stub = orderservice::OrderService::NewStub(channel);

  while (1) {
    // create request
    orderservice::CreateOrderRequest request;
    request.set_item_id(1);
    request.set_autostart(true);

    // create response
    orderservice::CreateOrderResponse response;

    // call rpc
    grpc::ClientContext context;
    grpc::Status status = stub->CreateOrder(&context, request, &response);

    // check response
    if (status.ok()) {
      if (response.success()) {
        std::cout << "Order created " << response.order_id() << std::endl;
      } else {
        std::cout << "Order creation failed: Server response indicates failure" << std::endl;
      }
    }
    else {
      std::cout << "Order creation failed: gRPC status code = " << status.error_code()
                << ", error message = " << status.error_message() << std::endl;
    }

    // sleep
    std::this_thread::sleep_for(std::chrono::seconds(20));
  }
}
