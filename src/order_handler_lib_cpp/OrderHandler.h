#pragma once

#include "IOrderHandler.h"
#include <map>
#include <mutex>
#include <condition_variable>

class OrderHandler : public IOrderHandler {
public:
  OrderHandler() = default;
  ~OrderHandler() override = default;

  std::pair<bool, IOrderHandler::OrderId> createOrder(ItemId itemId, bool autostart = true) override;
  
  // NICHT verwendbar für Aktualisierungen an den Client, weil der context nicht übergeben werden kann
  void setOrderStateListener(std::function<void(OrderId, OrderState)> listener) override;
  
  std::pair<OrderId, OrderState> waitForOrderStateChange() override;

private:
  // NICHT verwendbar für Aktualisierungen an den Client, weil der context nicht übergeben werden kann
  std::function<void(OrderId, OrderState)> m_listener;
  
  std::map<OrderId, OrderState> m_orders;
  OrderId m_currOrderIdCounter = 1;
  std::mutex m_mutex;
  std::condition_variable m_cv;
};