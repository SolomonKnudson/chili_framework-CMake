cmake_minimum_required(VERSION 3.5)

add_library(DXErr 
	${CMAKE_CURRENT_SOURCE_DIR}/src/DXErr.cpp
)

target_include_directories(DXErr PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(DXErr 	
	ChiliWin
)

add_dependencies(DXErr 
	ChiliWin
)
