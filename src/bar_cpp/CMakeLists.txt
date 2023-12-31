cmake_minimum_required(VERSION 3.10)

project(Bar)

set(SOURCES
  main.cpp
)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

# Importiere die statisch verlinkten libOrderHandler und libAuthService
add_library(libOrderHandler STATIC IMPORTED GLOBAL ${SOURCES})
set_target_properties(libOrderHandler PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../order_handler_lib_cpp/lib/libOrderHandler.a)
add_library(libAuthService STATIC IMPORTED GLOBAL ${SOURCES})
set_target_properties(libAuthService PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../auth_service_lib_cpp/lib/libAuthService.a)

# Setze den Include-Pfad für die Header-Dateien der libOrderHandler und libAuthService
target_include_directories(libOrderHandler INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/../order_handler_lib_cpp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../auth_service_lib_cpp/include
)

target_link_libraries(${PROJECT_NAME} libOrderHandler libAuthService gRPC::grpc++)
