cmake_minimum_required(VERSION 3.5)

add_library(Keyboard
	${CMAKE_CURRENT_SOURCE_DIR}/src/Keyboard.cpp
)

target_include_directories(Keyboard PUBLIC
	${CMAKE_SOURCE_DIR}/Engine/
)