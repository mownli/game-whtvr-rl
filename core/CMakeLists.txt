cmake_minimum_required(VERSION 3.5)

project(core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(sdl)

add_library(${PROJECT_NAME} core.cpp)
#target_link_libraries(${PROJECT_NAME} PRIVATE sdl)
