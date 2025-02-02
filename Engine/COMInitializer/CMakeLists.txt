cmake_minimum_required(VERSION 3.5)

add_library(COMInitializer)

target_sources(COMInitializer PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/COMInitializer.cpp
)

target_include_directories(COMInitializer PUBLIC
	$<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

target_link_libraries(COMInitializer PUBLIC	
	ChiliWin
)

add_dependencies(COMInitializer ChiliWin)