# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.22)

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

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# init proto env
message(STATUS "gRPC_DIR: $ENV{gRPC_DIR}")
set(gRPC_DIR $ENV{HOME}/.local)
message(STATUS "gRPC_DIR: $ENV{gRPC_DIR}")
set(_PROTOBUF_PROTOC "${gRPC_DIR}/bin/protoc")
set(_GRPC_CPP_PLUGIN_EXECUTABLE "${gRPC_DIR}/bin/grpc_cpp_plugin")

# compile protos
get_filename_component(hw_proto "protos/helloworld.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")

# Definieren der Ausgabedateien für die Standardausgabe und Fehlerausgabe
set(output_log "/workspace/grpc-example/custom_command_output.log")
set(error_log "/workspace/grpc-example/custom_command_error.log")

add_custom_command(
  OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
  COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/protos/compile_protos.sh"
  ARGS "${hw_proto}" "${hw_proto_path}" "${CMAKE_CURRENT_BINARY_DIR}" "${_PROTOBUF_PROTOC}" "${_GRPC_CPP_PLUGIN_EXECUTABLE}"
  DEPENDS "${hw_proto}"
  COMMENT "Running C++ protocol buffer compiler on ${hw_proto}"
  VERBATIM
  OUTPUT "${output_log}"
  ERROR_OUTPUT "${error_log}"
)


# debug the add_custom_command output
message("=========================================")
message(STATUS "_PROTOBUF_PROTOC: ${_PROTOBUF_PROTOC}")
message(STATUS "_GRPC_CPP_PLUGIN_EXECUTABLE: ${_GRPC_CPP_PLUGIN_EXECUTABLE}")
message(STATUS "cmd_result: ${cmd_result}")
message(STATUS "hw_proto: ${hw_proto}")
message(STATUS "hw_proto_path: ${hw_proto_path}")
message(STATUS "hw_proto_srcs: ${hw_proto_srcs}")
message(STATUS "hw_proto_hdrs: ${hw_proto_hdrs}")
message(STATUS "hw_grpc_srcs: ${hw_grpc_srcs}")
message(STATUS "hw_grpc_hdrs: ${hw_grpc_hdrs}")

# Create the shared library
add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

# Set the include directories for the library
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC gRPC::grpc++)

# Install the library and headers
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
install(FILES ${HEADERS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include)