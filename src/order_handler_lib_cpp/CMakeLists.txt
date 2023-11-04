# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.22)

# Set the name of the project
project(OrderHandler)

# Set the source files for the library
set(HEADERS
  IOrderHandler.h
  OrderHandler.h
  OrderState.h
  OrderServiceImpl.h
  RunServer.h
  RunClient.h
)

set(SOURCES
  OrderHandler.cpp
  RunClient.cpp
  RunServer.cpp
  OrderServiceImpl.cpp
)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# init proto env
set(gRPC_DIR $ENV{HOME}/.local)
set(gRPC_DIR_INCLUDE "${gRPC_DIR}/include")
set(_PROTOBUF_PROTOC "${gRPC_DIR}/bin/protoc")
set(_GRPC_CPP_PLUGIN_EXECUTABLE "${gRPC_DIR}/bin/grpc_cpp_plugin")

# compile protos
get_filename_component(orderservice_proto "protos/orderservice.proto" ABSOLUTE)
get_filename_component(orderservice_proto_path "${orderservice_proto}" PATH)
set(orderservice_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/orderservice.pb.cc")
set(orderservice_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/orderservice.pb.h")
set(orderservice_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/orderservice.grpc.pb.cc")
set(orderservice_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/orderservice.grpc.pb.h")

# debug the add_custom_command output
message("=========================================")
message(STATUS "gRPC_DIR: ${gRPC_DIR}")
message(STATUS "gRPC_DIR_INCLUDE: ${gRPC_DIR_INCLUDE}")
message(STATUS "_PROTOBUF_PROTOC: ${_PROTOBUF_PROTOC}")
message(STATUS "_GRPC_CPP_PLUGIN_EXECUTABLE: ${_GRPC_CPP_PLUGIN_EXECUTABLE}")
message(STATUS "orderservice_proto: ${orderservice_proto}")
message(STATUS "orderservice_proto_path: ${orderservice_proto_path}")
message(STATUS "orderservice_proto_srcs: ${orderservice_proto_srcs}")
message(STATUS "orderservice_proto_hdrs: ${orderservice_proto_hdrs}")
message(STATUS "orderservice_grpc_srcs: ${orderservice_grpc_srcs}")
message(STATUS "orderservice_grpc_hdrs: ${orderservice_grpc_hdrs}")

# Erzeugen Sie eine benutzerdefinierte CMake-Funktion, um Protobuf und gRPC-Dateien zu generieren.
function(generate_protobuf_files proto_file proto_file_path output_dir)
    # Überprüfen Sie, ob das Protobuf-Datei vorhanden ist
    if (NOT EXISTS ${proto_file})
        message(FATAL_ERROR "Protobuf file ${proto_file} does not exist.")
    endif()

    # Fügen Sie Ihre Protobuf- und gRPC-Generierungsschritte hier ein
    execute_process(
        COMMAND ${_PROTOBUF_PROTOC} --grpc_out=${output_dir} --cpp_out=${output_dir} --proto_path=${proto_file_path} --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE} ${proto_file}
        RESULT_VARIABLE protoc_result
    )

    if (protoc_result)
        message(FATAL_ERROR "Protobuf/gRPC generation failed with code ${protoc_result}")
    endif()
endfunction()


# Verwenden Sie die benutzerdefinierte CMake-Funktion, um Protobuf- und gRPC-Dateien zu generieren.
generate_protobuf_files(${orderservice_proto} ${orderservice_proto_path} ${CMAKE_CURRENT_BINARY_DIR})


# Create the static library
add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES} ${orderservice_proto_srcs} ${orderservice_proto_hdrs} ${orderservice_grpc_srcs} ${orderservice_grpc_hdrs})

# Set the include directories for the library
include_directories(${gRPC_DIR_INCLUDE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC gRPC::grpc++)

# Install the library and headers
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
install(FILES ${HEADERS} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include)