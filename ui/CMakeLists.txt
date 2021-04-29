cmake_minimum_required(VERSION 3.5)

project(ui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(enginesdl)

add_library(${PROJECT_NAME} ui.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE enginesdl)
