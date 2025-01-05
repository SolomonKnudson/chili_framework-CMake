cmake_minimum_required(VERSION 3.5)

add_library(Graphics 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsUtil.cpp
)

target_include_directories(Graphics PUBLIC
	${CMAKE_SOURCE_DIR}/Engine
)

target_link_libraries(Graphics)
