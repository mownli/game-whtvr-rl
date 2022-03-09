cmake_minimum_required(VERSION 3.5)

project(graph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} graph.cpp)
#target_link_libraries(${PROJECT_NAME} PRIVATE behaviour_tree)
