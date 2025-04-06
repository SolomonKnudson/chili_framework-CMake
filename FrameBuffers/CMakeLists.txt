cmake_minimum_required(VERSION 3.30)

add_library(FrameBuffers INTERFACE) 
add_library(chili_framework::FrameBuffers ALIAS FrameBuffers)

target_include_directories(FrameBuffers INTERFACE
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)
