cmake_minimum_required(VERSION 3.5)

add_library(DXErr 
	${CMAKE_CURRENT_SOURCE_DIR}/src/DXErr.cpp
)

target_include_directories(DXErr PUBLIC
	${CMAKE_SOURCE_DIR}/Engine/
)

target_link_libraries(DXErr INTERFACE	
	ChiliWin
)

add_dependencies(DXErr 
	ChiliWin
)
