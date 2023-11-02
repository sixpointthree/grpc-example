#include "OrderHandler.h"
//#include "OrderService.h"

#include <thread>

std::pair<bool, IOrderHandler::OrderId> OrderHandler::createOrder(ItemId itemId, bool autostart) {
  OrderId orderId = m_currOrderIdCounter++; 
  m_orders[orderId] = OrderState::Created;
  if (autostart) {
    m_orders[orderId] = OrderState::InProduction;
  }
  if (m_listener) {
    m_listener(orderId, m_orders[orderId]);
  }

  // Set the state of the order to Completed after 10 seconds
  std::thread([this, orderId]() {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    m_orders[orderId] = OrderState::Completed;
    if (m_listener) {
      m_listener(orderId, m_orders[orderId]);
    }
  }).detach();

  return {true, orderId};
}

void OrderHandler::setOrderStateListener(std::function<void(OrderId, OrderState)> listener) {
  m_listener = listener;
}