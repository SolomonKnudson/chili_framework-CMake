cmake_minimum_required(VERSION 3.5)

add_library(Mouse)

target_sources(Mouse PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Mouse.cpp
)

target_include_directories(Mouse PUBLIC
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

target_link_libraries(Mouse)
