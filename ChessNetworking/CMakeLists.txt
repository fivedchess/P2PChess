cmake_minimum_required(VERSION 3.26.4)
set(CMAKE_GENERATOR "Ninja")
project(ChessNetworking)

set(CMAKE_CXX_STANDART 20)

find_package(Boost 1.81.0 REQUIRED)
file(GLOB SOURCES src/* src/*/* src/*/*/*)
add_library(${PROJECT_NAME} ${SOURCES})
find_package(msgpack-cxx REQUIRED)
target_precompile_headers(ChessNetworking PUBLIC include/ChessNetworking/stdafx.h)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${Boost_INCLUDE_DIRS}
  PRIVATE

)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${Boost_LIBRARIES}
    ${MSGPACK_LIBRARIES}
)
target_compile_options(ChessNetworking PRIVATE -g -O3)
