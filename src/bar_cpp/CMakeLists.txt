cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

set(SOURCES
  main.cpp
)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../order_handler_lib_cpp)

find_package(OrderHandler REQUIRED)

message(OrderHandler_INCLUDE_DIRS ${OrderHandler_INCLUDE_DIRS})
message(OrderHandler_LIBRARIES ${OrderHandler_LIBRARIES})

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} private
  OrderHandler::OrderHandler
)