cmake_minimum_required(VERSION 3.5)

add_library(Game 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Game.cpp
)

target_include_directories(Game PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(Game PUBLIC 
	Graphics 
	Keyboard 
	MainWindow
	Mouse
)

add_dependencies(Game 
	Graphics 
	Keyboard 
	MainWindow
	Mouse
)
