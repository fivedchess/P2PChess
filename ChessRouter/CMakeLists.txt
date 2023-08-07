cmake_minimum_required(VERSION 3.26.4)
project(ChessRouter)

set(CMAKE_CXX_STANDART 20)

add_executable(${PROJECT_NAME} router.cpp)
include_directories("/usr/include")
target_include_directories(${PROJECT_NAME}
  PUBLIC
    ChessNetworking
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ChessNetworking
)
target_compile_options(ChessRouter PRIVATE -g -O3)
