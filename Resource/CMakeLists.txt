cmake_minimum_required(VERSION 3.29)

add_library(Resource INTERFACE)
add_library(chili_framework-CMake::Resource ALIAS Resource)

target_include_directories(Resource INTERFACE
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)
