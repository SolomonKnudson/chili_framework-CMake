cmake_minimum_required(VERSION 3.29)

add_executable(chili_framework-CMake WIN32
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp	
)

target_link_libraries(chili_framework-CMake PRIVATE
  chili_framework-CMake::chili_framework
  chili_framework-CMake::FrameBuffers
  chili_framework-CMake::Resource
  d3d11.lib
)

target_compile_features(chili_framework-CMake PRIVATE cxx_std_17)
set_target_properties(chili_framework-CMake PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED on
  CXX_EXTENSIONS off
)
