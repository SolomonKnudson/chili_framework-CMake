cmake_minimum_required(VERSION 3.29)

add_library(FrameBuffers INTERFACE) 
add_library(chili_framework-CMake::FrameBuffers ALIAS FrameBuffers)

target_include_directories(FrameBuffers INTERFACE
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)
