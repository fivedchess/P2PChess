cmake_minimum_required(VERSION 3.26.4)
project(P2PChess)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS NO)
add_subdirectory(ChessNetworking)
add_subdirectory(ChessRouter)
add_subdirectory(ChessSerialisation)

