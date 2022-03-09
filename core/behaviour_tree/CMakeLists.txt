cmake_minimum_required(VERSION 3.5)

project(behaviour_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} behaviour_tree.cpp)
