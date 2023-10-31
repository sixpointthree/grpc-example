#pragma once

#include <stdint.h>
#include "OrderState.h"
#include <functional>

class IOrderHandler {
public:
  using OrderId = int32_t;

  virtual ~IOrderHandler() = default;

  virtual bool createOrder(OrderId orderId, bool autostart = true) = 0;
  virtual void setOrderStateListener(std::function<void(OrderId, OrderState)> listener) = 0;
};