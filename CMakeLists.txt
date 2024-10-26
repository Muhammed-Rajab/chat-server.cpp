cmake_minimum_required(VERSION 3.10)

project(chatserver CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server server.cpp)
# target_compile_options(server PRIVATE -DDEBUG -Wall)
# target_include_directories(server PRIVATE /include/)
# target_link_libraries(server PRIVATE /include/)

add_executable(client client.cpp)