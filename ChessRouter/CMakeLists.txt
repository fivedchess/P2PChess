cmake_minimum_required(VERSION 3.26.5)
project(ChessRouter)

set(CMAKE_CXX_STANDART 20)
find_package(Boost 1.82.0 COMPONENTS program_options REQUIRED)


add_executable(${PROJECT_NAME} router.cpp)
include_directories("/usr/include")
target_include_directories(${PROJECT_NAME}
  PUBLIC
    ChessNetworking
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ChessNetworking
    Boost::program_options
)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
target_compile_options(ChessRouter PRIVATE -g -O3)
