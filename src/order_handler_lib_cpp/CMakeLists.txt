# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.10)

# Set the name of the project
project(OrderHandler CXX)
set(version 1.0)

# Set the headers and source files for the library
set(PUBLIC_HEADERS
  IOrderHandler.h
  OrderHandler.h
  OrderState.h
)

set(SOURCES
  OrderHandler.cpp
)

# Create the shared library
add_library(${PROJECT_NAME} SHARED ${PUBLIC_HEADERS} ${SOURCES})

# Install and export the target
install(FILES ${PUBLIC_HEADERS} DESTINATION include/${PROJECT_NAME}-${version})
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME}-${version} EXPORT ${PROJECT_NAME}-targets)
install(EXPORT ${PROJECT_NAME}-targets DESTINATION lib/${PROJECT_NAME}-${version}) 

# Create and install package config and version files
configure_file("${PROJECT_NAME}-config.cmake.in" "${PROJECT_NAME}-config.cmake" @ONLY)
configure_file("${PROJECT_NAME}-config-version.cmake.in" "${PROJECT_NAME}-config-version.cmake" @ONLY)
install(FILES "${PROJECT_NAME}-config.cmake" "${PROJECT_NAME}-config-version.cmake" DESTINATION lib/${PROJECT_NAME}-${version})

# Set the include directories for the library
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
