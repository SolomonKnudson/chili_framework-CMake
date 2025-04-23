cmake_minimum_required(VERSION 3.29)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

project(chili_framework-CMake 
  LANGUAGES CXX
  DESCRIPTION "This is the Chili Framework used in the world-famous, erection-inducing, cancer-curing Planet Chili C++ Tutorials now with CMake as the build system"
)

add_library(chili_framework)
add_library(chili_framework-CMake::chili_framework ALIAS chili_framework)

target_sources(chili_framework PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/D3PipeLine.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/D3PipeLineCreate.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/D3PipeLineInit.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/Graphics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/GraphicsException.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/GraphicsFrame.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Graphics/GraphicsUtil.cpp

  ${CMAKE_CURRENT_LIST_DIR}/src/COMInitializer.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/DXErr.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Game.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Keyboard.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/MainWindow.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/Mouse.cpp
)

target_include_directories(chili_framework PUBLIC
  $<BUILD_LOCAL_INTERFACE: ${CMAKE_CURRENT_LIST_DIR}/include/>
)

target_link_libraries(chili_framework PUBLIC d3d11.lib FrameBuffers)
add_dependencies(chili_framework FrameBuffers)

target_compile_features(chili_framework PUBLIC cxx_std_17)
set_target_properties(chili_framework PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED on
  CXX_EXTENSIONS off
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/App)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/FrameBuffers)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Resource)
