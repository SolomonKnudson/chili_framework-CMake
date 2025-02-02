cmake_minimum_required(VERSION 3.5)

add_library(Keyboard)

target_sources(Keyboard PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Keyboard.cpp
)

target_include_directories(Keyboard PUBLIC
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)