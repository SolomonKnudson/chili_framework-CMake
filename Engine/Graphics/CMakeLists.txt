cmake_minimum_required(VERSION 3.5)

add_library(Graphics 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsUtil.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsCreate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsFrame.cpp
)

target_include_directories(Graphics PUBLIC
	${CMAKE_SOURCE_DIR}/Engine
)

target_link_libraries(Graphics PUBLIC
	d3d11.lib 
	ChiliException 
	Colors 
	ChiliWin 
)

target_link_libraries(Graphics PRIVATE
	DXErr
)

add_dependencies(Graphics	
	ChiliException 
	Colors 
	ChiliWin 
	DXErr
)
