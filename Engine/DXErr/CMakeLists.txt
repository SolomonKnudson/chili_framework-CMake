cmake_minimum_required(VERSION 3.5)

add_library(DXErr) 

target_sources(DXErr PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/DXErr.cpp
)

target_include_directories(DXErr PUBLIC
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

target_link_libraries(DXErr 	
	ChiliWin
)

add_dependencies(DXErr 
	ChiliWin
)
