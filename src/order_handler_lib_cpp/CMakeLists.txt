# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.10)

# Set the name of the project
project(OrderHandler)

# Set the source files for the library
set(HEADERS
  IOrderHandler.h
  OrderHandler.h
  OrderState.h
)

set(SOURCES
  OrderHandler.cpp
)

# Create the shared library
add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

# Set the include directories for the library
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Install the library and headers
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
install(FILES ${HEADERS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include)
