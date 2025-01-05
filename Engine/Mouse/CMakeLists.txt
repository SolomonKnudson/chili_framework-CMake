cmake_minimum_required(VERSION 3.5)

add_library(Mouse
	${CMAKE_CURRENT_SOURCE_DIR}/src/Mouse.cpp
)

target_include_directories(Mouse PUBLIC
	${CMAKE_SOURCE_DIR}/Engine/
)

target_link_libraries(Mouse)
