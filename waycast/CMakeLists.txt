cmake_minimum_required(VERSION 3.17)
project(waycast)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5 COMPONENTS system window graphics)
find_package(SFML 2.5 COMPONENTS system window audio)

add_executable(waycast main.cpp)
target_link_libraries(waycast sfml-system sfml-window sfml-graphics sfml-audio)