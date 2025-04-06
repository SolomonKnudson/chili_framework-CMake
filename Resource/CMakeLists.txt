cmake_minimum_required(VERSION 3.30)

add_library(Resource INTERFACE)
add_library(chili_framework::Resource ALIAS Resource)

target_include_directories(Resource INTERFACE
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)
