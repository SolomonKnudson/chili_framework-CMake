cmake_minimum_required(VERSION 3.5)

add_library(COMInitializer 
	${CMAKE_CURRENT_SOURCE_DIR}/src/COMInitializer.cpp
)

target_include_directories(COMInitializer PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(COMInitializer PUBLIC	
	ChiliWin
)

add_dependencies(COMInitializer ChiliWin)