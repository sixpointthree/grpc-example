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
  void setOrderStateListener(std::function<void(OrderId, OrderState)> listener) override;
  std::pair<OrderId, OrderState> waitForOrderStateChange() override;

private:
  std::function<void(OrderId, OrderState)> m_listener;
  std::map<OrderId, OrderState> m_orders;
  OrderId m_currOrderIdCounter = 1;
  std::mutex m_mutex;
  std::condition_variable m_cv;
};