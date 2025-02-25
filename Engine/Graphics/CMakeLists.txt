cmake_minimum_required(VERSION 3.5)

add_library(Graphics)

target_sources(Graphics PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Graphics.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsUtil.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GraphicsFrame.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/D3PipeLIneInit.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/D3PipeLIneCreate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/D3PipeLIne.cpp
)

target_include_directories(Graphics PUBLIC
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

target_link_libraries(Graphics PUBLIC
	d3d11.lib 
	ChiliException 
	Colors 
	ChiliWin 
)

target_link_libraries(Graphics PRIVATE
	DXErr
	FrameBuffers
)

add_dependencies(Graphics	
	ChiliException 
	Colors 
	ChiliWin 
	DXErr
	FrameBuffers
)
