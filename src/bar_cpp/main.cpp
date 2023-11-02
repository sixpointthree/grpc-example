#include <iostream>
#include <RunServer.h>
#include <OrderHandler.h>
#include <memory>
#include <thread>

int main()
{
    std::shared_ptr<IOrderHandler> orderHandler = std::make_shared<OrderHandler>();
    orderHandler->setOrderStateListener([](IOrderHandler::OrderId orderId, OrderState orderState) {
        std::cout << "Order " << orderId << " changed state to " << std::to_string(static_cast<int>(orderState)) << std::endl;
    });
    RunServer(orderHandler);
    orderHandler->createOrder(1, true);
    std::this_thread::sleep_for(std::chrono::seconds(20));
    return 0;
}
