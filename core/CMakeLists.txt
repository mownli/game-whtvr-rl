cmake_minimum_required(VERSION 3.5)

project(core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(graph)
add_subdirectory(behaviour_tree)

#add_library(${PROJECT_NAME} core.cpp cell.cpp actions.cpp commands.cpp trait.cpp object.cpp)
add_library(
	${PROJECT_NAME}
	core.cpp
	unit.cpp
	level.cpp
	trait_mobile.cpp
	trait_dynamic.cpp
	trait_player.cpp
	trait_intelligent.cpp
	ai_bank.cpp
)
#add_library(${PROJECT_NAME}	core.cpp object.cpp	level.cpp trait_mobile.cpp trait_dynamic.cpp trait_player.cpp trait_intelligent.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE graph behaviour_tree)
