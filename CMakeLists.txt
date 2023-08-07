cmake_minimum_required(VERSION 3.26.4)
project(P2PChess)
add_definitions(-DBOOST_ASIO_SEPARATE_COMPILATION)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(ChessNetworking)
add_subdirectory(ChessRouter)


