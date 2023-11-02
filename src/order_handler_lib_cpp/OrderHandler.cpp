#include "OrderHandler.h"
// #include "OrderService.h"

#include <thread>
#include <iostream>

std::pair<bool, IOrderHandler::OrderId> OrderHandler::createOrder(ItemId itemId, bool autostart)
{
  OrderId orderId = m_currOrderIdCounter++;
  m_orders[orderId] = OrderState::Created;
  if (autostart)
  {
    std::thread([=]
                {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        {
          std::unique_lock<std::mutex> lock(m_mutex);
          m_orders[orderId] = OrderState::InProduction;
        }
        std::cout << __PRETTY_FUNCTION__ << "Order(" << orderId << ") changed to InProduction" << std::endl;
        m_cv.notify_all(); })
        .detach();
  }
  std::thread([=]
              { 
      std::this_thread::sleep_for(std::chrono::seconds(10));      
      {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_orders[orderId] = OrderState::Completed;
      }
      std::cout << __PRETTY_FUNCTION__ << "Order(" << orderId << ") changed to Completed" << std::endl;
      m_cv.notify_all(); })
      .detach();

  return {true, orderId};
}

std::pair<IOrderHandler::OrderId, OrderState> OrderHandler::waitForOrderStateChange()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_cv.wait(lock);

  // Wenn sich der Bestellstatus geändert hat, den Status der letzten Bestellung zurückgeben
  const auto it = m_orders.rbegin();
  IOrderHandler::OrderId orderId = it->first;
  OrderState state = it->second;
  std::cout << __PRETTY_FUNCTION__ << "Order(" << orderId << ") changed to " << static_cast<int>(state) << std::endl;
  return {orderId, state};
}

// NICHT verwendbar für Aktualisierungen an den Client, weil der context nicht übergeben werden kann
void OrderHandler::setOrderStateListener(std::function<void(OrderId, OrderState)> listener)
{
  // use m_mutex:
  const std::lock_guard<std::mutex> lock(m_mutex);
  m_listener = listener;
}