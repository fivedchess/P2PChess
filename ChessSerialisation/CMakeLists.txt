cmake_minimum_required(VERSION 3.26.5)
project(ChessSerialisation)
find_package(Protobuf REQUIRED CONFIG)

set(PROTO_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/sources")
set(CMAKE_UNITY_BUILD_BATCH_SIZE=1)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

list(APPEND PROTO_FILES
    "${CMAKE_CURRENT_LIST_DIR}/src/type.proto"
    "${CMAKE_CURRENT_LIST_DIR}/src/connectable.proto"
    "${CMAKE_CURRENT_LIST_DIR}/src/request.proto"
)

foreach(proto ${PROTO_FILES})
    get_filename_component(PROTO_NAME_WE ${proto} NAME_WE)

    add_library("${PROTO_NAME_WE}-proto" STATIC OBJECT ${proto})

    protobuf_generate(TARGET "${PROTO_NAME_WE}-proto"
        PROTOC_OUT_DIR "${PROTO_OUTPUT_DIR}/ChessSerialisation"
                      IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}/src")

    target_link_libraries("${PROTO_NAME_WE}-proto" PUBLIC
                                                          protobuf::libprotoc
                                                          protobuf::libprotobuf
                                                          protobuf::libprotobuf-lite)
    target_include_directories("${PROTO_NAME_WE}-proto" PUBLIC "$<BUILD_INTERFACE:${PROTO_OUTPUT_DIR}>")
endforeach()


