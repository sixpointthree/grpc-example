#pragma once

#include <memory>

class IOrderHandler;

void RunServer(std::shared_ptr<IOrderHandler> handler);
