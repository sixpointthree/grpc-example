#pragma once

#include <stdint.h>
#include "OrderState.h"
#include <functional>

class IOrderHandler {
public:
  using ItemId = int32_t;
  using OrderId = int32_t;

  virtual ~IOrderHandler() = default;

  virtual std::pair<bool, OrderId> createOrder(ItemId itemId, bool autostart = true) = 0;
  virtual void setOrderStateListener(std::function<void(OrderId, OrderState)> listener) = 0;
};