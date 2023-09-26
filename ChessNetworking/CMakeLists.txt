cmake_minimum_required(VERSION 3.26.5)
set(CMAKE_GENERATOR "Ninja")
project(ChessNetworking)

set(CMAKE_CXX_STANDART 20)

FIND_PACKAGE(Boost 1.82.0 COMPONENTS program_options REQUIRED)
file(GLOB SOURCES src/* src/*/* src/*/*/*)
add_library(${PROJECT_NAME} ${SOURCES})
target_precompile_headers(ChessNetworking PUBLIC include/ChessNetworking/stdafx.h)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${Boost_INCLUDE_DIRS}
    ChessSerialisation
  PRIVATE

)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ${Boost_LIBRARIES}
    connectable-proto
    type-proto
    request-proto
)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      UNITY_BUILD_MODE BATCH
                      UNITY_BUILD_BATCH_SIZE 200
                      )
TARGET_LINK_LIBRARIES(ChessNetworking LINK_PUBLIC ${Boost_LIBRARIES} )
target_compile_options(ChessNetworking PRIVATE -O3 -fno-omit-frame-pointer)
