cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_BUILD_TYPE Debug)

project(chili_framework-CMake LANGUAGES CXX)

add_executable(chili_framework-CMake WIN32
	${CMAKE_CURRENT_SOURCE_DIR}/Engine/App/main.cpp
)


target_link_libraries(chili_framework-CMake PUBLIC
	ChiliException
	ChiliWin
	Colors
	DXErr
	FrameBuffers
	Game
	Graphics
	Keyboard
	MainWindow
	Mouse
	Resource
)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/ChiliException)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/ChiliWin)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Colors)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/COMInitializer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/DXErr)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/FrameBuffers)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Game)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Graphics)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Keyboard)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/MainWindow)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Mouse)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine/Resource)