# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.22)

# Set the name of the project
project(AuthService)

# Set the source files for the library
set(HEADERS
  IAuthNService.h
  AuthNService.h
  RunAuthServer.h
  AuthNServiceAuthMetadataProcessor.h
  AuthNServiceFactory.h
  AuthMetadataProcessorFactory.h
)

set(SOURCES
  AuthNService.cpp
  RunAuthServer.cpp
  AuthNServiceAuthMetadataProcessor.cpp
  AuthNServiceFactory.cpp
  AuthMetadataProcessorFactory.cpp
)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# init proto env
set(gRPC_DIR $ENV{HOME}/.local)
set(gRPC_DIR_INCLUDE "${gRPC_DIR}/include")
set(_PROTOBUF_PROTOC "${gRPC_DIR}/bin/protoc")
set(_GRPC_CPP_PLUGIN_EXECUTABLE "${gRPC_DIR}/bin/grpc_cpp_plugin")

# compile protos
get_filename_component(auth_proto "protos/auth.proto" ABSOLUTE)
get_filename_component(auth_proto_path "${auth_proto}" PATH)
set(auth_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/auth.pb.cc")
set(auth_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/auth.pb.h")
set(auth_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/auth.grpc.pb.cc")
set(auth_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/auth.grpc.pb.h")
set(GENERATED_PROTOBUF_FILES "${auth_proto_srcs}" "${auth_proto_hdrs}" "${auth_grpc_srcs}" "${auth_grpc_hdrs}")

# debug the add_custom_command output
message("=========================================")
message(STATUS "gRPC_DIR: ${gRPC_DIR}")
message(STATUS "gRPC_DIR_INCLUDE: ${gRPC_DIR_INCLUDE}")
message(STATUS "_PROTOBUF_PROTOC: ${_PROTOBUF_PROTOC}")
message(STATUS "_GRPC_CPP_PLUGIN_EXECUTABLE: ${_GRPC_CPP_PLUGIN_EXECUTABLE}")
message(STATUS "auth_proto: ${auth_proto}")
message(STATUS "auth_proto_path: ${auth_proto_path}")
message(STATUS "auth_proto_srcs: ${auth_proto_srcs}")
message(STATUS "auth_proto_hdrs: ${auth_proto_hdrs}")
message(STATUS "auth_grpc_srcs: ${auth_grpc_srcs}")
message(STATUS "auth_grpc_hdrs: ${auth_grpc_hdrs}")

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
generate_protobuf_files(${auth_proto} ${auth_proto_path} ${CMAKE_CURRENT_BINARY_DIR})


# Create the static library
add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES} ${auth_proto_srcs} ${auth_proto_hdrs} ${auth_grpc_srcs} ${auth_grpc_hdrs})

# Set the include directories for the library
include_directories(${gRPC_DIR_INCLUDE})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC gRPC::grpc++)

# Install the library and headers
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
install(FILES ${HEADERS} ${GENERATED_PROTOBUF_FILES} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include)