#include <iostream>
#include <IOrderHandler.h>
#include <OrderHandler.h>
#include <memory>
#include <thread>

int main()
{
    std::unique_ptr<IOrderHandler> orderHandler = std::make_unique<OrderHandler>();
    orderHandler->setOrderStateListener([](IOrderHandler::OrderId orderId, OrderState orderState) {
        std::cout << "Order " << orderId << " changed state to " << std::to_string(static_cast<int>(orderState)) << std::endl;
    });
    orderHandler->createOrder(1, true);
    std::this_thread::sleep_for(std::chrono::seconds(20));
    return 0;
}
