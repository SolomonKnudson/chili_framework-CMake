cmake_minimum_required(VERSION 3.30)

add_executable(chili_framework-CMake WIN32
	${CMAKE_CURRENT_SOURCE_DIR}/App/main.cpp
)

target_link_libraries(chili_framework-CMake PRIVATE
  chili_framework::chili_framework
	chili_framework::FrameBuffers
	chili_framework::Resource
  d3d11.lib
)
add_dependencies(chili_framework d3d11.lib)

target_compile_features(chili_framework-CMake PRIVATE cxx_std_17)
set_target_properties(chili_framework-CMake PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED on
  CXX_EXTENSIONS off
)
