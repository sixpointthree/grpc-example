# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.10)

# Set the name of the project
project(OrderHandler)

# Set the source files for the library
set(SOURCES
  IOrderHandler.h
  OrderHandler.h
  OrderHandler.cpp
  OrderState.h
)

# Create the shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Set the include directories for the library
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
