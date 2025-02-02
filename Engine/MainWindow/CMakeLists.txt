cmake_minimum_required(VERSION 3.5)

add_library(MainWindow)

target_sources(MainWindow PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/MainWindow.cpp
)

target_include_directories(MainWindow PUBLIC
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

target_link_libraries(MainWindow PUBLIC
	ChiliException
	ChiliWin
	Graphics
	Keyboard
	Mouse
)

target_link_libraries(MainWindow PRIVATE
	Resource
)

add_dependencies(MainWindow 
	ChiliException 
	ChiliWin
	Keyboard 
	Mouse
	Resource
)
