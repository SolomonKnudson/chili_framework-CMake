cmake_minimum_required(VERSION 3.5)

add_library(MainWindow
	${CMAKE_CURRENT_SOURCE_DIR}/src/MainWindow.cpp
)

target_include_directories(MainWindow PUBLIC
	${CMAKE_SOURCE_DIR}/Engine/
)

target_link_libraries(MainWindow PUBLIC
	ChiliException
	ChiliWin
	Graphics
	Keyboard
	Mouse
)

add_dependencies(MainWindow 
	ChiliException 
	ChiliWin 
	Keyboard 
	Mouse
)
